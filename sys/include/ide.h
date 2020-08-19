#ifndef __ide_h__
#define __ide_h__
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif

#define IDE_CMD_NOP                    0
#define IDE_CMD_NOT_IMPLEMENTED1       1 
#define IDE_CMD_NOT_IMPLEMENTED2       2
#define IDE_CMD_NOT_IMPLEMENTED3       3
#define IDE_CMD_NOT_IMPLEMENTED4       4
#define IDE_CMD_NOT_IMPLEMENTED5       5
#define IDE_CMD_NOT_IMPLEMENTED6       6
#define IDE_CMD_NOT_IMPLEMENTED7       7
#define IDE_CMD_DEVICE_RESET           8
#define IDE_CMD_NOT_IMPLEMENTED9       9
#define IDE_CMD_NOT_IMPLEMENTEDa       0xa
#define IDE_CMD_NOT_IMPLEMENTEDb       0xb
#define IDE_CMD_NOT_IMPLEMENTEDc       0xc
#define IDE_CMD_NOT_IMPLEMENTEDd       0xd
#define IDE_CMD_NOT_IMPLEMENTEDe       0xe
#define IDE_CMD_NOT_IMPLEMENTEDf       0xf

#define IDE_CMD_RECALIB       0x10
#define IDE_CMD_RECALIB1      0x11 
#define IDE_CMD_RECALIB2      0x12
#define IDE_CMD_RECALIB3      0x13
#define IDE_CMD_RECALIB4      0x14
#define IDE_CMD_RECALIB5      0x15
#define IDE_CMD_RECALIB6      0x16
#define IDE_CMD_RECALIB7      0x17
#define IDE_CMD_RECALIB8      0x18
#define IDE_CMD_RECALIB9      0x19
#define IDE_CMD_RECALIBa      0x1a
#define IDE_CMD_RECALIBb      0x1b
#define IDE_CMD_RECALIBc      0x1c
#define IDE_CMD_RECALIBd      0x1d
#define IDE_CMD_RECALIBe      0x1e
#define IDE_CMD_RECALIBf      0x1f

#define IDE_CMD_READ_SECTOR_W_RETRY    0x20
#define IDE_CMD_READ_SECTOR_WO_RETRY   0x21
#define IDE_CMD_READ_LONG_W_RETRY      0x22
#define IDE_CMD_READ_LONG_WO_RETRY     0x23
#define IDE_CMD_NOT_IMPLEMENTED24      0x24
#define IDE_CMD_NOT_IMPLEMENTED25      0x25
#define IDE_CMD_NOT_IMPLEMENTED26      0x26
#define IDE_CMD_NOT_IMPLEMENTED27      0x27
#define IDE_CMD_NOT_IMPLEMENTED28      0x28
#define IDE_CMD_NOT_IMPLEMENTED29      0x29
#define IDE_CMD_NOT_IMPLEMENTED2a      0x2a
#define IDE_CMD_NOT_IMPLEMENTED2b      0x2b
#define IDE_CMD_NOT_IMPLEMENTED2c      0x2c
#define IDE_CMD_NOT_IMPLEMENTED2d      0x2d
#define IDE_CMD_NOT_IMPLEMENTED2e      0x2e
#define IDE_CMD_NOT_IMPLEMENTED2f      0x2f

#define IDE_CMD_WRITE_SECTOR_W_RETRY   0x30
#define IDE_CMD_WRITE_SECTOR_WO_RETRY  0x31
#define IDE_CMD_WRITE_LONG_W_RETRY     0x32
#define IDE_CMD_WRITE_LONG_WO_RETRY    0x33
#define IDE_CMD_NOT_IMPLEMENTED34      0x34
#define IDE_CMD_NOT_IMPLEMENTED35      0x35
#define IDE_CMD_NOT_IMPLEMENTED36      0x36
#define IDE_CMD_NOT_IMPLEMENTED37      0x37
#define IDE_CMD_NOT_IMPLEMENTED38      0x38
#define IDE_CMD_NOT_IMPLEMENTED39      0x39
#define IDE_CMD_NOT_IMPLEMENTED3a      0x3a
#define IDE_CMD_NOT_IMPLEMENTED3b      0x3b
#define IDE_CMD_WRITE_VERIFY           0x3c
#define IDE_CMD_NOT_IMPLEMENTED3d      0x3d
#define IDE_CMD_NOT_IMPLEMENTED3e      0x3e
#define IDE_CMD_NOT_IMPLEMENTED3f      0x3f

#define IDE_CMD_VERIFY_SECTOR         0x40
#define IDE_CMD_NOT_IMPLEMENTED41     0x41
#define IDE_CMD_VERIFY_SECTOR_EXT     0x42
#define IDE_CMD_NOT_IMPLEMENTED43     0x43
#define IDE_CMD_NOT_IMPLEMENTED44      0x44
#define IDE_CMD_NOT_IMPLEMENTED45      0x45
#define IDE_CMD_NOT_IMPLEMENTED46      0x46
#define IDE_CMD_NOT_IMPLEMENTED47      0x47
#define IDE_CMD_NOT_IMPLEMENTED48      0x48
#define IDE_CMD_NOT_IMPLEMENTED49      0x49
#define IDE_CMD_NOT_IMPLEMENTED4a      0x4a
#define IDE_CMD_NOT_IMPLEMENTED4b      0x4b
#define IDE_CMD_NOT_IMPLEMENTED4c      0x4c
#define IDE_CMD_NOT_IMPLEMENTED4d      0x4d
#define IDE_CMD_NOT_IMPLEMENTED4e      0x4e
#define IDE_CMD_NOT_IMPLEMENTED4f      0x4f

#define IDE_CMD_FORMAT_TRACK           0x50
#define IDE_CMD_NOT_IMPLEMENTED51      0x51
#define IDE_CMD_NOT_IMPLEMENTED52      0x52
#define IDE_CMD_NOT_IMPLEMENTED53      0x53
#define IDE_CMD_NOT_IMPLEMENTED54      0x54
#define IDE_CMD_NOT_IMPLEMENTED55      0x55
#define IDE_CMD_NOT_IMPLEMENTED56      0x56
#define IDE_CMD_NOT_IMPLEMENTED57      0x57
#define IDE_CMD_NOT_IMPLEMENTED58      0x58
#define IDE_CMD_NOT_IMPLEMENTED59      0x59
#define IDE_CMD_NOT_IMPLEMENTED5a      0x5a
#define IDE_CMD_NOT_IMPLEMENTED5b      0x5b
#define IDE_CMD_NOT_IMPLEMENTED5c      0x5c
#define IDE_CMD_NOT_IMPLEMENTED5d      0x5d
#define IDE_CMD_NOT_IMPLEMENTED5e      0x5e
#define IDE_CMD_NOT_IMPLEMENTED5f      0x5f

#define IDE_CMD_NOT_IMPLEMENTED60      0x60
#define IDE_CMD_NOT_IMPLEMENTED61      0x61
#define IDE_CMD_NOT_IMPLEMENTED62      0x62
#define IDE_CMD_NOT_IMPLEMENTED63      0x63
#define IDE_CMD_NOT_IMPLEMENTED64      0x64
#define IDE_CMD_NOT_IMPLEMENTED65      0x65
#define IDE_CMD_NOT_IMPLEMENTED66      0x66
#define IDE_CMD_NOT_IMPLEMENTED67      0x67
#define IDE_CMD_NOT_IMPLEMENTED68      0x68
#define IDE_CMD_NOT_IMPLEMENTED69      0x69
#define IDE_CMD_NOT_IMPLEMENTED6a      0x6a
#define IDE_CMD_NOT_IMPLEMENTED6b      0x6b
#define IDE_CMD_NOT_IMPLEMENTED6c      0x6c
#define IDE_CMD_NOT_IMPLEMENTED6d      0x6d
#define IDE_CMD_NOT_IMPLEMENTED6e      0x6e
#define IDE_CMD_NOT_IMPLEMENTED6f      0x6f

#define IDE_CMD_SEEK                   0x70
#define IDE_CMD_SEEK1      0x71
#define IDE_CMD_SEEK2      0x72
#define IDE_CMD_SEEK3      0x73
#define IDE_CMD_SEEK4      0x74
#define IDE_CMD_SEEK5      0x75
#define IDE_CMD_SEEK6      0x76
#define IDE_CMD_SEEK7      0x77
#define IDE_CMD_SEEK8      0x78
#define IDE_CMD_SEEK9      0x79
#define IDE_CMD_SEEKa      0x7a
#define IDE_CMD_SEEKb      0x7b
#define IDE_CMD_SEEKc      0x7c
#define IDE_CMD_SEEKd      0x7d
#define IDE_CMD_SEEKe      0x7e
#define IDE_CMD_SEEKf      0x7f
							   
