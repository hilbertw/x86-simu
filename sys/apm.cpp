#include <assert.h>
#include "addressmap.h"
#include "smi82801.h"
#include "acpi82801.h"
#include "log.h"

void invoke_smi(int type);

static unsigned char port_b3h;
static unsigned char port_b2h;
 void apm_handler(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 unsigned char a0=dwAddress&1;
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 if(nBytes==1)
		 {
			 *pszBuff=(dwAddress==0xb2)?port_b2h:port_b3h; 
		 }
		 else
		 {
			 pszBuff[0]=port_b2h;
			 pszBuff[1]=port_b3h;
		 }

		 break;
	 case ADDR_RANGE_WRITE: 
		 assert(nBytes==1);
		 		 
		 if(dwAddress==0xb2)
		{
			port_b2h=(unsigned char)*pszBuff;
			//report status at APM_STS and gen smi
			//invoke_smi(SMI_APMC_WRITE);
			//
			// write 0xe1/0x1e enales/disables acpi
			//
			if(port_b2h==0xe1)

				enable_acpi(true);
			else if(port_b2h==0xe1)
				enable_acpi(false);
		}
		else port_b3h=(unsigned char)*pszBuff;
		 break;
	 default:
		 return;
	 }
#if LOG_APM

	 log4(
		 "APM",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
 		nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }
