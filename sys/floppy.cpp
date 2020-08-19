#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <assert.h>
#include "filemap.h"
#include "addressmap.h"
#include "dma.h"
#include "log.h"
#include "irq-wire.h"
#pragma warning(disable: 4309)


#define FLOPPY_SIZE_360K (40*9*2*512)
#define FLOPPY_SIZE_720K (40*18*2*512)
#define FLOPPY_SIZE_1M2  (80*15*2*512)
#define FLOPPY_SIZE_1M44 (80*18*2*512)
#define FLOPPY_SIZE_2M88 (160*18*2*512)
//NEC uPD765  -  Floppy Disk Controller  -  8272A
//      PS/2 FDC Diskette Status Register A at 3F0h
//|7|6|5|4|3|2|1|0|  3F0h  PS/2 Disk Status Register A (read-only)
// | | | | | | | +------- direction
// | | | | | | +--------- write protect
// | | | | | +----------- index
// | | | | +------------- head 1 select
// | | | +--------------- track 0
// | | +----------------- step
// | +------------------- second drive installed
// +--------------------- interrupt pending
#pragma pack(1)
typedef struct floppy_status_a
{
	unsigned char dir:1;
	unsigned char wp:1;
	unsigned char index:1;
	unsigned char head1:1;
	unsigned char track0:1;
	unsigned char step:1;
	unsigned char second_drive_installed:1;
	unsigned char int_pending:1;
} FLOPPY_SRA;


//      PS/2 FDC Diskette Status Register B at 3F1h

//|7|6|5|4|3|2|1|0|  3F1h  PS/2 Disk Status Register B (read-only)
// | | | | | | | +------- motor enable 0
// | | | | | | +------- motor enable 1
// | | | | | +------- write enable
// | | | | +------- read data (toggles w/positive transition in -RD DATA)
// | | | +------- write data (toggles w/positive transition in WR DATA)
// | | +------- drive select
// +-+-------- reserved
typedef struct floppy_status_b
{
	unsigned char enable0:1;
	unsigned char enable1:1;
	unsigned char write_enable:1;
	unsigned char read:1;
	unsigned char write:1;
	unsigned char drive_sel:1;
	unsigned char reserved:2;
} FLOPPY_SRB;

//      FDC Digital Output Register at 3F2h  (all systems)

//|7|6|5|4|3|2|1|0|  port 3F2h  (write only)
// | | | | | | +-+-------- floppy drive select (0=A, 1=B, 2=floppy C, ...)
// | | | | | +------------- 1 = FDC enable, 0 = hold FDC at reset
// | | | | +--------------- 1 = DMA & I/O interface enabled  (reserved PS/2)
// | | | +----------------- 1 = turn floppy drive A motor on
// | | +------------------- 1 = turn floppy drive B motor on
// | +--------------------- 1 = turn floppy drive C motor on; (reserved PS/2)
// +----------------------- 1 = turn floppy drive D motor on; (reserved PS/2)

//- used to control drive motors, drive selection, and feature enable
//- PS/2 only uses bit 0 for floppy drive select; bit 1 is reserved
//- PS/2 only uses bits 5 & 4 for  motor enable; bits 7&6 are reserved
//- all DOR bits are cleared during controller reset
typedef struct _DOR
{
	unsigned char drive_sel:2;
	unsigned char fdc_enable:1;
	unsigned char dma_enable:1;
	unsigned char drivea_on:1;
	unsigned char driveb_on:1;
	unsigned char drivec_on:1;
	unsigned char drived_on:1;
} DOR;


//      FDC Main Status Register at 3F4h  (all systems)

//|7|6|5|4|3|2|1|0|  port 3F4h  (read only)
// | | | | | | | +------- floppy drive 0 in seek mode/busy
// | | | | | | +--------- floppy drive 1 in seek mode/busy
// | | | | | +----------- floppy drive 2 in seek mode/busy (reserved PS/2)
// | | | | +------------- floppy drive 3 in seek mode/busy (reserved PS/2)
// | | | +--------------- FDC read or write command in progress
// | | +----------------- FDC is in non-DMA mode
// | +------------------- I/O direction;  1 = FDC to CPU; 0 = CPU to FDC
// +--------------------- data reg ready for I/O to/from CPU (request for master)
typedef struct _MSR
{
	unsigned char drive0_busy:1;
	unsigned char drive1_busy:1;
	unsigned char drive2_busy:1;
	unsigned char drive3_busy:1;
	unsigned char busy:1;
	unsigned char non_dma:1;
	unsigned char io_dir:1;
	unsigned char ready_for_io:1;
} MSR;

//      FDC Command Status Register 0 at 3F5h  (all systems)

//|7|6|5|4|3|2|1|0|  Command Status Register 0 at port 3F5h
// | | | | | | +-+-------- unit selected at interrupt (0=A, 1=B, 2=...)
// | | | | | +------------- head number at interrupt (head 0 or 1)
// | | | | +--------------- not ready on read/write or SS access to head 1
// | | | +----------------- equipment check (see note)
// | | +------------------- set to 1 when FDD completes a seek command
// ++---------------------- last command status (see below)