#define IDE_CMD_NOT_IMPLEMENTED80      0x80
#define IDE_CMD_NOT_IMPLEMENTED81      0x81
#define IDE_CMD_NOT_IMPLEMENTED82      0x82
#define IDE_CMD_NOT_IMPLEMENTED83      0x83
#define IDE_CMD_NOT_IMPLEMENTED84      0x84
#define IDE_CMD_NOT_IMPLEMENTED85      0x85
#define IDE_CMD_NOT_IMPLEMENTED86      0x86
#define IDE_CMD_NOT_IMPLEMENTED87      0x87
#define IDE_CMD_NOT_IMPLEMENTED88      0x88
#define IDE_CMD_NOT_IMPLEMENTED89      0x89
#define IDE_CMD_NOT_IMPLEMENTED8a      0x8a
#define IDE_CMD_NOT_IMPLEMENTED8b      0x8b
#define IDE_CMD_NOT_IMPLEMENTED8c      0x8c
#define IDE_CMD_NOT_IMPLEMENTED8d      0x8d
#define IDE_CMD_NOT_IMPLEMENTED8e      0x8e
#define IDE_CMD_NOT_IMPLEMENTED8f      0x8f
							   

#define IDE_CMD_EXEC_DRIVE_DIAG        0x90
#define IDE_CMD_INIT_DRIVE_PARAMETERS  0x91
#define IDE_CMD_NOT_IMPLEMENTED92      0x92
#define IDE_CMD_NOT_IMPLEMENTED93      0x93
#define IDE_CMD_STANDBY_IMMEDIATE      0x94
#define IDE_CMD_IDLE_IMMEDIATE         0x95
#define IDE_CMD_STANDBY                0x96
#define IDE_CMD_IDLE                   0x97
#define IDE_CMD_CHECK_POWER            0x98
#define IDE_CMD_SLEEP                  0x99
#define IDE_CMD_VENDOR                 0x9a
#define IDE_CMD_NOT_IMPLEMENTED9b      0x9b
#define IDE_CMD_NOT_IMPLEMENTED9c      0x9c
#define IDE_CMD_NOT_IMPLEMENTED9d      0x9d
#define IDE_CMD_NOT_IMPLEMENTED9e      0x9e
#define IDE_CMD_NOT_IMPLEMENTED9f      0x9f

#define IDE_CMD_PACKET                 0xa0
#define IDE_CMD_IDENTIFY_PACKET_DEVICE 0xa1
#define IDE_CMD_NOT_IMPLEMENTEDa2      0xa2
#define IDE_CMD_NOT_IMPLEMENTEDa3      0xa3
#define IDE_CMD_NOT_IMPLEMENTEDa4      0xa4
#define IDE_CMD_NOT_IMPLEMENTEDa5      0xa5
#define IDE_CMD_NOT_IMPLEMENTEDa6      0xa6
#define IDE_CMD_NOT_IMPLEMENTEDa7      0xa7
#define IDE_CMD_NOT_IMPLEMENTEDa8      0xa8
#define IDE_CMD_NOT_IMPLEMENTEDa9      0xa9
#define IDE_CMD_NOT_IMPLEMENTEDaa      0xaa
#define IDE_CMD_NOT_IMPLEMENTEDab      0xab
#define IDE_CMD_NOT_IMPLEMENTEDac      0xac
#define IDE_CMD_NOT_IMPLEMENTEDad      0xad
#define IDE_CMD_NOT_IMPLEMENTEDae      0xae
#define IDE_CMD_NOT_IMPLEMENTEDaf      0xaf

#define IDE_CMD_NOT_IMPLEMENTEDb0      0xb0
#define IDE_CMD_CONFIG_SET             0xb1
#define IDE_CMD_NOT_IMPLEMENTEDb2      0xb2
#define IDE_CMD_NOT_IMPLEMENTEDb3      0xb3
#define IDE_CMD_NOT_IMPLEMENTEDb4      0xb4
#define IDE_CMD_NOT_IMPLEMENTEDb5      0xb5
#define IDE_CMD_NOT_IMPLEMENTEDb6      0xb6
#define IDE_CMD_NOT_IMPLEMENTEDb7      0xb7
#define IDE_CMD_NOT_IMPLEMENTEDb8      0xb8
#define IDE_CMD_NOT_IMPLEMENTEDb9      0xb9
#define IDE_CMD_NOT_IMPLEMENTEDba      0xba
#define IDE_CMD_NOT_IMPLEMENTEDbb      0xbb
#define IDE_CMD_NOT_IMPLEMENTEDbc      0xbc
#define IDE_CMD_NOT_IMPLEMENTEDbd      0xbd
#define IDE_CMD_NOT_IMPLEMENTEDbe      0xbe
#define IDE_CMD_NOT_IMPLEMENTEDbf      0xbf

#define IDE_CMD_NOT_IMPLEMENTEDc0      0xc0
#define IDE_CMD_NOT_IMPLEMENTEDc1      0xc1
#define IDE_CMD_NOT_IMPLEMENTEDc2      0xc2
#define IDE_CMD_NOT_IMPLEMENTEDc3      0xc3
#define IDE_CMD_READ_MULTIPLE          0xc4
#define IDE_CMD_WRITE_MULTIPLE         0xc5
#define IDE_CMD_SET_MULTI_MODE         0xc6
#define IDE_CMD_NOT_IMPLEMENTEDc7      0xc7
#define IDE_CMD_READ_DMA_W_RETRY       0xc8
#define IDE_CMD_READ_DMA_WO_RETRY      0xc9
#define IDE_CMD_WRITE_DMA_W_RETRY      0xca
#define IDE_CMD_WRITE_DMA_WO_RETRY     0xcb
#define IDE_CMD_NOT_IMPLEMENTEDcc      0xcc
#define IDE_CMD_NOT_IMPLEMENTEDcd      0xcd
#define IDE_CMD_NOT_IMPLEMENTEDce      0xce
#define IDE_CMD_NOT_IMPLEMENTEDcf      0xcf

#define IDE_CMD_NOT_IMPLEMENTEDd0      0xd0
#define IDE_CMD_NOT_IMPLEMENTEDd1      0xd1
#define IDE_CMD_NOT_IMPLEMENTEDd2      0xd2
#define IDE_CMD_NOT_IMPLEMENTEDd3      0xd3
#define IDE_CMD_NOT_IMPLEMENTEDd4      0xd4
#define IDE_CMD_NOT_IMPLEMENTEDd5      0xd5
#define IDE_CMD_NOT_IMPLEMENTEDd6      0xd6
#define IDE_CMD_NOT_IMPLEMENTEDd7      0xd7
#define IDE_CMD_NOT_IMPLEMENTEDd8      0xd8
#define IDE_CMD_NOT_IMPLEMENTEDd9      0xd9
#define IDE_CMD_NOT_IMPLEMENTEDda      0xda
#define IDE_CMD_ACK_MEDIA_CHANGE       0xdb
#define IDE_CMD_POSTBOOT               0xdc
#define IDE_CMD_PTRBOOT                0xdd
#define IDE_CMD_DOOR_LOCK              0xde
#define IDE_CMD_DOOR_UNLOCK            0xdf

#define IDE_CMD_STANDBY_IMMEDIATE1     0xe0
#define IDE_CMD_IDLE_IMMEDIATE1        0xe1
#define IDE_CMD_STANDBY1               0xe2
#define IDE_CMD_IDLE1                  0xe3
#define IDE_CMD_READ_BUFFER            0xe4
#define IDE_CMD_CHECK_POWER1           0xe5
#define IDE_CMD_SLEEP1                 0xe6
#define IDE_CMD_NOT_IMPLEMENTEDe7      0xe7
#define IDE_CMD_WRITE_BUFFER           0xe8
#define IDE_CMD_WRITE_SAME             0xe9
#define IDE_CMD_NOT_IMPLEMENTEDea      0xea
#define IDE_CMD_NOT_IMPLEMENTEDeb      0xeb
#define IDE_CMD_IDENTIFY_DEVICE        0xec
#define IDE_CMD_NOT_IMPLEMENTEDed      0xed
#define IDE_CMD_NOT_IMPLEMENTEDee      0xee
#define IDE_CMD_SET_FEATURE            0xef

#define IDE_CMD_NOT_IMPLEMENTEDf0      0xf0
#define IDE_CMD_NOT_IMPLEMENTEDf1      0xf1
#define IDE_CMD_NOT_IMPLEMENTEDf2      0xf2
#define IDE_CMD_NOT_IMPLEMENTEDf3      0xf3
#define IDE_CMD_NOT_IMPLEMENTEDf4      0xf4
#define IDE_CMD_NOT_IMPLEMENTEDf5      0xf5
#define IDE_CMD_NOT_IMPLEMENTEDf6      0xf6
#define IDE_CMD_NOT_IMPLEMENTEDf7      0xf7
#define IDE_CMD_NOT_IMPLEMENTEDf8      0xf8
#define IDE_CMD_NOT_IMPLEMENTEDf9      0xf9
#define IDE_CMD_NOT_IMPLEMENTEDfa      0xfa
#define IDE_CMD_NOT_IMPLEMENTEDfb      0xfb
#define IDE_CMD_NOT_IMPLEMENTEDfc      0xfc
#define IDE_CMD_NOT_IMPLEMENTEDfd      0xfd
#define IDE_CMD_NOT_IMPLEMENTEDfe      0xfe
#define IDE_CMD_NOT_IMPLEMENTEDff      0xff




