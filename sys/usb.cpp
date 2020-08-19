#include "usb.h"
#include "mch.h"
#include "log.h"
#include "pci-devs.h"
extern CMCH mch;
extern ICH82801_UHCI config_space_82801_uhci[];

UHCI uhci[4];
EHCI ehci;
extern bool step;

void usb_read( unsigned long address,unsigned char * pBuffer,unsigned long dwBytes,unsigned long no)
{

	if(no<4)
	{
		// port sc0/sc1/sc2/sc3..., bit 7 is 1
		uhci[no].db[0x10]|=0x80;
		uhci[no].db[0x12]|=0x80;
		for(int i=0;i<dwBytes;i++)
		{
			if(i+address<sizeof(UHCI))
			{
				pBuffer[i]=uhci[no].db[i+address];		
			}
			else break;

		}

	

	}
	else
	{
		for(int i=0;i<dwBytes;i++)
		{
			if(i+address<sizeof(EHCI))
			{
				pBuffer[i]=ehci.db[i+address];
			}
			else break;

		}

	}
}
void usb_write(unsigned long address,unsigned char * pBuffer,unsigned long dwBytes,unsigned long no)
{
	step=1;
	if(no<4)
	{
		for(int i=0;i<dwBytes;i++)
		{
			if(i==0x18) uhci[no].db[0x1a]=pBuffer[i];
			else 
			if(i+address<sizeof(UHCI))
			{
				if((1<<(i+address))&0xf000c)// r/wc bytes
				uhci[no].db[i+address]&=~pBuffer[i];
				else
				uhci[no].db[i+address]=pBuffer[i];
			
			}
		}
		//
		// hcreset.0
		//
		uhci[no].db[0]&=~2;
	}
	else
	{
		for(int i=0;i<dwBytes;i++)
		{
			if(i+address<sizeof(EHCI))
			{
				if(i+address<0x64&&i+address>0x44)// r/wc bytes
				ehci.db[i+address]&=~pBuffer[i];
				else
				ehci.db[i+address]=pBuffer[i];
			}
		}
	}
}

void dump_link(unsigned long addr  );

void dump_td(unsigned long addr)
{
	union
	{
		unsigned long dd;
		TD_PID pid;  
		TD_CTRL_STATUS ctrl_status;
	};

	if(addr >=CFG_RAM_SIZE-0x10)
		return;

	log0("TD at %x\n",addr);

	dd=mch.dword(addr+8);
	log0("Ctrl/Staus :%x\n",addr+4);

	dd=mch.dword(addr+8);
	log0("PID :%x\n",addr);

	dd=mch.dword(addr+0xc);
	log0("buffer:  %x\n",addr);
	dump_link(addr);
}


void dump_qh(unsigned long addr)
{
	if(addr >=CFG_RAM_SIZE-8)
		return;
	log0("QH at %x\n",addr);
	log0("element list:[\n");
	dump_link(addr+4);
	log0("]\n");
	dump_link(addr);
}

void dump_link(unsigned long addr  )
{
	union
	{
		unsigned long dd;
		UHCI_FRAME_POINTER link;  
	};
	if(addr <CFG_RAM_SIZE)
	dd=mch.dword(addr);
	else 
		return;
	//
	// following link
	//
	if(link.T==0)
	{
		if(link.qh_td==0)//td
		{
			dump_td(link.lp<<4);
		}
		else // qh
		{
			dump_qh(link.lp<<4);
		}
	}
}

void dump_frame_list()
{
	int i,j;
	for(i=0;i<4;i++)
	{
		unsigned long base=uhci[i].s.frbaseaddr;
		log0("UHCI %d, base:%x, rame number:%x:\n",i,base,uhci[i].s.frnum);

		base &=~0x3ff;
		for(j=0;j<0x1000;j+=4)
		{
			dump_link(base+j);
		 
		}

	}
}


void usb_schedule()
{
	int i;

	 
	
	for(i=0;i<4;i++)
	{
		if(uhci[i].s.usbcmd.rs==1)
		{
			uhci[i].s.frnum++;
			if(uhci[i].s.usbintr&4) 
				pci_raise_int(config_space_82801_uhci[i].pciHeader.InterruptPin);
		}
	}

}