// Bits
// 76  Last Command Status
// 00  command terminated successfully
// 01  command execution started but terminated abnormally
// 10  invalid command issued
// 11  command terminated abnormally due to a change in state of
//      the Ready Signal from the FDC  (reserved on PS/2)
//
//- equipment check can occur if FDD signals a fault or track zero is
//  not found after 77 steps on a recalibrate command
//- PS/2 only uses bits 1-0 for drive (values are 01b and 10b)
typedef struct floppy_ST0
{
	unsigned char unit_select:2;
	unsigned char head:1;
	unsigned char not_ready_or_ss:1;
	unsigned char equip_chk:1;
	unsigned char seek_complete:1;
	unsigned char last_cmd_status:2;
} FLOPPY_ST0;

//       FDC Command Status Register 1 at 3F5h  (all systems)

//|7|6|5|4|3|2|1|0|  Command Status Register 1 at port 3F5h
// | | | | | | | +------- FDC cannot find ID address mark (see reg 2)
// | | | | | | +--------- write protect detected during write
// | | | | | +----------- FDC cannot find sector ID
// | | | | +------------- unused (always zero)
// | | | +--------------- over-run;  FDC not serviced in reasonable time
// | | +----------------- data error (CRC) in ID field or data field
// | +------------------- unused (always zero)
// +--------------------- end of cylinder; sector# greater than sectors/track

//- bit 0 of Status Register 1 and bit 4 of Status Register 2 are
//  related and mimic each other
typedef struct floppy_ST1
{
	unsigned char cannot_find_address_mark:1;
	unsigned char wp:1;
	unsigned char cannot_find_sector:1;
	unsigned char unused:1;
	unsigned char over_run:1;
	unsigned char data_error:1;
	unsigned char unused_:1;
	unsigned char end_of_cylinder:1;
} FLOPPY_ST1;

//       FDC Command Status Register 2 at 3F5h  (all systems)

//|7|6|5|4|3|2|1|0|  Command Status Register 2 at port 3F5h
// | | | | | | | +------- missing address mark in data field
// | | | | | | +--------- bad cylinder, ID not found and Cyl Id=FFh
// | | | | | +----------- scan command failed, sector not found in cylinder
// | | | | +------------- scan command equal condition satisfied
// | | | +--------------- wrong cylinder detected
// | | +----------------- CRC error detected in sector data
// | +------------------- sector with deleted data address mark detected
// +--------------------- unused (always zero)

//- bit 0 of Status Register 1 and bit 4 of Status Register 2 are
//  related and mimic each other
typedef struct floppy_ST2
{
	unsigned char missing_address_mark:1;
	unsigned char bad_cylinder:1;
	unsigned char scan_cmd_failed:1;
	unsigned char scan_equal:1;
	unsigned char wrong_cylinder_detected:1;
	unsigned char crc_error:1;
	unsigned char sector_with_deleted_addr_mark:1;
	unsigned char unused:1;
} FLOPPY_ST2;

//       FDC Command Status Register 3 at 3F5h (FDD status, all systems)
//|7|6|5|4|3|2|1|0|  Floppy Disk Drive Status at port 3F5h
// | | | | | | +-+-------- FDD unit selected status (0=A, 1=B, 2=...)
// | | | | | +------------- FDD side head select status (0=head 0, 1=head 1)
// | | | | +--------------- FDD two sided status signal
// | | | +----------------- FDD track zero status signal
// | | +------------------- FDD ready status signal
// | +--------------------- FDD write protect status signal
// +----------------------- FDD fault status signal
typedef struct floppy_ST3
{
	unsigned char unit_select_status:2;
	unsigned char side_head_select_status:1;
	unsigned char two_sided_status:1;
	unsigned char track0_status:1;
	unsigned char ready_status:1;
	unsigned char wp_status:1;
	unsigned char fault_status:1;
} FLOPPY_ST3;

//       PS/2 FDC Digital Input Register at 3F7h
//|7|6|5|4|3|2|1|0|  3F7h  PS/2 Digital Input Register (read only)
// | | | | | | | +----- high density select
// | +-+-+-+-+-+-------- reserved
// +----------------------------- diskette change
typedef struct floppy_digital_input
{
	unsigned char high_density_select:1;
	unsigned char drc:2;
	unsigned char reserved:4;
	unsigned char disk_change:1;
} DIR;
//       PS/2 FDC Configuration Control Register at 3F7h
//|7|6|5|4|3|2|1|0|  3F7h  PS/2 Config. Control Register (write only)
// | | | | | | ++---- DRC1, DRC0  (see below)
// +-+-+-+-+-+---------- reserved
//
//DRC1 DRC0
//  0   0    500000 bit per second mode
//  0   1    reserved
//  1   0    250000 bit per second mode
//  1   1    reserved
//- Digital Input Register is used to sense the state of the
//  (-diskette change) and the (-high density select) signals
//- Configuration Control Register is used to set the transfer rate
typedef struct floppy_config_control
{
	unsigned char drc:2;
	unsigned char reserved:6;
} CCR;



//FDC Programming Considerations
//
//        Three phases of command execution:
//
//	1.  Command phase; commands are sent from the CPU to the FDC via
//	    port 3F5h;	bit 6 of the Status Register at 3F4h must be zero
//	2.  Execution phase; FDC executes instruction & generates INT 6
//	3.  Result phase; status and other information is available to CPU;
//	    INT 6 sets bit 7 of BIOS Data Area location 40:3E which can
//	    be polled for completion status
//
//
//        Example of a read operation:
//
//	1. turn disk motor on and set delay time for drive spin up
//	2. perform seek operation;  wait for disk interrupt
//	3. prepare DMA chip to move data to memory
//	4. send read command and wait for transfer complete interrupt
//	5. read status information
//	6. turn disk motor off
//

//
//       Key to Abbreviations
//HD = Head Number Selected	SK = SKip Deleted-data address mark
//MT = Multi-Track		US0 = drive select bit 0
//MF = MFM mode			US1 = drive select bit 1
//ND = Non-DMA mode

//Head Load Time = 2 to 254ms in 2ms increments
//Head Unload Time = 16 to 240ms in 16ms increments
//Step Rate Time = 1 to 16ms in 1ms increments


//- PS/2 systems use the 8272A diskette controller which is software
//  and port compatible with the NEC uPD765
//- accessed through ports 3F0h-3F7h;  NEC uPD765 is accessed through
//  ports 3F2h, 3F4h and 3F5h;   the 8272A uses ports 3F0h, 3F1h,
//  3F2h, 3F4h, 3F5h and 3F7h
//- data, command and status registers are all accessed through
//  port 3F5h a register stack with one address presented to the bus
//- bit 7 of BIOS Data Area byte 40:3E can be polled to determine
//  if a disk operation has completed;  this bit is set by the
//  interrupt handler when the operation has completed;  it should
//  be reset before continuing on with the next FDC operation
typedef  union floppy_command_buffer
{
	unsigned char cmd_bytes[9];
	struct cmd_scan
	{
	unsigned char  cmd_b0;
	unsigned char  cmd_b1;
	unsigned char  cylinder_number;
	unsigned char  head_number;
	unsigned char  sector_number;
	unsigned char  bytes_per_sector;
	unsigned char  end_of_track;// (last sector in track)
	unsigned char  gap_3_length;
	unsigned char  scan_test;	//(1=scan contiguous, 2=scan alternate)
	} scan;

	struct cmd_format
	{
	unsigned char  cmd_b0;
	unsigned char  cmd_b1;
	unsigned char  bytes_per_sector;
	unsigned char  sectors_per_track;
	unsigned char  gap_3_length;
	unsigned char  filler_byte;
	} format;

	struct cmd_read_track
	{
	unsigned char  cmd_b0;
	unsigned char  cmd_b1;
	unsigned char  cylinder_number;
	unsigned char  head_number;
	unsigned char  sector_number;
	unsigned char  bytes_per_sector;
	unsigned char  end_of_track;// (last sector in track)
	unsigned char  gap_3_length;
	unsigned char  data_length; //(if cmd byte 5==0);
	} 
	cmd_read_track,
	cmd_write_deleted,
	cmd_write_data,
	cmd_read_deleted,
	cmd_read_data,
	cmd_rd_or_wr;
} FLOPPY_COMMAND;

typedef  union floppy_result_buffer
{
	unsigned char result_bytes[9];
	struct result_def
	{
	FLOPPY_ST0 ST0;
	FLOPPY_ST1 ST1;
	FLOPPY_ST2 ST2;
	FLOPPY_ST3 ST3;
	unsigned char  cylinder_number;
	unsigned char  head_number;
	unsigned char  sector_number;
	unsigned char  bytes_per_sector;
	} s;
} FLOPPY_RESULT;

//
// reset is not a command 
//
#define CMD_RESET                     0
//
//Floppy Diskette Controller Operations (15 commands)
//
#define CMD_READ_TRACK                 2
#define CMD_SPECIF                     3
#define CMD_SENSE_DRIVE                4
#define CMD_WRITE_DATA                 5
#define CMD_READ_DATA                  6
#define CMD_SENSE_INT                  7
#define CMD_RECALIB                    8
#define CMD_WRITE_DELETED_DATA         9
#define CMD_READ_ID                    10
#define CMD_READ_DELETED_DATA          12
#define CMD_FORMAT_TRACK               13
#define CMD_DUMP_REG                   14
#define CMD_SEEK                       15
#define CMD_VERSION                    16
#define CMD_READ_SCAN_EQUAL            17
#define CMD_PERPENDICULAR_MODE         18
#define CMD_CONFIG                     19
#define CMD_LOCK                       20
#define CMD_VERIFY                    0x16
#define CMD_RELATIVE_SEEK             0x17
#define CMD_READ_SCAN_LOW_OR_EQUAL    0x19

#define CMD_READ_SCAN_HIGH_OR_EQUAL   0x1d
#define CMD_LIMIT                     0x1d
#define CMD_INVALID                   CMD_LIMIT

char cmd_parameter_bytes[]=
{
	0, //0
	0, //1
    9, //CMD_READ_TRACK                 2
    3, //CMD_SPECIFY                    3
    2, //CMD_SENSE_DRIVE                4
    9, //CMD_WRITE_DATA                 5
    9, //CMD_READ_DATA                  6
    2, //CMD_RECALIB                    7
    1, //CMD_SENSE_INT                  8
    9, //CMD_WRITE_DELETED_DATA         9
    2, //CMD_READ_ID                    10
	0, //11
    9, //CMD_READ_DELETED_DATA          12
    6, //CMD_FORMAT_TRACK               13
	1, //CMD_DUMP_REG                   14
    3, //CMD_SEEK                       15
	1, //CMD_VERSION                    16
    0, //CMD_READ_SCAN_EQUAL            17
	2, //CMD_PERPENDICULAR_MODE         18
	4, //CMD_CONFIG                     19
	1, //CMD_LOCK                       20
	0, //0x15
	9, //CMD_VERIFY                    0x16
	0, //0x17
	0, //0x18
    0, //CMD_READ_SCAN_LOW_OR_EQUAL    0x19
	0, //0x1a
	0, //0x1b
	0, //0x1c
    0 //CMD_READ_SCAN_HIGH_OR_EQUAL   0x1d
};

