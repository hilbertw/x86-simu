#define  _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>
#include <io.h>
#include "addressmap.h"
#include "dma.h"
#include "log.h"
#include "ide.h"
#include "atapi.h"
#include "irq-wire.h"

#include "cpu.h"

//#undef LOG_IDE
//#define LOG_IDE 1

#pragma warning(disable : 4309 4101)

extern void (*ide_cmds[0x100])(int controller);
//
// dma handler 
DMA_TASK dma_tasks[2]={{3,1},{3,1}};// dma controller 1, channel 3

void do_ide()
{
	if(dma_tasks[0].state==DMA_ST_PENDING)
	{
		run_dma_task(&dma_tasks[0]);
		if(dma_tasks[0].state==DMA_ST_DONE) 
		{
			assert_irq(0xe);
			dma_tasks[0].state=DMA_ST_IDLE;
		}
	}
	if(dma_tasks[1].state==DMA_ST_PENDING)
	{
		run_dma_task(&dma_tasks[1]);
		if(dma_tasks[1].state==DMA_ST_DONE) 
		{
			assert_irq(0xf);
			dma_tasks[1].state=DMA_ST_IDLE;
		}
	}
}
//
// dma interface
//

//
// i/o interface
//
//
//
//Channel IRQ Used   I/O Addresses Used  Popularity and Support
// Primary 14       1F0-1F7h and 3F6-3F7h Used by all PCs using IDE/ATA
// Secondary 15 (10) 170-177h and 376-377h Present on all modern PCs; 
//                                      usually used for an ATAPI optical drive
//
// control block: 3f6
//
static void reg_write_ctrl (int no, char dat,int controller_no)
{

	assert(no==6);

	controller[controller_no].device_control_reg=dat;
	//
	// go to hsr1
	//
	if(controller[controller_no].status.BSY==0)
	{
		if(0!=(dat&0x4))
		{
			controller[controller_no].status_reg=0x80;
#if  LOG_IDE
			char buffer[100];
			sprintf(buffer,"IDE reset:%s\xd\xa",controller_no==0?"primary":"secondary");
#endif
		}
		else if(0==(dat&0x2))// nIEN  goes zero, assert int if pending int exists
		{
			// to be simple, 
			 assert_irq(0xe+controller_no);
		}
	}
	else if(0==(dat&0x4))
	{
		int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
		controller[controller_no].status_reg=0x50;
		//If the device does not implement the PACKET command feature set, the signature shall be:
		//Sector Count 01h
		//LBA Low 01h
		//LBA Mid 00h
		//LBA High 00h
		//Device 00h
		controller[controller_no].cmd_blk.bytes[1]=0;  //1f1
		controller[controller_no].cmd_blk.bytes[2]=  //1f2
		controller[controller_no].cmd_blk.bytes[3]=1;//1f3
		if(!controller[controller_no].disk[drive].cdrom)
		{

			controller[controller_no].cmd_blk.bytes[4]=  //1f4
			controller[controller_no].cmd_blk.bytes[5]=  //1f5
			controller[controller_no].cmd_blk.bytes[6]=0;//1f6
		}
		else
		{
			controller[controller_no].cmd_blk.bytes[4]=0x14;  //1f4
			controller[controller_no].cmd_blk.bytes[5]=0xeb;  //1f5
			controller[controller_no].cmd_blk.bytes[6]&=0x10; //1f6 device
		}
	}
}



