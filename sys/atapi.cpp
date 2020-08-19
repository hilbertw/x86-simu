#define  _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>
#include <string.h>
#include "dma.h"
#include "log.h"
#include "ide.h"
#include "atapi.h"
#include "8259.h"
INQUIRY_DATA inqury_data=
{
	5,// cdrom,
	0,0,
	1,//A Removable Medium Bit (RMB) of zero indicates that the medium is not removable.
	0,//The ANSI-approved version field must contain a zero to comply with 
	  //this version of the Specification.
	1,
	1,
	1,//A Response Data Format value of 01h indicates that the data shall be 
	  //in the format specified in this Specification.
	  0,0,{0,0,0},
	{'V','e','t','e','r','a','i','n'},//The datashall be left aligned within this field.
	{'v','i','r','t','u','a','l',' ','c','d','/','d','v','d',' ',' '},
	{'2','9','9','9'}
};

unsigned short mode_data_2a[]=
{
0x4A00,0x0070,0x0000,0x0000,0x422A,0x373F,0x63F1,0x2329,
0x901B,0x0001,0xD007,0x901B,0x0000,0x901B,0x0C16,0x0100,
0x0000,0x0000,0x0C16,0x0800,0x0000,0x1321,0x0000,0x901B,
0x0000,0x0C16,0x0000,0x8910,0x0000,0xC80D,0x0000,0x060B,
0x0000,0xE406,0x0000,0xC102,0x0000,0x0000
};

unsigned short toc[]=
{
	0x1200,0x0101,0x1400,0x0001,0x0000,0x0002,0x1400,0xaa,0x100,0x0f29};

unsigned short d51[]=
{
0x2000,0x010E,0x0101,0x8001,0x0000,0x0000,0x0D00,0x10F6,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,0x0000,0x0000,0x0000
};

unsigned short sense_req_dat[]={0xF0 ,5 ,0 ,0xA00, 0, 0, 0x20, 0, 0};

#pragma warning(disable : 4309 4101)
 void _8259_assert_irq(int controller, int irq_no);
 void deassert_irq(int controller, int irq_no);

