#include <stdlib.h>
#include <assert.h>
#include "addressmap.h"
#include "log.h"
#include "sys.h"
#include "cpu.h"
#include "8259.h"
#include "irq-wire.h"

extern CCPU cpu[];

REGS_8259 reg_8259[2];

unsigned short isr;

void eoi_8259(int controller_no);
void fresh_int();

void reset_8259()
{
	reg_8259[0].ocw1=
	reg_8259[1].ocw1=0xff;
	*(char *)&reg_8259[0].ocw3=
	*(char *)&reg_8259[0].ocw3=0xa;//After initialization the 8259A is set to IRR.

	reg_8259[0].isr=
	reg_8259[1].isr=
	reg_8259[0].irr=
	reg_8259[1].irr=0;
}

void write_8259_reg( int controller, int a0, char dat)
{
	if(a0==0)
	{
		if(dat &0x10) {
			reg_8259[controller].icw[0]=dat;

			reg_8259[controller].state=
			reg_8259[controller].irr=
			reg_8259[controller].isr=0;
		}
		else if((dat&0x18)==0)
		{
				*(char *)&reg_8259[controller].ocw2=dat;
				eoi_8259(controller);
		}
		else *(char *)&reg_8259[controller].ocw3=dat;
	}
	else if(reg_8259[controller].state==3) // mask
	{
		reg_8259[controller].ocw1=dat;
		fresh_int();
	}
	else if(reg_8259[controller].state==0) 
	{
		reg_8259[controller].icw[1]=dat;
		// icw 3 can only be read in cascade mode?
		reg_8259[controller].state=reg_8259[controller].reg.icw1.sngl==1?1:2;
	}
	else if(reg_8259[controller].state==1) {reg_8259[controller].icw[2]=dat;reg_8259[controller].state=2;}
	else if(reg_8259[controller].state==2) 
	{
		reg_8259[controller].state=3;
		if(reg_8259[controller].reg.icw1.icw4)
			reg_8259[controller].icw[3]=dat;
		else
			reg_8259[controller].ocw1=dat;
	}


}

__inline char  read_8259_reg( int controller, int a0)
{
	unsigned char dat;

	if(a0)dat=reg_8259[controller].ocw1;
	else if (reg_8259[controller].ocw3.p==1)         // polling command
	{
		dat=0;
		for (int i=0;i<8;i++) 
			if((reg_8259[controller].irr&(1<<i))!=0) {dat=0x80|i;break;}

	}
	else if (reg_8259[controller].ocw3.rr==1) 
	{
		dat=reg_8259[controller].ocw3.ris?reg_8259[controller].isr:reg_8259[controller].irr;
	}
	else dat=0xff; // no action

	return dat;
}

 void io_8259(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 int controller=dwAddress>=0xa0?1:0;
	 int a0 =dwAddress&1;

	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 *pszBuff=read_8259_reg(controller,a0);

		 break;
	 case ADDR_RANGE_WRITE: 
		 write_8259_reg(controller,a0,*pszBuff);
		 break;
	 default:
		 return;
	 }
#if  LOG_8259
	 log4(
		 "8259",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }






//
// only 8086 mode
//
unsigned char inta_8259()
{
	int i,controller;

	short irr=(reg_8259[0].irr|(reg_8259[1].irr<<8));
	short mask=~(reg_8259[0].ocw1|(reg_8259[1].ocw1<<8));

	short x=irr&mask;

	for (i=0;i<16;i++) if((x&(1<<i))!=0) break;

	//If no interrupt request is present at step 4 of either
	//sequence (i.e., the request was too short in duration)
	//the 8259A will issue an interrupt level 7. Both the
	//vectoring bytes and the CAS lines will look like an
	//interrupt level 7 was requested.
	if(i==16) i=7;


	controller=i/8;
	i&=7;
	//Upon receiving an INTA,the highest priority ISR bit is set 
	// and the corresponding IRR bit is reset. 
	reg_8259[controller].isr  =1<<i;
	//reg_8259[controller].irr &=~reg_8259[controller].isr;

	//fresh_int();
	deassert_intr();

	return (reg_8259[controller].reg.icw2.t3_7<<3)|i;

}

void eoi_8259(int controller_no)
{
	reg_8259[controller_no].irr &=~reg_8259[controller_no].isr;
	reg_8259[controller_no].isr=0;
	fresh_int();
}

void _8259_assert_irq(int controller, int irq_no)
{
	 assert(irq_no<8);
	 assert(controller<2);

	 reg_8259[controller].irr|=(1<<irq_no);



	 //
	 // masked off?,assert intr if needed
	 //
	fresh_int();
}

void _8259_deassert_irq(int controller, int irq_no)
{
	 assert(irq_no<8);
	 assert(controller<2);


	 reg_8259[controller].irr&=~(1<<irq_no);
	 //
	 // irr changes, update INT 
	 //
	fresh_int();
}
//
// called when mask changes
//
void fresh_int()
{
	short mask=~(reg_8259[0].ocw1|(reg_8259[1].ocw1<<8));
	short irr=(reg_8259[0].irr|(reg_8259[1].irr<<8));

  	 if(0==(irr&mask))
		 deassert_intr();
	 else
	 {
		 //
		 //  priority check,higher than isr? not implemented.
		 //
		 assert_intr();
	 }
}