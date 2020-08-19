
#include "addressmap.h"
#include "log.h"
#include "smbus.h"
static	unsigned long  data;
void  vt_wr_port_80(unsigned char dat);
 

void port80_handler(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {


	 switch(op)
	 {
	 case ADDR_RANGE_READ: 
		 if(nBytes==1)  *pszBuff=(char)data;
		 else if(nBytes==2)  *(short *)pszBuff=(short)data;
		 else if(nBytes==4)  *(int *)pszBuff=data;
		 break;
	 case ADDR_RANGE_WRITE: 
		if((unsigned char )data!=*(unsigned char  *)pszBuff)
		{
			 vt_wr_port_80(*(unsigned char  *)pszBuff);
#if LOG_PORT80
			 log4(
				 "CHECKPOINT",
				 op==ADDR_RANGE_READ?'r':'w',
				 dwAddress,	
	 			nBytes==1? (unsigned char)pszBuff[0]:
				nBytes==2? *(unsigned short *)pszBuff:
				nBytes==4? *(unsigned long *)pszBuff:0);
#endif
		}

		 if(nBytes==1) data=*(unsigned char  *)pszBuff;
		 else if(nBytes==2) data= *(unsigned short *)pszBuff;
		 else if(nBytes==4) data= *(unsigned long *)pszBuff;

		 break;
	 default:
		 return;
	 }


 }

#if 1

void general_io(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {

		CSMBUS::buffer[0]|=0x80;

	 switch(op)
	 {
	 case ADDR_RANGE_READ: 
		 if(nBytes==1)  *pszBuff=(char)0xff;
		 else if(nBytes==2)  *(short *)pszBuff=-1;
		 else if(nBytes==4)  *(int *)pszBuff=-1;
		 break;
	 case ADDR_RANGE_WRITE: 
		 break;
	 default:
		 return;
	 }

#if LOG_IO
	 log4(
		 "IO",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }
#endif 
void unhandled_io(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {



	 switch(op)
	 {
	 case ADDR_RANGE_READ: 
		 if(nBytes==1)  *pszBuff='\xff';
		 else if(nBytes==2)  *(short *)pszBuff=-1;
		 else if(nBytes==4)  *(int *)pszBuff=-1;
		 break;
	 case ADDR_RANGE_WRITE: 
		 break;
	 default:
		 return;
	 }

#if LOG_IO
	 log4(
		 "IO",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }