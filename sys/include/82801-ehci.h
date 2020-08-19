#ifndef __82801_ehci_h__
#define __82801_ehci_h__

#include "pci.h"

extern CPCIBus pci_bus;

typedef struct ehci_interface_def
{
	CONFIG_SPACE_HDR common; // must be  the  first member
	PCI_HEADER pciHeader;

	char regs0[0x50-sizeof(PCI_HEADER)];
	char pwr_capid; //offset:0x50,
	char nxt_ptr1;  //offset:0x51
	short pwr_cap;  //offset:0x52
	short pwr_cntl_sts;//offset:0x54
	char regs1[0x58-0x55];
	char debug_capid; //offset:0x58,
	char nxt_ptr2;    //offset:0x59,
	short debug_base; //offset:0x5a,
	char regs2[0x60-0x5b];
	char usb_relnum;// offset:0x60
	char fl_adj;    // offset:0x61
	short pwake_cap;//offset:0x62
	char regs3[0x68-0x63];
	unsigned long  leg_nxt_cap;// offset:0x68
    unsigned long  leg_nxt_cs; // offset:0x6c
    unsigned long  special_smi;// offset:0x70
	char regs4[0x80-0x73];
	char access_cntl          ;//offset 0x80,

} ICH82801_EHCI;

class C82801EHCI:public CGenricPCIFunction
{
public:
	C82801EHCI(WORD dwAddress,void* pdata):
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
//C82801EHCI(PCI_FUN_ADDR(0,0x1f,1),config_space_82801_ehci);
#endif