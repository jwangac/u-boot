/*
 * This file contains the configuration parameters for the pb93 board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/ar7240.h>

#undef CONFIG_JFFS2_CMDLINE

#define FLASH_SIZE 8
/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS     1	    /* max number of memory banks */
#if (FLASH_SIZE == 16)
#define CFG_MAX_FLASH_SECT      256    /* max number of sectors on one chip */
#elif (FLASH_SIZE == 8)
#define CFG_MAX_FLASH_SECT      128    /* max number of sectors on one chip */
#else
#define CFG_MAX_FLASH_SECT      64    /* max number of sectors on one chip */
#endif

#define CFG_FLASH_SECTOR_SIZE   (64*1024)
#if (FLASH_SIZE == 16)
#define CFG_FLASH_SIZE          0x01000000 /* Total flash size */
#elif (FLASH_SIZE == 8)
#define CFG_FLASH_SIZE          0x00800000    /* max number of sectors on one chip */
#else
#define CFG_FLASH_SIZE          0x00400000 /* Total flash size */
#endif

#define ENABLE_DYNAMIC_CONF 1

#define CONFIG_WASP_SUPPORT 1
#undef CFG_ATHRS26_PHY

#if (CFG_MAX_FLASH_SECT * CFG_FLASH_SECTOR_SIZE) != CFG_FLASH_SIZE
#	error "Invalid flash configuration"
#endif

#define CFG_FLASH_WORD_SIZE     unsigned short

// Set this to zero, for 16bit ddr2
#define DDR2_32BIT_SUPPORT 	1

/* S16 specific defines */

#define CONFIG_AG7240_GE0_IS_CONNECTED 1
/********************/
/* squashfs &lzma   */
/********************/
#define CONFIG_FS		1
#define CFG_FS_SQUASHFS	1
#define CONFIG_SQUASHFS_LZMA
#define CONFIG_LZMA

/*
 * We boot from this flash
 */
#define CFG_FLASH_BASE			0x9f000000
#define FIRMWARE_INTEGRITY_CHECK 1
#define SECOND_PART_FIRMWARE_INTEGRITY_CHECK 1
#define FIRMWARE_RECOVER_FROM_TFTP_SERVER 1
#define CONFIG_JFFS2_PART_OFFSET	0x50040

#ifdef FIRMWARE_RECOVER_FROM_TFTP_SERVER
/*change image length to 0x720000, it is consistent with bootargs rootfs mtd partion */
#define CFG_IMAGE_LEN   0x720000
#define CFG_IMAGE_BASE_ADDR (CFG_FLASH_BASE + 0x50000)
#define CFG_IMAGE_ADDR_BEGIN (CFG_IMAGE_BASE_ADDR)
#define CFG_IMAGE_ADDR_END   (CFG_IMAGE_BASE_ADDR + CFG_IMAGE_LEN)
#define CFG_FLASH_CONFIG_BASE               (CFG_IMAGE_ADDR_END)
#define CFG_FLASH_CONFIG_PARTITION_SIZE     0x10000
#define CFG_STRING_TABLE_LEN 0x19000 /* Each string table takes 100K to save */
#define CFG_STRING_TABLE_NUMBER 10
#define CFG_STRING_TABLE_TOTAL_LEN 0x100000 /* Totally allocate 1024K to save all string tables */
#define CFG_STRING_TABLE_BASE_ADDR (CFG_FLASH_BASE + 0xef0000)
#define CFG_STRING_TABLE_ADDR_BEGIN (CFG_STRING_TABLE_BASE_ADDR)
#define CFG_STRING_TABLE_ADDR_END   (CFG_STRING_TABLE_ADDR_BEGIN + CFG_STRING_TABLE_TOTAL_LEN)
#endif

#define POWER_LED (1<<2)
#define TEST_LED (1<<1)
#define RESET_BUTTON_GPIO (1<<8)

#define CFG_NMRP                1
#define CFG_SINGLE_FIRMWARE     1

#define TFTP_RECOVER_MODE_PINGABLE 1

/*
 * Defines to change flash size on reboot
 */
#ifdef ENABLE_DYNAMIC_CONF
#define UBOOT_FLASH_SIZE          (256 * 1024)
#define UBOOT_ENV_SEC_START        (CFG_FLASH_BASE + UBOOT_FLASH_SIZE)