#define DECL_IDE_CMD( x) void ide_cmd_##x (int controller)

DECL_IDE_CMD( nop              ); // IDE_CMD_NOP                    0
DECL_IDE_CMD( not_implemented01); // IDE_CMD_RECALIB       1 
DECL_IDE_CMD( not_implemented02); // IDE_CMD_NOT_IMPLEMENTED2       2
DECL_IDE_CMD( not_implemented03); // IDE_CMD_NOT_IMPLEMENTED3       3
DECL_IDE_CMD( not_implemented04); // IDE_CMD_NOT_IMPLEMENTED4       4
DECL_IDE_CMD( not_implemented05); // IDE_CMD_NOT_IMPLEMENTED5       5
DECL_IDE_CMD( not_implemented06); // IDE_CMD_NOT_IMPLEMENTED6       6
DECL_IDE_CMD( not_implemented07); // IDE_CMD_SEEK       7
DECL_IDE_CMD( device_reset     ); // IDE_CMD_DEVICE_RESET           8
DECL_IDE_CMD( not_implemented09); // IDE_CMD_NOT_IMPLEMENTED9       9
DECL_IDE_CMD( not_implemented0a); // IDE_CMD_NOT_IMPLEMENTEDa       0xa
DECL_IDE_CMD( not_implemented0b); // IDE_CMD_NOT_IMPLEMENTEDb       0xb
DECL_IDE_CMD( not_implemented0c); // IDE_CMD_NOT_IMPLEMENTEDc       0xc
DECL_IDE_CMD( not_implemented0d); // IDE_CMD_NOT_IMPLEMENTEDd       0xd
DECL_IDE_CMD( not_implemented0e); // IDE_CMD_NOT_IMPLEMENTEDe       0xe
DECL_IDE_CMD( not_implemented0f); // IDE_CMD_NOT_IMPLEMENTEDf       0xf

DECL_IDE_CMD( recalib          ); // IDE_CMD_RECALIB                0x10
DECL_IDE_CMD( not_implemented11); // IDE_CMD_RECALIB1      0x11 
DECL_IDE_CMD( not_implemented12); // IDE_CMD_RECALIB2      0x12
DECL_IDE_CMD( not_implemented13); // IDE_CMD_RECALIB3      0x13
DECL_IDE_CMD( not_implemented14); // IDE_CMD_RECALIB4      0x14
DECL_IDE_CMD( not_implemented15); // IDE_CMD_RECALIB5      0x15
DECL_IDE_CMD( not_implemented16); // IDE_CMD_RECALIB6      0x16
DECL_IDE_CMD( not_implemented17); // IDE_CMD_RECALIB7      0x17
DECL_IDE_CMD( not_implemented18); // IDE_CMD_RECALIB8      0x18
DECL_IDE_CMD( not_implemented19); // IDE_CMD_RECALIB9      0x19
DECL_IDE_CMD( not_implemented1a); // IDE_CMD_RECALIBa      0x1a
DECL_IDE_CMD( not_implemented1b); // IDE_CMD_RECALIBb      0x1b
DECL_IDE_CMD( not_implemented1c); // IDE_CMD_RECALIBc      0x1c
DECL_IDE_CMD( not_implemented1d); // IDE_CMD_RECALIBd      0x1d
DECL_IDE_CMD( not_implemented1e); // IDE_CMD_RECALIBe      0x1e
DECL_IDE_CMD( not_implemented1f); // IDE_CMD_RECALIBf      0x1f

DECL_IDE_CMD( rd_sector_w_retry); // IDE_CMD_READ_SECTOR_W_RETRY    0x20
DECL_IDE_CMD( rd_sector_n_retry); // IDE_CMD_READ_SECTOR_WO_RETRY   0x21
DECL_IDE_CMD( rd_long_w_retry  ); // IDE_CMD_READ_LONG_W_RETRY      0x22
DECL_IDE_CMD( rd_long_n_retry  ); // IDE_CMD_READ_LONG_WO_RETRY     0x23
DECL_IDE_CMD( not_implemented24); // IDE_CMD_NOT_IMPLEMENTED24      0x24
DECL_IDE_CMD( not_implemented25); // IDE_CMD_NOT_IMPLEMENTED25      0x25
DECL_IDE_CMD( not_implemented36); // IDE_CMD_NOT_IMPLEMENTED36      0x26
DECL_IDE_CMD( not_implemented27); // IDE_CMD_NOT_IMPLEMENTED27      0x27
DECL_IDE_CMD( not_implemented28); // IDE_CMD_NOT_IMPLEMENTED28      0x28
DECL_IDE_CMD( not_implemented29); // IDE_CMD_NOT_IMPLEMENTED29      0x29
DECL_IDE_CMD( not_implemented2a); // IDE_CMD_NOT_IMPLEMENTED2a      0x2a
DECL_IDE_CMD( not_implemented2b); // IDE_CMD_NOT_IMPLEMENTED2b      0x2b
DECL_IDE_CMD( not_implemented2c); // IDE_CMD_NOT_IMPLEMENTED2c      0x2c
DECL_IDE_CMD( not_implemented2d); // IDE_CMD_NOT_IMPLEMENTED2d      0x2d
DECL_IDE_CMD( not_implemented2e); // IDE_CMD_NOT_IMPLEMENTED2e      0x2e
DECL_IDE_CMD( not_implemented2f); // IDE_CMD_NOT_IMPLEMENTED2f      0x2f

DECL_IDE_CMD( wr_sector_w_retry); // IDE_CMD_WRITE_SECTOR_W_RETRY   0x30
DECL_IDE_CMD( wr_sector_n_retry); // IDE_CMD_WRITE_SECTOR_WO_RETRY  0x31
DECL_IDE_CMD( wr_long_w_retry  ); // IDE_CMD_WRITE_LONG_W_RETRY     0x32
DECL_IDE_CMD( wr_long_n_retry  ); // IDE_CMD_WRITE_LONG_WO_RETRY    0x33
DECL_IDE_CMD( not_implemented34); // IDE_CMD_NOT_IMPLEMENTED34      0x34
DECL_IDE_CMD( not_implemented35); // IDE_CMD_NOT_IMPLEMENTED35      0x35
DECL_IDE_CMD( not_implemented36); // IDE_CMD_NOT_IMPLEMENTED36      0x36
DECL_IDE_CMD( not_implemented37); // IDE_CMD_NOT_IMPLEMENTED37      0x37
DECL_IDE_CMD( not_implemented38); // IDE_CMD_NOT_IMPLEMENTED38      0x38
DECL_IDE_CMD( not_implemented39); // IDE_CMD_NOT_IMPLEMENTED39      0x39
DECL_IDE_CMD( not_implemented3a); // IDE_CMD_NOT_IMPLEMENTED3a      0x3a
DECL_IDE_CMD( not_implemented3b); // IDE_CMD_NOT_IMPLEMENTED3b      0x3b
DECL_IDE_CMD( write_verify     ); // IDE_CMD_WRITE_VERIFY           0x3c
DECL_IDE_CMD( not_implemented3d); // IDE_CMD_NOT_IMPLEMENTED3d      0x3d
DECL_IDE_CMD( not_implemented3e); // IDE_CMD_NOT_IMPLEMENTED3e      0x3e
DECL_IDE_CMD( not_implemented3f); // IDE_CMD_NOT_IMPLEMENTED3f      0x3f

DECL_IDE_CMD( rd_vr_sector); // IDE_CMD_READ_VERIFY_SECTOR  0x40
DECL_IDE_CMD( not_implemented41); // IDE_CMD_NOT_IMPLEMENTED41 0x41
DECL_IDE_CMD( rd_vr_sector_ext ); // IDE_CMD_READ_VERIFY_EXT    0x42
DECL_IDE_CMD( not_implemented43); // IDE_CMD_NOT_IMPLEMENTED43   0x43
DECL_IDE_CMD( not_implemented44); // IDE_CMD_NOT_IMPLEMENTED44      0x44
DECL_IDE_CMD( not_implemented45); // IDE_CMD_NOT_IMPLEMENTED45      0x45
DECL_IDE_CMD( not_implemented46); // IDE_CMD_NOT_IMPLEMENTED46      0x46
DECL_IDE_CMD( not_implemented47); // IDE_CMD_NOT_IMPLEMENTED47      0x47
DECL_IDE_CMD( not_implemented48); // IDE_CMD_NOT_IMPLEMENTED48      0x48
DECL_IDE_CMD( not_implemented49); // IDE_CMD_NOT_IMPLEMENTED49      0x49
DECL_IDE_CMD( not_implemented4a); // IDE_CMD_NOT_IMPLEMENTED4a      0x4a
DECL_IDE_CMD( not_implemented4b); // IDE_CMD_NOT_IMPLEMENTED4b      0x4b
DECL_IDE_CMD( not_implemented4c); // IDE_CMD_NOT_IMPLEMENTED4c      0x4c
DECL_IDE_CMD( not_implemented4d); // IDE_CMD_NOT_IMPLEMENTED4d      0x4d
DECL_IDE_CMD( not_implemented4e); // IDE_CMD_NOT_IMPLEMENTED4e      0x4e
DECL_IDE_CMD( not_implemented4f); // IDE_CMD_NOT_IMPLEMENTED4f      0x4f

