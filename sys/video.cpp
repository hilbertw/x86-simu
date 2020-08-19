#include <stdlib.h>

#include "addressmap.h"
#include "vga.h"
#include "log.h"
extern unsigned int dbg_ip;
 void assert_irq(int controller, int irq_no);
 void deassert_irq(int controller, int irq_no);

 static unsigned int  last_rd_port,last_wr_port;
 static unsigned int  last_rd_port1,last_wr_port1;
 static unsigned int  last_rd_port2,last_wr_port2;
//
//3c0~3cf
//
void io_vga(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
{
	unsigned int i;
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 for(i=0;i<nBytes;i++) pszBuff[i]=read_vga_reg((dwAddress&0xf)+i);
		 //if(last_rd_port==dwAddress) return;
		 //last_rd_port=dwAddress;
		 break;

	 case ADDR_RANGE_WRITE: 
		 for(i=0;i<nBytes;i++) write_vga_reg((dwAddress&0xf)+i,pszBuff[i]);
		 //if(last_wr_port==dwAddress) return;
		 //last_wr_port=dwAddress;
		 break;
	 default:
		 return;
	 }
#if  LOG_VGA
	 log4(
		 "VGA",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned short *)pszBuff:0);
#endif
}
//
//3d4 3d5 3da
//
void io_vga_crtc(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 unsigned int i;
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 for(i=0;i<nBytes;i++) pszBuff[i]=read_crtc_reg((dwAddress&0xf)+i);
		 break;

	 case ADDR_RANGE_WRITE: 
		 for(i=0;i<nBytes;i++) write_crtc_reg((dwAddress&0xf)+i,pszBuff[i]);
		 //if(last_wr_port1==dwAddress) return;
		 //last_wr_port1=dwAddress;
		 break;
	 default:
		 return;
	 }
#if  LOG_VGA
	 log4(
		 "VGA 2",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned short *)pszBuff:0);
#endif

 }

//
//3d4~3d5
//
void io_cga(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 unsigned int i;
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 for(i=0;i<nBytes;i++) pszBuff[i]=read_6845_reg((dwAddress&0xf)+i);
		 break;

	 case ADDR_RANGE_WRITE: 
		 for(i=0;i<nBytes;i++) write_6845_reg((dwAddress&0xf)+i,pszBuff[i]);
		 //if(last_wr_port2==dwAddress) return;
		 //last_wr_port2=dwAddress;
		 break;
	 default:
		 return;
	 }
#if  LOG_CGA
	 log4(
		 "CGA",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned short *)pszBuff:0);
#endif
 }

//
//3b4~3b5
//
void io_mda(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 unsigned int i;
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 for(i=0;i<nBytes;i++) pszBuff[i]=read_6845_reg((dwAddress&0xf)+i);
		 break;

	 case ADDR_RANGE_WRITE: 
		 for(i=0;i<nBytes;i++) write_6845_reg((dwAddress&0xf)+i,pszBuff[i]);
		 break;
	 default:
		 return;
	 }
#if  LOG_MDA
	 log4(
		 "MDA",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned short *)pszBuff:0);
#endif

 }


//void do_vga()
//{
//	//
//	// vertical; retrace, gen int
//	//
//	if(read_6845_reg(0xa)&8)
//	assert_irq(0,2);
//}