static  char reg_read_ctrl (int no ,int controller_no)
{
	//does notimply interrupt acknowledge or clear a pending interrupt.
	return (no==6)?controller[controller_no].status_reg:0xff;
}
//
// command block
//
static void reg_write_cmd (int no,unsigned char dat,int controller_no)
{
	assert(no<0x10);
	assert(no!=0);
	if(controller[controller_no].status.BSY==0)
	{


		if(no==7)//&&controller[controller_no].cmd_blk.bytes[6]==0xa0
		{

#if  LOG_IDE
			log0("Do IDE Command:%02x [",(unsigned char)dat);	
			for(int i=1;i<7;i++) log0(" %02x",controller[controller_no].cmd_blk.bytes[i]);
			log0(" ] %s %s\xd\xa",
				controller_no==0?"primary":"secondary",
				(controller[controller_no].cmd_blk.bytes[6]&0x10)==0x0?"master":"slave");

#endif
			//controller[controller_no].status_reg = 0x80;
			int drive=drive_sel(controller_no);

			if(controller[controller_no].disk[drive].fpimg==NULL)// not installed.
				controller[controller_no].status_reg=0x0;
			else
				ide_cmds[dat](controller_no);

		}
		else 
		{
			if(no==6)  
			{
				int drive=(dat>>4)&1;

				if(controller[controller_no].disk[drive].fpimg==NULL)// not installed.
					controller[controller_no].status_reg=0x0;


				else if((dat&0x10)!=(controller[controller_no].cmd_blk.bytes[6]&0x10))
					controller[controller_no].status_reg=0x50;

			}
		}
		controller[controller_no].cmd_blk.bytes[no]=dat;
	}
}


static  char reg_read_cmd (int no,int controller_no )
{

	assert(no<0x10);
	char dat;
	
	//When BSY=1, a read of any Command Block Register shall 
	//return the contents of the Status Register.

	if(controller[controller_no].status.BSY)  no=7;

	if(no==7)
	{
		dat=controller[controller_no].status_reg;
		if (0!=controller[controller_no].status.ERR)
		{
			controller[controller_no].status_reg=0x50;
		}

	}

	else 
	{
		if(no==6)// int ack
		{
			deassert_irq(0xe+controller_no);

			//controller[controller_no].status_reg=0x50;
		}
		dat=controller[controller_no].cmd_blk.bytes[no];
	}
	
	
	return dat;
}

static void reg_write_data (int controller_no,unsigned  short dat )
{
	if(controller[controller_no].status.BSY==0)
	{
		if(controller[controller_no].pio_type==PIO_USERDATA_OUT)
		{
			controller[controller_no].buffer.words[controller[controller_no].buffer_ptr]=dat;
			//
			// a sector is buffered
			//
			if(controller[controller_no].buffer_ptr>=0xff)
			{
				int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
				assert(controller[controller_no].disk[drive].fpimg);

				fwrite(controller[controller_no].buffer.bytes,1,0x200,controller[controller_no].disk[drive].fpimg);



				if(controller[controller_no].n_sectors_to_transfer>1)
				{
					controller[controller_no].n_sectors_to_transfer--;
					controller[controller_no].buffer_ptr=0;

					if(controller[controller_no].n_sectors_transferred_within_blk==controller[controller_no].disk[drive].multi_io)
					{
						controller[controller_no].n_sectors_transferred_within_blk=0;
						if(controller[controller_no].device_control.nIEN==0)
							assert_irq(0xe+controller_no);
					}
					else
						controller[controller_no].n_sectors_transferred_within_blk++;
				}
				else
				{
					controller[controller_no].status_reg=0x50;
					assert_irq(0xe+controller_no);
				}
			}
			else
				controller[controller_no].buffer_ptr++;

		}
		else if(controller[controller_no].pio_type==COMMAND_PACKET)
		{
			//IDENTIFY PACKET DEVICE information word 0 defines command packet size
			if(controller[controller_no].buffer_ptr<COMMAND_PACKET_WORDS)
				controller[controller_no].buffer.words[controller[controller_no].buffer_ptr++]=dat;

			if(controller[controller_no].buffer_ptr==COMMAND_PACKET_WORDS)
			{
					atapi_do_packet_command(controller_no);
					//controller[controller_no].status_reg=0x80;
					//if(controller[controller_no].device_control.nIEN==0)
					//	assert_irq(0xe+controller_no);
			}	
		}
		else if(controller[controller_no].pio_type==PACKET_USERDATA_OUT)
		{
			controller[controller_no].buffer.words[controller[controller_no].buffer_ptr]=dat;

			//
			// a sector is buffered
			//
			if(controller[controller_no].buffer_ptr>=0xff)
			{
				int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
				assert(controller[controller_no].disk[drive].fpimg);

				fwrite(controller[controller_no].buffer.bytes,1,0x200,controller[controller_no].disk[drive].fpimg);



				if(controller[controller_no].n_sectors_to_transfer>1)
				{
					controller[controller_no].n_sectors_to_transfer--;
					controller[controller_no].buffer_ptr=0;

					controller[controller_no].n_sectors_transferred_within_blk++;
					if(controller[controller_no].n_sectors_transferred_within_blk==controller[controller_no].disk[drive].multi_io)
					{
						controller[controller_no].n_sectors_transferred_within_blk=0;
						if(controller[controller_no].device_control.nIEN==0)
							assert_irq(0xe+controller_no);
					}
				}
				else
				{
					controller[controller_no].status_reg=0x50;
					assert_irq(0xe+controller_no);
				}
			}
			else
				controller[controller_no].buffer_ptr++;

		}
		else //PACKET_DEVDATA_OUT
			assert(0);
	}
}


