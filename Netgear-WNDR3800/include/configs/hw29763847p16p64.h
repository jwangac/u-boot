/*
 * This file contains the configuration parameters for the dbau1x00 board.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/ar7240.h>
#include <config.h>
#undef CONFIG_JFFS2_CMDLINE
/*-----------------------------------------------------------------------
 * FLASH and environment organization
 *-----------------------------------------------------------------------
 */

#define CFG_MAX_FLASH_BANKS     1	    /* max number of memory banks */
#define CFG_MAX_FLASH_SECT      256    /* max number of sectors on one chip */
#define CFG_FLASH_SECTOR_SIZE   (64*1024)
#define CFG_FLASH_SIZE          0x01000000 /* Total flash size */

#undef CONFIG_BOOTDELAY
#define CONFIG_BOOTDELAY        2       /* autoboot after 2 seconds     */

/********************/
/* squashfs &lzma   */
/********************/
#undef CONFIG_FS
#undef CFG_FS_SQUASHFS
#undef CONFIG_SQUASHFS_LZMA
#define CONFIG_LZMA

#define ENABLE_DYNAMIC_CONF 1

#if (CFG_MAX_FLASH_SECT * CFG_FLASH_SECTOR_SIZE) != CFG_FLASH_SIZE
#	error "Invalid flash configuration"
#endif

#define CFG_FLASH_WORD_SIZE     unsigned short 

/* 
 * We boot from this flash
 */
#define CFG_FLASH_BASE		    0x9f000000

#ifdef COMPRESSED_UBOOT
#define BOOTSTRAP_TEXT_BASE         CFG_FLASH_BASE
#define BOOTSTRAP_CFG_MONITOR_BASE  BOOTSTRAP_TEXT_BASE
#endif

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
#endif

#define FIRMWARE_INTEGRITY_CHECK 1
#define SECOND_PART_FIRMWARE_INTEGRITY_CHECK 1
#define FIRMWARE_RECOVER_FROM_TFTP_SERVER 1

#ifdef FIRMWARE_RECOVER_FROM_TFTP_SERVER
#define CFG_IMAGE_BASE_ADDR (CFG_ENV_ADDR + CFG_ENV_SIZE)
#define CFG_IMAGE_ADDR_BEGIN (CFG_IMAGE_BASE_ADDR)
#define CFG_FLASH_CONFIG_BASE               0x9f3e0000
#define CFG_FLASH_CONFIG_PARTITION_SIZE     0x10000
#define CFG_STRING_TABLE_LEN 0x19000 /* Each string table takes 100K to save */
#define CFG_STRING_TABLE_NUMBER 1
#define CFG_STRING_TABLE_TOTAL_LEN 0x20000 /* Totally allocate 1024K to save all string tables */
#define CFG_STRING_TABLE_BASE_ADDR 0x9f3a0000
#define CFG_STRING_TABLE_ADDR_BEGIN (CFG_STRING_TABLE_BASE_ADDR)
#define CFG_STRING_TABLE_ADDR_END   (CFG_STRING_TABLE_ADDR_BEGIN + CFG_STRING_TABLE_TOTAL_LEN)
#endif

/*AR7240*/
#define POWER_LED 20
#define TEST_LED 21
#define CFG_NMRP 1
#define CFG_SINGLE_FIRMWARE 1

#define TFTP_RECOVER_MODE_PINGABLE 1

/* 
 * The following #defines are needed to get flash environment right 
 */
#define	CFG_MONITOR_BASE	TEXT_BASE
#define	CFG_MONITOR_LEN		(192 << 10)

#undef CONFIG_BOOTARGS
/* XXX - putting rootfs in last partition results in jffs errors */

/* default mtd partition table */
#undef MTDPARTS_DEFAULT

