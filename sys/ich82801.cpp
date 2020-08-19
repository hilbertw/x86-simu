#include <assert.h>
#include "pci.h"
#include "mch.h"
#include "acpi82801.h"
#include "smbus.h"
#include "usb.h"
#include "log.h"
extern CPCIBus pci_bus;


static void ich82801_device1f_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext);
static void ich82801_device1e_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext);
static void ich82801_ide_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext);
static void ich82801_smbus_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext);
static void ich82801_uhci_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext);
static void ich82801_ehci_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext);

//PCI to LPC Bridge1
// bus:0, dev:1f, func:0, configuration space:0~100
struct isa_bridge_def
{
	PCI_FUNCTION common; // must be  the  first member

	DWORD other_reg[0x40-sizeof(PCI_HEADER)/sizeof(DWORD)];

} device1f=  
{

0x100,                 // configuration space size
true,                  // enabled,
{0x31,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
{0xffffff01,0xffffffc1,0,0,0,0},         // bar 0, emu pmbase
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x2440,            // device id
	0x000F,            // address
	0x0280,            // status
	0x03,              // revision id
	{00,0x01,0x06},    // class code
	0,                 // cache line size
	0,                 // latency timer
	0x80,              // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0,                 //SubSystem VendorId
	0,                 //SubSystemId    
	0,                 //Expansion ROM Base
	0xa0,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,                 // int line
	0,                 // int pin
	0,                 // min grabt
	0                  // latency
},

{// byte:0x40
1, //ACPI Base Address Register
0, //ACPI Control
0x00000,
0,
// byte:0x50
0,
0,
0x000,
0,
00000,    //60
0x0,
0x00000,
0x0000000,
0x0000000,   //70
0x0000,
0x10,       // 78:DRAM Timing Register
0x000000,
0x0000,   //80
0,
0,
0x00000,
0x000000,  //90
0,
0,
0x00380200,  // 9d:System Management RAM Control,9e:Extended System Mgmt RAM Control
0x00000200,  //a0:GEN_PMCON_1,This read-only bit indicates the current state of the PWRBTN# signal.
             // a2:System Reset Status (SRS)¡ª R/WC. 
0x0,         // a4:GEN_PMCON_3,
0x0201CD00,  //a8:STPCLK_DEL
0x0,
0x0000,  // b0
0,
0,
0x0,
0x0,  // c0
0,
0,
0,
0x00000000,  // d0
0x00000004,  //D4h¨CD7h GEN_STS General Status             
0,
0,
0x0000000,  // e0
0xffff01,  //
0x00112233,//E8h¨CEBh FWH_SEL1 FWH Select 1 
0x56780000,//ec:LPC I/F General 2 Decode Range.ee:FWH Select 2
0x0000,  // f0
0x000000,
0,
0x00000   //fc
}

};
// bus:0, dev:1f, func:1, configuration space:0~58h
struct ide_interface_def
{
	PCI_FUNCTION common; // must be  the  first member

	char other_reg[0x55-sizeof(PCI_HEADER)];

} ich82801_ide=  
{

0x55,                 // configuration space size
true,                  // enabled,
{0x31,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
{0xfffffff9,0xffffffff,0xfffffff9,0xffffffff,0xfffffff1,0},        
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24CB,            // device id
	0x000,            // address
	0x0280,            // status
	0x03,              // revision id
	{0x8a,0x01,0x01},  // class code
	0,                 // cache line size
	0,                 // latency timer
	0x80,              // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0,                 //SubSystem VendorId
	0,                 //SubSystemId    
	0,                 //Expansion ROM Base
	0xa0,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,                 // int line
	0,                 // int pin
	0,                 // min grabt
	0                  // latency
},

{// byte:0x40
0,0,//0x40-41 Primary IDE Timing
0,0,//0x42-43 Secondary IDE Timing
0,  //0x44 Slave IDE Timing
0,0,0,//
0,  //0x48 Synchronous DMA Control Register
0,
0, // 0x4a-4bSynchronous DMA Timing Register
0,0,0,0,0,0,0,0,
0  //0x54IDE I/O Configuration Register
}

};