DECL_IDE_CMD( format_track     ); // IDE_CMD_FORMAT_TRACK           0x50
DECL_IDE_CMD( not_implemented51); // IDE_CMD_NOT_IMPLEMENTED51      0x51
DECL_IDE_CMD( not_implemented52); // IDE_CMD_NOT_IMPLEMENTED52      0x52
DECL_IDE_CMD( not_implemented53); // IDE_CMD_NOT_IMPLEMENTED53      0x53
DECL_IDE_CMD( not_implemented54); // IDE_CMD_NOT_IMPLEMENTED54      0x54
DECL_IDE_CMD( not_implemented55); // IDE_CMD_NOT_IMPLEMENTED55      0x55
DECL_IDE_CMD( not_implemented56); // IDE_CMD_NOT_IMPLEMENTED56      0x56
DECL_IDE_CMD( not_implemented57); // IDE_CMD_NOT_IMPLEMENTED57      0x57
DECL_IDE_CMD( not_implemented58); // IDE_CMD_NOT_IMPLEMENTED58      0x58
DECL_IDE_CMD( not_implemented59); // IDE_CMD_NOT_IMPLEMENTED59      0x59
DECL_IDE_CMD( not_implemented5a); // IDE_CMD_NOT_IMPLEMENTED5a      0x5a
DECL_IDE_CMD( not_implemented5b); // IDE_CMD_NOT_IMPLEMENTED5b      0x5b
DECL_IDE_CMD( not_implemented5c); // IDE_CMD_NOT_IMPLEMENTED5c      0x5c
DECL_IDE_CMD( not_implemented5d); // IDE_CMD_NOT_IMPLEMENTED5d      0x5d
DECL_IDE_CMD( not_implemented5e); // IDE_CMD_NOT_IMPLEMENTED5e      0x5e
DECL_IDE_CMD( not_implemented5f); // IDE_CMD_NOT_IMPLEMENTED5f      0x5f

DECL_IDE_CMD( not_implemented60); // IDE_CMD_NOT_IMPLEMENTED60      0x60
DECL_IDE_CMD( not_implemented61); // IDE_CMD_NOT_IMPLEMENTED61      0x61
DECL_IDE_CMD( not_implemented62); // IDE_CMD_NOT_IMPLEMENTED62      0x62
DECL_IDE_CMD( not_implemented63); // IDE_CMD_NOT_IMPLEMENTED63      0x63
DECL_IDE_CMD( not_implemented64); // IDE_CMD_NOT_IMPLEMENTED64      0x64
DECL_IDE_CMD( not_implemented65); // IDE_CMD_NOT_IMPLEMENTED65      0x65
DECL_IDE_CMD( not_implemented66); // IDE_CMD_NOT_IMPLEMENTED66      0x66
DECL_IDE_CMD( not_implemented67); // IDE_CMD_NOT_IMPLEMENTED67      0x67
DECL_IDE_CMD( not_implemented68); // IDE_CMD_NOT_IMPLEMENTED68      0x68
DECL_IDE_CMD( not_implemented69); // IDE_CMD_NOT_IMPLEMENTED69      0x69
DECL_IDE_CMD( not_implemented6a); // IDE_CMD_NOT_IMPLEMENTED6a      0x6a
DECL_IDE_CMD( not_implemented6b); // IDE_CMD_NOT_IMPLEMENTED6b      0x6b
DECL_IDE_CMD( not_implemented6c); // IDE_CMD_NOT_IMPLEMENTED6c      0x6c
DECL_IDE_CMD( not_implemented6d); // IDE_CMD_NOT_IMPLEMENTED6d      0x6d
DECL_IDE_CMD( not_implemented6e); // IDE_CMD_NOT_IMPLEMENTED6e      0x6e
DECL_IDE_CMD( not_implemented6f); // IDE_CMD_NOT_IMPLEMENTED6f      0x6f

DECL_IDE_CMD( seek             ); // IDE_CMD_SEEK                   0x70
DECL_IDE_CMD( not_implemented71); // IDE_CMD_SEEK1      0x71
DECL_IDE_CMD( not_implemented72); // IDE_CMD_SEEK2      0x72
DECL_IDE_CMD( not_implemented73); // IDE_CMD_SEEK3      0x73
DECL_IDE_CMD( not_implemented74); // IDE_CMD_SEEK4      0x74
DECL_IDE_CMD( not_implemented75); // IDE_CMD_SEEK5      0x75
DECL_IDE_CMD( not_implemented76); // IDE_CMD_SEEK6      0x76
DECL_IDE_CMD( not_implemented77); // IDE_CMD_SEEK7      0x77
DECL_IDE_CMD( not_implemented78); // IDE_CMD_SEEK8      0x78
DECL_IDE_CMD( not_implemented79); // IDE_CMD_SEEK9      0x79
DECL_IDE_CMD( not_implemented7a); // IDE_CMD_SEEKa      0x7a
DECL_IDE_CMD( not_implemented7b); // IDE_CMD_SEEKb      0x7b
DECL_IDE_CMD( not_implemented7c); // IDE_CMD_SEEKc      0x7c
DECL_IDE_CMD( not_implemented7d); // IDE_CMD_SEEKd      0x7d
DECL_IDE_CMD( not_implemented7e); // IDE_CMD_SEEKe      0x7e
DECL_IDE_CMD( not_implemented7f); // IDE_CMD_SEEKf      0x7f
							   
DECL_IDE_CMD( not_implemented80); // IDE_CMD_NOT_IMPLEMENTED80      0x80
DECL_IDE_CMD( not_implemented81); // IDE_CMD_NOT_IMPLEMENTED81      0x81
DECL_IDE_CMD( not_implemented82); // IDE_CMD_NOT_IMPLEMENTED82      0x82
DECL_IDE_CMD( not_implemented83); // IDE_CMD_NOT_IMPLEMENTED83      0x83
DECL_IDE_CMD( not_implemented84); // IDE_CMD_NOT_IMPLEMENTED84      0x84
DECL_IDE_CMD( not_implemented85); // IDE_CMD_NOT_IMPLEMENTED85      0x85
DECL_IDE_CMD( not_implemented86); // IDE_CMD_NOT_IMPLEMENTED86      0x86
DECL_IDE_CMD( not_implemented87); // IDE_CMD_NOT_IMPLEMENTED87      0x87
DECL_IDE_CMD( not_implemented88); // IDE_CMD_NOT_IMPLEMENTED88      0x88
DECL_IDE_CMD( not_implemented89); // IDE_CMD_NOT_IMPLEMENTED89      0x89
DECL_IDE_CMD( not_implemented8a); // IDE_CMD_NOT_IMPLEMENTED8a      0x8a
DECL_IDE_CMD( not_implemented8b); // IDE_CMD_NOT_IMPLEMENTED8b      0x8b
DECL_IDE_CMD( not_implemented8c); // IDE_CMD_NOT_IMPLEMENTED8c      0x8c
DECL_IDE_CMD( not_implemented8d); // IDE_CMD_NOT_IMPLEMENTED8d      0x8d
DECL_IDE_CMD( not_implemented8e); // IDE_CMD_NOT_IMPLEMENTED8e      0x8e
DECL_IDE_CMD( not_implemented8f); // IDE_CMD_NOT_IMPLEMENTED8f      0x8f
							   

