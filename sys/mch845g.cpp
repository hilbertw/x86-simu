#include <assert.h>
#include "pci.h"
#include "mch.h"
#include "log.h"
extern CPCIBus pci_bus;
void __cdecl vt_show_smram(bool opened, bool enabled);

extern CMCH mch;  // 845g will update pam

static void mch845g_device0_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext);
static void mch845g_device1_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext);


// bus:0, dev:0, func:0, configuration space:0~100
struct host_hub_interface_bridge_def
{
	PCI_FUNCTION common; // must be  the  first member

	DWORD other_reg[0x30];

} device0=  

{

{

0x100,                 // configuration space size
true,                  // enabled,
{1,0,0,0xc0,0,0,2,0},     // all registers but reg 0 are not read only
{0xff000008,0,0,0,0,0},   // bar 0, apbase  16M,mem
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x1a30,            // device id
	0x0006,            // cmd
	0x0090,            // status
	0x03,              // revision id
	{00,0x00,0x06},    // class code
	0,                 // cache line size
	0,                 // latency timer
	0,                 // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0x0,            //SubSystem VendorId
	0x0,            //SubSystemId    
	0,                 //Expansion ROM Base
	0xa0,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,              // int line
	0,              // int pin
	0,               // min grabt
	0                // latency
}

},

{
// byte:0x40
0,
0,
0,
0,

0,// byte:0x50
0,
0,
0,

0,    //60
0,
0,
0,
0,   //70
0,
0xff000020,       // 78:DRAM Timing Register
0x20005001,        //7e:no ECC
0,          //80
0,
0,
0,
0,          //90
0,
0,
0x00380200, // 9d:System Management RAM Control,9e:Extended System Mgmt RAM Control
0x200002,  //a0:AGP Capability Identifier
0x1F000216,//AGP Status
0x0201CD00,//AGP Command
0x0,
0x0000,  // b0
0,
0,
0x0,
0x0,  // c0
0x08001000, // c4:Top of Low Memory, 256M,c7:133M sdram
0x0,
0x0,
0x00000000,  // d0
0,
0,
0,
0x0000000,  // e0
0x20000000,  //Product Specific Capability Identifier
0,
0,
0x0000,  // f0
0x000000,
0,
0x00000   //fc
}

};


// bus:0, dev:0, func:0, configuration space:0~100
struct bridge_def
{
	PCI_FUNCTION1 common; // must be  the  first member

	DWORD other_reg[0x40-sizeof(PCI_HEADER)/sizeof(DWORD)];

} device1=  
{
0x100,                 // configuration space size
true,                  // enabled,
{1,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
{0,0,0,0,0,0},// bar 0, io space. size0x10 
                       // pci header type 1
{
	0x8086,            // vendor id
	0x1a31,            // device id
	0x0006,            // address
	0x00a0,            // status
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
	0x2a0,             //dwSecondaryStatus
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
    0,                 // int line
	0,                 // int pin
	0                  // dwBridgeControl
},



};
//
// 32 bit pci.don't care about ddAddrHi 
//
static unsigned char data[0x10];
//
//


static void mch845g_device0_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext)
{
	if(Op==IO_WR)
	{
		//data[ddAddrLo]=pBuffer[0];
	}
	else 	if(Op==IO_RD)
	{
		//assert(ddAddrLo<0x10);
	
	//	memset(pBuffer,0,dwBytes);

		//if(ddAddrLo==0) 
	}
	else 	if(Op==MEM_RD)
	{
		//assert(ddAddrLo<0x10);
	
		memset(pBuffer,0,dwBytes);

		//if(ddAddrLo==0) 
	}
	else 	if(Op==MEM_WR)
	{
		//assert(ddAddrLo<0x10);
	
		//memset(pBuffer,0,dwBytes);

		//if(ddAddrLo==0) 
	}
}

static void mch845g_device1_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num,void * pContext)
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
static void mch845g_ConfigCallback0(int cmd,unsigned int address,void * pContext)
{
	//
	// read config reg, no op
	//
	if(cmd<'w') return;


	if(address==0x90)
	{//PAM0[7:4] R R WE RE 0F0000h¨C0FFFFFh BIOS Area 90h
		mch.set_pam_rd_en(0xf000,(((char*)&device0.common.pciHeader)[0x90]&0x10)?0xf000:0);
		mch.set_pam_wr_en(0xf000,(((char*)&device0.common.pciHeader)[0x90]&0x20)?0xf000:0);
	}
	else if(address<=0x96 &&address>0x90)
	{
		char c=((char*)&device0.common.pciHeader)[address];
		int pos=(address-0x91)<<1;

		int rd_enable=(c&1)|((c&0x10)>>3);

		int wr_enable=((c&2)|((c&0x20)>>3))>>1;

		mch.set_pam_rd_en(3<<pos,rd_enable<<pos);
		mch.set_pam_wr_en(3<<pos,wr_enable<<pos);
	}
	else if((address&0xfc)==0xc4)
	{
     //((char*)&device0.common.pciHeader)[0xc4]=0x0;
     //((char*)&device0.common.pciHeader)[0xc5]=0x2;
	}
	else if((address&0xfe)==0xc8)
	{

	
		//log("AGP ERROR:%x EAP:%x\n",
		//((unsigned short *)&device0.common.pciHeader)[0xc8],
		//((unsigned long *)&device0.common.pciHeader)[0x8c]
		//);                      

		((char*)&device0.common.pciHeader)[0xc8]=0x0;
	}
	else if((address&0xfc)==0x7c)
	{
             
	//	((char*)&device0.common.pciHeader)[0x7c]|=1;//ddr  sdram
	}

	else if((address)==0xb4)
	{
		//
		// set aperture base22:31
		//
		device0.common.pciHeader.ddBase[0]=

			(1+~(((unsigned char *)&device0.common.pciHeader)[0xb4]&0x1f))<<22;
	}
	else if((address&0xfc)==0x9c)
	{
		if(cmd>='w') 
		{
			bool opened,enabled;
			//
			// d_open=1, d_lock=0?_cls=1, d_lock=0?
			//
			opened=((((char*)&device0.common.pciHeader)[0x9d]&0x70)==0x40);
			//
			// d_en
			//
			enabled=((((char*)&device0.common.pciHeader)[0x9d]&0x8)==0x8);

			mch.open_smram(opened );
			mch.en_smram(enabled);

			log("SMRAM: %s,%s\n",opened?"Opened":"Closed",enabled?"Enabled":"Disabled");
			vt_show_smram(opened,enabled);

			if(0!=(((char*)&device0.common.pciHeader)[0x9d]&0x10))
			{
				//d_lock, once set, can not reset again, make 0x9c~0x9f readonly 
				device0.common.aReadOnlyFields[0x9c/32]|=((1<<((0x9c>>2)&0x7)));
			}
		}
	}
}

static void mch845g_ConfigCallback1(int cmd,unsigned int address,void * pContext)
{
}
void mch845g_register()
{
	// bus 0, dev 0, func 0
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0,0),(void*)&device0,mch845g_ConfigCallback0,DefaultDoTYpe1AddrSel,mch845g_device0_busop);
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,1,0),(void*)&device1,mch845g_ConfigCallback1,DefaultDoTYpe1AddrSel,mch845g_device1_busop);
}