// bus:0, dev:1e, func:0, configuration space:0~100
struct hub_pci_bridge_def
{
	PCI_FUNCTION1 common; // must be  the  first member

	DWORD other_reg[0x40-sizeof(PCI_HEADER)/sizeof(DWORD)];

} device1e=  
{
0x100,                 // configuration space size
true,                  // enabled,
{1,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
{0,0,0,0,0,0},// bar 0, io space. size0x10 
                       // pci header type 1
{
	0x8086,            // vendor id
	0x2448,            // device id
	0x0001,            // address
	0x0080,            // status
	0x03,              // revision id
	{00,0x04,0x06},    // class code
	0,                 // cache line size
	0,                 // latency timer
	1,                 // header type
	0,                 // BIST
	{0,0},             // bar
	0,                 //PrimaryBusNumber
	0,                 //SecondaryBusNumber
	0,                 //SubordinateBusNumber
	0,                 //SecondaryLatencyTimer
	0xf0,              //IOBase
	0,                 //IOLimit
	0x280,             //dwSecondaryStatus
	0xfff0,            //dwMemBase
	0,                 //dwMemLimit
	0xFFF0,            //dwPrefetchableMemBase
	0,                 //dwPrefetchableMemLimit
	0,                 //ddPrefetchableMemBaseHi
	0,                 //ddPrefetchableMemLimitHi
	0,                 //dwIOBaseHi
	0,                 //dwIOLimitHi
	0xa0,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 //Expansion ROM Base
    0,                  // int line
	0,                  // int pin
	0                   // dwBridgeControl
},

{// byte:0x44
0,
0x00000,
0,
// byte:0x50
0,
0,
0x000,
0,
00000,    //60
0x0,
0x00000,
0x0000000,
0x0000000,   //70
0x0000,
0x10,       // 78:DRAM Timing Register
0x000000,
0x0000,   //80
0,
0,
0x00000,
0x000000,  //90
0,
0,
0x00380200,  // 9d:System Management RAM Control,9e:Extended System Mgmt RAM Control
0x200002,  //a0:AGP Capability Identifier
0x1F000216,//AGP Status
0x0201CD00,//AGP Command
0x0,
0x0000,  // b0
0,
0,
0x0,
0x0,  // c0
0x00000000,  // d0
0,
0,
0,
0x0000000,  // e0
0xF104A009,  //Product Specific Capability Identifier
0,
0,
0x0000,  // f0
0x000000,
0,
0x00000   //fc
}

};

// bus:0, dev:1f, func:0, configuration space:0~40
struct ich82801_smbus_def
{
	PCI_FUNCTION common; // must be  the  first member

	DWORD other_reg[1];

} ich82801_smbus=  
{
0x44,                 // configuration space size
true,                  // enabled,
{1,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
{0,0,0,0,0xffffff01,0},         // bar5  smbus base 
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24c3,            // device id
	0x000F,            // address
	0x0280,            // status
	0x03,              // revision id
	{00,0x05,0x0c},    // class code
	0,                 // cache line size
	0,                 // latency timer
	0x80,              // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0,                 //SubSystem VendorId
	0,                 //SubSystemId    
	0,                 //Expansion ROM Base
	0xa0,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,                 // int line
	0,                 // int pin
	0,                 // min grabt
	0                  // latency
},

{// byte:0x40
	
}

};


// bus:0, dev:1f, func:1, configuration space:0~58h
struct usb_interface_def
{
	PCI_FUNCTION common; // must be  the  first member

	char other_reg[0xc5-sizeof(PCI_HEADER)];

} ich82801_usb[3]=  
{

{
0xc5,                 // configuration space size
true,                  // enabled,
{0xf,0xfc,-1,-1,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are not read only
{0,0,0,0,1|~0x1f,0},        
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24C2,            // device id
	0x000,             // address
	0x0280,            // status
	0x03,              // revision id
	{0,3,0x0c},        // class code
	0,                 // cache line size
	0,                 // latency timer
	0x80,                 // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0,                 //SubSystem VendorId
	0,                 //SubSystemId    
	-1,                //Expansion ROM Base
	0,                 //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,                 // int line
	1,                 // int pin
	0,                 // min grabt
	0                  // latency
},

{// byte:0x40
0
}
}
,

{
0xc5,                 // configuration space size
true,                  // enabled,
{0xf,0xfc,-1,-1,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are not read only
{0,0,0,0,1|~0x1f,0},        
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24C4,            // device id
	0x000,            // address
	0x0280,            // status
	0x03,              // revision id
	{0,3,0x0c},    // class code
	0,                 // cache line size
	0,                 // latency timer
	0,                 // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0x0,            //SubSystem VendorId
	0x0,            //SubSystemId    
	-1,                 //Expansion ROM Base
	0,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,              // int line
	2,              // int pin
	0,               // min grabt
	0                // latency
},

{// byte:0x40
0
}
}
,
{
0xc5,                 // configuration space size
true,                  // enabled,
{0xf,0xfc,-1,-1,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are not read only
{0,0,0,0,1|~0x1f,0},    // 0x20 register space    
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24C7,            // device id
	0x000,            // address
	0x0280,            // status
	0x03,              // revision id
	{0,3,0x0c},    // class code
	0,                 // cache line size
	0,                 // latency timer
	00,                 // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0x0,            //SubSystem VendorId
	0x0,            //SubSystemId    
	-1,                 //Expansion ROM Base
	0,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,              // int line
	3,              // int pin
	0,               // min grabt
	0                // latency
},

{// byte:0x40
0
}
}

};

// bus:0, dev:1f, func:1, configuration space:0~58h
struct ehci_interface_def
{
	PCI_FUNCTION common; // must be  the  first member

	char other_reg[0x80-sizeof(PCI_HEADER)];

} ich82801_ehci=	{
0x80,                 // configuration space size
true,                  // enabled,
{0xf,0xfc,-1,-1,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are not read only
{~0x3ff,0,0,0,0,0},    // bar0, 1K memory space    
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24dd,            // device id
	0x000,            // address
	0x0290,            // status
	0x03,              // revision id
	{0,3,0x0c},       // class code
	0,                 // cache line size
	0,                 // latency timer
	00,                 // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0x0,                //SubSystem VendorId
	0x0,                //SubSystemId    
	-1,                 //Expansion ROM Base
	0x50,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,                 // int line
	4,                 // int pin
	0,                 // min grabt
	0                // latency
},

{// byte:0x40
0
}

};

static unsigned char data[0x10], gpio_reg[0x40];

//unsigned char  acpi_reg[0x80];
//
//static unsigned char acpi_reg_attr[0x10]={
//	3,0,0,0,0,0x0c,0xc0,0x0c,
//	2,0,0,0,0,0x00,0x80,0x01};
//
// 32 bit pci.don't care about ddAddrHi 
//
static void ich82801_device1f_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext)
{
		assert((bar_num==0&&ddAddrLo<0x80&&ddAddrLo+dwBytes<0x80)
			||(bar_num==1&&ddAddrLo<0x40&&ddAddrLo+dwBytes<0x40));



	if(Op==IO_WR)
	{
		if(bar_num==0)
		{
			
			unsigned  int i;
			for(i=0;i<dwBytes;i++)
			{
				acpi82801.set_reg(ddAddrLo+i,pBuffer[i]);
			/*	if((acpi_reg_attr[i>>3]&(1<<(i&7)))==0)
					acpi_reg[ddAddrLo]= *pBuffer;// r/w
				else 
					acpi_reg[ddAddrLo]&= *pBuffer;// r/wc*/
			}
			
			
		}
		else
		{
		if(dwBytes==4) *(DWORD *)(gpio_reg+ddAddrLo)= *(DWORD *)pBuffer;
		else if(dwBytes==2) *(WORD *)(gpio_reg+ddAddrLo)= *(WORD *)pBuffer;
		if(dwBytes==1) *(gpio_reg+ddAddrLo)= *pBuffer;	
		}
	}
	else 	if(Op==IO_RD)
	{
		if(bar_num==0)
		{
			unsigned int i;
			for(i=0;i<dwBytes;i++)
				pBuffer[i]=acpi82801[ddAddrLo+i];
		}
		else
		{
			if(dwBytes==4)  *(DWORD *)pBuffer=*(DWORD *)(gpio_reg+ddAddrLo);
			else if(dwBytes==2)  *(WORD *)pBuffer=*(WORD *)(gpio_reg+ddAddrLo);
			if(dwBytes==1)  *pBuffer=*(gpio_reg+ddAddrLo);
		}

	}
#if LOG_GPIO
	if(bar_num!=0)

	log4("GPIO",(Op==IO_RD)?'r':'w',ddAddrLo,
	    dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned long *)pBuffer:0);
