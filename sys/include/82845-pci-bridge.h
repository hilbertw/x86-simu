#ifndef __82845_hub_pci_bridge_h__
#define __82845_hub_pci_bridge_h__

#include "pci.h"

extern CPCIBus pci_bus;

typedef struct _82845_hub_pci_bridge_def
{
	CONFIG_SPACE_HDR common; // must be  the  first member
	PCI_HEADER1 pciHeader;
//ERRCMD1 Error Command 00h R/W
//41每4Fh 〞 Reserved 〞 〞
//50每57h DWTC DRAM Write Thermal Management Control 00000000 R/W/L

//Symbol Name Default Access
//58每5Fh DRTC DRAM Read Thermal Management Control 00000000 R/W/L
//59每FFh 〞 Reserved

	 char errcmd1;
	 char regs0[0xf];
	 int dwtc[2];
	 int drtc[2];
	 char regs[0x100-0x60];

} MCH82845_PCIBRIDGE_IF;

class C82845PCIBRIDGE:public CGenricPCIBridge
{
public:

	C82845PCIBRIDGE(WORD dwAddress,void* pdata):
	CGenricPCIBridge (dwAddress,pdata)
	{}
	C82845PCIBRIDGE() {}

	//
	// config space r/w notify
	//
	virtual  void ConfigCallback(int dwCommand,unsigned int address);
	//
	// mem/i/o bus operation on function
	//
	//virtual DWORD DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num);

	//
	// address select
	//
	//virtual int DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi);


};
//example: 82845 chipset:
//C82845PCIBRIDGE(PCI_FUN_ADDR(0,0,1),config_space_82801_pci_bridge);
#endif