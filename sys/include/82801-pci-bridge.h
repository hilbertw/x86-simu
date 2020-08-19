#ifndef __82801_pci_bridge_h__
#define __82801_pci_bridge_h__

#include "pci.h"

extern CPCIBus pci_bus;

typedef struct hub_pci_bridge_def
{
	CONFIG_SPACE_HDR common; // must be  the  first member
	PCI_HEADER1 pciHeader;

//40¨C43h HI1_CMD Hub Interface 1 Command Control 76202802h R/W, RO
//44¨C45h DEVICE_HIDE Secondary PCI Device Hiding 00 R/W
//50¨C53h CNF Policy Configuration 00406402h R/W
//70h MTT Multi-Transaction Timer 20h R/W
//82h PCI_MAST_STS PCI Master Status 00h R/WC
//90h ERR_CMD Error Command 00h R/W
//92h ERR_STS Error Status 00h R/WC
	int hi1_cmd;
	short device_hide;
	char regs0[0x50-0x46];
	int cnf;
	char regs1[0x70-0x54];
	char mtt;
	char regs2[0x82-0x71];
	char pci_mast_sts;
	char regs3[0x90-0x83];
	char err_cmd;
	char regs4[1];
	char err_sts;

} ICH82801_PCIBRIDGE_IF;

class C82801PCIBRIDGE:public CGenricPCIBridge
{
public:

	C82801PCIBRIDGE(WORD dwAddress,void* pdata):
	CGenricPCIBridge (dwAddress,pdata)
	{}
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
//C82801PCIBRIDGE(PCI_FUN_ADDR(0,0x1f,0),config_space_82801_pci_bridge);
#endif