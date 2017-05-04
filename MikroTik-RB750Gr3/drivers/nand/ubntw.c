#include <malloc.h>
#include "nand_def.h"
#include <linux/errno.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <rt_mmap.h>
#include "mtk_nand.h"
#include "mt6575_typedefs.h"
#include "nand_device_list.h"
#include <ubntw.h>
#ifdef PMT
#include "partition_define.h"
#endif

#if defined(__UBOOT_NAND__)
#include <common.h>
#include <command.h>
static struct nand_buffers chip_buffers;
unsigned int CFG_BLOCKSIZE;
#endif

extern int ranand_read(char *buf, unsigned int from, int datalen);
extern int ranand_erase_write(char *buf, unsigned int offs, int count);

int last_port_id = 0;
int factory_reset_gpio = 0;

static struct board_type_info board_types[] = { 
	{"e50", {0xee, 0x41, 0x07, 0x77}},
	{"e51", {0xee, 0x42, 0x07, 0x77}},
	{"e52", {0xee, 0x43, 0x07, 0x77}},
	{"e53", {0xee, 0x44, 0x07, 0x77}},
	{"e54", {0xee, 0x45, 0x07, 0x77}},
	{NULL, {0xff, 0xff, 0xff, 0xff}}
};

static struct board_type_info *find_board_id(const char *bid)
{
	int i;
	for (i = 0; board_types[i].type; i++)
		if (memcmp(bid, board_types[i].dr_id, 4) == 0)
			return &(board_types[i]);
	return NULL;
}

static struct board_type_info *find_board_type(const char *btype)
{
	int i;
	for (i = 0; board_types[i].type; i++)
		if (strcmp(btype, board_types[i].type) == 0)
			return &(board_types[i]);
	return NULL;
}

#define POLYNOMIAL 0xA097

static unsigned short CrcTable[256];

static void UpdateCRC(unsigned char DataByte, unsigned short *crc)
{
	unsigned long new_crc;

	new_crc = *crc;
	new_crc = (new_crc << 8) ^ (CrcTable[new_crc >> 8] ^ DataByte);
	*crc = (unsigned short) new_crc;
}

static void GenCrcTable(void)
{
	unsigned short CrcCode;
	unsigned char DataByte;
	unsigned long index, BitNo;
	unsigned long CarryBit, DataBit;

	for (index = 0; index <= 255; index++) {
		CrcCode = 0;
		DataByte = index;
		for (BitNo = 1; BitNo <= 8; BitNo++) {
			CarryBit = ((CrcCode & 0x8000) != 0);
			DataBit = ((DataByte & 0x80) != 0);
			CrcCode = CrcCode << 1;
			if (CarryBit ^ DataBit)
				CrcCode = CrcCode ^ POLYNOMIAL;
			DataByte = DataByte << 1;
		}
		CrcTable[index] = CrcCode;
	}
}

static unsigned short calc_crc(u8 *fp, int len)
{
	unsigned short crc = 0;
	unsigned int i;

	for (i = 0; i < len; i++) {
		UpdateCRC(fp[i], &crc);
	}

	return crc;
}

unsigned int ubnt_get_kern_addr(void)
{
	struct ubnt_bd_t _ubnt_bd;
	ubnt_bd_read(&_ubnt_bd);
	if (_ubnt_bd.kidx == 1)
		return CFG_KERN2_ADDR;
	else
		return CFG_KERN_ADDR;
}

static int ubnt_fill_dr(u8 *buf, u8 *mac, u8 *dr_id, u32 mpart, u8 mrev)
{
	u8 data[DR_LEN];
	int i;
	memset(data, 0, DR_LEN);
	for (i = 0; i < 6; i++) {
		data[i] = mac[i];
		data[i + 6] = mac[i];
	}   
	data[6] |= 0x2;
	for (i = 0; i < 4; i++) {
		data[(12 + i)] = dr_id[i];
	}   
	for (i = 0; i < 3; i++) {
		int sh = (2 - i) * 8;
		data[(16 + i)] = ((mpart >> sh) & 0xff);
	}   
	data[19] = mrev;
	memcpy(buf, data, DR_LEN);
	return DR_LEN;
}