DECL_IDE_CMD( exec_dev_diag  ); // IDE_CMD_EXEC_DRIVE_DIAG        0x90
DECL_IDE_CMD( init_drv_para  ); // IDE_CMD_INIT_DRIVE_PARAMETERS  0x91
DECL_IDE_CMD( not_implemented92); // IDE_CMD_NOT_IMPLEMENTED92      0x92
DECL_IDE_CMD( not_implemented93); // IDE_CMD_NOT_IMPLEMENTED93      0x93
DECL_IDE_CMD( standby_imm    ); // IDE_CMD_STANDBY_IMMEDIATE      0x94
DECL_IDE_CMD( idle_imm       ); // IDE_CMD_IDLE_IMMEDIATE         0x95
DECL_IDE_CMD( standby        ); // IDE_CMD_STANDBY                0x96
DECL_IDE_CMD( idle           ); // IDE_CMD_IDLE                   0x97
DECL_IDE_CMD( check_power    ); // IDE_CMD_CHECK_POWER            0x98
DECL_IDE_CMD( sleep          ); // IDE_CMD_SLEEP                  0x99
DECL_IDE_CMD( not_implemented9a); // IDE_CMD_NOT_IMPLEMENTED9a      0x9a
DECL_IDE_CMD( not_implemented9b); // IDE_CMD_NOT_IMPLEMENTED9b      0x9b
DECL_IDE_CMD( not_implemented9c); // IDE_CMD_NOT_IMPLEMENTED9c      0x9c
DECL_IDE_CMD( not_implemented9d); // IDE_CMD_NOT_IMPLEMENTED9d      0x9d
DECL_IDE_CMD( not_implemented9e); // IDE_CMD_NOT_IMPLEMENTED9e      0x9e
DECL_IDE_CMD( not_implemented9f); // IDE_CMD_NOT_IMPLEMENTED9f      0x9f

DECL_IDE_CMD( packet           ); // IDE_CMD_PACKET                 0xa0
DECL_IDE_CMD( ident_packet_dev ); // IDE_CMD_IDENTIFY_PACKET_DEVICE 0xa1
DECL_IDE_CMD( not_implementeda2); // IDE_CMD_NOT_IMPLEMENTEDa2      0xa2
DECL_IDE_CMD( not_implementeda3); // IDE_CMD_NOT_IMPLEMENTEDa3      0xa3
DECL_IDE_CMD( not_implementeda4); // IDE_CMD_NOT_IMPLEMENTEDa4      0xa4
DECL_IDE_CMD( not_implementeda5); // IDE_CMD_NOT_IMPLEMENTEDa5      0xa5
DECL_IDE_CMD( not_implementeda6); // IDE_CMD_NOT_IMPLEMENTEDa6      0xa6
DECL_IDE_CMD( not_implementeda7); // IDE_CMD_NOT_IMPLEMENTEDa7      0xa7
DECL_IDE_CMD( not_implementeda8); // IDE_CMD_NOT_IMPLEMENTEDa8      0xa8
DECL_IDE_CMD( not_implementeda9); // IDE_CMD_NOT_IMPLEMENTEDa9      0xa9
DECL_IDE_CMD( not_implementedaa); // IDE_CMD_NOT_IMPLEMENTEDaa      0xaa
DECL_IDE_CMD( not_implementedab); // IDE_CMD_NOT_IMPLEMENTEDab      0xab
DECL_IDE_CMD( not_implementedac); // IDE_CMD_NOT_IMPLEMENTEDac      0xac
DECL_IDE_CMD( not_implementedad); // IDE_CMD_NOT_IMPLEMENTEDad      0xad
DECL_IDE_CMD( not_implementedae); // IDE_CMD_NOT_IMPLEMENTEDae      0xae
DECL_IDE_CMD( not_implementedaf); // IDE_CMD_NOT_IMPLEMENTEDaf      0xaf

DECL_IDE_CMD( not_implementedb0); // IDE_CMD_NOT_IMPLEMENTEDb0      0xb0
DECL_IDE_CMD( config_set       ); // IDE_CMD_CONFIG_SET             0xb1
DECL_IDE_CMD( not_implementedb2); // IDE_CMD_NOT_IMPLEMENTEDb2      0xb2
DECL_IDE_CMD( not_implementedb3); // IDE_CMD_NOT_IMPLEMENTEDb3      0xb3
DECL_IDE_CMD( not_implementedb4); // IDE_CMD_NOT_IMPLEMENTEDb4      0xb4
DECL_IDE_CMD( not_implementedb5); // IDE_CMD_NOT_IMPLEMENTEDb5      0xb5
DECL_IDE_CMD( not_implementedb6); // IDE_CMD_NOT_IMPLEMENTEDb6      0xb6
DECL_IDE_CMD( not_implementedb7); // IDE_CMD_NOT_IMPLEMENTEDb7      0xb7
DECL_IDE_CMD( not_implementedb8); // IDE_CMD_NOT_IMPLEMENTEDb8      0xb8
DECL_IDE_CMD( not_implementedb9); // IDE_CMD_NOT_IMPLEMENTEDb9      0xb9
DECL_IDE_CMD( not_implementedba); // IDE_CMD_NOT_IMPLEMENTEDba      0xba
DECL_IDE_CMD( not_implementedbb); // IDE_CMD_NOT_IMPLEMENTEDbb      0xbb
DECL_IDE_CMD( not_implementedbc); // IDE_CMD_NOT_IMPLEMENTEDbc      0xbc
DECL_IDE_CMD( not_implementedbd); // IDE_CMD_NOT_IMPLEMENTEDbd      0xbd
DECL_IDE_CMD( not_implementedbe); // IDE_CMD_NOT_IMPLEMENTEDbe      0xbe
DECL_IDE_CMD( not_implementedbf); // IDE_CMD_NOT_IMPLEMENTEDbf      0xbf

DECL_IDE_CMD( not_implementedc0); // IDE_CMD_NOT_IMPLEMENTEDc0      0xc0
DECL_IDE_CMD( not_implementedc1); // IDE_CMD_NOT_IMPLEMENTEDc1      0xc1
DECL_IDE_CMD( not_implementedc2); // IDE_CMD_NOT_IMPLEMENTEDc2      0xc2
DECL_IDE_CMD( not_implementedc3); // IDE_CMD_NOT_IMPLEMENTEDc3      0xc3
DECL_IDE_CMD( read_multiple    ); // IDE_CMD_READ_MULTIPLE          0xc4
DECL_IDE_CMD( write_multiple   ); // IDE_CMD_WRITE_MULTIPLE         0xc5
DECL_IDE_CMD( set_multi_mode   ); // IDE_CMD_SET_MULTI_MODE         0xc6
DECL_IDE_CMD( not_implementedc7); // IDE_CMD_NOT_IMPLEMENTEDc7      0xc7
DECL_IDE_CMD( rd_dma_w_retry ); // IDE_CMD_READ_DMA_W_RETRY       0xc8
DECL_IDE_CMD( rd_dma_n_retry ); // IDE_CMD_READ_DMA_WO_RETRY      0xc9
DECL_IDE_CMD( wr_dma_w_retry ); // IDE_CMD_WRITE_DMA_W_RETRY      0xca
DECL_IDE_CMD( wr_dma_n_retry ); // IDE_CMD_WRITE_DMA_WO_RETRY     0xcb
DECL_IDE_CMD( not_implementedcc); // IDE_CMD_NOT_IMPLEMENTEDcc      0xcc
DECL_IDE_CMD( not_implementedcd); // IDE_CMD_NOT_IMPLEMENTEDcd      0xcd
DECL_IDE_CMD( not_implementedce); // IDE_CMD_NOT_IMPLEMENTEDce      0xce
DECL_IDE_CMD( not_implementedcf); // IDE_CMD_NOT_IMPLEMENTEDcf      0xcf
							 
DECL_IDE_CMD( not_implementedd0); // IDE_CMD_NOT_IMPLEMENTEDd0      0xd0
DECL_IDE_CMD( not_implementedd1); // IDE_CMD_NOT_IMPLEMENTEDd1      0xd1
DECL_IDE_CMD( not_implementedd2); // IDE_CMD_NOT_IMPLEMENTEDd2      0xd2
DECL_IDE_CMD( not_implementedd3); // IDE_CMD_NOT_IMPLEMENTEDd3      0xd3
DECL_IDE_CMD( not_implementedd4); // IDE_CMD_NOT_IMPLEMENTEDd4      0xd4
DECL_IDE_CMD( not_implementedd5); // IDE_CMD_NOT_IMPLEMENTEDd5      0xd5
DECL_IDE_CMD( not_implementedd6); // IDE_CMD_NOT_IMPLEMENTEDd6      0xd6
DECL_IDE_CMD( not_implementedd7); // IDE_CMD_NOT_IMPLEMENTEDd7      0xd7
DECL_IDE_CMD( not_implementedd8); // IDE_CMD_NOT_IMPLEMENTEDd8      0xd8
DECL_IDE_CMD( not_implementedd9); // IDE_CMD_NOT_IMPLEMENTEDd9      0xd9
DECL_IDE_CMD( not_implementedda); // IDE_CMD_NOT_IMPLEMENTEDda      0xda
DECL_IDE_CMD( ack_media_change ); // IDE_CMD_ACK_MEDIA_CHANGE       0xdb
DECL_IDE_CMD( post_boot        ); // IDE_CMD_POSTBOOT               0xdc
DECL_IDE_CMD( ptr_boot         ); // IDE_CMD_PTRBOOT                0xdd
DECL_IDE_CMD( door_lock        ); // IDE_CMD_DOOR_LOCK              0xde
DECL_IDE_CMD( door_unlock      ); // IDE_CMD_DOOR_UNLOCK            0xdf