#define CFG_FLASH_MAGIC           0xaabacada
#define CFG_FLASH_MAGIC_F         (UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x20)
#define CFG_FLASH_SECTOR_SIZE_F   *(volatile int *)(CFG_FLASH_MAGIC_F + 0x4)
#define CFG_FLASH_SIZE_F          *(volatile int *)(CFG_FLASH_MAGIC_F + 0x8) /* Total flash size */
#define CFG_MAX_FLASH_SECT_F      (CFG_FLASH_SIZE / CFG_FLASH_SECTOR_SIZE) /* max number of sectors on one chip */
#else
#define CFG_FLASH_SIZE_F          CFG_FLASH_SIZE
#define CFG_FLASH_SECTOR_SIZE_F   CFG_FLASH_SECTOR_SIZE
#endif


/*
 * The following #defines are needed to get flash environment right
 */
#define	CFG_MONITOR_BASE	TEXT_BASE
#define	CFG_MONITOR_LEN		(192 << 10)

#undef CONFIG_BOOTARGS

#ifndef ROOTFS
#define ROOTFS 1
#endif

#if (ROOTFS == 2) /* make squashfs as rootfs type */
#define CONFIG_ROOTFS_TYPE "rootfstype=squashfs"
#else
#define CONFIG_ROOTFS_TYPE "rootfstype=jffs2"
#endif

#define CONFIG_EXTRA_ENV_SETTINGS		\
	"lu=tftp 0x80060000 u-boot.bin&&"		\
		"erase 0x9f000000 +0x50000;"		\
		"cp.b $fileaddr 0x9f000000 $filesize\0"	\
	""
#undef MTDPARTS_DEFAULT
#define MTDPARTS_DEFAULT	"mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),6336k(rootfs),1408k(uImage),64k(mib0),64k(ART)"
#define CONFIG_ROOT_BOOTARGS	"root=31:02 " CONFIG_ROOTFS_TYPE

#define	CONFIG_BOOTARGS     "console=ttyS0,115200 " CONFIG_ROOT_BOOTARGS " init=/sbin/init " MTDPARTS_DEFAULT

#undef CFG_PLL_FREQ

//#define CFG_PLL_FREQ	CFG_PLL_600_266_133
//#define CFG_PLL_FREQ	CFG_PLL_600_525_262
//#define CFG_PLL_FREQ	CFG_PLL_566_550_275
//#define CFG_PLL_FREQ	CFG_PLL_566_525_262
//#define CFG_PLL_FREQ	CFG_PLL_600_332_166
//#define CFG_PLL_FREQ	CFG_PLL_566_475_237
//#define CFG_PLL_FREQ	CFG_PLL_600_575_287
//#define CFG_PLL_FREQ	CFG_PLL_400_400_200
//#define CFG_PLL_FREQ	CFG_PLL_533_400_200
//#define CFG_PLL_FREQ	CFG_PLL_600_450_200
//#define CFG_PLL_FREQ	CFG_PLL_600_500_1G_250
//#define CFG_PLL_FREQ	CFG_PLL_600_550_1_1G_275
//#define CFG_PLL_FREQ	CFG_PLL_600_350_175
//#define CFG_PLL_FREQ	CFG_PLL_600_300_150
//#define CFG_PLL_FREQ	CFG_PLL_600_1_2G_400_200
//#define CFG_PLL_FREQ	CFG_PLL_300_300_150
//#define CFG_PLL_FREQ	CFG_PLL_500_400_200
//#define CFG_PLL_FREQ	CFG_PLL_400_200_200
#define CFG_PLL_FREQ    CFG_PLL_566_475_237

#undef CFG_HZ
/*
 * MIPS32 24K Processor Core Family Software User's Manual
 *
 * 6.2.9 Count Register (CP0 Register 9, Select 0)
 * The Count register acts as a timer, incrementing at a constant
 * rate, whether or not an instruction is executed, retired, or
 * any forward progress is made through the pipeline.  The counter
 * increments every other clock, if the DC bit in the Cause register
 * is 0.
 */
