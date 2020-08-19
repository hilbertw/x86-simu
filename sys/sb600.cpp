#include <assert.h>
#include "pci.h"
#include "excel.h"
extern CPCIBus pci_bus;
extern CExcelUtil ExcelUtil;
static void smbus_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes,int bar_num);

// bus:0, dev:14h, func:0, configuration space:0~100
struct smbus_def
{
	PCI_FUNCTION common; // must be  the  first member

	DWORD other_reg[0x40-sizeof(PCI_HEADER)/sizeof(DWORD)];

} smbus=  
{
smbus_busop,
0x100,                 // configuration space size
true,                  // enabled,
{1,0,0,0,0,0,0,0},     // all registers but reg 0 are not read only
{0xfffffff1,0,0,0,0,0},// bar 0, io space. size0x10 
                       // pci header
{
	0x1002,            // vendor id
	0x4385,            // device id
	0x0401,            // cmd
	0x0230,            // status
	0x13,              // revision id
	{00,0x05,0x0C},    // clas code
	0,                 // cache line size
	0,                 // latency timer
	0x80,              // header type:multifunction,format 0
	0,                 // BIST
	{0,0,0,0,0,0},     // bar
	0,                 //ddCISPointer
	0x1043,            //SubSystem VendorId
	0x8288,            //SubSystemId    
	0,                 //Expansion ROM Base
	0xB0,              //CapabilityPointer
	{0,0,0},           //reserved
	0,                 // reserved
    0xD4,              // int line
	0x2B,              // int pin
	0x0,               // min grabt
	0x4                // latency
},

{
// 0x40
0,
0,
0x0000FF0F,
0,
0,           //50
0,
0x0FF00B11,
0,
00240001,    //60
0x8356FDBF,
0x0000903F,
0x00000020,
0x00000100,   //70
0xFEC00008,
0x00002EFF,
0x08800000,
0x4BB00050,   //80
0,
0,
0x8000008C,
0x00000B01,  //90
0,
0,
0x00FFFEBB,
0xFFFF0000,  //a0
0x0070FFFF,
0x0201CD00,
0x18207916,
0xA8020008,  // b0
0,
0,
0x1A080FF0,
0xFFFFFFFF,  // c0
0x00010000,  // d0
0,
0,
0,
0x00000020,  // e0
0,
0,
0,
0x00000CD8,  // f0
0x00440000,
0,
0x010000AA   //fc
}

};
typedef enum ambus_reg {
	   SMBusStatus,       // 00
		SMBusSlaveStatus, // 01
		SMBusControl,     // 02
		SMBusHostCmd,     // 03
		SMBusAddress,     // 04
		SMBusData0,       // 05
		SMBusData1,       // 06
		SMBusBlockData,   // 07
		SMBusSlaveControl,// 08
		SMBusShadowCmd,   // 09
		SMBusSlaveEventLo,// 0A
		SMBusSlaveEventHi,// 0B
		SlaveDataLo,      // 0C
		SlaveDataHi,      // 0D
		SMBusTiming       //0E
} SMBUS_REG;
//
// 32 bit pci.don't care about ddAddrHi 
//
static unsigned char data[0x10];


static void log_cmd()
{
	CString sSql;
	extern unsigned int dbg_ip;

		sSql.Format(L"insert into   [pci$]  (cmd, dat) values  ('%02X',':%08X-%08X-%08X-%08X @ %x');",
		data[3],
		((unsigned long *)data)[0],
		((unsigned long *)data)[1],
		((unsigned long *)data)[2],
		((unsigned long *)data)[4],
		dbg_ip
		);
		ExcelUtil.ExecSql(sSql);
	
}

static void smbus_busop(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes,int bar_num)
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

		if(ddAddrLo==SMBusStatus) data[0]++;
	}

}

static void smbus_command(int cmd,unsigned int address)
{
}

void smbus_register()
{
	pci_bus.RegisterFunction(PCI_FUN_ADDR(0,0x14,0),(PCI_FUNCTION*)&smbus,smbus_command);
}