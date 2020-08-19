#include <assert.h>

#include "addressmap.h"
#include "sys.h"
#include "dma.h"
#include "fsb.h"
#include "log.h"
extern CFrontSystemBus fsb;

//8237A Internal Registers
//
//Base Address Registers      16 bits 4
//Base Word Count Registers    16 bits 4
//Current Address Registers    16 bits 4
//Current Word Count Registers 16 bits 4
//Temporary Address Register   16 bits 1
//Temporary Word Count Register 16 bits 1
//Status Register              8 bits 1
//Command Register             8 bits 1
//Temporary Register           8 bits 1
//Mode Registers               6 bits 4
//Mask Register                4 bits 1
//Request Register             4 bits 1

#pragma  pack(1)
typedef struct  dmac8237
{
	struct channel_reg
	{
		unsigned short  base_address;
		unsigned short  current_address;
		unsigned short  base_count;
		unsigned short  current_count;
		unsigned  char page;
		struct _mode_8237
		{
			unsigned char ch:2;
			unsigned char op:2;
			unsigned char auto_init:1;
			unsigned char incr:1;
			unsigned char transfer_mode:2;
		} mode;
	} channels[4];

	unsigned short  temp_count;
	unsigned short  temp_address;
	unsigned char   temp;
	//
	// 8 bit temp buffer for 16-bit register access
	//
	unsigned char r[8];          
	char rw_order[8];// for 16 bit registers,internal flip-flop
	//
	//  controller register
	//

	unsigned char cmd;               //port 08h
	unsigned char tc;                //08h request|tc for read
	unsigned char mask;              //0fh
	unsigned char request;           //09h
} DMAC8237;

#define DEMAND_TRANSFER   0
#define SINGLE_TRANSFER   1
#define BLOCK_TRANSFER    2
#define CASCADE_TRANSFER  3

#define OP_VERIFY  0
#define OP_WRITE   1
#define OP_READ    2
#define OP_ILLEGAL 3

DMAC8237 dma[2];

//
// 00~0f
// c0~cf
// d0~df
// 80~8f
//Miscellaneous Registers
//Primary Controller 
//I/O port Access Description I/O port Access Description 
//08H Read/Write Command and Status Register 
//09H Write only Request Register 
//0AH Write only Single Mask Register 
//0BH Write only Mode Register 
//0CH Write only Clear Flip-Flop Register 
//0DH Write only Master Reset Register 
//0EH Write only Master Enable Register 
//0FH Write only Master Mask Register 
//
//Secondary Controller 
//D0H Read/Write Command and Status Register 
//D2H Write only Request Register 
//D4H Write only Single Mask Register 
//D6H Write only Mode Register 
//D8H Write only Clear Flip-Flop Register 
//DAH Write only Master Reset Register 
//DCH Write only Master Enable Register 
//DEH Write only Master Mask Register 



//DMA Channel Registers
//Channel 0 (8-bit) 
//00H Read/Write Offset Register 
//01H Read/Write Block Size Register
//87H Write only Page Register 
//Channel 1 (8-bit) 
//02H Read/Write Offset Register 
//03H Read/Write Block Size Register 
//83H Write only Page Register 
//
//Channel 2 (8-bit) 
//04H Read/Write Offset Register 
//05H Read/Write Block Size Register 
//81H Write only Page Register 
//
//Channel 3 (8-bit) 
//06H Read/Write Offset Register 
//07H Read/Write Block Size Register 
//82H Write only Page Register 
//
//Channel 4 (16-bit) 
//C0H Read/Write Offset Register 
//C2H Read/Write Block Size Register 
//8FH Write only Page Register 
//
//Channel 5 (16-bit) 
//C4H Read/Write Offset Register 
//C6H Read/Write Block Size Register 
//8BH Write only Page Register 
//
//Channel 6 (16-bit) 
//C8H Read/Write Offset Register 
//CAH Read/Write Block Size Register 
//89H Write only Page Register
//Channel 7 (16-bit) 
//CCH Read/Write Offset Register 
//CEH Read/Write Block Size Register 
//8AH Write only Page Register 
void reset_8237()
{
	memset(dma,0,sizeof(dma));
}
//
// I/O interface
//
static char read_reg(unsigned char no)
{
	char dat;
	int controller=0;

	// 0-0x10: controller1
	//0xc0~0xe0: controller2
	if(no>=0xc0) {no=(no-0xc0)>>1;controller=1;}


	//
	//0~7 : 4 channel regiters
	//
	if(no<8)
	{
		int channel=no/2;
		if( dma[controller].rw_order[no]==0)
		{
			dat=(no&1)?(char)(dma[controller].channels[channel].current_count):
				(char)(dma[controller].channels[channel].current_address);
			dma[controller].rw_order[no]=1;
		}
		else
		{
			dat=(no&1)?	(char)(dma[controller].channels[channel].current_count>>8):
				(char)(dma[controller].channels[channel].current_address>>8);

			dma[controller].rw_order[no]=0;
		}

	}
	else if(no==8)// status
	{
		dat=dma[controller].tc|(dma[controller].request<<4);
		//Bits 0¡À3 are set every time a TC is reached by that channel or an
		//external EOP is applied. These bits are cleared upon Reset and on each Status Read.
		dma[controller].tc=0;
	}
	else // write only
		dat=(char)0;

	return dat;
}