#endif	

#if LOG_ACPI
	if(bar_num==0)
		log4("ACPI",(Op==IO_RD)?'r':'w',ddAddrLo,
		dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned long *)pBuffer:0);
#endif	

}

static void ich82801_device1e_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext)
{
	assert(dwBytes==1);
	if(Op==IO_WR)
	{
		assert(ddAddrLo<0x10);
		data[ddAddrLo]=pBuffer[0];
	}
	else 	if(Op==IO_RD)
	{
		assert(ddAddrLo<0x10);


	
		pBuffer[0]=data[ddAddrLo];

		if(ddAddrLo==0) data[0]++;
	}

}
char bus_master[0x10];

static void ich82801_ide_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext)
{
	char * ide_desc[]={"pri address","pri contrl","sec address","sec contrl","bus master"};


	if(Op==IO_WR)
	{
		if(bar_num==4) 
		{
			if(dwBytes==1)bus_master[ddAddrLo]=pBuffer[0];
			else if(dwBytes==2)*(short *)&bus_master[ddAddrLo]=*(short *)pBuffer;
			else if(dwBytes==4)*(int *)&bus_master[ddAddrLo]=*(int *)pBuffer;
		}
	}
	else if(Op==IO_RD)
	{
		if(bar_num==4) 
		{
			if(dwBytes==1)pBuffer[0]=bus_master[ddAddrLo];
			else if(dwBytes==2)*(short *)pBuffer=*(short *)&bus_master[ddAddrLo];
			else if(dwBytes==4)*(int *)pBuffer=*(int *)&bus_master[ddAddrLo];
		}
	}
	log("IDE %s : %c %x %x\n",ide_desc[bar_num],(Op==IO_RD)?'r':'w',ddAddrLo,
	 	dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned int *)pBuffer:0);
}

