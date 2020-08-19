#include <stdlib.h>

#include "addressmap.h"
#include "sys.h"
#include "log.h"
#include "8253.h"
#include "irq-wire.h"
#define ASSERT_IRQ0() assert_irq(0)
#define DEASSERT_IRQ0() 



STATE_8253 timer[3]={{0xffff},{0xffff},{0xffff}};
//The Counter does not stop when it reaches zero.

void do_pulse(int timer_no)
{


	if(timer[timer_no].u.count!=0)
		timer[timer_no].u.count--;
	else
	{
		//
		// timer 0, gen int8
		//
		if(timer_no==0)
			 ASSERT_IRQ0();
		//
		// reset when write to 8253 ch 2.
		//
		else if(timer_no==2)
		{
			extern char port_61h;	
			port_61h ^=0x20;
		}

		timer[timer_no].u.count=timer[timer_no].u_to_reload.count;
#if 0	
		switch(timer[timer_no].mode.mode)
		{
		case 0:
		//000  mode 0, interrupt on terminal count;  countdown, interrupt,
		//     then wait for a new mode or count; loading a new count in the
		//     middle of a count stops the countdown
		case 1:
		//001  mode 1, programmable one-shot; countdown with optional
		//     restart; reloading the counter will not affect the countdown
		//     until after the following trigger

		case 2:
		//010  mode 2, rate generator; generate one pulse after 'count' CLK
		//     cycles; output remains high until after the new countdown has
		//     begun; reloading the count mid-period does not take affect
		//     until after the period
		case 3:
		//011  mode 3, square wave rate generator; generate one pulse after
		//     'count' CLK cycles; output remains high until 1/2 of the next
		//     countdown; it does this by decrementing by 2 until zero, at
		//     which time it lowers the output signal, reloads the counter
		//     and counts down again until interrupting at 0; reloading the
		//     count mid-period does not take affect until after the period

		case 4:
		//100  mode 4, software triggered strobe; countdown with output high
		//     until counter zero;  at zero output goes low for one CLK
		//     period;  countdown is triggered by loading counter;  reloading
		//     counter takes effect on next CLK pulse
		case 5:
		//101  mode 5, hardware triggered strobe; countdown after triggering
		//     with output high until counter zero; at zero output goes low
		//     for one CLK period
			timer[timer_no].u.count=0xffff;
			// 8253 wrap around to 0xffff. 
			// wrap around in bcd, not support
			//
			break;
		}
#endif
	}
}

char read_timer(int timer_no)
{
	char data;

	//
	// read back cmd?
	//
	if(mode_ctrl.counter_sel==3
		&& port43h&(1<<(3+timer_no))) data=*(char *) &timer[timer_no].mode;// output  pin,don't care now
	else

	//
	// rw formatlsb,msb...
	//
	switch(timer[timer_no].mode.wrl_fmt)
	{

	case MSB_ONLY:
		data=timer[timer_no].u.w.msb;
		break;
	case LATCH_PRESENT_COUNTER:
	case LSB_MSB:
		if(timer[timer_no].rw_state==1)
		{
			data=timer[timer_no].u.w.msb;
			timer[timer_no].rw_state=0;
		}
		else 
		{
			data=timer[timer_no].u.w.lsb;
			timer[timer_no].rw_state=1;
		}
		break;
	case LSB_ONLY:

		data=timer[timer_no].u.w.lsb;
		break;

	}
	return data;
}
void write_timer(int timer_no,char data)
{
	switch(timer[timer_no].mode.wrl_fmt)
	{
	case LSB_ONLY:
		timer[timer_no].u_to_reload.w.lsb=data;
		break;
	case MSB_ONLY:
		timer[timer_no].u_to_reload.w.msb=data;
		break;
	case LSB_MSB:
		if(timer[timer_no].rw_state==1)
		{
			timer[timer_no].u_to_reload.w.msb=data;
			timer[timer_no].rw_state=0;
		}
		else 
		{
			timer[timer_no].u_to_reload.w.lsb=data;
			timer[timer_no].rw_state=1;
		}

		break;
	default://LATCH_PRESENT_COUNTER
		 timer[timer_no].u_to_reload=timer[timer_no].u;
		 break;
	}

#if  LOG_8253
	log4("Timer",'0'+timer_no,timer[timer_no].u_to_reload.count,*(unsigned char *)&timer[timer_no].mode);
#endif

	if(timer[timer_no].u_to_reload.count==0)
		timer[timer_no].u_to_reload.count=0xffff;

	timer[timer_no].u=timer[timer_no].u_to_reload;

	//
	// reset when write to 8253 ch 2.
	//
	if(timer_no==2)
	{
		extern char port_61h;
		port_61h &=~0x20;
	}

}

 void io_8253(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,                    // optional 
	  unsigned int nBytes )            // in or out buffer
 {
	 switch(op)
	 {
	 case ADDR_RANGE_READ:  
		 if(dwAddress==0x43) 
		 {
		 *pszBuff=port43h;
		 }
		 else
		  *pszBuff=read_timer(dwAddress-0x40);

		 break;
	 case ADDR_RANGE_WRITE: 
		 if(dwAddress==0x43) 
		{
			port43h=*pszBuff;

			if(mode_ctrl.counter_sel!=3)
			{
				timer[mode_ctrl.counter_sel].mode=mode_ctrl;
				timer[mode_ctrl.counter_sel].rw_state=0;
			}
		}
		else 
			write_timer(dwAddress-0x40,*pszBuff);

		 break;
	 default:
		 return;
	 }
#if  LOG_8253
	 log4(
		 "8253",
		 op==ADDR_RANGE_READ?'r':'w',
		 dwAddress,	
	 	nBytes==1? (unsigned char)pszBuff[0]:
		nBytes==2? *(unsigned short *)pszBuff:
		nBytes==4? *(unsigned long *)pszBuff:0);
#endif
 }

 void do_8253()
 {
	 for(int i=0;i<3;i++)
	 {
		 do_pulse(i);
	 }
 }