void do_reset();
void do_invalid();
void do_read_track();
void do_specify();
void do_sense_drive();
void do_write_data();
void do_read_data();
void do_sense_int();
void do_recalib();
void do_write_deleted_data();
void do_read_id();
void do_read_deleted_data();
void do_format_track();
void do_dump_reg();
void do_seek();
void do_version();
void do_read_scan_equal();
void do_pp_mode();
void do_config();
void do_lock();
void do_verify();
void do_rel_seek();
void do_read_scan_low_or_equal();
void do_read_scan_hi_or_equal();

void (*do_cmd[])()=
{
	do_invalid, //0
	do_invalid, //1
    do_read_track,                 //CMD_READ_TRACK                 2
    do_specify,                    //CMD_SPECIFY                    3
    do_sense_drive,                //CMD_SENSE_DRIVE                4
    do_write_data,                 //CMD_WRITE_DATA                 5
    do_read_data,                  //CMD_READ_DATA                  6
    do_recalib,                    //CMD_RECALIB                    7
    do_sense_int,                  //CMD_SENSE_INT                  8
    do_write_deleted_data,         //CMD_WRITE_DELETED_DATA         9
    do_read_id,                    //CMD_READ_ID                    10
	do_invalid,                    //11
    do_read_deleted_data,          //CMD_READ_DELETED_DATA          12
    do_format_track,               //CMD_FORMAT_TRACK               13
	do_dump_reg,                   //CMD_DUMP_REG                   14
    do_seek,                       //CMD_SEEK                       15
	do_version,                    //CMD_VERSION                    16
    do_read_scan_equal,            //CMD_READ_SCAN_EQUAL            17
	do_pp_mode,                    //CMD_PERPENDICULAR_MODE         18
	do_config,                     //0x13
	do_lock,                       //0x14
	do_invalid,                    //0x15
	do_verify,                     //0x16
	do_rel_seek,                   //0x17
	do_invalid,                    //0x18
    do_read_scan_low_or_equal,     //CMD_READ_SCAN_LOW_OR_EQUAL    0x19
	do_invalid, //0x1a
	do_invalid, //0x1b
	do_invalid, //0x1c
    do_read_scan_hi_or_equal       //CMD_READ_SCAN_HIGH_OR_EQUAL   0x1d
};



//
// controller tates
//
CCR ccr;
DOR dor;
DIR dir={1,2,0xf,1};
MSR msr={0,0,0,0,0,0,0,1};  // always ready
FLOPPY_SRA SRA;
FLOPPY_SRB SRB={1,1,1};

unsigned char hlt=2,srt_hut=0xdf;
unsigned char eis_efifo_poll_fifothr=7;
unsigned char pretrk;
unsigned char lock_d_gap_wg=0x80;

#define __DOR__ (*(char *)&dor)
#define __MSR__ (*(char *)&msr)
#define __DIR__ (*(char *)&dir)
#define __CCR__ (*(char *)&ccr)
#define DIO    msr.io_dir
#define RQM    msr.ready_for_io
#define NONDMA ((hlt&1)==1)
#define CB      msr.busy
//
// rive states
//
typedef struct floppy_state 
{
	FLOPPY_ST0 ST0;
	FLOPPY_ST3 ST3;

	//position of head
	int pcn; // cylinder
	char head;//head
	char sn;  // sector 

	CFileMap file_map; 
	int size;
	int tracks,sectors_per_track,bytes_per_track;
} DRIVE_STATE;
static DRIVE_STATE drive_state[2]={{0},{1,1,1,1}};
//
// command stack
//
FLOPPY_COMMAND command;
FLOPPY_RESULT result;
int cmd_buffer_top;
int result_buffer_top;
int cmd_buffer_limit;      // acording to command
int result_buffer_limit;   // acording to command
int  drive_sel_on_int;           // save drive no for  sense int.


//
// dma handler 
//
unsigned char * buffer;
unsigned short bytes;
DMA_TASK dma_task={2,0};// dma controller 0, channel 3