static void ich82801_uhci_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext)
{
	if(Op==IO_WR)
	{
	}
	else if(Op==IO_RD)
	{
		if(dwBytes==1)* pBuffer=0;
		else if(dwBytes==2)* (WORD*)pBuffer=0;
		else * (DWORD*)pBuffer=0;


	}
#if LOG_USB	
	log("USB %c: %x %x\n",(Op==IO_RD)?'r':'w',ddAddrLo,
	 	dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned int *)pBuffer:0);
#endif
}

static void ich82801_ehci_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext)
{
	if(Op==IO_WR)
	{
		usb_write(ddAddrLo,pBuffer,dwBytes,context);
	}
	else if(Op==MEM_RD)
	{
		usb_read(ddAddrLo,pBuffer,dwBytes,context);
		if(dwBytes==1)* pBuffer=0;
		else if(dwBytes==2)* (WORD*)pBuffer=0;
		else * (DWORD*)pBuffer=0;


	}
#if LOG_USB	
	log("USB %d %c: %x %x\n",context,(Op==MEM_RD)?'r':'w',ddAddrLo,
	 	dwBytes==1? (unsigned char)pBuffer[0]:
		dwBytes==2? *(unsigned short *)pBuffer:
		dwBytes==4? *(unsigned int *)pBuffer:0);
#endif
}


