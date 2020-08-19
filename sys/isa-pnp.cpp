#include "cpu.h"
#include "addressmap.h"

#include "log.h"

#define SZ_KEY  32
#define NUM_REG 32
char reg_isapnp[NUM_REG],idx,rd_ptr,wr_ptr;

char buffer[SZ_KEY];// write when idx=1

//0xa79
void isa_pnp_wrp(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {


	 switch(op)
	 {
	 case ADDR_RANGE_READ: // won't happen
		 break;

	 case ADDR_RANGE_WRITE: // reser generator
			if(idx<NUM_REG) reg_isapnp[idx]= pszBuff[0];

		 break;

	 default:
		 return;
	 }

#if LOG_ISAPNP
	 log4(
		 "ISA PNP",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }

//0x213
void isa_pnp_rdp(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {


	 switch(op)
	 {
	 case ADDR_RANGE_READ: 
 		 if(idx<NUM_REG) pszBuff[0]=reg_isapnp[idx] ;
		 break;

	 case ADDR_RANGE_WRITE: // won't go here
		 break;

	 default:
		 return;
	 }

#if LOG_ISAPNP
	 log4(
		 "ISA PNP",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }

//0x279
void isa_pnp_pidxr(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {


	 switch(op)
	 {
	 case ADDR_RANGE_READ: 
		pszBuff[0]=idx;
		 break;

	 case ADDR_RANGE_WRITE: // reser generator
		 idx=pszBuff[0];

		 //if(wr_ptr<SZ_KEY) buffer[wr_ptr++]=pszBuff[0];

		 break;

	 default:
		 return;
	 }

#if LOG_ISAPNP
	 log4(
		 "ISA PNP",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }
