#ifndef UBNT_BD_H
#define UBNT_BD_H

struct board_type_info {
        const char *type;
        uint8_t dr_id[4];
};

#define UBNT_FACT_OFFSET	(CFG_FACTORY_ADDR - CFG_FLASH_BASE)
#define UBNT_FACT_SIZE		CFG_FACTORY_SIZE

#define DR_LEN 32
#define BD_LEN sizeof(struct ubnt_bd_t) /* UBNT BD */

struct ubnt_bd_t {
	u16 crc16;
	u8 mac[6];
	u16 mrev;
	char serial[32];
	char type[32];
	/* 74 bytes */
	u8 pad[54]; /* pad to 128 bytes */
	u8 kidx;
	u8 pad1[1];
	/* 130 bytes */
};

int ubnt_bd_read(struct ubnt_bd_t *ubnt_bd);
int misc_init_r (void);
unsigned int ubnt_get_kern_addr(void);

#endif /* UBNT_BD_H */