void set_common_result();
void do_floppy()
{
	while(dma_task.state==DMA_ST_PENDING)
	{
			unsigned int offset=drive_state[drive_sel_on_int].pcn*drive_state[drive_sel_on_int].bytes_per_track
				+(drive_state[drive_sel_on_int].head?(drive_state[drive_sel_on_int].bytes_per_track>>1):0)
				+drive_state[drive_sel_on_int].sn*0x200-0x200;			
			
			//log("floppy: c:%x h:%x s:%x eot:%x offset:%x\n",
			//	drive_state[drive_sel_on_int].pcn,
			//	drive_state[drive_sel_on_int].head,
			//	drive_state[drive_sel_on_int].sn,
			//	command.cmd_read_data.end_of_track,
			//	offset);
			//
			//DMA should have been initialized prior to the issuing of read/write cmd
			//
			dma_task.buffer=(char *)drive_state[drive_sel_on_int].file_map+offset;
			dma_task.bytes=0x200;//drive_state[drive_sel_on_int].size-offset;// available bytes to transfer

			bool TC=run_dma_task(&dma_task);
	
			if(drive_state[drive_sel_on_int].sn==command.cmd_read_data.end_of_track)
			{
				//
				// MT: C++ head ++ when @eot
				//
				if(command.cmd_bytes[0]&0x80)
				{
					if(drive_state[drive_sel_on_int].head==1)
					drive_state[drive_sel_on_int].pcn++;
					drive_state[drive_sel_on_int].head^=1;
				}
				else
				{
					drive_state[drive_sel_on_int].sn++;
				}
				drive_state[drive_sel_on_int].sn=1;
			}
			else
				drive_state[drive_sel_on_int].sn++;
		
		//
		// trandfer dsta, if TC, end the task
		//
		if(TC)
		{
			set_common_result();
			assert_irq(6);

			dma_task.state=DMA_ST_IDLE;
			__MSR__=0xd0;
		}
	}

}
//
// parameter checker
//
bool bad_rd_or_wr_parameter()
{
	if(command.cmd_rd_or_wr.end_of_track>(2*drive_state[drive_sel_on_int].sectors_per_track))
	{
		log4("eot:",
			command.cmd_rd_or_wr.head_number?'1':'0',
			command.cmd_rd_or_wr.end_of_track,
			command.cmd_rd_or_wr.cylinder_number);
	}
	//
	//MT
	//
	return command.cmd_rd_or_wr.cylinder_number>=drive_state[drive_sel_on_int].tracks
		||command.cmd_rd_or_wr.head_number>1
		//||command.cmd_rd_or_wr.end_of_track>(2*drive_state[drive_sel_on_int].sectors_per_track)
		||command.cmd_rd_or_wr.sector_number>command.cmd_rd_or_wr.end_of_track
		||command.cmd_rd_or_wr.bytes_per_sector!=0x2;
}

void set_common_result()
{
	result.result_bytes[0]=*(char *)&drive_state[drive_sel_on_int].ST0;
	result.result_bytes[1]=   //ST1;
	result.result_bytes[2]=0;//ST2;
	result.result_bytes[3]=drive_state[drive_sel_on_int].pcn;//cylinder number
	result.result_bytes[4]=drive_state[drive_sel_on_int].head;//head number
	result.result_bytes[5]=drive_state[drive_sel_on_int].sn;//sector number
	result.result_bytes[6]=2;                          //512 bytes per sector
 	result_buffer_limit=7;
	}
//LOCK 0 D3 D2 D1 D0 GAP WG
void do_pp_mode()
{
	lock_d_gap_wg=command.cmd_bytes[1];
}
//
// rsults:
//
//----------------------- PCN-Drive 0--------------------
//----------------------- PCN-Drive 1 -------------------
//----------------------- PCN-Drive 2--------------------
//----------------------- PCN-Drive 3 -------------------
//--------SRT ------------------ | --------- HUT --------
//----------- HLT -----------------------------------| ND
//------------------------ SC/EOT ----------------------
//LOCK 0 D3 D2 D1 D0 GAP WG
//0 EIS EFIFO POLL | ------ FIFOTHR --------
//-----------------------PRETRK -------------------------
void do_dump_reg()
{
	result.result_bytes[0]=drive_state[0].pcn;
	result.result_bytes[1]=drive_state[1].pcn;
	result.result_bytes[2]=
	result.result_bytes[3]=0;
	result.result_bytes[4]=srt_hut;
	result.result_bytes[5]=hlt;
	result.result_bytes[6]=0x12;
	result.result_bytes[7]=lock_d_gap_wg;
	result.result_bytes[8]=eis_efifo_poll_fifothr;
	result.result_bytes[9]=pretrk;

	result_buffer_limit=10;
}
void do_version()
{
	result.result_bytes[0]=0x90;
	result_buffer_limit=1;
}

//Configure information
//0 0 0 1 0 0 1 1 
//0 0 0 0 0 0 0 0
//0 EIS EFIFO POLL | ------ FIFOTHR ----|
//| --------------------PRETRK ----------------------- |
void do_config()
{
	eis_efifo_poll_fifothr=command.cmd_bytes[2];
	pretrk=command.cmd_bytes[3];
}

//Command W LOCK 0 0 1 0 1 0 0 Command Code
//Result R 0 0 0 LOCK 0 0 0 0
void do_lock()
{
	if(command.cmd_bytes[0]&0x80)
	{
	lock_d_gap_wg|=0x80;
	result.result_bytes[0]=0x10;
	}
	else
	{
	lock_d_gap_wg&=~0x80;
	result.result_bytes[0]=0x0;
	}
	result_buffer_limit=1;
}


void do_verify()
{
}
void do_rel_seek()
{
}

//
//        uPD765 Version     D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:     ?	?   ?	1   0	0   0	0
//	result byte 0:	   status register 0
//			   90h = uPD765B;  80h = uPD765A or uPD765A-2
//
//        Invalid Command
//
//	result byte 0:	   status register 0 (value of 80h)
void do_invalid()
{
	result.result_bytes[0]=0x80;
	result_buffer_limit=1;
}
//        Read a Track       D7  D6  D5  D4  D3  D2  D1  D0
//        (Diagnostic)
//	command byte 0:     0  MF  SK	0   0	0   1	0
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    cylinder number
//	command byte 3:    head number
//	command byte 4:    sector number
//	command byte 5:    bytes per sector
//	command byte 6:    end of track (last sector in track)
//	command byte 7:    gap 3 length
//	command byte 8:    data length (if cmd byte 5==0)
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector
void do_read_track()
{
	result.result_bytes[0]=0;//ST0
	result.result_bytes[1]=0;//ST1
	result.result_bytes[2]=0;//ST2
	result.result_bytes[3]=0;//cylinder number
	result.result_bytes[4]=0;//head number
	result.result_bytes[5]=0;//sector number
	result.result_bytes[6]=2;//bytes per sector
 	result_buffer_limit=7;
}
//
//        Specify Step &     D7  D6  D5  D4  D3  D2  D1  D0
//          Head Load
//	command byte 0:     0	0   0	0   0	0   1	1
//	command byte 1:    step rate time | head unload time
//	command byte 2:    ------------head load time------------	ND
//	returns nothing

