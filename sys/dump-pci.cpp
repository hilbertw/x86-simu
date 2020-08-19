#include "stdio.h"
#include "assert.h"

#include "addressmap.cpp"
#include "pci.h"
#include "log.h"
#include "util.h"

extern CPCIBus pci_bus;
void dump_pci()
{
	char buffer[0x500];// conf space is 256 bytes at most
	for(int i=0;i<pci_bus.n_functions;i++)
	{
		log0("bus:%x,dev:%x,function:%x:\n",
			(unsigned char)(pci_bus.functions[i].Address.w>>8),
			(unsigned char)(pci_bus.functions[i].Address.w>>3)&0x1f,
			(unsigned char)pci_bus.functions[i].Address.w&7);
		CONFIG_SPACE_HDR* pConfigSpaceHdr=(CONFIG_SPACE_HDR*)pci_bus.functions[i].lpPciFunction->pConfigSpaceHdr;
		sprint_mem32(buffer,0,
			(char *)(pConfigSpaceHdr+1),
			pConfigSpaceHdr->dwHeaderSize);
		log0("%s\n",buffer);

	}
}