void atapi_do_packet_command(int controller_no)
{
	char buffer[1000];
	unsigned int i,lba;
	unsigned __int64 pos64;

	int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;

#if LOG_ATAPI
	log(buffer,"Do Packet Command:[");	
	for(i=0;i<COMMAND_PACKET_WORDS*2;i++) 
		log0(" %02x",controller[controller_no].buffer.bytes[i]);
	log0("-%02x %02x",controller[controller_no].cmd_blk.bytes[4],controller[controller_no].cmd_blk.bytes[5]);
	log0(" ] %s %s\xd\xa",
		controller_no==0?"primary":"secondary",
		(controller[controller_no].cmd_blk.bytes[6]&0x10)==0x0?"master":"slave");
#endif

	switch(controller[controller_no].buffer.bytes[0])
	{


	case PACKET_CMD_INQUIRY:   //0x12 //INQUIRY Command
		//
		memcpy(
			controller[controller_no].buffer.bytes,(char *)&inqury_data,sizeof(inqury_data));
data_in:
		controller[controller_no].buffer_ptr=0;
		controller[controller_no].pio_type=PACKET_DEVDATA_IN;
		controller[controller_no].status_reg=0x58;
		controller[controller_no].cmd_blk.bytes[1]=0; //1f1
		controller[controller_no].cmd_blk.bytes[2]=2; //1f2 tag,rel,i/o,c/d

		break;
	case PACKET_CMD_WRITEV:    //0x2e //WRITE AND VERIFY Command
		break;
	case PACKET_CMD_WRITE12:   //0xaa //WRITE (12) Command
		break;
	case PACKET_CMD_WRITE10:   //0x2a //WRITE (10) Command
		break;

	case PACKET_CMD_REQUEST:   //0x03 //REQUEST SENSE Command
		//controller[controller_no].buffer_ptr=0;
		if(controller[controller_no].dwByteCountlimit>controller[controller_no].buffer.bytes[4])
       controller[controller_no].dwByteCountlimit=controller[controller_no].buffer.bytes[4];

		if(controller[controller_no].dwByteCountlimit>sizeof(controller[controller_no].buffer.bytes))
			controller[controller_no].dwByteCountlimit=sizeof(controller[controller_no].buffer.bytes);
		memset(
			controller[controller_no].buffer.bytes,0,controller[controller_no].dwByteCountlimit);
		goto data_in;

	case PACKET_CMD_READ_FCAP: //0x23 //READ FORMAT CAPACITIES Command
		break;
	case PACKET_CMD_READ_CAP:  //0x25 //READ CAPACITY Command
		//0400 3FFC 0000 0008
		if(controller[controller_no].dwByteCountlimit>8)
		controller[controller_no].dwByteCountlimit=8;

		controller[controller_no].buffer.bytes[0]=(char)((controller[controller_no].disk[drive].sectors_lo>>2)>>24);
		controller[controller_no].buffer.bytes[1]=(char)((controller[controller_no].disk[drive].sectors_lo>>2)>>16);
		controller[controller_no].buffer.bytes[2]=(char)((controller[controller_no].disk[drive].sectors_lo>>2)>>8);
		controller[controller_no].buffer.bytes[3]=(char)((controller[controller_no].disk[drive].sectors_lo>>2));
		controller[controller_no].buffer.bytes[4]=
		controller[controller_no].buffer.bytes[5]=
		controller[controller_no].buffer.bytes[7]=0;
		controller[controller_no].buffer.bytes[6]=0x8;
		goto data_in;

	case PACKET_CMD_READ12:    //0xa8 //READ (12) Command
		break;
	case PACKET_CMD_READ10:    //0x28 //READ (10) Command
		controller[controller_no].dwByteCountlimit=controller[controller_no].cmd_blk.bytes[4]+
			(controller[controller_no].cmd_blk.bytes[5]<<8);
		//
		// a zero byte count limit wont transfer, it is not an error
		//
		if(controller[controller_no].dwByteCountlimit==0)
		{
			controller[controller_no].status_reg=0x50;
			controller[controller_no].cmd_blk.bytes[1]=0; //1f1
			controller[controller_no].cmd_blk.bytes[2]=3; //1f2 tag,rel,i/o,c/d
			break;
		}

		//the byte count shall be less than or equal to FFFEh;? sense =5,asc=21
		if(controller[controller_no].dwByteCountlimit==0xffff)
		{
			controller[controller_no].dwByteCountlimit=0x8000;
		    controller[controller_no].cmd_blk.bytes[4]=0;
			controller[controller_no].cmd_blk.bytes[5]=0x80;
		}
//		}

		lba=
			(controller[controller_no].buffer.bytes[2]<<24)+
			(controller[controller_no].buffer.bytes[3]<<16)+
			(controller[controller_no].buffer.bytes[4]<<8)+
			controller[controller_no].buffer.bytes[5];

		// logic blks to sectoes
		controller[controller_no].n_sectors_to_transfer=
			((controller[controller_no].buffer.bytes[7]<<8)+
			controller[controller_no].buffer.bytes[8])<<2;

if(lba>0x100)
log0("read lba=%x,blks=%x\n",lba,controller[controller_no].n_sectors_to_transfer);


		pos64=lba<<11;
		assert(controller[controller_no].disk[drive].fpimg);
		_fseeki64(controller[controller_no].disk[drive].fpimg,pos64,0);//2k block
udata_in:
		controller[controller_no].buffer_ptr=0;
		controller[controller_no].pio_type=PACKET_USERDATA_IN;
		controller[controller_no].status_reg=0x58;
		controller[controller_no].cmd_blk.bytes[1]=0; //1f1
		controller[controller_no].cmd_blk.bytes[2]=2; //1f2 tag,rel,i/o,c/d


		controller[controller_no].n_sectors_transferred_within_blk=(controller[controller_no].dwByteCountlimit+0x1ff)>>9;
		
		if(controller[controller_no].n_sectors_to_transfer<controller[controller_no].n_sectors_transferred_within_blk)
		{
		    controller[controller_no].cmd_blk.bytes[4]=0;
			controller[controller_no].cmd_blk.bytes[5]=controller[controller_no].n_sectors_to_transfer<<1;
		}
		break;

	case PACKET_CMD_MODE_SENSE: //0x5a //MODE SENSE Command
		//CD-ROM Capabilities & Mechanical Status Page
		if(controller[controller_no].buffer.bytes[2]==0x2a)
		{
			//controller[controller_no].buffer_ptr=0;
			//
			i=(controller[controller_no].buffer.bytes[7]<<8)+controller[controller_no].buffer.bytes[8];
			if(controller[controller_no].dwByteCountlimit>i)
			controller[controller_no].dwByteCountlimit=i;

			if(controller[controller_no].dwByteCountlimit>sizeof mode_data_2a)
				controller[controller_no].dwByteCountlimit=sizeof mode_data_2a;

			memcpy(
				controller[controller_no].buffer.bytes,mode_data_2a,controller[controller_no].dwByteCountlimit);

			// media type
			controller[controller_no].buffer.bytes[2]=0x41;//120mm dvd data
			goto data_in;

		}
		else // page code not supported, return check code
		{
			//CHECK_CONDITION:
			controller[controller_no].status_reg=0x51;
			controller[controller_no].cmd_blk.bytes[1]=5<<5;// sense key:5
			// asc,ascq...
			break;
		}
	case PACKET_CMD_TEST:       //0x00 TEST UNIT READY Command
		//
		// check condition:

		//controller[controller_no].cmd_blk.bytes[1]=0x60; //1f1
		//controller[controller_no].status_reg=0x51;
		//controller[controller_no].cmd_blk.bytes[2]=3; //1f2 tag,rel,i/o,c/d
		//break;

	case PACKET_CMD_FORMAT:     //0x04 FORMAT UNIT Command
	case PACKET_CMD_MED_REMOVAL://0x1e PREVENT/ALLOW MEDIUM REMOVAL Command
	case PACKET_CMD_VERIFY:     //0x2f VERIFY Command
	case PACKET_CMD_STARTSTOP:  //0x1b START/STOP UNIT Command
	case PACKET_CMD_SEEK:       //0x2b SEEK Command
			controller[controller_no].status_reg=0x50;
			//controller[controller_no].cmd_blk.bytes[1]=1;
			controller[controller_no].cmd_blk.bytes[2]=3;
			controller[controller_no].cmd_blk.bytes[3]=
			controller[controller_no].cmd_blk.bytes[4]=
			controller[controller_no].cmd_blk.bytes[5]=0;
			controller[controller_no].dwByteCountlimit=0;
		break;

	case PACKET_CMD_READ_TOC:   // 0x43h READ TOC

		//The Table of Contents (TOC) is the area where the layout of the tracks on the disc is described. 
		//It is located in the lead-in area of the disc session. The TOC on discs is in principle similar to partition table on hard drives.
		//controller[controller_no].buffer_ptr=0;

		i=(controller[controller_no].buffer.bytes[7]<<8)+controller[controller_no].buffer.bytes[8];
		if(controller[controller_no].dwByteCountlimit>i)
			controller[controller_no].dwByteCountlimit=i;

		if(controller[controller_no].dwByteCountlimit>sizeof(toc)) controller[controller_no].dwByteCountlimit=sizeof(toc);

		memcpy(	controller[controller_no].buffer.bytes,toc,controller[controller_no].dwByteCountlimit);
		goto data_in;

	case 0x51:
		//controller[controller_no].buffer_ptr=0;
		i=(controller[controller_no].buffer.bytes[7]<<8)+controller[controller_no].buffer.bytes[8];
		if(controller[controller_no].dwByteCountlimit>i)
			controller[controller_no].dwByteCountlimit=i;

		if(controller[controller_no].dwByteCountlimit>sizeof(d51)) controller[controller_no].dwByteCountlimit=sizeof(d51);

		memcpy(
			controller[controller_no].buffer.bytes,d51,controller[controller_no].dwByteCountlimit);
		goto data_in;


	case PACKET_CMD_READ_SUB_CHANNEL	://0x42hREAD SUB-CHANNEL		
	case PACKET_CMD_MODE_SEL:   //0x55 //MODE SELECT Command
		//break;
	case PACKET_CMD_LOAD_UNLOAD	://0xA6hLOAD/UNLOAD CD	
		//break;


	case PACKET_CMD_READ_HEADER	   :// 0x44hREAD HEADER	
		//break;
	case PACKET_CMD_PLAY_AUDIO	   :// 0x45hPLAY AUDIO (10)		
		//break;
	case PACKET_CMD_PLAY_AUDIO_MSF	   :// 0x47hPLAY AUDIO MSF	
		//break;
	case PACKET_CMD_PAUSE_RESUME	:// 0x4BhPAUSE/RESUME		
		//break;
	case PACKET_CMD_READ_CD_MSF	    :// 0xB9hREAD CD MSF	
		//break;
	case PACKET_CMD_SCAN        ://  0xBAhSCAN		
		//break;
	case PACKET_CMD_SET_CD_SPEED :// 0xBBh SET CD SPEED	
		//break;
	case PACKET_CMD_PLAY_CD	     ://0xBCh	PLAY CD	
		//break;
	case PACKET_CMD_MECHANISM_STATUS	:// 0xBDhMECHANISM STATUS	
		//break;
	case PACKET_CMD_READ_CD	       :// 0xBEhREAD CD	
		//break;
	case PACKET_CMD_STOP_PLAY_SCAN  :// 0x4EhSTOP PLAY / SCAN
		//break;
	default:
      //not support, set CHECK_CONDITION:
			controller[controller_no].status_reg=0x51;
			controller[controller_no].cmd_blk.bytes[1]=0x50;// sense key:5
			controller[controller_no].cmd_blk.bytes[2]=3;
			controller[controller_no].cmd_blk.bytes[3]=
			controller[controller_no].cmd_blk.bytes[4]=
			controller[controller_no].cmd_blk.bytes[5]=0;
			controller[controller_no].dwByteCountlimit=0;

	}

	//controller[controller_no].cmd_blk.bytes[3]=  //1f3
	//controller[controller_no].cmd_blk.bytes[4]=  //1f4
	//controller[controller_no].cmd_blk.bytes[5]=  //1f5
	//controller[controller_no].cmd_blk.bytes[6]   //1f6

	if(controller[controller_no].device_control.nIEN==0)
	_8259_assert_irq(1,6+controller_no);


#if LOG_ATAPI
		//print_mem32((char *)controller[controller_no].buffer.bytes,0,controller[controller_no].dwByteCountlimit);

#endif
}





void do_atapi()
{

	if(controller[0].status.DRQ)
	{
	if(controller[0].device_control.nIEN==0)
	_8259_assert_irq(1,6+0);		
	}

	if(controller[1].status.DRQ)
	{
	if(controller[1].device_control.nIEN==0)
	_8259_assert_irq(1,6+1);
	}
}