static void write_reg(unsigned char no,char dat)
{
	static char ch_map[8]={-1,2,3,1,-1,-1,-1,0};
	int controller=0;
	// 0-0x10: controller1
	//0xc0~0xe0: controller2
	// 0x8x: mapped to 0x10~0x17

	if(no>=0xc0) {no=(no-0xc0)>>1;controller=1;}

	if(no<8)
	{
		if( dma[controller].rw_order[no]==0)
		{
	        dma[controller].r[no]=dat;
			dma[controller].rw_order[no]=1;
		}
		else
		{
			if((no&1)==0)
			{
				dma[controller].channels[no/2].current_address=
				dma[controller].channels[no/2].base_address= (dat<<8) +dma[controller].r[no];
			}
			else
			{
				dma[controller].channels[no/2].current_count=
				dma[controller].channels[no/2].base_count= (dat<<8) +(dma[controller].r[no]);
			}
			dma[controller].rw_order[no]=0;
		}
	}
	else if(no<0x10)
	{
		switch(no)
		{
		case 8: // command
			dma[controller].cmd=dat;
			//if((dat&4)==0) //
			//	dma[controller].tc=0;
			break;
		case 9: // Request Register
			if(dat &4)
			dma[controller].request|=(1<<(dat&3));
			else
			dma[controller].request&=~(1<<(dat&3));
			break;
		case 10: // Single Mask Register Bit
			if(dat &4)
			dma[controller].mask|=1<<(dat&3);
			else
			{
				dma[controller].mask&=~(1<<(dat&3));
				dma[controller].tc&=~(1<<(dat&3));
			}
			break;
		case 11: // 0 Write Mode Register
			*(char *)&dma[controller].channels[dat&3].mode=dat;
			break;
		case 12: //Clear Byte Pointer Flip/Flop
			memset(dma[controller].rw_order,0,sizeof dma[controller].rw_order);
			break;
		case 13: // Master Clear. 
			    //This software instruction has the same effect as the hardware Reset.
			reset_8237();
			break;
		case 14: // Clear Mask Register
			dma[controller].mask=0;
			break;
		case 15: // Write All Mask Register Bits
			dma[controller].mask=dat;
			break;
		}
	}
	else // page register
	{

		no&=0xf;
		controller=no>8?1:0;

		int ch= ch_map[no];// 0x87->ch:0,0x81/2/3->ch:2/3/1


		dma[controller].channels[ch].page=dat;


	}

}
 void io_8237(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 *pszBuff=read_reg(dwAddress);
		 //if(dwAddress==6)
		 //{
			// printf("...");
		 //}
		 break;
	 case ADDR_RANGE_WRITE: 
		 write_reg(dwAddress,*pszBuff);
		 break;
	 default:
		 return;

	 }
#if  LOG_8237
	 log4(
		 "8237",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }

 //
 // DMA main function
 //
