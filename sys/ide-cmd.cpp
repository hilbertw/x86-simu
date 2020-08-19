#define  _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>
#include <string.h>
#include "util.h"
#include "dma.h"
#include "log.h"
#include "ide.h"
#include "irq-wire.h"
#pragma warning(disable : 4309 4101)

#define BUS_MASTER_NOTIFY()

DEVICE_CONFIG_IDENTITY dev_config=
{
	1,//Word 0 shall contain the value 0001h.
	0,
	0,
	0x400000, // 4M x 512=2G
	0,
	0,
	{0},
	0xa5,   //Bits (7:0) of this word shall contain the value A5h.
	~0xe5   //

};
DISK_INFO disk_info=
{
	0x427a,    // 0 general_config;    
	0x3fff,    // 1 cylinders;         
	0xC837,    // 2 reserved;          
	0x0010,    // 3 heads;             
	0x0000,    // 4 bytes_per_track;   
	0x0200,    // 5 bytes_per_sector;   
	0x003f,    // 6 sectors_per_track; 
	{0,0,0},   // 7 vendor_unique[3];  
	"NS0-000-000-00\0\0",    
	           // 10 serial[20];        NS-000-000-000
	0,         // 20 buffer_type;
	0x4000,    // 21 buffer_size;
	0x0041,    // 22 ecc_bytes;
	"0v0.00\0",  
	           // 23 firmware_revision [4]; v00.00
	"iVtrau liDks\0\0",
	           // 27 model_number[40];  Virtual Disk
	0x10,      // 47 multiple_rw;     
	0x80,      //    vendor_unique0;          msb of word47
	0,         // 48 dword_io;           1:support double word io
	0x2f00,    // 49 caps;                 dma, LBA
	0x4001,    // 50 word50;
	
	0,         // 51 pio_timing_mode;     
	0,         //    vendor_unique1;         

	0,         // 52 dma_timing_mode;     
	0,         //    vendor_unique2;        

	0x0007,    // flag_w53; 

	0x3fff,    // 54 number_of_current_cylinders;
	0x0010,    // 55 current_heads;
	0x003f,    // 56 current_sectors_per_track;  0x40
	0xffffffff,   // 57 capacity_in_sectors;
	0x10,      // 59 sectors_in_multi_rw;        0x80
	0x1,       //    multi_sector_setting_valid;

	0xffffffff,// 60 total_sectors;              0x8000
	0,         // 62 single_word_dma_supported;
	0,         //    single_word_dma_active;
	0,         // 63 multi_word_dma_supported;
	0,         //    multi_word_dma_active;
	{0,3,120,120,120},     //word64_to_74;
	0,	     //queue_depth;                 //75
	{0},	 //word76_to_79[79-75];         //76~79
	0x7e,	 //major_version;
	0,	     //minor_version;
	{0},	 //word82_to_87[87-81];         //82~87
	1,		 //ultra_dma_modes;
	{0},	 //word89_to_99[99-88];         //89~99
	0,		 //max_lba_lo;
	0,		 //max_lba_hi;
	{0},	 //word104_to_126[126-103];         //104~126
	0,		 //media_status_notify;            //127
	0,		 //security_status;                //128
	{0},	 //word129_to_175[175-128];        // 129~175
	{0},	 //media_serial_number[205-175];        // 176~205
	{0},	 //word206_to_254[254-205];             // 206~254
	0xa5,	 //signature;
	0,		 //checksum;

};

unsigned short ident_packet[0x100]=
{
0x85C0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//0
0x0000,0x0000,0x0036,0x4244,0x3041,0x4133,0x3838,0x3935,//1
0x3720,0x2020,0x2020,0x2020,0x3453,0x0080,0x0000,0x312E,//2
0x3234,0x2020,0x2020,0x5669,0x7274,0x7561,0x6c20,0x4344,//3
0x2f44,0x5644,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,//4
0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x0000,//5
0x0000,0x0F00,0x0000,0x0200,0x0200,0x0006,0x0000,0x0000,//6
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0007,//7
0x0003,0x0078,0x0078,0x00F0,0x0078,0x0000,0x0000,0x0000,//8
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//9
0x003C,0x0000,0x0218,0x4000,0x4000,0x0218,0x0000,0x4000,//10
0x041F,0x0000,0x0000,0x0000,0x0000,0x603B,0x0000,0x0000,//11
0x0000,0x0000,0x0000,0x0000,0x4002,0x0000,0x0000,0x0000,//12
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//13
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//14
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//15
0x0000,0x0001,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//16
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//17
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//18
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//19
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//20
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//21
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//22
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//23
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//24
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//25
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//26
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//27
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//28
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//29
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//30
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00A5 //31
};

unsigned short ident_ide[0x100]=
{
0x427A,0x3FFF,0xC837,0x0010,0x0000,0x0000,0x003F,0x0000,
0x0000,0x0000,0x2020,0x2020,0x2057,0x442D,0x5743,0x414D,
0x5232,0x3731,0x3139,0x3532,0x0000,0x4000,0x0041,0x3038,
0x2E30,0x354A,0x3038,0x5744,0x4320,0x5744,0x3332,0x3030,
0x4A42,0x2D30,0x304B,0x4641,0x3020,0x2020,0x2020,0x2020,
0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x2020,0x8010,
0x0000,0x2F00,0x4001,0x0000,0x0000,0x0007,0x3FFF,0x0010,
0x003F,0xFC10,0x00FB,0x0110,0xFFFF,0x0FFF,0x0000,0x0007,
0x0003,0x0078,0x0078,0x0078,0x0078,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x007E,0x0000,0x346B,0x7F01,0x4003,0x3468,0x3C01,0x4003,
0x043F,0x0000,0x0000,0x0000,0xFFFE,0x6B00,0x80FE,0x0000,
0x0000,0x0000,0x0000,0x0000,0xEAB0,0x2542,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0009,0x0000,0x0000,0x0000,0x0000,0x125B,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0004,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x001F,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1EA5
};