static void ich82801_smbus_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext)
{
	assert(dwBytes==1);
	if(Op==IO_WR)
	{
		assert(ddAddrLo<SMBUS_BUFFSIZE);
		if(ddAddrLo==HST_STS)
			CSMBUS::buffer[ddAddrLo]&=~pBuffer[0];
		else
		CSMBUS::buffer[ddAddrLo]=pBuffer[0];
	}
	else 	if(Op==IO_RD)
	{
		assert(ddAddrLo<SMBUS_BUFFSIZE);

		pBuffer[0]=CSMBUS::buffer[ddAddrLo];
		//INUSE_STS ¡ª R/WC (special). This bit is used as semaphore among various independent
		//software threads that may need to use the ICH4¡¯s SMBus logic, and has no other effect on
		//hardware.
		//0 = After a full PCI reset, a read to this bit returns a 0.
		//1 = After the first read, subsequent reads will return a 1. A write of a 1 to this bit will reset the next
		//read value to 0. Writing a 0 to this bit has no effect. Software can poll this bit until it reads a 0,
		//and will then own the usage of the host controller.
		if(ddAddrLo==HST_STS)
		{
			CSMBUS::buffer[ddAddrLo]|=0x40;
		}
	}

#if LOG_SMBUS	
//	log4("SMBUS",(Op==IO_RD)?'r':'w',ddAddrLo,*pBuffer);
#endif
	//
	// start a  address
	//
	if(ddAddrLo==2&&(pBuffer[0]&0x40)!=0) 
	{
#if LOG_SMBUS
		log("SMBUS tx:%x address:%x type (%s)  (%c)\n",
		CSMBUS::buffer[XMIT_SLVA]>>1,
		CSMBUS::buffer[HST_CMD],
		CSMBUS::cmd_type[(CSMBUS::buffer[HST_CNT]>>2)&7],
		(CSMBUS::buffer[XMIT_SLVA]&1)?'r':'w');
#endif
		CSMBUS::buffer[HST_STS]=CSMBUS::buffer[HST_CMD]==0xd?0x88:0x82;
		CSMBUS::buffer[HST_D0]=
			CSMBUS::buffer[HST_CMD]==0x2?(CSMBUS::buffer[XMIT_SLVA]==0xa3?7:0):
			CSMBUS::buffer[HST_CMD]==0x15?0:
			CSMBUS::buffer[HST_CMD]==0xb ?0x2:
			CSMBUS::buffer[HST_CMD]==0x5 ?0:0x1b;
		
	}
}

static void ich82801_ConfigCallback1f(int cmd,unsigned int address,void * pContext)
{
	// set acpi base register
	if((address&0xfc)==0x40)
	{
		device1f.common.pciHeader.ddBase[0]=((DWORD*)&device1f.common.pciHeader)[0x10]&(-2);
		printf("acpi base:%x %x\n",((DWORD*)&device1f.common.pciHeader)[0x10],device1f.common.pciHeader.ddBase[0]);
	}
	// gpio base
	else if((address&0xfc)==0x58)
	{
		device1f.common.pciHeader.ddBase[1]=((DWORD*)&device1f.common.pciHeader)[0x58/4]&(-2);
		printf("gpio base:%x %x\n",((DWORD*)&device1f.common.pciHeader)[0x58/4],device1f.common.pciHeader.ddBase[1]);
	}
	else if(address>=0xa0&&address<=0xa8)
	{

		//log4("General PM Configuration",cmd,address,*(long *)((char*)&device1f.common.pciHeader+address));

		if(0xa1==address)
		((char*)&device1f.common.pciHeader)[0xa1]=2;//PWRBTN_LVL ¡ª RO. This read-only bit indicates 
		                                            //the current state of the PWRBTN# signal.
													//0 = Low.
													//1 = High. 	
	}	
}