void do_specify()
{
	srt_hut=command.cmd_bytes[1];
	hlt=command.cmd_bytes[2];
}
//
//        Sense Drive        D7  D6  D5  D4  D3  D2  D1  D0
//          Status
//	command byte 0:     0	0   0	0   0	1   0	0
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	result byte 0:	   status register 3

void do_sense_drive()
{
	// bit 5 of st3 is always 1
	drive_sel_on_int=command.cmd_bytes[1]&1;
	result.result_bytes[0]=*(char *)&drive_state[drive_sel_on_int].ST3;
 	result_buffer_limit=1;
}
//
//        Write Data         D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:    MT  MF   0	0   0	1   0	1
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    cylinder number
//	command byte 3:    head number
//	command byte 4:    sector number
//	command byte 5:    bytes per sector
//	command byte 6:    end of track (last sector in track)
//	command byte 7:    gap 3 length
//	command byte 8:    data length (if cmd byte 5==0)
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector

void do_write_data()
{
	drive_sel_on_int=command.cmd_read_data.cmd_b0&1;

	//
	// validate parameters
	//
	if(bad_rd_or_wr_parameter())
	{
		result.result_bytes[0]=0x80|drive_sel_on_int;//ST0
		result.result_bytes[1]=5; //ST1: can not find sector/track,
 		result_buffer_limit=2;
	}
	else
	{
		drive_state[drive_sel_on_int].head=command.cmd_read_data.head_number;
		drive_state[drive_sel_on_int].pcn=command.cmd_read_data.cylinder_number;
		drive_state[drive_sel_on_int].sn=command.cmd_read_data.sector_number;
		dma_task.dir=DMA_DIR_FROM_MEM;
		dma_task.state=DMA_ST_PENDING;
	}
}
//        Read Data          D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:    MT  MF  SK	0   0	1   1	0
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    cylinder number
//	command byte 3:    head number
//	command byte 4:    sector number
//	command byte 5:    bytes per sector
//	command byte 6:    end of track (last sector in track)
//	command byte 7:    gap 3 length
//	command byte 8:    data length (if cmd byte 5==0)
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector

void do_read_data()
{
	drive_sel_on_int=command.cmd_read_data.cmd_b0&1;

	//
	// validate parameters
	//
	if(bad_rd_or_wr_parameter())
	{
		result.result_bytes[0]=0x80|drive_sel_on_int;//ST0
		result.result_bytes[1]=5; //ST1: can not find sector/track,
 		result_buffer_limit=2;
	}
	else
	{
		drive_state[drive_sel_on_int].head=command.cmd_read_data.head_number;
		drive_state[drive_sel_on_int].pcn=command.cmd_read_data.cylinder_number;
		drive_state[drive_sel_on_int].sn=command.cmd_read_data.sector_number;
		dma_task.dir=DMA_DIR_TO_MEM;
		dma_task.state=DMA_ST_PENDING;
	}
}
//
//        Sense Interrupt    D7  D6  D5  D4  D3  D2  D1  D0
//            Status
//	command byte 0:     0	0   0	0   1	0   0	0
//	result byte 0:	   status register 0
//	result byte 1:	   present cylinder number

void do_sense_int()
{
	//
	// enter result phase,set MRQ,DIO,BSY.The SENSE INTERRUPT STATUS command resets
	// the interrupt signal
	//
	result.s.ST0=drive_state[drive_sel_on_int].ST0;              //ST0 
	result.result_bytes[1]=drive_state[drive_sel_on_int].pcn;    //pcn: head position
 	result_buffer_limit=2;
	SRA.int_pending=0;

	//
	// set  ST0
	//
	*(char *)&drive_state[drive_sel_on_int].ST0=drive_sel_on_int;
}
//
//        Recalibrate        D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:     0	0   0	0   0	1   1	1
//	command byte 1:     ?	?   ?	?   ?	0  US1 US0
//	returns nothing
//
void do_recalib()
{
	drive_sel_on_int=(command.cmd_bytes[1]&1);
	*(char *)&drive_state[drive_sel_on_int].ST0|=0x20;
	// set track0
	*(char *)&drive_state[drive_sel_on_int].ST3|=0x10;

	drive_state[drive_sel_on_int].pcn=0;
	drive_state[drive_sel_on_int].head=0;

}
//
//        Write Deleted Data D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:    MT  MF   0	0   1	0   0	1
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    cylinder number
//	command byte 3:    head number
//	command byte 4:    sector number
//	command byte 5:    bytes per sector
//	command byte 6:    end of track (last sector in track)
//	command byte 7:    gap 3 length
//	command byte 8:    data length (if cmd byte 5==0)
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector
//
void do_write_deleted_data()
{
}

