#ifndef __82801_smbus_h__
#define __82801_smbus_h__

#include "pci.h"

extern CPCIBus pci_bus;

typedef struct ich82801_smbus_def
{
	CONFIG_SPACE_HDR common; // must be  the  first member
	PCI_HEADER pciHeader;

//40h HOSTC Host Configuration 00h R/W
	char hostc;

} ICH82801_SMBUS;

class C82801SMBUS:public CGenricPCIFunction
{
public:

	C82801SMBUS(WORD dwAddress,void* pdata):
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
//C82801SMBUS(PCI_FUN_ADDR(0,0x1f,),config_space_82801_smbus);
#endif