static int ubnt_fill_bd(u8 *buf, u8 *mac, char *serial, char *type, u8 mrev, u8 kidx)
{
	u16 crc16;
	int i;
	struct ubnt_bd_t *pUbnt_bd = (struct ubnt_bd_t *)buf;

	memset(pUbnt_bd, 0, sizeof(struct ubnt_bd_t));
	for (i = 0; i < 6; i++) {
		pUbnt_bd->mac[i] = mac[i];
	}
	strncpy(pUbnt_bd->type, type, strlen(type));
	strncpy(pUbnt_bd->serial, serial, strlen(serial));
	pUbnt_bd->mrev = (u16)mrev;
	pUbnt_bd->kidx = kidx;

	printf("buf %p buf+2 %p size %d\n", buf, (buf+2), sizeof(struct ubnt_bd_t)-2);
	crc16 = calc_crc((u8 *)buf + 2, BD_LEN - 2 - 2);
	printf("crc16 %x\n", crc16);
	pUbnt_bd->crc16 = crc16;

	return sizeof(struct ubnt_bd_t);
}

static int ubnt_set_bd_kidx(u8 *buf, u8 kidx)
{
	struct ubnt_bd_t *pUbnt_bd = (struct ubnt_bd_t *)(buf + DR_LEN);

	pUbnt_bd->kidx = kidx;
	return 0;
}

int ubnt_bd_read(struct ubnt_bd_t *ubnt_bd)
{
	char buf[BD_LEN];

	if (ranand_read(buf, UBNT_FACT_OFFSET+DR_LEN, BD_LEN) < 0) {
		printf("Flash read fail\n");
		return 1;
	}
	memcpy((char *)ubnt_bd, buf, BD_LEN);
	return 0;
}

int ubnt_dr_read(u8 *dr)
{
	char buf[DR_LEN];

	if (ranand_read(buf, UBNT_FACT_OFFSET, DR_LEN) < 0) {
		printf("Flash read fail\n");
		return 1;
	}
	memcpy(dr, buf, DR_LEN);
	return 0;
}

int ubnt_str2mac(char *str, u8 *mac)
{
	char tmp_str[3];
	int i = 0;

	while (i < 6) {
		strncpy(tmp_str, str+(i*2), 2);
		mac[i] = simple_strtoul(tmp_str, NULL, 16);
		i++;
	}
	return 0;
}

void ubnt_bd_dump(struct ubnt_bd_t *ubnt_bd)
{
	printf("UBNT BD type=%s, mac=%02X%02X%02X%02X%02X%02X, s/n=%s, mrev=%d, k_idx=%d\n",
		ubnt_bd->type, ubnt_bd->mac[0], ubnt_bd->mac[1], ubnt_bd->mac[2],
		ubnt_bd->mac[3], ubnt_bd->mac[4], ubnt_bd->mac[5], ubnt_bd->serial,
		ubnt_bd->mrev, ubnt_bd->kidx);
}