static void ich82801_smbus_ConfigCallback(int cmd,unsigned int address,void * pContext)
{
	if((address&0xfc)==0x20)
	{
		log2("smbus base",((DWORD*)&ich82801_smbus.common.pciHeader)[8]);
	}
}

static void ich82801_uhci_ConfigCallback(int cmd,unsigned int address,void * pContext)
{
	if((address&0xfc)==0x20)
	{
		log("USB %d %c Base:%x\n",context,cmd,((DWORD*)&ich82801_usb[context].common.pciHeader)[8]);
	}
}
static void ich82801_ide_ConfigCallback(int cmd,unsigned int address,void * pContext)
{
	if(address>=0x10&&address<=0x13)
	log2("ide pri cmdbase",ich82801_ide.common.pciHeader.ddBase[0]);
    else if(address>=0x14&&address<=0x17)
	log2("ide pri ctrlbase",ich82801_ide.common.pciHeader.ddBase[1]);
    else if(address>=0x18&&address<=0x1b)
	log2("ide sec cmdbase",ich82801_ide.common.pciHeader.ddBase[2]);
    else if(address>=0x1c&&address<=0x1f)
	log2("ide sec ctrl base",ich82801_ide.common.pciHeader.ddBase[3]);
    else if(address>=0x20&&address<=0x23)
	log2("ide bus master base",ich82801_ide.common.pciHeader.ddBase[4]);
}
static void ich82801_null_ConfigCallback(int cmd,unsigned int address)
{
}
static void ich82801_ConfigCallback1e(int cmd,unsigned int address,void * pContext)
{
}
void ich82801_register()
{
	//PM1a_Cnt.pm1_scontrol.SLP_TYPx=0;
	//PM1a_Cnt.pm1_scontrol.SLP_EN=0;
	//Global Release GBL_RLS=0;
	// SCI Enable SCI_EN=0;
	acpi82801.r[0x44]=0xff;
	*(long *)&acpi82801.r[4]=0;
	*(long *)&acpi82801.r[0]=0x8500;//15:Wake Status (WAK_STS) 
	                            //10:RTC_STS
	                             // 8:PWRBTN_STS

	// bus 0, dev 0, func 0
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x1f,0),(void*)&device1f,ich82801_ConfigCallback1f,DefaultDoTYpe1AddrSel,ich82801_device1f_busop);
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x1e,0),(void*)&device1e,ich82801_ConfigCallback1e,DefaultDoTYpe1AddrSel,ich82801_device1e_busop);
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x1f,3),(void*)&ich82801_smbus,ich82801_smbus_ConfigCallback,DefaultDoTYpe1AddrSel,ich82801_smbus_busop);
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x1f,1),(void*)&ich82801_ide,ich82801_ide_ConfigCallback,DefaultDoTYpe1AddrSel,ich82801_ide_busop);
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x1d,0),(void*)&ich82801_usb[0],ich82801_uhci_ConfigCallback,DefaultDoTYpe1AddrSel,ich82801_uhci_busop);
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x1d,1),(void*)&ich82801_usb[1],ich82801_uhci_ConfigCallback,DefaultDoTYpe1AddrSel,ich82801_uhci_busop);
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x1d,2),(void*)&ich82801_usb[2],ich82801_uhci_ConfigCallback,DefaultDoTYpe1AddrSel,ich82801_uhci_busop);
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x1d,7),(void*)&ich82801_ehci,ich82801_ehci_ConfigCallback,DefaultDoTYpe1AddrSel,ich82801_ehci_busop);
}


	unsigned char ich82801_get_sci_irq()
	{
		return (unsigned char)((DWORD*)&device1f.common.pciHeader)[0x44];
	}