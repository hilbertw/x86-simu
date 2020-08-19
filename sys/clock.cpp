#include "cpu.h"
#include "usb.h"
#include "acpi82801.h"
#include "smi82801.h"
void do_8237();
void do_8253();
void do_pulse(int timer_no);
void do_floppy();
void do_ide();
void do_atapi();
bool do_cpu();

bool vt_msg_loop_once();
void do_cmos_rtc();
void do_vga();
void do_vt();
extern CAPICBus apic_bus;
unsigned short sys_hb;
//
// processor runs @ 256Mh, 256 insn= 1us
//
void run_clock()
{
l1:

	sys_hb++;
	if((char)(sys_hb)==0)//by 8253 clock,about 1Mhz
	{

		do_8253();	    // The PC uses a base rate of 1193180 Hz,about 0.84us
		do_cmos_rtc();	// gen 1s, and periodic alarm
		do_tco();       // 0.6s
		do_acpi_timer();//2.34s
		
		do_smi_timer();        //64ms
		do_smi(); 
		do_atapi();

	//	do_ide();
	//	do_8237();
		do_vga();
		do_vt();



		do_floppy();
	}

	if(sys_hb==0)
	{
		apic_bus.do_clock();
		usb_schedule();
	}

	if(!do_cpu())return;

	goto l1;
}