CONTROLLER_STATE controller[2];

#define USE_IDE_CMD( x) ide_cmd_##x 
#define UNUSE_IDE_CMD( x) ide_cmd_not_implemented 
#define COMMON_NOERR_IDE_CMD( x) ide_cmd_common_noerr

void ide_cmd_common_noerr(int controller);

void (*ide_cmds[0x100])(int controller)=
{
UNUSE_IDE_CMD( nop          ), // IDE_CMD_NOP                    0
USE_IDE_CMD( not_implemented), // IDE_CMD_RECALIB                1 
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2       2
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3       3
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4       4
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5       5
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6       6
USE_IDE_CMD( not_implemented), // IDE_CMD_SEEK                   7
USE_IDE_CMD( device_reset   ), // IDE_CMD_DEVICE_RESET           8
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9       9
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa       0xa
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb       0xb
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc       0xc
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd       0xd
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDe       0xe
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf       0xf

COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB       0x10
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB1      0x11 
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB2      0x12
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB3      0x13
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB4      0x14
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB5      0x15
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB6      0x16
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB7      0x17
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB8      0x18
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIB9      0x19
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIBa      0x1a
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIBb      0x1b
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIBc      0x1c
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIBd      0x1d
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIBe      0x1e
COMMON_NOERR_IDE_CMD( recalib), // IDE_CMD_RECALIBf      0x1f

UNUSE_IDE_CMD( rd_sector_w_retry), // IDE_CMD_READ_SECTOR_W_RETRY    0x20
UNUSE_IDE_CMD( rd_sector_n_retry), // IDE_CMD_READ_SECTOR_WO_RETRY   0x21
UNUSE_IDE_CMD( rd_long_w_retry  ), // IDE_CMD_READ_LONG_W_RETRY      0x22
UNUSE_IDE_CMD( rd_long_n_retry  ), // IDE_CMD_READ_LONG_WO_RETRY     0x23
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED24      0x24
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED25      0x25
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED36      0x26
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED27      0x27
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED28      0x28
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED29      0x29
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2a      0x2a
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2b      0x2b
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2c      0x2c
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2d      0x2d
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2e      0x2e
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2f      0x2f

UNUSE_IDE_CMD( wr_sector_w_retry), // IDE_CMD_WRITE_SECTOR_W_RETRY   0x30
UNUSE_IDE_CMD( wr_sector_n_retry), // IDE_CMD_WRITE_SECTOR_WO_RETRY  0x31
UNUSE_IDE_CMD( wr_long_w_retry  ), // IDE_CMD_WRITE_LONG_W_RETRY     0x32
UNUSE_IDE_CMD( wr_long_n_retry  ), // IDE_CMD_WRITE_LONG_WO_RETRY    0x33
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED34      0x34
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED35      0x35
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED36      0x36
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED37      0x37
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED38      0x38
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED39      0x39
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3a      0x3a
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3b      0x3b
UNUSE_IDE_CMD( write_verify ), // IDE_CMD_WRITE_VERIFY           0x3c
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3d      0x3d
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3e      0x3e
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3f      0x3f

USE_IDE_CMD( rd_vr_sector   ), // IDE_CMD_READ_VERIFY_SECTOR      0x40
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED41       0x41
USE_IDE_CMD( rd_vr_sector_ext), // IDE_CMD_READ_VERIFY_SECTOR_EXT 0x42
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED43      0x43
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED44      0x44
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED45      0x45
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED46      0x46
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED47      0x47
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED48      0x48
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED49      0x49
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4a      0x4a
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4b      0x4b
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4c      0x4c
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4d      0x4d
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4e      0x4e
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4f      0x4f

UNUSE_IDE_CMD( format_track ), // IDE_CMD_FORMAT_TRACK           0x50
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED51      0x51
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED52      0x52
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED53      0x53
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED54      0x54
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED55      0x55
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED56      0x56
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED57      0x57
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED58      0x58
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED59      0x59
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5a      0x5a
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5b      0x5b
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5c      0x5c
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5d      0x5d
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5e      0x5e
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5f      0x5f

USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED60      0x60
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED61      0x61
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED62      0x62
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED63      0x63
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED64      0x64
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED65      0x65
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED66      0x66
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED67      0x67
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED68      0x68
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED69      0x69
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6a      0x6a
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6b      0x6b
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6c      0x6c
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6d      0x6d
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6e      0x6e
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6f      0x6f

COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK       0x70
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK1      0x71
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK2      0x72
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK3      0x73
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK4      0x74
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK5      0x75
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK6      0x76
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK7      0x77
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK8      0x78
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEK9      0x79
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEKa      0x7a
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEKb      0x7b
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEKc      0x7c
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEKd      0x7d
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEKe      0x7e
COMMON_NOERR_IDE_CMD( seek), // IDE_CMD_SEEKf      0x7f
							   
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED80      0x80
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED81      0x81
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED82      0x82
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED83      0x83
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED84      0x84
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED85      0x85
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED86      0x86
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED87      0x87
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED88      0x88
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED89      0x89
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8a      0x8a
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8b      0x8b
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8c      0x8c
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8d      0x8d
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8e      0x8e
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8f      0x8f
							   

UNUSE_IDE_CMD( exec_dev_diag), // IDE_CMD_EXEC_DRIVE_DIAG        0x90
USE_IDE_CMD( init_drv_para  ), // IDE_CMD_INIT_DRIVE_PARAMETERS  0x91
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED92      0x92
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED93      0x93
UNUSE_IDE_CMD( standby_imm  ), // IDE_CMD_STANDBY_IMMEDIATE      0x94
UNUSE_IDE_CMD( idle_imm     ), // IDE_CMD_IDLE_IMMEDIATE         0x95
UNUSE_IDE_CMD( standby      ), // IDE_CMD_STANDBY                0x96
USE_IDE_CMD( idle           ), // IDE_CMD_IDLE                   0x97
USE_IDE_CMD( check_power    ), // IDE_CMD_CHECK_POWER            0x98
UNUSE_IDE_CMD( sleep        ), // IDE_CMD_SLEEP                  0x99
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9a      0x9a
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9b      0x9b
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9c      0x9c
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9d      0x9d
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9e      0x9e
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9f      0x9f

USE_IDE_CMD( packet),            // IDE_CMD_PACKET                 0xa0
USE_IDE_CMD( ident_packet_dev ), // IDE_CMD_IDENTIFY_PACKET_DEVICE 0xa1
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa2      0xa2
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa3      0xa3
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa4      0xa4
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa5      0xa5
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa6      0xa6
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa7      0xa7
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa8      0xa8
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa9      0xa9
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDaa      0xaa
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDab      0xab
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDac      0xac
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDad      0xad
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDae      0xae
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDaf      0xaf

USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb0      0xb0
USE_IDE_CMD( config_set     ), // IDE_CMD_CONFIG_SET             0xb1
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb2      0xb2
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb3      0xb3
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb4      0xb4
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb5      0xb5
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb6      0xb6
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb7      0xb7
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb8      0xb8
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb9      0xb9
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDba      0xba
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbb      0xbb
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbc      0xbc
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbd      0xbd
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbe      0xbe
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbf      0xbf

USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc0      0xc0
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc1      0xc1
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc2      0xc2
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc3      0xc3
USE_IDE_CMD( read_multiple    ), // IDE_CMD_READ_MULTIPLE          0xc4
USE_IDE_CMD( write_multiple   ), // IDE_CMD_WRITE_MULTIPLE         0xc5
USE_IDE_CMD( set_multi_mode   ), // IDE_CMD_SET_MULTI_MODE         0xc6
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc7      0xc7
UNUSE_IDE_CMD( rd_dma_w_retry ), // IDE_CMD_READ_DMA_W_RETRY       0xc8
UNUSE_IDE_CMD( rd_dma_n_retry ), // IDE_CMD_READ_DMA_WO_RETRY      0xc9
UNUSE_IDE_CMD( wr_dma_w_retry ), // IDE_CMD_WRITE_DMA_W_RETRY      0xca
UNUSE_IDE_CMD( wr_dma_n_retry ), // IDE_CMD_WRITE_DMA_WO_RETRY     0xcb
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDcc      0xcc
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDcd      0xcd
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDce      0xce
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDcf      0xcf
							 
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd0      0xd0
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd1      0xd1
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd2      0xd2
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd3      0xd3
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd4      0xd4
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd5      0xd5
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd6      0xd6
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd7      0xd7
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd8      0xd8
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd9      0xd9
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDda      0xda
UNUSE_IDE_CMD( ack_media_change ), // IDE_CMD_ACK_MEDIA_CHANGE       0xdb
UNUSE_IDE_CMD( post_boot        ), // IDE_CMD_POSTBOOT               0xdc
UNUSE_IDE_CMD( ptr_boot         ), // IDE_CMD_PTRBOOT                0xdd
UNUSE_IDE_CMD( door_lock        ), // IDE_CMD_DOOR_LOCK              0xde
UNUSE_IDE_CMD( door_unlock      ), // IDE_CMD_DOOR_UNLOCK            0xdf

UNUSE_IDE_CMD( standby_imm   ), // IDE_CMD_STANDBY_IMMEDIATE1     0xe0
UNUSE_IDE_CMD( idle_imm      ), // IDE_CMD_IDLE_IMMEDIATE1        0xe1
UNUSE_IDE_CMD( standby       ), // IDE_CMD_STANDBY1               0xe2
USE_IDE_CMD( idle          ), // IDE_CMD_IDLE1                  0xe3
UNUSE_IDE_CMD( read_buffer   ), // IDE_CMD_READ_BUFFER            0xe4
USE_IDE_CMD( check_power   ), // IDE_CMD_CHECK_POWER1           0xe5
UNUSE_IDE_CMD( sleep         ), // IDE_CMD_SLEEP1                 0xe6
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDe7      0xe7
UNUSE_IDE_CMD( write_buffer     ), // IDE_CMD_WRITE_BUFFER           0xe8
UNUSE_IDE_CMD( write_same       ), // IDE_CMD_WRITE_SAME             0xe9
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDea      0xea
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDeb      0xeb
USE_IDE_CMD( identify_device  ), // IDE_CMD_IDENTIFY_DEVICE        0xec
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDed      0xed
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDee      0xee
USE_IDE_CMD( set_feature    ), // IDE_CMD_SET_FEATURE            0xef

USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf0      0xf0
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf1      0xf1
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf2      0xf2
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf3      0xf3
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf4      0xf4
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf5      0xf5
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf6      0xf6
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf7      0xf7
USE_IDE_CMD( read_max_n_addr), // IDE_CMD_NOT_IMPLEMENTEDf8      0xf8
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf9      0xf9
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfa      0xfa
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfb      0xfb
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfc      0xfc
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfd      0xfd
USE_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfe      0xfe
USE_IDE_CMD( not_implemented) // IDE_CMD_NOT_IMPLEMENTEDff      0xff


};


#define DEF_IDE_CMD( x) void __cdecl ide_cmd_##x (int controller_no)

DEF_IDE_CMD( nop            )  // IDE_CMD_NOP                    0
{
	//This command always fails with an error.
	controller[controller_no].status_reg=0x51;
	controller[controller_no].cmd_blk.chs_cmd.error=4;
}

//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED1       1 
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2       2
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3       3
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4       4
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5       5
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6       6
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED7       7
DEF_IDE_CMD( device_reset   ) // IDE_CMD_DEVICE_RESET           8
{
	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
	//Use prohibited when the PACKET Command feature set is not implemented.
	//If this command is not supported and the device has the BSY bit and the DRQ bit cleared to
	//zero when the command is written, the device shall respond with command aborted.
	//
	// don't support packet, report  ABORT error
	//
	controller[controller_no].cmd_blk.bytes[2]=1; //1f2
	controller[controller_no].cmd_blk.bytes[3]=1;//1f3
	if(!controller[controller_no].disk[drive].cdrom)
	{
		controller[controller_no].status_reg=0x50;
		controller[controller_no].cmd_blk.bytes[4]=  //1f4
		controller[controller_no].cmd_blk.bytes[5]=0;  //1f5
		controller[controller_no].cmd_blk.bytes[6]&=0x10; //1f6device
	}
	else
	{
		controller[controller_no].status_reg=0x0;
		controller[controller_no].cmd_blk.bytes[1]=0; //1f2
		controller[controller_no].cmd_blk.bytes[4]=0x14;  //1f4
		controller[controller_no].cmd_blk.bytes[5]=0xeb;  //1f5
		controller[controller_no].cmd_blk.bytes[6]&=0x10; //1f6device
	}
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9       9
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa       0xa
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb       0xb
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc       0xc
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd       0xd
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDe       0xe
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf       0xf
//
DEF_IDE_CMD( recalib) // IDE_CMD_RECALIB       0x10
{
	//clearing BSY and generating an interrupt.
	controller[controller_no].status_reg=0x50;
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}

//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB1      0x11 
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB2      0x12
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB3      0x13
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB4      0x14
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB5      0x15
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB6      0x16
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB7      0x17
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB8      0x18
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIB9      0x19
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIBa      0x1a
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIBb      0x1b
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIBc      0x1c
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIBd      0x1d
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIBe      0x1e
//DEF_IDE_CMD( recalib), // IDE_CMD_RECALIBf      0x1f
//
//DEF_IDE_CMD( rd_sector_w_retry) // IDE_CMD_READ_SECTOR_W_RETRY    0x20

//DEF_IDE_CMD( rd_sector_n_retry), // IDE_CMD_READ_SECTOR_WO_RETRY   0x21
//DEF_IDE_CMD( rd_long_w_retry  ), // IDE_CMD_READ_LONG_W_RETRY      0x22
//DEF_IDE_CMD( rd_long_n_retry  ), // IDE_CMD_READ_LONG_WO_RETRY     0x23
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED24      0x24
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED25      0x25
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED36      0x26
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED27      0x27
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED28      0x28
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED29      0x29
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2a      0x2a
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2b      0x2b
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2c      0x2c
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2d      0x2d
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2e      0x2e
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED2f      0x2f
//
//DEF_IDE_CMD( wr_sector_w_retry), // IDE_CMD_WRITE_SECTOR_W_RETRY   0x30
//DEF_IDE_CMD( wr_sector_n_retry), // IDE_CMD_WRITE_SECTOR_WO_RETRY  0x31
//DEF_IDE_CMD( wr_long_w_retry  ), // IDE_CMD_WRITE_LONG_W_RETRY     0x32
//DEF_IDE_CMD( wr_long_n_retry  ), // IDE_CMD_WRITE_LONG_WO_RETRY    0x33
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED34      0x34
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED35      0x35
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED36      0x36
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED37      0x37
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED38      0x38
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED39      0x39
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3a      0x3a
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3b      0x3b
//DEF_IDE_CMD( write_verify     ), // IDE_CMD_WRITE_VERIFY           0x3c
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3d      0x3d
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3e      0x3e
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED3f      0x3f


//READ VERIFY SECTOR
DEF_IDE_CMD( rd_vr_sector) // IDE_CMD_READ_VERIFY_SECTOR_W_RETRY  0x40
{
	controller[controller_no].status_reg=0x50;
	controller[controller_no].n_sectors_to_transfer=0;
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}
//DEF_IDE_CMD( not_implemented), // IDE_CMD_READ_VERIFY_SECTOR_WO_RETRY 0x41
DEF_IDE_CMD( rd_vr_sector_ext  ) // IDE_CMD_VERIFY_LONG_W_RETRY    0x42
{
	controller[controller_no].status_reg=0x50;
	controller[controller_no].n_sectors_to_transfer=0;
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}
//DEF_IDE_CMD( not_implemented  ), // IDE_CMD_VERIFY_LONG_WO_RETRY   0x43
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED44      0x44
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED45      0x45
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED46      0x46
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED47      0x47
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED48      0x48
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED49      0x49
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4a      0x4a
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4b      0x4b
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4c      0x4c
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4d      0x4d
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4e      0x4e
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED4f      0x4f
//
//DEF_IDE_CMD( format_track     ), // IDE_CMD_FORMAT_TRACK           0x50
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED51      0x51
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED52      0x52
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED53      0x53
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED54      0x54
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED55      0x55
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED56      0x56
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED57      0x57
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED58      0x58
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED59      0x59
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5a      0x5a
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5b      0x5b
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5c      0x5c
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5d      0x5d
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5e      0x5e
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED5f      0x5f
//
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED60      0x60
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED61      0x61
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED62      0x62
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED63      0x63
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED64      0x64
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED65      0x65
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED66      0x66
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED67      0x67
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED68      0x68
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED69      0x69
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6a      0x6a
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6b      0x6b
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6c      0x6c
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6d      0x6d
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6e      0x6e
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED6f      0x6f
//
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK       0x70
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK1      0x71
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK2      0x72
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK3      0x73
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK4      0x74
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK5      0x75
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK6      0x76
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK7      0x77
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK8      0x78
//DEF_IDE_CMD( seek), // IDE_CMD_SEEK9      0x79
//DEF_IDE_CMD( seek), // IDE_CMD_SEEKa      0x7a
//DEF_IDE_CMD( seek), // IDE_CMD_SEEKb      0x7b
//DEF_IDE_CMD( seek), // IDE_CMD_SEEKc      0x7c
//DEF_IDE_CMD( seek), // IDE_CMD_SEEKd      0x7d
//DEF_IDE_CMD( seek), // IDE_CMD_SEEKe      0x7e
//DEF_IDE_CMD( seek), // IDE_CMD_SEEKf      0x7f
//							   
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED80      0x80
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED81      0x81
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED82      0x82
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED83      0x83
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED84      0x84
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED85      0x85
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED86      0x86
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED87      0x87
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED88      0x88
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED89      0x89
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8a      0x8a
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8b      0x8b
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8c      0x8c
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8d      0x8d
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8e      0x8e
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED8f      0x8f
//							   
//
//DEF_IDE_CMD( exec_dev_diag  ), // IDE_CMD_EXEC_DRIVE_DIAG        0x90
DEF_IDE_CMD( init_drv_para  ) // IDE_CMD_INIT_DRIVE_PARAMETERS  0x91
{
	//The only two register values used by this command are the Sector Count
	//Register which specifies the number of sectors per track, and the Drive/Head
	//Register which specifies the number of heads minus 1. The DRV bit designates
	//these values to Drive 0 or Drive 1, as appropriate.
	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;

	controller[controller_no].disk[drive].heads=(controller[controller_no].cmd_blk.bytes[6]&0xf)+1;
	controller[controller_no].disk[drive].sectors_per_track=controller[controller_no].cmd_blk.bytes[2];

	//Upon receipt of the command, the drive
	//sets BSY, saves the parameters, clears BSY, and generates an interrupt.
	controller[controller_no].status_reg=0x50;
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}

	log0("set %x sectors per track, %x heads.\n",
		controller[controller_no].disk[drive].sectors_per_track,
		controller[controller_no].disk[drive].heads	);
}
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED92      0x92
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED93      0x93
//DEF_IDE_CMD( standby_imm    ), // IDE_CMD_STANDBY_IMMEDIATE      0x94
//DEF_IDE_CMD( idle_imm       ), // IDE_CMD_IDLE_IMMEDIATE         0x95
//DEF_IDE_CMD( standby        ), // IDE_CMD_STANDBY                0x96
DEF_IDE_CMD( idle           ) // IDE_CMD_IDLE                   0x97
{
		//The interrupt is generated even though the drive may
		//not have fully transitioned to Idle Mode.
		if(controller[controller_no].device_control.nIEN==0)
		{
			assert_irq(0xe+controller_no);
			BUS_MASTER_NOTIFY();
		}
}
DEF_IDE_CMD( check_power    ) // IDE_CMD_CHECK_POWER            0x98
{
	//This command checks the power mode.
	//If the drive is in, going to, or recovering from the Standby Mode the drive
	//shall set BSY, set the Sector Count Register to 00h, clear BSY, and generate
	//an interrupt.
	//If the drive is in the Idle Mode, the drive shall set BSY, set the Sector
	//Count Register to FFh, clear BSY, and generate an interrupt.

		controller[controller_no].status_reg=0x50;
		controller[controller_no].cmd_blk.chs_cmd.sector_count=0xff;

		if(controller[controller_no].device_control.nIEN==0)
		{
			assert_irq(0xe+controller_no);
			BUS_MASTER_NOTIFY();
		}
}
//DEF_IDE_CMD( sleep          ), // IDE_CMD_SLEEP                  0x99
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9a      0x9a
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9b      0x9b
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9c      0x9c
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9d      0x9d
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9e      0x9e
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTED9f      0x9f
//
DEF_IDE_CMD( packet) // IDE_CMD_NOT_IMPLEMENTEDa0      0xa0
{
	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
	//
	// don't support packet, report  ABORT error
	//
	if(controller[controller_no].disk[drive].cdrom)
	{
	controller[controller_no].dwByteCountlimit=
		*(WORD *)&controller[controller_no].cmd_blk.bytes[4];


	controller[controller_no].cmd_blk.bytes[1]=0; //1f1
	controller[controller_no].cmd_blk.bytes[2]=1; //1f2 tag,rel,i/o,c/d
	//controller[controller_no].cmd_blk.bytes[3]=  //1f3
	//controller[controller_no].cmd_blk.bytes[4]=  //1f4
	//controller[controller_no].cmd_blk.bytes[5]=  //1f5
	//controller[controller_no].cmd_blk.bytes[6]   //1f6

	//SDRQ=1,recv command by pio
	controller[controller_no].status_reg=0x8; 
	controller[controller_no].buffer_ptr=0;
	//12 byte command packet,word0 of identify_device
	controller[controller_no].pio_type=COMMAND_PACKET;
	}
	else
	{
		controller[controller_no].status_reg=0x51;
		controller[controller_no].cmd_blk.chs_cmd.error=4;
	}

	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}