int do_ubntw(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char op;
	void *buf = NULL;
	u8 kern_idx;

	if (!argv[1]) {       
		printf("Operation required\n");
		return 1;     
	}
	op = argv[1][0];

	if (op == 'k') {
		if (argc < 3) { 
			printf("Missing argument(s)\n");
			goto invalid; 
		}
		kern_idx = simple_strtoul(argv[2], NULL, 10);
		if (kern_idx >= 2) {
                        printf("Kernel index can be only 0 or 1\n"); 
                        goto invalid;           
		}

		if (!(buf = (void *) malloc(UBNT_FACT_SIZE))) {
			printf("Failed to allocate memory\n");
			goto failed;
		}

		if (ranand_read(buf, UBNT_FACT_OFFSET, UBNT_FACT_SIZE) < 0) {
			printf("Flash read fail\n");
			goto failed;
		}

		ubnt_set_bd_kidx(buf, kern_idx);

		if (ranand_erase_write(buf, UBNT_FACT_OFFSET, UBNT_FACT_SIZE) < 0) {
			printf("Flash write fail\n");
			goto failed;
		}
	} else if (op == 'd') {
		struct ubnt_bd_t ubnt_bd;
	
		printf("Reading NAND flash factory partition 0x%08X size %d\n",
			UBNT_FACT_OFFSET, UBNT_FACT_SIZE);
		ubnt_bd_read(&ubnt_bd);
		ubnt_bd_dump(&ubnt_bd);
	} else if (op == 'a') {
		int len;
		u8 mac[6];
		struct board_type_info *binfo;
		u32 mpart;
		u8 mrev;

		if (argc < 8) { 
			printf("Missing argument(s)\n");
			goto failed; 
		}

		ubnt_str2mac(argv[2], mac);
		if (strlen(argv[3]) >= 32) {           
			printf("Serial number too long\n"); 
			goto invalid;           
		}
		if (strlen(argv[4]) >= 32) {
			printf("Type too long\n");
			goto invalid;
		}
		if (!(binfo = find_board_type(argv[4]))) {
			printf("Invalid type\n");
			goto invalid;
		}
		kern_idx = simple_strtoul(argv[7], NULL, 10);
		if (kern_idx >= 2) {
			printf("Kernel index can be only 0 or 1\n"); 
			goto invalid;           
		}
		mpart = simple_strtoul(argv[5], NULL, 10);
		mrev = simple_strtoul(argv[6], NULL, 10);

		if (!(buf = (void *) malloc(UBNT_FACT_SIZE))) {
			printf("Failed to allocate memory\n");
			goto failed;
		}

		if (ranand_read(buf, UBNT_FACT_OFFSET, UBNT_FACT_SIZE) < 0) {
			printf("Flash read fail\n");
			goto failed;
		}

		len = ubnt_fill_dr(buf, mac, binfo->dr_id, mpart, mrev);
		len += ubnt_fill_bd(buf+len, mac, argv[3], argv[4], mrev, kern_idx);

		if (ranand_erase_write(buf, UBNT_FACT_OFFSET, UBNT_FACT_SIZE) < 0) {
			printf("Flash write fail\n");
			goto failed;
		}
	}

	if (buf) {
		free(buf);
	}
	return 0;

invalid:
	printf("Invalid operation\n");
	return 1;

failed:
	if (buf)
		free(buf);
	return 1;
}

#if defined(CONFIG_MISC_INIT_R)
int ubnt_bd_valid = 0;
int misc_init_r (void)
{
	struct ubnt_bd_t ubnt_bd;
	u8 dr_data[DR_LEN];
	u16 crc16;
	char mac_str[24];

	GenCrcTable();

	if (ubnt_bd_read(&ubnt_bd) != 0 || ubnt_dr_read(dr_data) != 0) {
		printf("Flash read fail\n");
		goto exit;
	} else {
        	crc16 = calc_crc(((u8 *)&ubnt_bd) + 2, BD_LEN - 2 - 2);
		if (crc16 == ubnt_bd.crc16) {
			u32 mpart;
			mpart = (dr_data[16] << 16) + (dr_data[17] << 8) + dr_data[18];
			printf("\nUBNT %s 13-%05u-%02u %s\n", ubnt_bd.type, mpart, ubnt_bd.mrev,
				ubnt_bd.serial);
			ubnt_bd_dump(&ubnt_bd);
			ubnt_bd_valid = 1;
		} else {
			printf("UBNT BD CRC fail\n");
			ubnt_bd.mac[0] = 0x0;
			ubnt_bd.mac[1] = 0xDE;
			ubnt_bd.mac[2] = 0xAD;
			ubnt_bd.mac[3] = 0xBE;
			ubnt_bd.mac[4] = 0xEF;
			ubnt_bd.mac[5] = 0x0;
		}
		sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", ubnt_bd.mac[0], ubnt_bd.mac[1],
			ubnt_bd.mac[2], ubnt_bd.mac[3], ubnt_bd.mac[4], ubnt_bd.mac[5]);
		setenv("ethaddr", mac_str);
	}

	/* init platform variables */
	if (strcmp(ubnt_bd.type, "e50") == 0 ||
		strcmp(ubnt_bd.type, "e51") == 0 ||
		strcmp(ubnt_bd.type, "e52") == 0 ||
		strcmp(ubnt_bd.type, "e53") == 0 ||
		strcmp(ubnt_bd.type, "e54") == 0) {
		last_port_id = 5;
		factory_reset_gpio = 12;
	} else {
		last_port_id = 5;
		factory_reset_gpio = 12;
	}
exit:
	return 0;
}
#endif

U_BOOT_CMD(ubntw, 9, 1, do_ubntw, 
           "ubntw      - ubntw command\n", 
           "  all <mac> <s/n> <type> <part> <minor> <kidx> - write ubnt info\n"
           "  kidx <0/1> - set bootup kernel index\n"
           "  dump - dump ubntw info\n"); 


