#include <assert.h>

#include "apic.h"

#include "addressmap.h"
#include "log.h"

unsigned long apic_reg[0x40];
unsigned int x;
unsigned long d[]={0,0,3,3,0xff,0xff,0,0,0xff,0xff,0,0,0xff,0xff,0,0,0xff,0xff};

#define tbl_size (sizeof(d)/sizeof(d[0]))
void apic_read(
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,           // optional 
	  unsigned int nBytes )           // in or out buffer
{
	assert(nBytes==4);

	if(dwAddress==ICRLO)
	{
		*(unsigned int *)pszBuff=0x20000;
	}
	else	if(dwAddress==0xc0)
	{
		*(unsigned int *)pszBuff=x<tbl_size?d[x++]:0xff;
	}
	else

	*(unsigned int *)pszBuff=apic_reg[dwAddress>>4];

}
void apic_write(
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,           // optional 
	  unsigned int nBytes )           // in or out buffer
{
	assert(nBytes==4);
	apic_reg[dwAddress>>4]=*(unsigned int *)pszBuff;

}

void apic_mem(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,           // optional 
	  unsigned int nBytes )           // in or out buffer
{
	switch(op)
	{
	 case ADDR_RANGE_READ:
		 apic_read(dwAddress-0xfee00000,pszBuff,nBytes);
		 break;
	 case ADDR_RANGE_WRITE: 
		 apic_write(dwAddress-0xfee00000,pszBuff,nBytes);
		 break;
	 default:
		 return;
	}


	 log4(
		 "APIC",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
}


static unsigned char port_22h;
unsigned char imcr;

void io_imcr(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 if(nBytes==1)
		 {
			 *pszBuff=(dwAddress==0x22)?port_22h:imcr; 
		 }
		 else
		 {
			 pszBuff[0]=port_22h;
			 pszBuff[1]=imcr;
		 }
		 break;
	 case ADDR_RANGE_WRITE: 
		 assert(nBytes==1);
		 		 
		 if(dwAddress==0x22)
		{
			port_22h=(unsigned char)*pszBuff;
			//report status at APM_STS and gen smi
			//invoke_smi(SMI_APMC_WRITE);
		}
		else if(port_22h==0x70)
		{
			imcr=(unsigned char)*pszBuff;
#if LOG_APIC
			log("Select %cPIC mode.\n",imcr?'A':' ');
#endif
		}
		 break;
	 default:
		 return;
	 }

 }