#define	CONFIG_BOOTARGS     "console=ttyS0,115200 root=31:02 rootfstype=squashfs init=/sbin/init mtdparts=ar7240-nor0:256k(u-boot),64k(u-boot-env),3392k(rootfs),64k(config),128k(language_table),64k(pot),64k(traffic_meter),64k(ART)"
#define	MTDPARTS_DEFAULT    "mtdparts=ar7240-nor0:256k(u-boot),64k(u-boot-env),3392k(rootfs),64k(config),128k(language_table),64k(pot),64k(traffic_meter),64k(ART)"

#undef CFG_PLL_FREQ

#ifdef CONFIG_HORNET_EMU
    #ifdef CONFIG_HORNET_EMU_HARDI_WLAN
    #define CFG_PLL_FREQ	CFG_PLL_48_48_24
    #else
    #define CFG_PLL_FREQ	CFG_PLL_80_80_40
    #endif
#else
//#define CFG_PLL_FREQ	CFG_PLL_300_300_150
#define CFG_PLL_FREQ	CFG_PLL_400_400_200
#endif


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
#define CPU_PLL_DITHER_FRAC_VAL 0x001003e8
#define CPU_CLK_CONTROL_VAL2 0x00008000

#if (CFG_PLL_FREQ == CFG_PLL_200_200_100)
#	define CFG_HZ          (200000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_300_300_150)
#	define CFG_HZ          (300000000/2)
    #if CONFIG_40MHZ_XTAL_SUPPORT
        #define CPU_PLL_CONFIG_VAL1 0x40813C00
        #define CPU_PLL_CONFIG_VAL2 0x00813C00    
    #else
        #define CPU_PLL_CONFIG_VAL1 0x40816000
        #define CPU_PLL_CONFIG_VAL2 0x00816000
    #endif
#elif (CFG_PLL_FREQ == CFG_PLL_350_350_175)
#	define CFG_HZ          (350000000/2)
    #if CONFIG_40MHZ_XTAL_SUPPORT
        #undef CPU_PLL_DITHER_FRAC_VAL
        #define CPU_PLL_DITHER_FRAC_VAL 0x001803E8
        #define CPU_PLL_CONFIG_VAL1 0x40814600
        #define CPU_PLL_CONFIG_VAL2 0x00814600    
    #else
        #define CPU_PLL_CONFIG_VAL1 0x40817000
        #define CPU_PLL_CONFIG_VAL2 0x00817000
    #endif
#elif (CFG_PLL_FREQ == CFG_PLL_333_333_166)
#	define CFG_HZ          (333000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_266_266_133)
#	define CFG_HZ          (266000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_266_266_66)
#	define CFG_HZ          (266000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_400_400_200) || (CFG_PLL_FREQ == CFG_PLL_400_400_100)
#	define CFG_HZ          (400000000/2)
    #if CONFIG_40MHZ_XTAL_SUPPORT
        #define CPU_PLL_CONFIG_VAL1 0x40815000
        #define CPU_PLL_CONFIG_VAL2 0x00815000    
    #else
        #define CPU_PLL_CONFIG_VAL1 0x40818000
        #define CPU_PLL_CONFIG_VAL2 0x00818000
    #endif
#elif (CFG_PLL_FREQ == CFG_PLL_320_320_80) || (CFG_PLL_FREQ == CFG_PLL_320_320_160)
#	define CFG_HZ          (320000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_410_400_200)
#	define CFG_HZ          (410000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_420_400_200)
#	define CFG_HZ          (420000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_362_362_181)
#	define CFG_HZ          (326500000/2)
    #define CPU_PLL_CONFIG_VAL1 0x40817400
    #define CPU_PLL_CONFIG_VAL2 0x00817400
#elif (CFG_PLL_FREQ == CFG_PLL_80_80_40)
#	define CFG_HZ          (80000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_64_64_32)
#	define CFG_HZ          (64000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_48_48_24)
#	define CFG_HZ          (48000000/2)
#elif (CFG_PLL_FREQ == CFG_PLL_32_32_16)
#	define CFG_HZ          (32000000/2)
#endif

#if CONFIG_40MHZ_XTAL_SUPPORT
    #define CPU_PLL_SETTLE_TIME_VAL    0x00000550
