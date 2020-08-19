#include "82845-pci-bridge.h"
#include "log.h"

//
// config space r/w notify
//
 void C82845PCIBRIDGE::ConfigCallback(int dwCommand,unsigned int address)
 {
	 log("82845 hub-PCI  I/f:%c %x\n",dwCommand<'w'?'r':'w',address);
 }
#if 0
//
// mem/i/o bus operation on function
//
DWORD C82845PCIBRIDGE::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
}

//
// address select
//
int C82845PCIBRIDGE::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{

}
#endif