void log_dma(unsigned int n,short x, char dir);
//
// tranfer once. return  tc
//
 bool run_dma_task(DMA_TASK* p_task)
 {
//
// task pending?
//
	 assert(p_task->state==DMA_ST_PENDING);
	 assert(p_task->dma_ch<4);

//
// controller enabled ?
// channel masked off ?
//  channel  is being requested by others?
// bus locked/available?
//
	 char ch_mask=(1<<p_task->dma_ch);

	 if( (dma[p_task->dma_controller].cmd&0x4)==0
		 && (dma[p_task->dma_controller].mask&ch_mask)==0
		 && (dma[p_task->dma_controller].request&ch_mask)==0
		 &&fsb.try_lock())
	 {
		 unsigned long phy_addr=dma[p_task->dma_controller].channels[p_task->dma_ch].current_address+
			 (dma[p_task->dma_controller].channels[p_task->dma_ch].page<<16);

		 unsigned short dwBytes=min(dma[p_task->dma_controller].channels[p_task->dma_ch].current_count+1,p_task->bytes);

		 //assert(p_task->bytes>=dma[p_task->dma_controller].channels[p_task->dma_ch].base_count+1);
		 //
		 // incre/decr address.
		 //
		 if(dma[p_task->dma_controller].channels[p_task->dma_ch].mode.incr==1)
		 {
			 dma[p_task->dma_controller].channels[p_task->dma_ch].current_address-=dwBytes;
			phy_addr-=p_task->bytes;
		 }
		 else
			 dma[p_task->dma_controller].channels[p_task->dma_ch].current_address+=dwBytes;
		 //
		 // transter
		 //
		 if(p_task->dir==DMA_DIR_TO_MEM)
		 {
			 fsb.mem_write_string(phy_addr,p_task->buffer,dwBytes);
			 assert(dma[p_task->dma_controller].channels[p_task->dma_ch].mode.op==OP_WRITE);
		 }
		 else
		 {
			 fsb.mem_read_string(phy_addr,p_task->buffer,dwBytes);
			 assert(dma[p_task->dma_controller].channels[p_task->dma_ch].mode.op==OP_READ);
		 }
		 
		 //
		 // block transfer mode
		 //
		 switch(dma[p_task->dma_controller].channels[p_task->dma_ch].mode.transfer_mode)
		 {
		 case DEMAND_TRANSFER:
		 case BLOCK_TRANSFER:
			 break;
		 case SINGLE_TRANSFER:
			 break;
		 }

		 if(dma[p_task->dma_controller].channels[p_task->dma_ch].current_count<dwBytes)
		 {
			 dma[p_task->dma_controller].tc |=ch_mask;
			 dma[p_task->dma_controller].channels[p_task->dma_ch].current_count=0xffff; 
		 	 //p_task->bytes=0;

		 }
		 else
		 {
		 	 //p_task->bytes=
			 dma[p_task->dma_controller].channels[p_task->dma_ch].current_count-=dwBytes;

		 }

	
	
#if  LOG_DMA
		 log("DMA transfer:%x/%x bytes %s %x/%x\n",
			 dwBytes,
			 dma[p_task->dma_controller].channels[p_task->dma_ch].base_count+1,
			 p_task->dir?"from":"to",
			 phy_addr,
			 dma[p_task->dma_controller].channels[p_task->dma_ch].base_address
			 +(dma[p_task->dma_controller].channels[p_task->dma_ch].page<<16)
			 );
#endif
	 }
	return  0!=(dma[p_task->dma_controller].tc &ch_mask);
 }

void do_8237()
{
	int controller,dma_ch;

	for(controller=0;controller<2;controller++)
	{
		//
		// scan for request
		//
		 if( (dma[controller].cmd&0x4)==0)
		 {
			if(dma[controller].request!=0)
			for(dma_ch=0;dma_ch<4;dma_ch++)
			{
				 char ch_mask=(1<<dma_ch);

				 if((dma[controller].request&ch_mask)!=0)
				 {
					 unsigned long phy_addr=dma[controller].channels[dma_ch].current_address+
				 (dma[controller].channels[dma_ch].page<<16);
					//In order to make a software request,the channel must be in Block Mode.
					 assert(dma[controller].channels[dma_ch].mode.transfer_mode==BLOCK_TRANSFER);
				 }

			}
			if(dma[controller].tc!=0)
			for(dma_ch=0;dma_ch<4;dma_ch++)
			{
				 char ch_mask=(1<<dma_ch);

				 //
				 // auto init?
				 //
				 if(dma[controller].channels[dma_ch].mode.auto_init)
				 {
				 }
			}
		}
	}
	


}