static  unsigned  short reg_read_data (int controller_no )
{
	 unsigned  short dat;

	if(controller[controller_no].status.BSY==0)
	{
		if(controller[controller_no].pio_type==PIO_USERDATA_IN)
		{
			//
			// a sector is buffered
			//
			if(controller[controller_no].buffer_ptr==0)
			{
				int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
				assert(controller[controller_no].disk[drive].fpimg);

				int n_rd=fread(controller[controller_no].buffer.bytes,1,0x200,controller[controller_no].disk[drive].fpimg);

				memset((char *)controller[controller_no].buffer.bytes+n_rd,0xff,0x200-n_rd );

//				print_mem32((char *)controller[controller_no].buffer.bytes,0,0x200);
			}

			dat=controller[controller_no].buffer.words[controller[controller_no].buffer_ptr];

			//
			// a sector is buffered
			//
			if(controller[controller_no].buffer_ptr>=0xff)
			{
				int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;

				if(controller[controller_no].n_sectors_to_transfer>1)
				{
					controller[controller_no].n_sectors_to_transfer--;
					controller[controller_no].buffer_ptr=0;

					controller[controller_no].n_sectors_transferred_within_blk++;
					if(controller[controller_no].n_sectors_transferred_within_blk==controller[controller_no].disk[drive].multi_io)
					{
						controller[controller_no].n_sectors_transferred_within_blk=0;
						if(controller[controller_no].device_control.nIEN==0)
							assert_irq(0xe+controller_no);
					}
				}
				else
				{
					controller[controller_no].status_reg=0x50;
					if(controller[controller_no].device_control.nIEN==0)
						assert_irq(0xe+controller_no);
				}

			}
			else
				controller[controller_no].buffer_ptr++;
		}
		else  if(controller[controller_no].pio_type==PIO_DEVDATA_IN)
		{
			dat=controller[controller_no].buffer.words[controller[controller_no].buffer_ptr++];

			if(controller[controller_no].buffer_ptr==0x100)
			{
					controller[controller_no].status_reg=0x50;
					if(controller[controller_no].device_control.nIEN==0)
						assert_irq(0xe+controller_no);
			}	
		}

	    else if(controller[controller_no].pio_type==PACKET_USERDATA_IN)
		{

			//
			// a sector is buffered
			//
			if(controller[controller_no].buffer_ptr==0)
			{
				int drive=(controller[controller_no].cmd_blk.bytes[6]>>4)&1;
				assert(controller[controller_no].disk[drive].fpimg);

				int n_rd=fread(controller[controller_no].buffer.bytes,1,0x200,controller[controller_no].disk[drive].fpimg);

				// log disk error
				memset((char *)controller[controller_no].buffer.bytes+n_rd,0xff,0x200-n_rd );
				//print_mem32((char *)controller[controller_no].buffer.bytes,0,0x200);
			}

			dat=controller[controller_no].buffer.words[controller[controller_no].buffer_ptr];
			controller[controller_no].buffer_ptr++;

			if(controller[controller_no].buffer_ptr>=sizeof(controller[controller_no].buffer.words)/sizeof(controller[controller_no].buffer.words[0]))
			{
				controller[controller_no].n_sectors_to_transfer--;
				if(controller[controller_no].n_sectors_to_transfer!=0)
				{	
		
					controller[controller_no].n_sectors_transferred_within_blk--;
					if(controller[controller_no].n_sectors_transferred_within_blk==0)
					{
						controller[controller_no].n_sectors_transferred_within_blk=(controller[controller_no].dwByteCountlimit+0x1ff)>>9;

						if(controller[controller_no].n_sectors_to_transfer<controller[controller_no].n_sectors_transferred_within_blk)
						{
							controller[controller_no].cmd_blk.bytes[4]=0;
							controller[controller_no].cmd_blk.bytes[5]=controller[controller_no].n_sectors_to_transfer<<1;
						}
						if(controller[controller_no].device_control.nIEN==0)
							assert_irq(0xe+controller_no);
					}
					controller[controller_no].buffer_ptr=0;
				}
				else
				{
					// successful exe of packet command
					controller[controller_no].cmd_blk.bytes[2]=3;
					controller[controller_no].status_reg=0x50;
					if(controller[controller_no].device_control.nIEN==0)
						assert_irq(0xe+controller_no);
				}

			}
		}
		else//PACKET_DEVDATA_IN
		{
			dat=controller[controller_no].buffer.words[controller[controller_no].buffer_ptr++];

			if(controller[controller_no].buffer_ptr>=(controller[controller_no].dwByteCountlimit+1)/2)
			{
				// successful exe of packet command
				controller[controller_no].cmd_blk.bytes[2]=3;
				controller[controller_no].status_reg=0x40;
				if(controller[controller_no].device_control.nIEN==0)
					assert_irq(0xe+controller_no);
			}	

		}
	}
	else dat= *(unsigned char *)&controller[controller_no].status_reg;

	return dat;
}
void io_ide1(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 unsigned long data;
	 char controller_no=*(char *)lpContext;
	 switch(op)
	 {
	 case ADDR_RANGE_READ: 
		 if((dwAddress&0xf)==0)
		 {
			 if(nBytes==2)
			 
			 *(unsigned short *)pszBuff=reg_read_data(controller_no);
			 
			 else if(nBytes==4)
			 {

			 *(unsigned short *)pszBuff=reg_read_data(controller_no);
			 *(unsigned short *)(pszBuff+2)=reg_read_data(controller_no);
			 }
			 else 
			 {
				 short tmp=reg_read_data(controller_no);
				 *pszBuff=(unsigned char)tmp;
				 log("8 bit read lo:\n");
			 }

		 }
		 else
		 {
			 assert(nBytes==1);
			 pszBuff[0]=reg_read_cmd(dwAddress&0xf,controller_no);
		 }
		 break;
	 case ADDR_RANGE_WRITE: 
		 if((dwAddress&0xf)==0)
		 {
			 if (nBytes==2)
			 reg_write_data(controller_no,*(unsigned short *)pszBuff);
			 else if (nBytes==4)
			 {
			 reg_write_data(controller_no,((unsigned short *)pszBuff)[0]);
 			 reg_write_data(controller_no,((unsigned short *)pszBuff)[1]);
			 }
			 else
			 {

				unsigned short tmp=*pszBuff;
				 reg_write_data(controller_no,tmp);
				 log("8 bit write:\n");
			 }

		 }
		 else
		 {
			 assert(nBytes==1);
			 reg_write_cmd(dwAddress&0xf,pszBuff[0],controller_no);
		 }

		 break;
	 default:
		 return;
	 }

#if  LOG_IDE
	if ((dwAddress&0xf)!=0)
	 log4(
		 "IDECMD",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }


void io_ide2(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 unsigned long data;
	 char controller_no=*(char *)lpContext;
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 assert(nBytes==1);
		 pszBuff[0]=reg_read_ctrl(dwAddress&0xf,controller_no);
		 break;
	 case ADDR_RANGE_WRITE: 
		 assert(nBytes==1);
		 reg_write_ctrl(dwAddress&0xf,pszBuff[0],controller_no);

		 break;
	 default:
		 return;
	 }
#if  LOG_IDE
	 log4(
		 "IDECON",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
}




extern DISK_INFO disk_info;


//
// mounting interface
//
bool ide_mount(wchar_t * wcsImage, int drive_no,unsigned int size,bool cdrom)
{
	int controller_no=(drive_no>>1)&1;
	int disk=drive_no&1;

	controller[controller_no].cmd_blk.bytes[6]=disk?0xb0:0xa0;
	//
	// umount old media
	//
	if(controller[controller_no].disk[disk].fpimg!=NULL)
	fclose(controller[controller_no].disk[disk].fpimg);
	controller[controller_no].disk[disk].fpimg=NULL;
	//
	// mount new media
	//
	FILE* fp;
	
	if(cdrom)
	{
		fp=_wfopen(wcsImage,L"rb");
	}else
	{
		fp=_wfopen(wcsImage,L"r+b");
		if(!fp) 
		fp=_wfopen(wcsImage,L"w+b");
	}

	if(fp)
	{
		unsigned int n_cylinders;
		
		unsigned __int64 n_sectors;
		unsigned __int64 vol;

		if(0==size)
		{
			_fseeki64(fp,0,2);
			vol=_ftelli64(fp);
		}
		else
		{
			vol = size << 30;
			_fseeki64(fp, vol, SEEK_SET);
			SetEndOfFile((HANDLE)_get_osfhandle(_fileno(fp)));
		}

		
			

		n_sectors=vol>>9;
		n_cylinders=(n_sectors+N_SECTORS_PER_TRACK-1)/N_SECTORS_PER_TRACK;

		controller[controller_no].disk[disk].fpimg=fp;
		controller[controller_no].disk[disk].n_cylinders=n_cylinders;
		controller[controller_no].disk[disk].sectors_lo=(unsigned long )n_sectors;
		controller[controller_no].disk[disk].sectors_hi=(unsigned long )(n_sectors>>32);
		controller[controller_no].disk[disk].cdrom=cdrom;
		controller[controller_no].disk[disk].sectors_per_track=N_SECTORS;
			controller[controller_no].disk[disk].heads=N_HEADS;
		controller[controller_no].status_reg=0x50;

		log0("IDE disk mounted@%d:%d( %08x%08x sectors,%x cylinders)\n",controller_no,disk,
			(unsigned long )(n_sectors>>32),
			(unsigned long )n_sectors,
			n_cylinders);
		return true;
	}
	else
	{
		controller[controller_no].status_reg=0x0;
		return false;
	}

}

void ide_unmount( int drive_no)
{

	int controller_no=(drive_no>>1)&1;
	int disk=drive_no&1;
	
	controller[controller_no].cmd_blk.bytes[6]=disk?0xb0:0xa0;

	if(controller[controller_no].disk[disk].fpimg!=NULL)
	{
		unsigned long size;
		fseek(controller[controller_no].disk[disk].fpimg,0,2);
		size=ftell(controller[controller_no].disk[disk].fpimg);
		log("image size:%x\n",size);
		fclose(controller[controller_no].disk[disk].fpimg);
	}

	controller[controller_no].disk[disk].fpimg=NULL;
	controller[controller_no].status_reg=0x0;
}