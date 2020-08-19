#include "82801-pci-bridge.h"
#include "log.h"

extern CPCIBus pci_bus;
//
// config space r/w notify
//
 void C82801PCIBRIDGE::ConfigCallback(int dwCommand,unsigned int address)
 {
	 log("82801 hub-PCI  I/f:%c %x\n",dwCommand<'w'?'r':'w',address);

	 if(dwCommand>='w')
	 {
		 int bytes=dwCommand-'w';
		 if(address<=0x19&&address+bytes>0x19)
		 {
				DECL_CONFIG_SPACE_REG
				pci_bus.assign_sbus_num(PCI_HEADER1()->SecondaryBusNumber);
		 }
	 }
 }
#if 0
//
// mem/i/o bus operation on function
//
DWORD C82801PCIBRIDGE::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
}
//
// address select
//
int C82801PCIBRIDGE::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{

}
#endif