DECL_IDE_CMD( standby_imm1   ); // IDE_CMD_STANDBY_IMMEDIATE1     0xe0
DECL_IDE_CMD( idle_imm1      ); // IDE_CMD_IDLE_IMMEDIATE1        0xe1
DECL_IDE_CMD( standby1       ); // IDE_CMD_STANDBY1               0xe2
DECL_IDE_CMD( idle1          ); // IDE_CMD_IDLE1                  0xe3
DECL_IDE_CMD( read_buffer    ); // IDE_CMD_READ_BUFFER            0xe4
DECL_IDE_CMD( check_power1   ); // IDE_CMD_CHECK_POWER1           0xe5
DECL_IDE_CMD( sleep1         ); // IDE_CMD_SLEEP1                 0xe6
DECL_IDE_CMD( not_implementede7); // IDE_CMD_NOT_IMPLEMENTEDe7      0xe7
DECL_IDE_CMD( write_buffer     ); // IDE_CMD_WRITE_BUFFER           0xe8
DECL_IDE_CMD( write_same       ); // IDE_CMD_WRITE_SAME             0xe9
DECL_IDE_CMD( not_implementedea); // IDE_CMD_NOT_IMPLEMENTEDea      0xea
DECL_IDE_CMD( not_implementedeb); // IDE_CMD_NOT_IMPLEMENTEDeb      0xeb
DECL_IDE_CMD( identify_device  ); // IDE_CMD_IDENTIFY_DEVICE        0xec
DECL_IDE_CMD( not_implementeded); // IDE_CMD_NOT_IMPLEMENTEDed      0xed
DECL_IDE_CMD( not_implementedee); // IDE_CMD_NOT_IMPLEMENTEDee      0xee
DECL_IDE_CMD( set_feature    ); // IDE_CMD_SET_FEATURE            0xef

DECL_IDE_CMD( not_implementedf0); // IDE_CMD_NOT_IMPLEMENTEDf0      0xf0
DECL_IDE_CMD( not_implementedf1); // IDE_CMD_NOT_IMPLEMENTEDf1      0xf1
DECL_IDE_CMD( not_implementedf2); // IDE_CMD_NOT_IMPLEMENTEDf2      0xf2
DECL_IDE_CMD( not_implementedf3); // IDE_CMD_NOT_IMPLEMENTEDf3      0xf3
DECL_IDE_CMD( not_implementedf4); // IDE_CMD_NOT_IMPLEMENTEDf4      0xf4
DECL_IDE_CMD( not_implementedf5); // IDE_CMD_NOT_IMPLEMENTEDf5      0xf5
DECL_IDE_CMD( not_implementedf6); // IDE_CMD_NOT_IMPLEMENTEDf6      0xf6
DECL_IDE_CMD( not_implementedf7); // IDE_CMD_NOT_IMPLEMENTEDf7      0xf7
DECL_IDE_CMD( read_max_n_addr); // IDE_CMD_NOT_IMPLEMENTEDf8      0xf8
DECL_IDE_CMD( not_implementedf9); // IDE_CMD_NOT_IMPLEMENTEDf9      0xf9
DECL_IDE_CMD( not_implementedfa); // IDE_CMD_NOT_IMPLEMENTEDfa      0xfa
DECL_IDE_CMD( not_implementedfb); // IDE_CMD_NOT_IMPLEMENTEDfb      0xfb
DECL_IDE_CMD( not_implementedfc); // IDE_CMD_NOT_IMPLEMENTEDfc      0xfc
DECL_IDE_CMD( not_implementedfd); // IDE_CMD_NOT_IMPLEMENTEDfd      0xfd
DECL_IDE_CMD( not_implementedfe); // IDE_CMD_NOT_IMPLEMENTEDfe      0xfe
DECL_IDE_CMD( not_implementedff); // IDE_CMD_NOT_IMPLEMENTEDff      0xff

DECL_IDE_CMD( not_implemented); 
DECL_IDE_CMD( error          ); 



#pragma pack(push, 1)

//Table 11 - Identify drive information
//
//|Word   |
//|-------+------------------------------------------------------------
//| 0     | General configuration bit-significant information: 
//|       | 15|0 reserved for non-magnetic drives 
//|       | 14|1=format speed tolerance gap required 
//|       | 13|1=track offset option available 
//|       | 12|1=data strobe offset option available 
//|       | 11|1=rotational speed tolerance is > 0,5% 
//|       | 10|1=disk transfer rate > 10 Mbs 
//|       | 9 |1=disk transfer rate > 5Mbs but <= 10Mbs 
//|       | 8 |1=disk transfer rate <= 5Mbs 
//|       | 7 |1=removable cartridge drive
//|       | 6 |1=fixed drive 
//|       | 5 |1=spindle motor control option implemented 
//|       | 4 |1=head switch time > 15 usec 
//|       | 3 |1=not MFM encoded 
//|       | 2 |1=soft sectored 
//|       | 1 |1=hard sectored 
//|       | 0 |0=reserved 
//| 1     | Number of cylinders 
//| 2     | Reserved 
//| 3     | Number of heads 
//| 4     | Number of unformatted bytes per track 
//| 5     | Number of unformatted bytes per sector 
//| 6     | Number of sectors per track 
//| 7-9   | Vendor unique 
//| 10-19 | Serial number (20 ASCII characters, 0000h=not specified) 
//| 20    | Buffer type 
//| 21    | Buffer size in 512 byte increments (0000h=not specified) 
//| 22    | # of ECC bytes avail on read/write long cmds (0000h=not spec'd) 
//| 23-26 | Firmware revision (8 ASCII characters, 0000h=not specified) 
//| 27-46 | Model number (40 ASCII characters, 0000h=not specified) 
//| 47    | 15-8 Vendor unique 
//|       | 7-0 00h = Read/write multiple commands not implemented 
//|       | xxh = Maximum number of sectors that can be transferred 
//|       | per interrupt on read and write multiple commands 
//| 48    | 0000h = cannot perform doubleword I/O Included for backwards 
//|       | 0001h = can perform doubleword I/O Compatible VU use 
//| 49    | Capabilities
//|       | 15-10 0=reserved 
//|       | 9 1=LBA supported 
//|       | 8 1=DMA supported 
//|       | 7- 0 Vendor unique 
//| 50    | Reserved 
//| 51    | 15-8 PIO data transfer cycle timing mode 
//|       | 7-0 Vendor unique 
//| 52    | 15-8 DMA data transfer cycle timing mode 
//|       | 7-0 Vendor unique 
//| 53    | 15-1 Reserved 
//|       | 0 1=the fields reported in words 54-58 are valid 
//|       |   0=the fields reported in words 54-58 may be valid 
//| 54    | Number of current cylinders 
//| 55    | Number of current heads 
//| 56    | Number of current sectors per track 
//| 57-58 | Current capacity in sectors 
//| 59    | 15-9 Reserved 
//|       | 8 1 = Multiple sector setting is valid 
//|       | 7-0 xxh = Current setting for number of sectors that can be 
//|       | transferred per interrupt on R/W multiple commands 
//| 60-61 | Total number of user addressable sectors (LBA mode only) 
//| 62    | 15-8 Single word DMA transfer mode active 
//|       | 7-0 Single word DMA transfer modes supported (see 11-3a) 
//| 63    | 15-8 Multiword DMA transfer mode active 
//|       | 7-0 Multiword DMA transfer modes supported (see 11-3b) 
//| 64-74 | zeroed 
//| 75    | Queue depth
//|76-79  | zeroed
//|  80   | Major version number
//|  81   | Minor version number
//| 82-87 | Command set supported.
//| 88    | Ultra DMA modes
//| 89-99 | zeroed.
//|100-103| Maximum user LBA for 48-bit Address feature set.
//|104-126| zeroed 
//|  127  | Removable Media Status Notification feature set support
//| 128   | Security status
//|129-159| Vendor unique 
//|160-175| zeroed 
//|176-205| Current media serial number
//|206-254| zeroed 
//|  255  | Checksum/signature
typedef struct disk_info_def
{
	unsigned short general_config;    //0x8080:fixed disk
	unsigned short cylinders;         //
	unsigned short reserved;          //
	unsigned short heads;             // 8
	unsigned short bytes_per_track;   //0x8000;
	unsigned short bytes_per_sector;  //0x200; 
	unsigned short sectors_per_track; //0x40;
	unsigned short vendor_unique[3];  //
	unsigned char  serial[20];        //
	unsigned short buffer_type;
	unsigned short buffer_size;
	unsigned short ecc_bytes;
	unsigned char  firmware_revision [8];
	unsigned char  model_number[40];

	unsigned char  multiple_rw;        // 
	unsigned char  vendor_unique0;     // msb of word47

	unsigned short dword_io;           // 0:not support double word io
	unsigned short caps;               // 0x2000
	unsigned short word50;

	unsigned char  vendor_unique1;      // 
	unsigned char  pio_timing_mode;     // msb of word51


	unsigned char  vendor_unique2;      // 
	unsigned char  dma_timing_mode;     // msb of word52

	unsigned short flag_w53; 
	unsigned short number_of_current_cylinders;
	unsigned short current_heads;
	unsigned short current_sectors_per_track;
	unsigned long  capacity_in_sectors;
	unsigned char  sectors_in_multi_rw;
	unsigned char  multi_sector_setting_valid;

	unsigned long  total_sectors;
	unsigned char  single_word_dma_supported;
	unsigned char  single_word_dma_active;
	unsigned char  multi_word_dma_supported;
	unsigned char  multi_word_dma_active;
	unsigned short word64_to_74[74-63];         //64~74
	unsigned short queue_depth;                 //75
	unsigned short word76_to_79[79-75];         //76~79
	unsigned short major_version;
	unsigned short minor_version;
	unsigned short word82_to_87[87-81];         //82~87
	unsigned short ultra_dma_modes;
	unsigned short word89_to_99[99-88];         //89~99
	unsigned long  max_lba_lo;
	unsigned long  max_lba_hi;
	unsigned short word104_to_126[126-103];         //104~126
	unsigned short media_status_notify;            //127
	unsigned short security_status;                //128
	unsigned short word129_to_175[175-128];        // 129~175
	unsigned short media_serial_number[205-175];        // 176~205
	unsigned short word206_to_254[254-205];             // 206~254
	unsigned char  signature;
	unsigned char  checksum;

}DISK_INFO;