DEF_IDE_CMD( ident_packet_dev ) // IDE_CMD_IDENTIFY_PACKET_DEVICE 0xa1
{
	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
	//
	// don't support packet, report  ABORT error
	//
	if(!controller[controller_no].disk[drive].cdrom)
	{
		controller[controller_no].status_reg=0x51;
		controller[controller_no].cmd_blk.chs_cmd.error=4;
	}
	else
	{
		controller[controller_no].status_reg=0x58;
		memcpy(controller[controller_no].buffer.bytes,(unsigned char *)ident_packet,sizeof(ident_packet));
		controller[controller_no].buffer.bytes[510]=calc_chksum(ident_packet);

		//controller[controller_no].n_sectors_to_transfer=1;
		controller[controller_no].buffer_ptr=0;
		controller[controller_no].pio_type=PIO_DEVDATA_IN;
	}
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa2      0xa2
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa3      0xa3
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa4      0xa4
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa5      0xa5
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa6      0xa6
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa7      0xa7
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa8      0xa8
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDa9      0xa9
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDaa      0xaa
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDab      0xab
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDac      0xac
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDad      0xad
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDae      0xae
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDaf      0xaf
//
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb0      0xb0
DEF_IDE_CMD( config_set       ) // IDE_CMD_CONFIG_SET             0xb1
{
	switch(controller[controller_no].cmd_blk.bytes[1])// feature
	{
	case 0xc2:
		break;
	case 0xc3://DEVICE CONFIGURATION SET
		//
		// don't support  report  ABORT error
		//

		controller[controller_no].status_reg=0x51;
		controller[controller_no].cmd_blk.chs_cmd.error=4;
		break;
	}
}
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb2      0xb2
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb3      0xb3
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb4      0xb4
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb5      0xb5
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb6      0xb6
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb7      0xb7
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb8      0xb8
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDb9      0xb9
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDba      0xba
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbb      0xbb
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbc      0xbc
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbd      0xbd
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbe      0xbe
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDbf      0xbf
//
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc0      0xc0
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc1      0xc1
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc2      0xc2
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc3      0xc3
DEF_IDE_CMD( read_multiple    ) // IDE_CMD_READ_MULTIPLE          0xc4
{
	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
	unsigned long lba;
	if(controller[controller_no].disk[drive].multi_io==0) // multi  disabled
	{
		controller[controller_no].status_reg=0x51;
	}
	else if(controller[controller_no].cmd_blk.bytes[6]&0x40) // a lba
	{
		//Sector Count -number of sectors to be transferred. 
		//A value of 00h specifies that 256 sectors are to be transferred.
		//LBA Low -starting LBA bits (7:0).
		//LBA Mid -starting LBA bits (15:8).
		//LBA High -starting LBA bits (23:16).
		//Device -DEV shall specify the selected device.
		//bits (3:0) starting LBA bits (27:24).

		lba=*(unsigned long *)(controller[controller_no].cmd_blk.bytes+3)&0xfffffff;  

l_read_lba:
		int sc=controller[controller_no].cmd_blk.lba_cmd.sector_count;
		if(sc==0)sc=0x100;
		controller[controller_no].status_reg=0x58;
		controller[controller_no].n_sectors_to_transfer=sc; // number of words;
		controller[controller_no].n_sectors_transferred_within_blk=
		controller[controller_no].buffer_ptr=0;
		controller[controller_no].pio_type=PIO_USERDATA_IN;
		//	
		// seek
		//

		unsigned __int64 pos=lba<<9;
		assert(controller[controller_no].disk[drive].fpimg);
		_fseeki64(controller[controller_no].disk[drive].fpimg,pos,0);

		log0("Read %x sectors ,LBA:%x of Drive%d:%d,pos:%08x%08x\n",sc,lba,controller_no,drive,
			(unsigned long)(pos>>32),
			(unsigned long)pos);

	}
	else   // chs
	{
		unsigned int h=(controller[controller_no].cmd_blk.bytes[6]&0xf);
		unsigned int c=*(unsigned short *)&controller[controller_no].cmd_blk.bytes[4];

		lba=controller[controller_no].cmd_blk.bytes[3]-1
			+(c*controller[controller_no].disk[drive].heads+h)*controller[controller_no].disk[drive].sectors_per_track;
		goto l_read_lba;

	}
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}

DEF_IDE_CMD( write_multiple   ) // IDE_CMD_WRITE_MULTIPLE         0xc5
{
	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
	unsigned long lba;

	if(controller[controller_no].disk[drive].multi_io==0) // multi  disabled
	{
		controller[controller_no].status_reg=0x51;
	}
	else if(controller[controller_no].cmd_blk.bytes[6]&0x40) // a lba
	{

		//Sector Count -number of sectors to be transferred. 
		//A value of 00h specifies that 256 sectors are to be transferred.
		//LBA Low -starting LBA bits (7:0).
		//LBA Mid -starting LBA bits (15:8).
		//LBA High -starting LBA bits (23:16).
		//Device -DEV shall specify the selected device.
		//bits (3:0) starting LBA bits (27:24).

		lba=*(unsigned long *)(controller[controller_no].cmd_blk.bytes+3)&0xfffffff;  
l_write_lba:
		int sc=controller[controller_no].cmd_blk.lba_cmd.sector_count;
		if(sc==0)sc=0x100;
		controller[controller_no].status_reg=0x58;
		controller[controller_no].n_sectors_to_transfer=sc; // number of words;
		controller[controller_no].n_sectors_transferred_within_blk=
		controller[controller_no].buffer_ptr=0;
		controller[controller_no].pio_type=PIO_USERDATA_OUT;
		//	
		// seek
		//
		int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
		unsigned __int64 pos=lba<<9;
		assert(controller[controller_no].disk[drive].fpimg);
		_fseeki64(controller[controller_no].disk[drive].fpimg,pos,0);

		log0("Write %x sectors,LBA:%x of Drive%d:%d\n",sc,lba,controller_no,drive);

	}
	else
	{
		unsigned int h=(controller[controller_no].cmd_blk.bytes[6]&0xf);
		unsigned int c=*(unsigned short *)&controller[controller_no].cmd_blk.bytes[4];

		lba=controller[controller_no].cmd_blk.bytes[3]-1
			+(c*controller[controller_no].disk[drive].heads+h)*controller[controller_no].disk[drive].sectors_per_track;
		goto l_write_lba;
	}
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}
DEF_IDE_CMD( set_multi_mode   ) // IDE_CMD_SET_MULTI_MODE         0xc6
{
		int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
		controller[controller_no].status_reg=0x50;
		controller[controller_no].disk[drive].multi_io=controller[controller_no].cmd_blk.chs_cmd.sector_count;
		if(controller[controller_no].device_control.nIEN==0)
		{
			assert_irq(0xe+controller_no);
			BUS_MASTER_NOTIFY();
		}

		log0("set multi-io:%x of Drive%d:%d\n",controller[controller_no].disk[drive].multi_io,controller_no,drive);
}
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDc7      0xc7
DEF_IDE_CMD( rd_dma_w_retry ) // IDE_CMD_READ_DMA_W_RETRY       0xc8
{
}
DEF_IDE_CMD( rd_dma_n_retry ) // IDE_CMD_READ_DMA_WO_RETRY      0xc9
{
}
DEF_IDE_CMD( wr_dma_w_retry ) // IDE_CMD_WRITE_DMA_W_RETRY      0xca
{
}
DEF_IDE_CMD( wr_dma_n_retry ) // IDE_CMD_WRITE_DMA_WO_RETRY     0xcb
{
}
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDcc      0xcc
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDcd      0xcd
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDce      0xce
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDcf      0xcf
//							 
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd0      0xd0
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd1      0xd1
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd2      0xd2
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd3      0xd3
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd4      0xd4
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd5      0xd5
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd6      0xd6
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd7      0xd7
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd8      0xd8
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDd9      0xd9
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDda      0xda
//DEF_IDE_CMD( ack_media_change ), // IDE_CMD_ACK_MEDIA_CHANGE       0xdb
//DEF_IDE_CMD( post_boot        ), // IDE_CMD_POSTBOOT               0xdc
//DEF_IDE_CMD( ptr_boot         ), // IDE_CMD_PTRBOOT                0xdd
//DEF_IDE_CMD( door_lock        ), // IDE_CMD_DOOR_LOCK              0xde
//DEF_IDE_CMD( door_unlock      ), // IDE_CMD_DOOR_UNLOCK            0xdf
//
//DEF_IDE_CMD( standby_imm1   ), // IDE_CMD_STANDBY_IMMEDIATE1     0xe0
//DEF_IDE_CMD( idle_imm1      ), // IDE_CMD_IDLE_IMMEDIATE1        0xe1
//DEF_IDE_CMD( standby1       ), // IDE_CMD_STANDBY1               0xe2
//DEF_IDE_CMD( idle1          ), // IDE_CMD_IDLE1                  0xe3
//DEF_IDE_CMD( read_buffer    ), // IDE_CMD_READ_BUFFER            0xe4
//DEF_IDE_CMD( check_power1   ), // IDE_CMD_CHECK_POWER1           0xe5
//DEF_IDE_CMD( sleep1         ), // IDE_CMD_SLEEP1                 0xe6
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDe7      0xe7
//DEF_IDE_CMD( write_buffer   ), // IDE_CMD_WRITE_BUFFER           0xe8
//DEF_IDE_CMD( write_same     ), // IDE_CMD_WRITE_SAME             0xe9
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDea      0xea
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDeb      0xeb
DEF_IDE_CMD( identify_device) // IDE_CMD_IDENTIFY_DEVICE        0xec
{
	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
	//
	//  DRQ=1,BSY=0
	//

	if(!controller[controller_no].disk[drive].cdrom)
	{
		controller[controller_no].status_reg=0x58;

		disk_info.max_lba_lo=
		disk_info.total_sectors=
		disk_info.capacity_in_sectors=controller[controller_no].disk[drive].sectors_lo;
		disk_info.max_lba_hi=controller[controller_no].disk[drive].sectors_hi;

		disk_info.cylinders=controller[controller_no].disk[drive].n_cylinders;
		disk_info.number_of_current_cylinders=controller[controller_no].disk[drive].n_cylinders;
		memcpy(controller[controller_no].buffer.bytes,(unsigned char *)&disk_info,0x200);

		//controller[controller_no].n_sectors_to_transfer=1;
		controller[controller_no].buffer_ptr=0;
		controller[controller_no].pio_type=PIO_DEVDATA_IN;
	}
	else
	{
		//In response to this command, devices that implement the PACKET Command feature set shall post command
		//aborted and place the PACKET Command feature set signature in the Command Block registers (see 9.12).
		controller[controller_no].status_reg=0x51;
		controller[controller_no].cmd_blk.chs_cmd.error=4;
		controller[controller_no].cmd_blk.bytes[2]=1; //1f2
		controller[controller_no].cmd_blk.bytes[3]=1;//1f3
		controller[controller_no].cmd_blk.bytes[4]=0x14;  //1f4
		controller[controller_no].cmd_blk.bytes[5]=0xeb;  //1f5
		controller[controller_no].cmd_blk.bytes[6]&=0x10; //1f6device
	}

	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}

//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDed      0xed
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDee      0xee

DEF_IDE_CMD( set_feature    ) // IDE_CMD_SET_FEATURE            0xef
{
	//Table 44 - SET FEATURES register definitions
	//Value
	//01h Enable 8-bit PIO transfer mode (CFA feature set only)
	//02h Enable write cache
	//03h Set transfer mode based on value in Sector Count register. Table 45 defines values.
	//04h Obsolete
	//05h Enable advanced power management
	//06h Enable Power-Up In Standby feature set.
	//07h Power-Up In Standby feature set device spin-up.
	//09h Reserved for Address offset reserved area boot method technical report
	//0Ah Enable CFA power mode 1
	//31h Disable Media Status Notification
	//33h Obsolete
	//42h Enable Automatic Acoustic Management feature set
	//44h Obsolete
	//54h Obsolete
	//55h Disable read look-ahead feature
	//5Dh Enable release interrupt
	//5Eh Enable SERVICE interrupt
	//66h Disable reverting to power-on defaults
	//77h Obsolete
	//81h Disable 8-bit PIO transfer mode (CFA feature set only)
	//82h Disable write cache
	//84h Obsolete
	//85h Disable advanced power management
	//86h Disable Power-Up In Standby feature set.
	//88h Obsolete
	//89h Reserved for Address offset reserved area boot method technical report
	//8Ah Disable CFA power mode 1
	//95h Enable Media Status Notification
	//99h Obsolete
	//9Ah Obsolete
	//Aah Enable read look-ahead feature
	//Abh Obsolete
	//BBh Obsolete
	//C2h Disable Automatic Acoustic Management feature set
	//CCh Enable reverting to power-on defaults
	//DDh Disable release interrupt
	//DEh Disable SERVICE interrupt
	//F0h-FFh Reserved for assignment by the CompactFlash Association

	// If the value in the register is not supported or is
	//invalid, the drive posts an Aborted Command error.
	//c) The drive sets BSY.
		controller[controller_no].status_reg=0x50;


	//d) When the drive has completed processing, it clears BSY and assertsINTRQ.
		if(controller[controller_no].device_control.nIEN==0)
		{
			assert_irq(0xe+controller_no);
			BUS_MASTER_NOTIFY();
		}
}

//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf0      0xf0
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf1      0xf1
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf2      0xf2
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf3      0xf3
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf4      0xf4
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf5      0xf5
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf6      0xf6
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf7      0xf7
DEF_IDE_CMD( read_max_n_addr) // IDE_CMD_NOT_IMPLEMENTEDf8      0xf8
{
	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;

	// the LBA bit shall be set to one to specify the address is an LBA.
	if(controller[controller_no].cmd_blk.bytes[6]&0x40) 
	{
		controller[controller_no].cmd_blk.bytes[3]=controller[controller_no].disk[drive].sectors_lo;
		controller[controller_no].cmd_blk.bytes[4]=controller[controller_no].disk[drive].sectors_lo>>8;
		controller[controller_no].cmd_blk.bytes[5]=controller[controller_no].disk[drive].sectors_lo>>16;
		controller[controller_no].cmd_blk.bytes[6]=
			(controller[controller_no].disk[drive].sectors_lo>>24)&0xf+(drive&~0xf);

		controller[controller_no].status_reg=0x50;

	}
	else
	{
		controller[controller_no].status_reg=0x51;
		controller[controller_no].cmd_blk.bytes[1]=4;//abort
	}

	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}


	log0("max address:%02x%02x%02x%02x\n",
		controller[controller_no].cmd_blk.bytes[6],
		controller[controller_no].cmd_blk.bytes[5],
		controller[controller_no].cmd_blk.bytes[4],
		controller[controller_no].cmd_blk.bytes[3]);
}
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDf9      0xf9
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfa      0xfa
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfb      0xfb
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfc      0xfc
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfd      0xfd
//DEF_IDE_CMD( not_implemented), // IDE_CMD_NOT_IMPLEMENTEDfe      0xfe
//DEF_IDE_CMD( not_implemented) // IDE_CMD_NOT_IMPLEMENTEDff      0xff




DEF_IDE_CMD( not_implemented)
{
	log0(" IDE command %02x not implemented.\n",controller[controller_no].cmd_blk.bytes[7]);

	controller[controller_no].status_reg=0x51;
	controller[controller_no].cmd_blk.chs_cmd.error=4;

	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}

DEF_IDE_CMD( common_noerr)
{
	controller[controller_no].status_reg=0x50;
	if(controller[controller_no].device_control.nIEN==0)
	{
		assert_irq(0xe+controller_no);
		BUS_MASTER_NOTIFY();
	}
}

unsigned char calc_chksum(unsigned short * d)
{
	unsigned short x=0;
	for(int i=0;i<0x100;i++) x+=d[i];

	x+=(x<<8);
	return ~x;
}

