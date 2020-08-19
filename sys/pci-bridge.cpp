#include "string.h"
#include "pci.h"
#include "log.h"
//
// config space r/w notify
//
void CGenricPCIBridge::ConfigCallback(int dwCommand,unsigned int address)
{
}
//
// mem/i/o bus operation on function
//
DWORD CGenricPCIBridge::DoBusOp(BUSOP Op,BYTE * pBuffer,DWORD ddAddrLo,DWORD ddAddrHi,DWORD dwBytes, int bar_num)
{
	if(Op==MEM_RD||Op==IO_RD)
		memset(pBuffer,0xff,dwBytes);
	return dwBytes;
}
//
// address select. matching bar[0-5]
//
int CGenricPCIBridge::DoDeviceSelect(bool io,DWORD ddAddrLo,DWORD ddAddrHi)
{
	DECL_CONFIG_SPACE_REG

	if(io)
	{
	//IO_BASE ¡Ü address ¡Ü ¦© O_LIMIT
#if LOG_PCI
		log(" pci bridge,iobase:%04x%02x000,iolimit: %04x%02xfff\n",
			PCI_HEADER1()->dwIOBaseHi,PCI_HEADER1()->IOBase>>4,
			PCI_HEADER1()->dwIOLimitHi,PCI_HEADER1()->IOLimit>>4);
#endif
	}

	else
	{
	//MEMORY_BASE1 ¡Ü address ¡Ü MEMORY_LIMIT1
	//IO_BASE ¡Ü address ¡Ü ¦© O_LIMIT

#if LOG_PCI
		log(" pci bridge,mbase:%08x00000,mlimit: %04xfffff\n",
			PCI_HEADER1()->dwMemBase>>4,
			PCI_HEADER1()->dwMemLimit>>4);
#endif
	}

	return -1;
}

