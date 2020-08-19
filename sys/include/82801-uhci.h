#ifndef __82801_uhci_h__
#define __82801_uhci_h__

#include "pci.h"

extern CPCIBus pci_bus;

typedef struct uhci_interface_def
{
	CONFIG_SPACE_HDR common; // must be  the  first member
	PCI_HEADER pciHeader;
	char regs0[0x60-sizeof(PCI_HEADER)];
	char usb_relnum;// offset:0x60
	char regs1[0xc0-0x60-1];
	unsigned long  usb_legkey;// offset:0xc0
	char usb_res;//offset 0xc4,resumbe enable;
	// not in config space,
	char ctrl_no;   // controller number:0/1/2/3/4, function number
}ICH82801_UHCI;

class C82801UHCI:public CGenricPCIFunction
{
public:

	C82801UHCI(WORD dwAddress,void* pdata):
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
//C82801UHCI(PCI_FUN_ADDR(0,0x1f,1),config_space_82801_uhci);
#endif