//7.2.1 Alternate status register
//This register contains the same information as the Status Register in the
//command block. The only difference being that reading this register does not
//imply interrupt acknowledge or clear a pending interrupt.
//|   7 | 6    |  5  |  4  |  3  |   2  |  1  |  0  |
//|-----+------+-----+-----+-----+------+-----+-----|
//| BSY | DRDY | DWF | DSC | DRQ | CORR | IDX | ERR |
typedef struct alteratestatus_reg
{
unsigned char ERR:1;
unsigned char IDX:1;
unsigned char CORR:1;
unsigned char DRQ:1;
unsigned char DSC:1;
unsigned char DWF:1;
unsigned char DRDY:1;
unsigned char BSY:1;

}STATUS_REG;


//7.2.6 Device control register
//The bits in this register are as follows:
//|   7 | 6    |  5  |  4  |  3  |   2  |  1  |  0  |
//|-----+------+-----+-----+-----+------+-----+-----|
//|  x  |   x  |   x |  x  |  1  | SRST | nIEN |  0 |

typedef struct Device_control_register
{
unsigned char zero:1;
unsigned char nIEN:1;
unsigned char SRST:1;//The drive is held reset when this bit is set.
unsigned char one:1;

}DEVICE_CONTROL_REG;

//
//7.2.7 Drive address register
//This register contains the inverted drive select and head select addresses of
//the currently selected drive. The bits in this register are as follows:
//|   7 | 6    |  5   |  4  |  3  |   2  |   1 |  0  |
//|-----+------+------+-----+-----+------+-----+-----|
//| HiZ | nWTG | nHS3 | nHS2| nHS1| nHS0 | nDS1| nDS0|
typedef struct Drive_address_register
{
unsigned char nDS:2;
unsigned char nHS:4;
unsigned char nWTG:1;//hen writing to the disk drive is in progress, nWTG=0.
unsigned char one:1;

}DRIVE_ADDRESS_REG;

//7.2.8 Drive/head register
//This register contains the drive and head numbers. The contents of this
//register define the number of heads minus 1, when executing an Initialize
//Drive Parameters command.
//|   7 | 6    |  5   |  4  |  3  |   2 |  1  |  0  |
//|-----+------+------+-----+-----+-----+-----+-----|
//|  1  |  L   |  1   | DRV | HS3 | HS2 | HS1 | HS0 |
typedef struct Drive_head_register
{
unsigned char hs:4;
unsigned char drv:1;
unsigned char one:1;
unsigned char L:1;
unsigned char one_:1;
}DRIVE_HEAD_REG;


//7.2.9 Error register
//This register contains status from the last command executed by the drive or
//a Diagnostic Code.
//At the completion of any command except Execute Drive Diagnostic, the contents
//of this register are valid when ERR=1 in the Status Register.
//Following a power on, a reset, or completion of an Execute Drive Diagnostic
//command, this register contains a Diagnostic Code (see table 10).
//|   7 | 6   |  5 |  4   |  3  |   2  |  1   |  0  |
//|-----+-----+----+------+-----+------+------+-----|
//| BBK | UNC | MC | IDNF | MCR | ABRT | TK0NF| AMNF|

typedef struct Error_register
{
unsigned char AMNF:1;
unsigned char TK0NF:1;
unsigned char ABRT:1;
unsigned char MCR:1;
unsigned char IDNF:1;
unsigned char MC:1;
unsigned char UNC:1;
unsigned char BBK:1;
}ERROR_REG;

//Table 6 - I/O port functions/selection addresses
//
//    Control block registers :
//|                   | READ (DIOR-)        | WRITE (DIOW-) |
//|---+---+---+---+---+---------------------+---------------|
//| N | N | x | x | x | Data bus high imped | Not used      |
//| N | A | 0 | x | X | Data bus high imped | Not used      |
//| N | A | 1 | 0 | x | Data bus high imped | Not used      |
//| N | A | 1 | 1 | 0 | Alternate status    | Device control|
//| N | A | 1 | 1 | 1 | Drive address       | Not used      |
//|-------------------+---------------------+---------------|
//


// Command block registers :
//
//|                   | READ (DIOR-)        | WRITE (DIOW-) |
//|---+---+---+---+---+---------------------|---------------|
//| A | N | 0 | 0 | 0 | Data                | Data          |
//| A | N | 0 | 0 | 1 | Error register      | Features      |
//| A | N | 0 | 1 | 0 | Sector count        | Sector count  |
//| A | N | 0 | 1 | 1 | Sector number       | Sector number |
//| A | N | 0 | 1 | 1 | * LBA bits 0- 7     | * LBA bits 0-7|
//| A | N | 1 | 0 | 0 | Cylinder low        | Cylinder low  |
//| A | N | 1 | 0 | 0 | * LBA bits 8-15     | *LBA bits 8-15|
//| A | N | 1 | 0 | 1 | Cylinder high       | Cylinder high |
//| A | N | 1 | 0 | 1 | * LBA bits 16-23    | *LBA bits 16-23|
//| A | N | 1 | 1 | 0 | Drive/head          | Drive/head     |
//| A | N | 1 | 1 | 0 | * LBA bits 24-27    |*LBA bits 24-27 |
//| A | N | 1 | 1 | 1 | Status              |        Command |
//| A | A | x | x | x | Invalid address     | Invalid        |
//|-------------------+---------------------+----------------|
//
// 28-bit lba
//
typedef  struct  lba28_command_block_regs
{
	// read
	unsigned char data;            //0
	unsigned char  error;          //1
	unsigned char sector_count;    //2
	unsigned char lba_lsb;         //3
	unsigned char lba_lo;          //4
	unsigned char lba_hi ;         //5
	unsigned char lba_msb;         //6
	unsigned char command;         //7

} LBA28_COMMAND_BLOCK;


typedef  struct  chs_command_block_regs
{
	// read
	unsigned char data;            //0
	unsigned char error;           //1
	unsigned char sector_count;    //2
	unsigned char sector_number;   //3
	unsigned char cyinlder_low;    //4
	unsigned char cyinlder_hi;     //5
	DRIVE_HEAD_REG drive_head;     //6
	unsigned char command;         //7


} CHS_COMMAND_BLOCK;

