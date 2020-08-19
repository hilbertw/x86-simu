#ifndef __82845_host_h__
#define __82845_host_h__

#include "pci.h"

extern CPCIBus pci_bus;

typedef struct host_hub_interface_bridge_def
{
	CONFIG_SPACE_HDR common; // must be  the  first member
	PCI_HEADER pciHeader;

	DWORD reg[0x40-sizeof(PCI_HEADER)/sizeof(DWORD)];

} MCH82845_HOST_IF;

class C82845HOST:public CGenricPCIFunction
{
public:

	C82845HOST(WORD dwAddress,void* pdata):
	CGenricPCIFunction (dwAddress,pdata)
	{}

	//
	// config space r/w notify
	//
	virtual  void ConfigCallback(int dwCommand,unsigned int address);
#if 0
	//
	// mem/i/o bus operation on function
	//
	virtual DWORD DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num);
	//
	// address select
	//
	virtual int DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi);
#endif
};
//example: 82845 chipset:
//C82845HOST(PCI_FUN_ADDR(0,0,0),config_space_82845_host);
#endif