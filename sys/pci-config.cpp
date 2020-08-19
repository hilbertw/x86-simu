#include "stdio.h"
#include "assert.h"

#include "addressmap.cpp"
#include "pci.h"
#include "log.h"

extern CPCIBus pci_bus;
void reset(int i);

static union tag_index_cf8
{
	unsigned long l;
	unsigned char c[4];
	struct pci_addr
	{
		unsigned char reg;
		unsigned char func:3;
		unsigned char dev:5;
		unsigned char bus;
		unsigned char flag;
	} pci_addr;

} index_cf8;

void pci_handler(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 assert((dwAddress&3)+nBytes<=4);

		 if(dwAddress==0xcf8)	 
		 {
			 //assert(nBytes==4);
			 //*(unsigned long *)pszBuff=index_cf8.l;

			 if (nBytes==4)*( unsigned int *)pszBuff =index_cf8.l;
			 else if (nBytes==1) *pszBuff=index_cf8.c[0];
			 else assert(0);
		 }
		 
		 else if (dwAddress>=0xcfc
			 &&dwAddress<=0xcff)
		 {
			 pci_bus.read_config_reg(index_cf8.l|(dwAddress&3),(unsigned char *)pszBuff,nBytes);
			 //
			 //bytes assembly
			 //
		 }
		 else
		 {
			 assert(nBytes==1);
			 pszBuff[0]=index_cf8.c[dwAddress&3];
		 }
		 break;
	 case ADDR_RANGE_WRITE: 
		 if(dwAddress==0xcf8)
		 {
			 
			 if (nBytes==4) index_cf8.l=*( unsigned int *)pszBuff;
			 else if (nBytes==1) index_cf8.c[0]=*pszBuff;
			 else assert(0);

		 }
		 else if (dwAddress==0xcf9)
		 {
			 if(6==(pszBuff[0]&7))
			 reset(1);

		 }
		 else if (dwAddress>=0xcfc
			 &&dwAddress<=0xcff)
		 {
			 assert(dwAddress-0xcfc+nBytes<=4);

			 //if( (index_cf8.l& 0x00ffff00)!=0xa000)  // smbus has been implemented
			 //pci_write(pszBuff,dwAddress-0xcfc,nBytes);
			 //else
			 {
			 pci_bus.write_config_reg(index_cf8.l|(dwAddress&3),(unsigned char *)pszBuff,nBytes);
			 }

		 }
		 else
		 {
			 assert(nBytes==1);
			 index_cf8.c[dwAddress&3]=pszBuff[0];

		 }
		 break;
	 default:
		 return;
	 }

#if  LOG_PCI	
	 if(dwAddress>=0xcfc)
		 //if(index_cf8.pci_addr.dev==0x1f
			// &&index_cf8.pci_addr.func==0
			// &&index_cf8.pci_addr.reg>=0xa0
			// &&index_cf8.pci_addr.reg<0xcf)
	 log(
	 " PCI:%c %x %x (bus %x dev %x func %x reg %x)\n",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0,

		index_cf8.pci_addr.bus,
		index_cf8.pci_addr.dev,
		index_cf8.pci_addr.func,
		index_cf8.pci_addr.reg&0xfc);

	 else if(dwAddress==0xcf9)
		 log("PCI:%c %x %x\n",
			 op==ADDR_RANGE_READ?'r':'w',
			 dwAddress, (unsigned char)pszBuff[0]);
#endif
 }