//
//        Read ID            D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:     0  MF   0	0   1	0   1	0
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector
void do_read_id()
{
	int drive_sel_on_int=command.cmd_bytes[1]&1;

	result.result_bytes[0]=*(char *)&drive_state[drive_sel_on_int].ST0;
	result.result_bytes[1]=0;//ST1;
	result.result_bytes[2]=0;//ST2;
	result.result_bytes[3]=drive_state[drive_sel_on_int].pcn;//cylinder number
	result.result_bytes[4]=drive_state[drive_sel_on_int].head;//head number
	result.result_bytes[5]=drive_state[drive_sel_on_int].sn;//sector number
	result.result_bytes[6]=2;                          //512 bytes per sector
 	result_buffer_limit=7;
}
//
//        Read Deleted Data  D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:    MT  MF  SK	0   1	1   0	0
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    cylinder number
//	command byte 3:    head number
//	command byte 4:    sector number
//	command byte 5:    bytes per sector
//	command byte 6:    end of track (last sector in track)
//	command byte 7:    gap 3 length
//	command byte 8:    data length (if cmd byte 5==0)
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector



void do_read_deleted_data()
{
}
//
//        Format a Track     D7  D6  D5  D4  D3  D2  D1  D0
//        (Write Sector IDs)
//	command byte 0:     0  MF   0	0   1	1   0	1
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    bytes per sector
//	command byte 3:    sectors per track
//	command byte 4:    gap 3 length
//	command byte 5:    filler pattern to write in each byte
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector

void do_format_track()
{
	drive_sel_on_int=(command.cmd_bytes[1]&1);
	result.result_bytes[0]=0;//ST0
	result.result_bytes[1]=0;//ST1
	result.result_bytes[2]=0;//ST2
	result.result_bytes[3]=drive_state[drive_sel_on_int].pcn;//cylinder number
	result.result_bytes[4]=drive_state[drive_sel_on_int].head;//head number
	result.result_bytes[5]=1;//sector number
	result.result_bytes[6]=2;//bytes per sector
 	result_buffer_limit=7;
}
//
//        Seek               D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:     0	0   0	0   1	1   1	1
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    new cylinder number
//	returns nothing
void do_seek()
{
	drive_sel_on_int=(command.cmd_bytes[1]&1);
	*(char *)&drive_state[drive_sel_on_int].ST0=0x20|(command.cmd_bytes[1]&1);
	drive_state[drive_sel_on_int].pcn=command.cmd_bytes[2];
	drive_state[drive_sel_on_int].head=0;
}
//
//        Scan Equal         D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:    MT  MF  SK	1   0	0   0	1
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    cylinder number
//	command byte 3:    head number
//	command byte 4:    sector number
//	command byte 5:    bytes per sector
//	command byte 6:    end of track (last sector in track)
//	command byte 7:    gap 3 length
//	command byte 8:    scan test (1=scan contiguous, 2=scan alternate)
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector
void do_read_scan_equal()
{
}
//
//        Scan Low or Equal  D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:    MT  MF  SK	1   1	0   0	1
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    cylinder number
//	command byte 3:    head number
//	command byte 4:    sector number
//	command byte 5:    bytes per sector
//	command byte 6:    end of track (last sector in track)
//	command byte 7:    gap 3 length
//	command byte 8:    scan test (1=scan contiguous, 2=scan alternate)
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector
void do_read_scan_low_or_equal()
{
}
//
//        Scan High or Equal D7  D6  D5  D4  D3  D2  D1  D0
//
//	command byte 0:    MT  MF  SK	1   1	1   0	1
//	command byte 1:     ?	?   ?	?   ?  HD  US1 US0
//	command byte 2:    cylinder number
//	command byte 3:    head number
//	command byte 4:    sector number
//	command byte 5:    bytes per sector
//	command byte 6:    end of track (last sector in track)
//	command byte 7:    gap 3 length
//	command byte 8:    scan test (1=scan contiguous, 2=scan alternate)
//	result byte 0:	   status register 0
//	result byte 1:	   status register 1
//	result byte 2:	   status register 2
//	result byte 3:	   cylinder number
//	result byte 4:	   head number
//	result byte 5:	   sector number
//	result byte 6:	   bytes per sector
void do_read_scan_hi_or_equal()
{
}
//
// register read/write
//
void reg_write (int no, char dat)
{
	switch(no)
	{
	case 2:
		//
		// activate code for 4 drive_state:1c,2d,4e,8f
		//
		if((dat&0x4)!=(__DOR__&4))
		{
			SRA.int_pending=1;

			drive_state[dor.drive_sel].pcn=0;
			*(char*)&drive_state[dor.drive_sel].ST0|=0xc0;
			drive_sel_on_int=dor.drive_sel;

			__DIR__&=0xf0;
			__DIR__|=5;

			assert_irq(6); //irq6

		}
		
		__DOR__=dat;

		//
		// fdc disabled preiously. 
		// softreset floppy drive, won't affect ccr,dsr,and other bit is dor
		//
		if(dor.fdc_enable==0) 
		{
			result_buffer_top=
			cmd_buffer_top=
			result_buffer_limit=
			cmd_buffer_limit=0;

#ifdef LOG_FLOPPY
			log("floppy reset\xd\xa");
#endif
			__MSR__=0;
		}
		else
			__MSR__=0x80;

		break;
	case 5:
		//
		// data regiter
		//
		if(cmd_buffer_top==0)
		{
			int cmd=dat&0x1f;
			if(cmd>=CMD_LIMIT) cmd=CMD_INVALID;
			cmd_buffer_limit=cmd_parameter_bytes[cmd];
		}

		command.cmd_bytes[cmd_buffer_top++]=dat;


		// if the last byte of the parameter is seen, go to execution phase
		if(cmd_buffer_limit<=cmd_buffer_top)
		{
#if  LOG_FLOPPY

			log ("Do floppy command:");
			for(int j=0;j<cmd_buffer_limit;j++)
				log0(" %02x",command.cmd_bytes[j]);
			log0("\n");
#endif
			result_buffer_limit=
			result_buffer_top=0;

			do_cmd[command.cmd_bytes[0]&0x1f]();

			if( dma_task.state!=DMA_ST_PENDING)
			{
				//
				// raise interrupt to start result phase
				// rqm and dio be both 1
				if( result_buffer_limit!=0)
				{
					__MSR__=0xd0;
				}
				else
				{
					cmd_buffer_top=0; 
					__MSR__=0x80;
				}

				if((command.cmd_bytes[0]&0x1f)!=8)
				{
					//SRA.int_pending=1;
					if(dor.dma_enable)
						assert_irq(6); //irq6	
				}

		
			}
			else // read/write, dma pending
					__MSR__=0x10;
		}
		break;
	case 7:
		__CCR__=dat;
		//
		// PS/2 mode
		//
		dir.drc=ccr.drc;
		if(ccr.drc==0||ccr.drc==3)
			dir.high_density_select=0;
		else 
			dir.high_density_select=1;
		break;
	}
}


