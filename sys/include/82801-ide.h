#ifndef __82801_ide_h__
#define __82801_ide_h__

#include "pci.h"

extern CPCIBus pci_bus;

typedef struct ide_interface_def
{
	CONFIG_SPACE_HDR common; // must be  the  first member
	PCI_HEADER pciHeader;
//40¨C41h IDE_TIMP Primary IDE Timing 0000h R/W
//42¨C43h IDE_TIMS Secondary IDE Timing 0000h R/W
//44h SLV_IDETIM Slave IDE Timing 00h R/W
//48h SDMA_CNT Synchronous DMA Control 00h R/W
//4A¨C4Bh SDMA_TIM Synchronous DMA Timing 0000h R/W
//54h IDE_CONFIG IDE I/O Configuration 00h R/W
	short ide_timep;
	short ide_tims;
	char  slv_idetim;
	char regs0[3];
	char sdma_cnt;
	char  regs1;
	short sdma_tim;
	char regs2[8];
	char ide_config;
	char bus_master[0x10];
} ICH82801_IDE;

class C82801IDE:public CGenricPCIFunction
{
public:
	C82801IDE(WORD dwAddress,void* pdata):
	CGenricPCIFunction (dwAddress,pdata)
	{}

	//
	// config space r/w notify
	//
	virtual  void ConfigCallback(int dwCommand,unsigned int address);
	//
	// mem/i/o bus operation on function
	//
	virtual DWORD DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num);
	//
	// address select
	//
	//virtual int DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi);

};
//example: 82845 chipset:
//C82801IDE(PCI_FUN_ADDR(0,0x1f,1),config_space_82801_ide);
#endif