#else
    #define CPU_PLL_SETTLE_TIME_VAL    0x00000352
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

#ifndef COMPRESSED_UBOOT
#define	CFG_ENV_IS_IN_FLASH    1
#undef CFG_ENV_IS_NOWHERE  
#else
#undef  CFG_ENV_IS_IN_FLASH
#define CFG_ENV_IS_NOWHERE  1
#endif /* #ifndef COMPRESSED_UBOOT */

/* Address and size of Primary Environment Sector	*/
#define CFG_ENV_ADDR		0x9f040000
#define CFG_ENV_SIZE		0x10000

#define CONFIG_BOOTCOMMAND "bootm 9f050000"

//#define CONFIG_FLASH_16BIT

/*
** Extra environmental variables useful for loading the board
*/

#define CONFIG_EXTRA_ENV_SETTINGS \
"clearenv=erase 0x9f040000 +0x10000\0" \
"ddr_ext_mode_value=0x0\0" \
"magic_number=0x0"

/*
 * Magic number definition
 */
#define IH_MAGIC_DEFAULT       0x31303034

/* DDR init values */

#define CONFIG_NR_DRAM_BANKS	2
#if CONFIG_40MHZ_XTAL_SUPPORT
#define CFG_DDR_REFRESH_VAL     0x4270
#else
#define CFG_DDR_REFRESH_VAL     0x4186
#endif
#define CFG_DDR_CONFIG_VAL      0x7fbc8cd0
#define CFG_DDR_MODE_VAL_INIT   0x133
#ifdef LOW_DRIVE_STRENGTH
#	define CFG_DDR_EXT_MODE_VAL    0x2
#else
#	define CFG_DDR_EXT_MODE_VAL    0x0
#endif
#define CFG_DDR_MODE_VAL        0x33

#define CFG_DDR_TRTW_VAL        0x1f
#define CFG_DDR_TWTR_VAL        0x1e

//#define CFG_DDR_CONFIG2_VAL	 0x99d0e6a8     // HORNET 1.0
#define CFG_DDR_CONFIG2_VAL	 0x9dd0e6a8         // HORNET 1.1

#define CFG_DDR_RD_DATA_THIS_CYCLE_VAL  0x00ff

#ifndef CONFIG_HORNET_EMU
#define CFG_DDR_TAP0_VAL        0x8
#define CFG_DDR_TAP1_VAL        0x9
#else
#define CFG_DDR_TAP0_VAL        0x8
#define CFG_DDR_TAP1_VAL        0x9
#endif

/* DDR2 Init values */
#define CFG_DDR2_EXT_MODE_VAL    0x402

/* DDR value from Flash */
#ifdef ENABLE_DYNAMIC_CONF
#define CFG_DDR_MAGIC           0xaabacada
#define CFG_DDR_MAGIC_F         (UBOOT_ENV_SEC_START + CFG_FLASH_SECTOR_SIZE - 0x30)
#define CFG_DDR_CONFIG_VAL_F    *(volatile int *)(CFG_DDR_MAGIC_F + 4)
#define CFG_DDR_CONFIG2_VAL_F	*(volatile int *)(CFG_DDR_MAGIC_F + 8)
#define CFG_DDR_EXT_MODE_VAL_F  *(volatile int *)(CFG_DDR_MAGIC_F + 12)
#endif

#define CONFIG_NET_MULTI

#define CONFIG_MEMSIZE_IN_BYTES

#ifndef COMPRESSED_UBOOT
#	ifndef CONFIG_MACH_HORNET
#		define CONFIG_PCI 1
#	endif
#else
#	define ATH_NO_PCI_INIT
#endif /* #ifndef COMPRESSED_UBOOT */

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#ifndef COMPRESSED_UBOOT
#define CONFIG_COMMANDS	(( CONFIG_CMD_DFL | CFG_CMD_DHCP | CFG_CMD_PCI |	\
	CFG_CMD_MII | CFG_CMD_PING | CFG_CMD_NET | CFG_CMD_ENV |	\
	CFG_CMD_FLASH | CFG_CMD_RUN | CFG_CMD_ETHREG) & ~CFG_CMD_LOADS & ~CFG_CMD_LOADB )