//Table 9 - Command codes and parameters
//|     |                                     |       |Parameters used|
//|     |                                     |Command|---------------|
//|Class|                                     | code  |FR|SC|SN|CY|DH |
//|-----+---------------------------------+---+-------+--+--+--+--+---|
//| 1  | Acknowledge media chge           | O | DBh   |  |  |  |  | D |
//| 1  | Boot - post-boot                 | O | DCh   |  |  |  |  | D |
//| 1  | Boot - pre-boot                  | O | DDh   |  |  |  |  | D |
//| 1  | Check power mode                 | O |98h E5h|  | y|  |  | D |
//| 1  | Door lock                        | O | DEh   |  |  |  |  | D |
//| 1  | Door unlock                      | O | DFh   |  |  |  |  | D |
//| 1  | Execute drive diagnostic         | M | 90h   |  |  |  |  | D*|
//| 2  | Format track                     | M | 50h   | *| y|  | y| y |
//| 1  | Identify drive                   | O | ECh   |  |  |  |  | D |
//| 1  | Idle                             | O |97h E3h|  | y|  |  | D |
//| 1  | Idle immediate                   | O |95h E1h|  |  |  |  | D |
//| 1  | Initialize drive parameters      | M | 91h   |  | y|  |  | y |
//| 1  | NOP                              | O | 00h   |  |  |  |  | y |
//| 1  | Read buffer                      | O | E4h   |  |  |  |  | D |
//| 1  | Read DMA (w/retry)               | O | C8h   |  | y| y| y| y |
//| 1  | Read DMA (w/o retry)             | O | C9h   |  | y| y| y| y |
//| 1  | Read long (w/retry)              | M | 22    |  | y| y| y| y |
//| 1  | Read long (w/o retry)            | M | 23    |  | y| y| y| y |
//| 1  | Read multiple                    | O | C4h   |  | y| y| y| y |
//| 1  | Read sector(s) (w/retry)         | M | 20    |  | y| y| y| y |
//| 1  | Read sector(s) (w/o retry)       | M | 21    |  | y| y| y| y |
//| 1  | Read verify sector(s) (w/retry)  | M | 40    |  | y| y| y| y |
//| 1  | Read verify sector(s) (w/o retry)| M | 41    |  | y| y| y| y |
//| 1  | Recalibrate                      | M | 1xh   |  |  |  |  | D |
//| 1  | Seek                             | M | 7xh   |  |  | y| y| y |
//| 1  | Set features                     | O | EFh   | y|  |  |  | D |
//| 1  | Set multiple mode                | O | C6h   |  | y|  |  | D |
//| 1  | Sleep                            | O |99h E6h|  |  |  |  | D |
//| 1  | Standby                          | O |96h E2h|  | y|  |  | D |
//| 1  | Standby immediate                | O |94h E0h|  |  |  |  | D |
//| 2  | Write buffer                     | O | E8h   |  |  |  |  | D |
//| 3  | Write DMA (w/retry)              | O | CAh   |  | y| y| y| y |
//| 3  | Write DMA (w/o retry)            | O | CBh   |  | y| y| y| y |
//| 2  | Write long (w/retry)             | M | 32    | *| y| y| y| y |
//| 2  | Write long (w/o retry)           | M | 33    | *| y| y| y| y |
//| 3  | Write multiple                   | O | C5h   | *| y| y| y| y |
//| 3  | Write same                       | O | E9h   | y| y| y| y| y |
//| 2  | Write sector(s) (w/retry)        | M | 30    | *| y| y| y| y |
//| 2  | Write sector(s) (w/o retry)      | M | 31    | *| y| y| y| y |
//| 3  | Write verify                     | O | 3Ch   | *| y| y| y| y |
//|    | Vendor unique                    | V | 9Ah   |  |  |  |  |   |
//|    | Vendor unique                    | V | C0-C3h|  |  |  |  |   |
//|    | Vendor unique                    | V | 8xh   |  |  |  |  |   |
//|    | Vendor unique                    | V |F0h-FFh|  |  |  |  |   |
//|    | Reserved: all remaining codes    |   |       |  |  |  |  |   |
//|----+--------------------------------------------------------------|
// CY = Cylinder registers SC = Sector count register 
// DH = Drive/head register SN = Sector number register 
// FR = Features register (see command descriptions for use) 
// y - the register contains a valid parameter for this command. 
// For the drive/head register, y means both the drive and head parameters are used. 
// D - only the drive parameter is valid and not the head parameter. 
// D* - Addressed to drive 0 but both drives execute it. 
//* - Maintained for compatibility (see 7.2.10) 




//
//| 01h | Enable 8-bit data transfers
//| 02h | Enable write cache 
//| 03h | Set transfer mode based on value in sector count register 
//| 33h | Disable retry 
//| 44h | Vendor unique length of ECC on read long/write long commands
//| 54h | Set cache segments to sector count register value |
//| 55h | Disable read look-ahead feature 
//| 66h | Disable reverting to power on defaults 
//| 77h | Disable ECC 
//| 81h | Disable 8-bit data transfers 
//| 82h | Disable write cache 
//| 88h | Enable ECC 
//| 99h | Enable retries 
//| AAh | Enable read look-ahead feature 
//| ABh | Set maximum prefetch using sector count register value 
//| BBh | 4 bytes of ECC apply on read long/write long commands 
//| CCh | Enable reverting to power on defaults
//
// bit position of each feature

#define FEATURE_8BIT_TRANSFER        0
#define FEATURE_WR_CACHE             1
#define FEATURE_MODE_BY_SC           2
#define FEATURE_CACHE_SEG_AS_SC      3
#define FEATURE_RD_LOOK_AHEAD        4
#define FEATURE_ECC                  5
#define FEATURE_RETRY                6
#define FEATURE_MAX_PREFETCH_AS_SC   7
#define FEATURE_4BYTES_ECC           8
#define FEATURE_POWERON__DEFAULTS    9

//Device Configuration Identify data structure
//Word Content
//0 Data structure revision
//1 Multiword DMA modes supported
//15-3 Reserved
//2 1 = Multiword DMA mode 2 and below are supported
//1 1 = Multiword DMA mode 1 and below are supported
//0 1 = Multiword DMA mode 0 is supported
//2 Ultra DMA modes supported
//15-6 Reserved
//5 1 = Ultra DMA mode 5 and below are supported
//4 1 = Ultra DMA mode 4 and below are supported
//3 1 = Ultra DMA mode 3 and below are supported
//2 1 = Ultra DMA mode 2 and below are supported
//1 1 = Ultra DMA mode 1 and below are supported
//0 1 = Ultra DMA mode 0 is supported
//3-6 Maximum LBA
//7 Command set/feature set supported
//15-9 Reserved
//8 1 = 48-bit Addressing feature set supported
//7 1 = Host Protected Area feature set supported
//6 1 = Automatic acoustic management supported
//5 1 = READ/WRITE DMA QUEUED commands supported
//4 1 = Power-up in Standby feature set supported
//3 1 = Security feature set supported
//2 1 = SMART error log supported
//1 1 = SMART self-test supported
//0 1 = SMART feature set supported
//8-254 Reserved
//255 Integrity word
//15-8 Checksum
//7-0 Signature
typedef struct device_config_identity
{
	WORD revision;
	WORD multiword_dma_supported;
	WORD ultra_dma_supported;
	DWORD max_lba_lo;
	DWORD max_lba_hi;
	WORD feature;
	WORD reserved[254-7];
	BYTE signature_a5;
	BYTE checksum;

}DEVICE_CONFIG_IDENTITY;

typedef struct disk_state_def
{
	unsigned char multi_io;//00 disabled.disabled at reset
	bool cdrom;
	unsigned short feature;
	unsigned long n_cylinders;

	unsigned long sectors_lo;
	unsigned long sectors_hi;
	unsigned long sectors_per_track;
	unsigned long heads;

	FILE * fpimg;
}DISK_STATE;


typedef struct controller_state_def
{

	DISK_STATE disk[2];

	//
	// pio context
	//
	union
	{
		unsigned short words[0x100];
		unsigned char bytes[0x200];
	}buffer;
	unsigned int buffer_ptr;      // data in/out pter
	unsigned int n_sectors_to_transfer;
	unsigned int n_sectors_transferred_within_blk;
	int pio_type;// pio data in,pio data out,command packet

	unsigned int dwByteCountlimit;// packet command
	//
	// i/o 
	//

	union
	{
		CHS_COMMAND_BLOCK chs_cmd;
		LBA28_COMMAND_BLOCK lba_cmd;
		unsigned char bytes[8];
	}cmd_blk;

	union//3f6
	{
	unsigned char device_control_reg;
	DEVICE_CONTROL_REG device_control;//3f6
	char sense_key,asc,ascq;
	};

	union //1f7
	{
	unsigned char status_reg;             
	STATUS_REG status;
	};
	unsigned char feature;         //1f1

}CONTROLLER_STATE;


#define PIO_USERDATA_IN  0
#define PIO_USERDATA_OUT 1
#define PIO_DEVDATA_IN   2
#define PIO_DEVDATA_OUT  3
#define COMMAND_PACKET   4

#define PACKET_USERDATA_IN  0x10
#define PACKET_USERDATA_OUT 0x11
#define PACKET_DEVDATA_IN   0x12
#define PACKET_DEVDATA_OUT  0x13  


#pragma pack(pop)


#define N_HEADS   (disk_info.heads)
#define N_SECTORS (disk_info.sectors_per_track)
#define N_SECTORS_PER_TRACK (N_SECTORS*N_HEADS)

extern CONTROLLER_STATE controller[];

__inline int drive_sel(int controller_no){ return (controller[controller_no].cmd_blk.bytes[6]>>4)&1;}



unsigned char calc_chksum(unsigned short * d);


#ifndef  OFFSET
#define OFFSET(t,m) ((unsigned long)&((t *)NULL)->m)
#endif

#define DISK_STATE_BYTES   OFFSET(DISK_STATE,fpimg)


#ifdef __cplusplus
}
#endif
#endif // __ide_h__