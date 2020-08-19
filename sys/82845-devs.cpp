#include "pci-devs.h"
#include "irq-wire.h"
#include "log.h"

//
// ehci, sits at: bus:0,dev:1d, function:7
//
ICH82801_EHCI config_space_82801_ehci=	{
0x80,                 // configuration space size
true,                  // enabled,
{0xef,-1,-1,0x40,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are  read only
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

{0},
0x1,   // pwr_cap
0x58,  // nxt_ptr1
(short)0xc9c2,// pwr_cap
0,     // pwr_cntl_sts,
{0},
0xa,    // debug_capid
0x00,   // nxt_ptr2
(short)0x20a0,//debug_base
{0},
0x20,  //relnum
0x20,  //fl_adj
0x1ff, //pwake_cap
{0},
1,      // leg_nxt_cap
0,
};

C82801EHCI ich82801_ehci(PCI_FUN_ADDR(0,0x1d,7),(void*)&config_space_82801_ehci);


//
// ehci, sits at: bus:0,dev:1d, function:0/1/2/3
//
ICH82801_UHCI config_space_82801_uhci[4]=  
{

{
0xc5,                 // configuration space size
true,                  // enabled,
{0xff,0xfe,-1,-1,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are not read only
{0,0,0,0,1|~0x1f,0},        
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24D2,            // device id
	0x000,             // address
	0x0280,            // status
	0x03,              // revision id
	{0,3,0x0c},        // class code
	0,                 // cache line size
	0,                 // latency timer
	0x80,              // header type:multifunction,format 0
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

{0},
0x10,   // usb_relnum
{0},
0x2000, // usb_lekey
0,      // usb_res
0,
}
,

{
0xc5,                 // configuration space size
true,                  // enabled,
{-1,-2,-1,-1,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are not read only
{0,0,0,0,1|~0x1f,0},        
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24D4,            // device id
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

{0},
0x10,   // usb_relnum
{0},
0x2000, // usb_lekey
0,      // usb_res
1,
}
,
{
0xc5,                 // configuration space size
true,                  // enabled,
{-1,-2,-1,-1,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are not read only
{0,0,0,0,1|~0x1f,0},    // 0x20 register space    
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24D7,            // device id
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

{0},
0x10,   // usb_relnum
{0},
0x2000, // usb_lekey
0,      // usb_res
2,
}
,
{
0xc5,                 // configuration space size
true,                  // enabled,
{-1,-2,-1,-1,-1,-1,-2,-1},     // all unimplementedregisters &  reg 0 are not read only
{0,0,0,0,1|~0x1f,0},   // 0x20 register space    
                       // pci header
//pciheader.pciHeader=
{
	0x8086,            // vendor id
	0x24de,            // device id
	0x000,             // address
	0x0280,            // status
	0x03,              // revision id
	{0,3,0x0c},        // class code
	0,                 // cache line size
	0,                 // latency timer
	00,                // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0x0,               //SubSystem VendorId
	0x0,               //SubSystemId    
	-1,                //Expansion ROM Base
	0,                 //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0,                 // int line
	1,                 // int pin
	0,                 // min grabt
	0                  // latency
},

{0},
0x10,   // usb_relnum
{0},
0x2000, // usb_lekey
0,      // usb_res
3
}

};

C82801UHCI ich82801_uhci0(PCI_FUN_ADDR(0,0x1d,0),(void*)&config_space_82801_uhci[0]);
C82801UHCI ich82801_uhci1(PCI_FUN_ADDR(0,0x1d,1),(void*)&config_space_82801_uhci[1]);
C82801UHCI ich82801_uhci2(PCI_FUN_ADDR(0,0x1d,2),(void*)&config_space_82801_uhci[2]);
C82801UHCI ich82801_uhci3(PCI_FUN_ADDR(0,0x1d,3),(void*)&config_space_82801_uhci[3]);



//
// , sits at: bus:0,dev:1f, function:0
//
ICH82801_LPC_IF config_space_82801_lpc=  
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
}

};
C82801LPC ich82801_lpc(PCI_FUN_ADDR(0,0x1f,0),(void*)&config_space_82801_lpc);

//
// , sits at: bus:0,dev:1f, function:1
//
ICH82801_IDE config_space_82801_ide=  
{

0x55,                 // configuration space size
true,                  // enabled,
{0x31,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
{0xfffffff9,0xfffffffd,0xfffffff9,0xfffffffd,0xfffffff1,0},        
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
	1,                 // int pin
	0,                 // min grabt
	0                  // latency
},

// byte:0x40
0,0,//0x40-41 Primary IDE Timing
0,0,//0x42-43 Secondary IDE Timing
0,  //0x44 Slave IDE Timing
{0},//
0,  //0x48 Synchronous DMA Control Register
0,
0, // 0x4a-4bSynchronous DMA Timing Register
{0},
0  //0x54IDE I/O Configuration Register

};

C82801IDE ich82801_ide(PCI_FUN_ADDR(0,0x1f,1),(void*)&config_space_82801_ide);


//
// , sits at: bus:0,dev:1f, function:2
//
//ICH82801_SATA config_space_82801_sata;
//C82801SATA ich82801_sata(PCI_FUN_ADDR(0,0x1f,2),config_space_82801_sata);


//
// , sits at: bus:0,dev:1f, function:3
//
ICH82801_SMBUS config_space_82801_smbus=  
{
0x44,                 // configuration space size
true,                  // enabled,
{1,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
{0,0,0,0,1|(~0x1f),0},         // bar5  smbus base 
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
}
};
C82801SMBUS ich82801_smbus(PCI_FUN_ADDR(0,0x1f,3),(void*)&config_space_82801_smbus);






//
// , sits at: bus:0,dev:1e, function:0
//
ICH82801_PCIBRIDGE_IF config_space_82801_pci_bridge=  
{
	{
		0x100,                 // configuration space size
		true,                  // enabled,
		{1,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
		{0,0,0,0,0,0},// bar 0, io space. size0x10 
							   // pci header type 1
	},
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
		                 
		{0},              // reserved ,0ffset:0xf~0x17
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

	// byte:0x40
	0x76202802,
	0,
	{0},
	0x406402,
	{0},
	0x20,
	{0},
	0,
	{0},
	0,
	{0},
	0
};

C82801PCIBRIDGE ich82801_pci_bridge(PCI_FUN_ADDR(0,0x1e,0),(void*)&config_space_82801_pci_bridge);



//
// , sits at: bus:0,dev:0, function:0
//
MCH82845_HOST_IF config_space_82845_host=
{

{

	0x100,                 // configuration space size
	true,                  // enabled,
	{1,0,0,0xc0,0,0,2,0},     // all registers but reg 0 are not read only
	{0xff000008,0,0,0,0,0},   // bar 0, apbase  16M,mem
},
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
},


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


};

C82845HOST mch82845_host_if(PCI_FUN_ADDR(0,0,0),(void*)&config_space_82845_host);


//
// , sits at: bus:0,dev:0, function:1
//
MCH82845_PCIBRIDGE_IF config_space_82845_pci_bridge=  
{
{
	0x100,                 // configuration space size
	true,                  // enabled,
	{1,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
	{0,0,0,0,0,0}// bar 0, io space. size0x10 
},
                       // pci header type 1
{
	0x8086,            // vendor id
	0x1a31,            // device id
	0x0006,            // cmd
	0x00a0,            // status
	0x03,              // revision id
	{00,0x04,0x06},    // class code
	0,                 // cache line size
	0,                 // latency timer
	1,                 // header type
	                   
	{0},               // reserved:offset:0xf~0x17
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
}
};

C82845PCIBRIDGE mch82845_pci_bridge(PCI_FUN_ADDR(0,1,0),(void*)&config_space_82845_pci_bridge);





unsigned char ich82801_get_acpi_cntl()
{
	return config_space_82801_lpc.u.s.acpi_cntl;
}

void dump_irq_rout()
{
	for(int i=0;i<4;i++)
		log0("INT#%c->IRQ%d\n",'A'+i,config_space_82801_lpc.u.s.pirq_rout0[i]&0xf);
	for(int i=0;i<4;i++)
		log0("INT#%c->IRQ%d\n",'E'+i,config_space_82801_lpc.u.s.pirq_rout1[i]&0xf);
}


void pci_raise_int(int no)
{
	if(no<4) assert_irq(config_space_82801_lpc.u.s.pirq_rout0[no]&0xf);
	else if(no<8) assert_irq(config_space_82801_lpc.u.s.pirq_rout1[no-4]&0xf);
	else log0("bad pirq#%d\n",no);
}


#define SH sizeof(CONFIG_SPACE_HDR)
#define SAVE_DAT(x) if(!err)  if (fwrite((char *)&x.pciHeader,1,sizeof(x)-SH,wfp)!=sizeof(x)-SH) err=true;
#define LOAD_DAT(x) if(!err)   if (fread((char *)&x.pciHeader,1,sizeof(x)-SH,rfp)!=sizeof(x)-SH) err=true;

bool pci_save_states(FILE *wfp)
{
	bool err=false;

	SAVE_DAT(config_space_82801_ehci);
	SAVE_DAT(config_space_82801_uhci[0]);
	SAVE_DAT(config_space_82801_uhci[1]);
	SAVE_DAT(config_space_82801_uhci[2]);
	SAVE_DAT(config_space_82801_uhci[3]);
	SAVE_DAT(config_space_82801_lpc);
	SAVE_DAT(config_space_82801_ide);
	SAVE_DAT(config_space_82801_smbus);
	SAVE_DAT(config_space_82801_pci_bridge);
	SAVE_DAT(config_space_82845_host);
	SAVE_DAT(config_space_82845_pci_bridge);
	return err;
}

bool pci_load_states(FILE *rfp)
{
	bool err=false;

	LOAD_DAT(config_space_82801_ehci);
	LOAD_DAT(config_space_82801_uhci[0]);
	LOAD_DAT(config_space_82801_uhci[1]);
	LOAD_DAT(config_space_82801_uhci[2]);
	LOAD_DAT(config_space_82801_uhci[3]);
	LOAD_DAT(config_space_82801_lpc);
	LOAD_DAT(config_space_82801_ide);
	LOAD_DAT(config_space_82801_smbus);
	LOAD_DAT(config_space_82801_pci_bridge);
	LOAD_DAT(config_space_82845_host);
	LOAD_DAT(config_space_82845_pci_bridge);
	return err;
}


//void bus_master_notify()
//{
//	pci_raise_int(config_space_82801_ide.pciHeader.InterruptPin);
//}