/* Since the count is incremented every other tick, divide by 2 */
/* XXX derive this from CFG_PLL_FREQ */
#if (CFG_PLL_FREQ == CFG_PLL_200_200_100)
#   define CFG_HZ          (200000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_300_300_150)
#   define CFG_HZ          (300000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_350_350_175)
#   define CFG_HZ          (350000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_333_333_166)
#   define CFG_HZ          (333000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_266_266_133)
#   define CFG_HZ          (266000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_266_266_66)
#   define CFG_HZ          (266000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_400_400_200) || (CFG_PLL_FREQ == CFG_PLL_400_400_100)
#   define CFG_HZ          (400000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_320_320_80) || (CFG_PLL_FREQ == CFG_PLL_320_320_160)
#   define CFG_HZ          (320000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_410_400_200)
#   define CFG_HZ          (410000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_420_400_200)
#   define CFG_HZ          (420000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_240_240_120)
#   define CFG_HZ          (240000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_160_160_80)
#   define CFG_HZ          (160000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_400_200_200)
#   define CFG_HZ          (400000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_500_400_200)
#   define CFG_HZ          (500000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_400_200) || (CFG_PLL_FREQ == CFG_PLL_600_450_200) || \
      (CFG_PLL_FREQ == CFG_PLL_600_600_300) || (CFG_PLL_FREQ == CFG_PLL_600_550_275) || \
      (CFG_PLL_FREQ == CFG_PLL_600_332_166) || (CFG_PLL_FREQ == CFG_PLL_600_575_287) || \
      (CFG_PLL_FREQ == CFG_PLL_600_525_262) || (CFG_PLL_FREQ == CFG_PLL_600_332_200) || \
      (CFG_PLL_FREQ == CFG_PLL_600_266_133) || (CFG_PLL_FREQ == CFG_PLL_600_266_200) || \
      (CFG_PLL_FREQ == CFG_PLL_600_650_325) ||  (CFG_PLL_FREQ == CFG_PLL_600_1_2G_400_200)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_1_2G_400_200)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_500_1G_250)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_550_1_1G_275)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_500_250)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_350_175)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_600_300_150)
#   define CFG_HZ          (600000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_533_400_200) || (CFG_PLL_FREQ == CFG_PLL_533_500_250)
#   define CFG_HZ          (533000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_700_400_200)
#   define CFG_HZ          (700000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_650_600_300)
#   define CFG_HZ          (650000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_560_480_240)
#   define CFG_HZ          (560000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_566_475_237) || (CFG_PLL_FREQ == CFG_PLL_566_450_225) || \
      (CFG_PLL_FREQ == CFG_PLL_566_550_275) || (CFG_PLL_FREQ == CFG_PLL_566_525_262) || \
      (CFG_PLL_FREQ == CFG_PLL_566_400_200) || (CFG_PLL_FREQ == CFG_PLL_566_500_250)
#   define CFG_HZ          (566000000/2)
#endif

/*
 * timeout values are in ticks
 */
#define CFG_FLASH_ERASE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2 * CFG_HZ) /* Timeout for Flash Write */

/*
 * Cache lock for stack
 */
#define CFG_INIT_SP_OFFSET	0x1000

#define	CFG_ENV_IS_IN_FLASH    1
#undef CFG_ENV_IS_NOWHERE

/* Address and size of Primary Environment Sector	*/
#define CFG_ENV_SIZE		CFG_FLASH_SECTOR_SIZE

#define CFG_ENV_ADDR		0x9f040000
#define CONFIG_BOOTCOMMAND	"bootm 0x9f050000"

//#define CONFIG_FLASH_16BIT

/* DDR init values */

#define CONFIG_NR_DRAM_BANKS	2

/* DDR settings for WASP */

#define CFG_DDR_REFRESH_VAL     0x4270
#define CFG_DDR_CONFIG_VAL      0xc7bc8cd0
#define CFG_DDR_MODE_VAL_INIT   0x133
#define CFG_DDR_EXT_MODE_VAL    0x0
#define CFG_DDR_MODE_VAL        0x33

#define CFG_DDR_TRTW_VAL        0x1f
#define CFG_DDR_TWTR_VAL        0x1e

#define CFG_DDR_CONFIG2_VAL     0x9dd0e6a8



#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_32	0xff
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_16	0xffff

#if DDR2_32BIT_SUPPORT
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL		CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_32
#else
#define CFG_DDR2_RD_DATA_THIS_CYCLE_VAL		CFG_DDR2_RD_DATA_THIS_CYCLE_VAL_16
#endif