#else
#define CONFIG_COMMANDS	(( CONFIG_CMD_DFL | CFG_CMD_PING | CFG_CMD_NET | CFG_CMD_MII))
#endif /* #ifndef COMPRESSED_UBOOT */

#define CFG_ATHRS26_PHY  1

#define CONFIG_IPADDR   192.168.1.10
#define CONFIG_SERVERIP 192.168.1.27
#define CONFIG_ETHADDR 0x00:0xaa:0xbb:0xcc:0xdd:0xee
#define CFG_FAULT_ECHO_LINK_DOWN    1


#define CFG_PHY_ADDR 0 

#ifdef CONFIG_HORNET_EMU
#define CFG_AG7240_NMACS 1
#else
#define CFG_AG7240_NMACS 2
#endif
#define CFG_GMII     0
#define CFG_MII0_RMII             1
#define CFG_AG7100_GE0_RMII             1

#define CFG_BOOTM_LEN	(16 << 20) /* 16 MB */
#define DEBUG
#define CFG_HUSH_PARSER
#define CFG_PROMPT_HUSH_PS2 "hush>"

#define CONFIG_JFFS2_PART_OFFSET	(CFG_IMAGE_BASE_ADDR - CFG_FLASH_BASE + 0x40)

/*
** Parameters defining the location of the calibration/initialization
** information for the two Merlin devices.
** NOTE: **This will change with different flash configurations**
*/

#define WLANCAL                        0xbfff1000
#define BOARDCAL                       0x9fff0000

#define ATHEROS_PRODUCT_ID             138
#define CAL_SECTOR                     (CFG_MAX_FLASH_SECT - 1)
#define LAN_MAC_OFFSET                 0x06
#define WAN_MAC_OFFSET                 0x00
#define WLAN_MAC_OFFSET                0x108c

/* For Kite, only PCI-e interface is valid */
#define AR7240_ART_PCICFG_OFFSET        3

/* 6(LAN mac) + 6(WAN mac) = 12(0x0c) */
#define WPSPIN_OFFSET           0x0c
#define WPSPIN_LENGTH           8

/* 12(WPSPIN_OFFSET) + 8(WPSPIN_LENGTH) = 20(0x14) */
#define SERIAL_NUMBER_OFFSET        0x14
#define SERIAL_NUMBER_LENGTH        13

/* 20(SERIAL_NUMBER_OFFSET) + 13(SERIAL_NUMBER_LENGTH) = 33(0x21) */
#define REGION_NUMBER_OFFSET        0x21
#define REGION_NUMBER_LENGTH        2

/*
 * wnr1000v4 hardware's PCB number is 2976375400, 4M Flash, 32M Ram
 * It's HW_ID would be "29763754+16+64".
 * "(8 MSB of PCB number)+(Flash size)+(SDRam size)", length should be 13
 */
#define BOARD_HW_ID_OFFSET          (REGION_NUMBER_OFFSET + REGION_NUMBER_LENGTH)
#define BOARD_HW_ID_LENGTH          14

/*
 * There are two known model use wnr1000v4 hardware, wnr1000v4 & wnr1000v4-vc
 */
#define BOARD_MODEL_ID_OFFSET       (BOARD_HW_ID_OFFSET + BOARD_HW_ID_LENGTH)
#define BOARD_MODEL_ID_LENGTH       12

#define NETGEAR_BOARD_ID_SUPPORT    1

/* Skip Atheros Ethernet Address Checking*/
#define SKIP_ATHEROS_ETHADDR_CHECKING 1

/* Enable misc_init_r() */
#define CONFIG_MISC_INIT_R 1

#include <cmd_confdefs.h>
#define CONFIG_ETH_SOFT_LED

#endif	/* __CONFIG_H */