// cmd:3f5h, status 3f4: gen int 6
 char reg_read (int no )
{
	char dat;

	switch(no)
	{
	case 0:
		dat=*(char *)&SRA;
		break;
	case 1:
		dat=*(char *)&SRB;
		break;
	case 3:
		dat=0;
		break;
	case 4:
		dat= __MSR__;
		//if(dat==0x90)__MSR__=0x80;
		break;
	case 5:
		if(result_buffer_top<result_buffer_limit)
		{
			dat= result.result_bytes[result_buffer_top++];
	
			if(result_buffer_top==result_buffer_limit)
			{
				cmd_buffer_top=0;
				result_buffer_top=0;
				result_buffer_limit=0;

				__MSR__=0x80; 
			}	
	
		}
		break;

	case 6:
		dat=0;
		break;
	case 7:
		dat=__DIR__;
		break;
	}
	return dat;
}
//
// i/o  interface
//
void floppy_handler(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 char controller_no=*(char *)lpContext;

	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 assert(nBytes==1);
		 pszBuff[0]=reg_read(dwAddress&0xf);

			 break;
	 case ADDR_RANGE_WRITE: 
		 assert(nBytes==1);
		 reg_write(dwAddress&0xf,pszBuff[0]);

		 break;
	 default:
		 return;
	 }
#if  LOG_FLOPPY
	 log4(
		 "FLOPPY",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned int *)pszBuff:0);
#endif
 }
//
// mounting interface
//
bool floppy_mount(wchar_t * wcsImage, int drive_no,bool write_protected)
{
	//
	// close previously opened image, and load new image
	//
	drive_state[drive_no].file_map.Close();
	if(0!=drive_state[drive_no].file_map.LoadFile(wcsImage,0,0,0,write_protected))
		return false;

	DWORD image_size_hi;
	DWORD image_size_lo=GetFileSize(drive_state[drive_no].file_map.hFile,&image_size_hi);
	//
	// validate size, only 360k/720k/1.2m/1.44m/2.88m are recognized
	//
	if(image_size_lo==FLOPPY_SIZE_360K)
	{
		drive_state[drive_no].tracks=40;
		drive_state[drive_no].sectors_per_track=9;
	}
	else if(image_size_lo==FLOPPY_SIZE_360K)
	{
		drive_state[drive_no].tracks=40;
		drive_state[drive_no].sectors_per_track=9;
	}
	else if(image_size_lo==FLOPPY_SIZE_720K)
	{
		drive_state[drive_no].tracks=80;
		drive_state[drive_no].sectors_per_track=9;
	}
	else if(image_size_lo==FLOPPY_SIZE_1M44)
	{
		drive_state[drive_no].tracks=80;
		drive_state[drive_no].sectors_per_track=18;
	}
	else if(image_size_lo==FLOPPY_SIZE_1M2)
	{
		drive_state[drive_no].tracks=80;
		drive_state[drive_no].sectors_per_track=15;
	}
	else if(image_size_lo==FLOPPY_SIZE_2M88)
	{
		drive_state[drive_no].tracks=160;
		drive_state[drive_no].sectors_per_track=18;
	}
	else // unrecognized  disk size
	{
		drive_state[drive_no].file_map.Close();
		return false;
	}

	//
	// ready, write_proteced, two-side
	//
	drive_state[drive_no].size=image_size_lo;
	drive_state[drive_no].bytes_per_track=drive_state[drive_no].sectors_per_track*0x200*2;
	//drive_state[drive_no].bytes_per_side=image_size_lo>>1;
	*(char *)&drive_state[drive_no].ST3=(write_protected?0x68:0x28)|drive_no;
	dir.disk_change=0;
	return true;
}

void floppy_unmount( int drive_no)
{
	drive_state[drive_no].file_map.Close();
	*(char *)&drive_state[drive_no].ST3=drive_no; // clear ready
	drive_state[drive_no].size=0;
	//DSKCHG#:This signal is active low at power on and whenever the diskette is removed.
	dir.disk_change=1;
}