#define CFG_DDR1_RD_DATA_THIS_CYCLE_VAL		0xffff
#define CFG_SDRAM_RD_DATA_THIS_CYCLE_VAL	0xffffffff

/* DDR2 Init values */
#define CFG_DDR2_EXT_MODE_VAL    0x402


#ifdef ENABLE_DYNAMIC_CONF
#define CFG_DDR_MAGIC           0xaabacada
#define CFG_DDR_MAGIC_F         (UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x30)
#define CFG_DDR_CONFIG_VAL_F    *(volatile int *)(CFG_DDR_MAGIC_F + 4)
#define CFG_DDR_CONFIG2_VAL_F	*(volatile int *)(CFG_DDR_MAGIC_F + 8)
#define CFG_DDR_EXT_MODE_VAL_F  *(volatile int *)(CFG_DDR_MAGIC_F + 12)
#endif

#define CONFIG_NET_MULTI
#define CONFIG_MEMSIZE_IN_BYTES
#define CONFIG_PCI 1

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_COMMANDS	(( CONFIG_CMD_DFL | CFG_CMD_DHCP | CFG_CMD_ELF | CFG_CMD_PCI |\
	CFG_CMD_MII | CFG_CMD_PING | CFG_CMD_NET | CFG_CMD_JFFS2 | CFG_CMD_ENV | CFG_CMD_PLL| \
	CFG_CMD_FLASH | CFG_CMD_LOADS | CFG_CMD_RUN | CFG_CMD_LOADB | CFG_CMD_ELF | CFG_CMD_ETHREG))

#define CONFIG_IPADDR   192.168.1.1
#define CONFIG_SERVERIP 192.168.1.10
#define CONFIG_ETHADDR 0x00:0xaa:0xbb:0xcc:0xdd:0xee
#define CFG_FAULT_ECHO_LINK_DOWN    1


#define CFG_PHY_ADDR 0
#define CFG_GMII     0
#define CFG_MII0_RMII             1
#define CFG_AG7100_GE0_RMII             1

#define CFG_BOOTM_LEN	(16 << 20) /* 16 MB */
#define DEBUG
#define CFG_HUSH_PARSER
#define CFG_PROMPT_HUSH_PS2 "hush>"

/*
** Parameters defining the location of the calibration/initialization
** information for the two Merlin devices.
** NOTE: **This will change with different flash configurations**
*/

#define WLANCAL                         0x9fff1000
#define BOARDCAL                        0x9fff0000
#define ATHEROS_PRODUCT_ID              137
#define CAL_SECTOR                      (CFG_MAX_FLASH_SECT - 1)
#define LAN_MAC_OFFSET			0x00
#define WAN_MAC_OFFSET			0x06
#define WLAN_MAC_OFFSET			0x0c

#define WPSPIN_OFFSET           0x12
#define WPSPIN_LENGTH           8

/* For Merlin, both PCI, PCI-E interfaces are valid */
#define AR7240_ART_PCICFG_OFFSET        12

#include <cmd_confdefs.h>

/*12(lan/wan) + 6 (wlan5g) + 8(wpspin)=26 0x1a */
#define SERIAL_NUMBER_OFFSET		0x1a
#define SERIAL_NUMBER_LENGTH		13

/* WNDR3700 has a MAC address for 5Ghz. */
/* The offset of region number will be 0x27. */
#define REGION_NUMBER_OFFSET		0x27
#define REGION_NUMBER_LENGTH		2

/*
 * wndr3700v3 hardware's PCB number is 2976371901, 8M Flash, 128M Ram
 * It's HW_ID would be "29763719+8+128".
 * "(8 MSB of PCB number)+(Flash size)+(SDRam size)", length should be 14
 */
#define BOARD_HW_ID_OFFSET          (REGION_NUMBER_OFFSET + REGION_NUMBER_LENGTH)
#define BOARD_HW_ID_LENGTH          14

/*
 * Model ID: "wndr3700v3"
 */
#define BOARD_MODEL_ID_OFFSET       (BOARD_HW_ID_OFFSET + BOARD_HW_ID_LENGTH)
#define BOARD_MODEL_ID_LENGTH       10

#define NETGEAR_BOARD_ID_SUPPORT    1

#undef CONFIG_BOOTDELAY
#define CONFIG_BOOTDELAY	1	/* autoboot after 1 seconds	*/

#endif	/* __CONFIG_H */
