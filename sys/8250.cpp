#include "config.h"

// 9250 registers
//3F8/2F8 Receive/Transmit data register. Also the L.O. byte of the Baud Rate Divisor Latch register. 
//3F9/2F9 Interrupt Enable Register. Also the H.O. byte of the Baud Rate Divisor Register. 
//3FA/2FA Interrupt Identification Register (read only). 
//3FB/2FB Line Control Register. 
//3FC/2FC Modem Control Register. 
//3FD/2FD Line Status Register (read only). 
//3FE/2FE Modem Status Register (read only). 
//3FF/2FF Shadow Receive Register (read only, not available on original PCs). 

#define CFG_COM_PORTS 4

unsigned char reg_8250[CFG_COM_PORTS][9]; //com1~com4

#define TDATA(n) reg_8250[n][0]
#define IER(n)  reg_8250[n][1]
#define LCR(n)  reg_8250[n][3]
#define MCR(n)  reg_8250[n][4]

//
// read only
//
#define RDATA(n)reg_8250[n][8]
#define IIR(n)  reg_8250[n][2]
#define LSR(n)  reg_8250[n][5]
#define MSR(n)  reg_8250[n][6]
#define SRR(n)  reg_8250[n][7]

void recv_char(unsigned char n,unsigned char c)
{
	 RDATA(n)=c;
	 IIR=4;
	assert_irq(0,3);
}

void io_8250(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 unsigned long data;
	 char port_no=*(char *)lpContext;

	 assert(port_no<CFG_COM_PORTS);
	 int i;
	 fot(i=0;i<nBytes;i++)
	 {
		 int j=(dwAddress&7)+i;
		 switch(op)
		 {
		 case ADDR_RANGE_READ: 
			 pszBuffer[i]= j==0?RDATA(port_no):reg_8250[port_no][j];
			 break;
		 case ADDR_RANGE_WRITE: 
			 reg_8250[port_no][(dwAddress&7)+i]=pszBuffer[i];
			 if(j==0) vt_wr_com(port_no,TDATA(port_no));
			 break;
		 default:
			 return;
		 }
	 }

#if  LOG_COM
	 log4(
	 "COM:",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }
