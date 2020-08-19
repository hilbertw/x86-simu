#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "apic.h"
#include "log.h"
#include "mch.h"
#include "8253.h"
#include "8259.h"

//#undef LOG_APIC
//#define LOG_APIC 1

#define  HIDE_APIC 0
void  pre_warm_boot_8042();
extern  STATE_8253 timer[3];

extern CAPICBus apic_bus;
extern CMCH mch;
extern unsigned char cmos[];
extern bool step;


char * szAPIC_MsgType[]=
{
"Fixed",//000b = Fixed
"LowestPriority",//001b = Lowest Priority
"SMI",//010b = SMI
"Remote Read",//011b = Remote Read
"NMI",//100b = NMI
"INIT",//101b = INIT
"STARTUP",//110b = Startup
"External Interrupt",//111b = External Interrupt
};



void CAPICBus::do_clock()
{
	int i;
#if 0
	//
	// sender  has halted  for a while to wait for receiver .now  continue
	//
	if( num_delay>0)
	{	
		num_delay--;
		if( num_delay==0)
		{
			nodes[last_send_node]->p_cpu->halt=false;
			timer[0].run=true;
		}
	
	}
#endif
	//
	// do apic clock
	//
	for( i=0;i<n_nodes;i++)
	{
		nodes[i]->do_clock();
	}
	//
	// ioapic shows irq pending?
	//

}
void CAPICBus ::dump_msg(APIC_MSG &msg)
{
#if LOG_APIC
	log0("send apic msg %x->",msg.src,msg.dst);
	if(msg.dst_mode==DST_UNI)
		log0(" node %x",msg.dst);
	else
		log0(" %s",msg.dst_mode==DST_ALL?"all":msg.dst_mode==DST_ALLOTHERS?" others":"none");
	log0(",%x(%s)\n",msg.vector,szAPIC_MsgType[msg.delivery_mode]);
#endif
}

void CAPICBus ::transfer_msg(APIC_MSG & msg)
{

	int i,j=0;

	dump_msg(msg);

	if(msg.dst_mode==DST_NONE) return;
	for(i=0;i<n_nodes;i++)
	{
		switch(msg.dst_mode)
		{

		case DST_ALL: break;
		case DST_ALLOTHERS:
			if(nodes[i]->id()==msg.src) continue;
			break;

		case DST_UNI:
			if(msg.logical_dest==0&& nodes[i]->id()!=msg.dst) continue; //physical:apic id
			if(msg.logical_dest==1&&nodes[i]->logical_dest()!=msg.dst) continue;//ldr
			break;
		}

		nodes[i]->do_recv(msg);
		j++;
	}

#if 0	
	 //pull the sender to halt for while
	if(j)
	for(i=0;i<n_nodes;i++)
		if(nodes[i]->id()==msg.src) 
		{
			timer[0].run=0;
			last_send_node=i;
			num_delay=10;
			nodes[i]->p_cpu->halt=true;
		}
#endif	

}

void CLocalAPIC ::do_recv(APIC_MSG & msg)
{
#if LOG_APIC
	log0("node %x recv msg from %x, vector:%x,level:%x .(%s)\n",id(),msg.src,msg.vector,msg.level,szAPIC_MsgType[msg.delivery_mode]);
#endif
	//
	// toggle the bit in the destined node's irr 
	//
	switch(msg.delivery_mode)
	{

		case DELIVERY_MODE_SMI:
			// enter smi
			p_cpu->ipi_smi();
			break;
		case DELIVERY_MODE_NMI:
			// raise nmi.vector ignored
	
			break;
		case DELIVERY_MODE_INIT:

			// assume init state
			init();
			//
			// cmos [0f]=0xa
			//
			if (cmos[0xf]==0xa)
			{
				 unsigned long v;
				 v=mch.mem_read_int(0x467);
#if LOG_APIC
				 log0("JMP DWORD: %x:%x\n",v>>16,(unsigned short )v);
#endif
				 v=mch.mem_read_int(((v>>12)&0xf)+(unsigned short )v);
				 if(v!=0)
				 {
	
					//pre_warm_boot_8042();// make bios happy
					//p_cpu->ipi_init();// lapic init-ed,apic id not affected.
#if LOG_APIC
					log0("skip INIT\n");	
#endif
				 }

				}
#if LOG_APIC
				else
					log0("cmos  shutdown status: %x\n",cmos[0xf]);
#endif
			break;

		case DELIVERY_MODE_STARTUP:
			// real mode: vv00:0000
			if(state==0)
			{
				p_cpu->ipi_startup((unsigned char )msg.vector);
			}
			state++;


			break;

		case DELIVERY_MODE_EXTINT:
		case DELIVERY_MODE_FIXED:
		case DELIVERY_MODE_LOWEST_PRIORITY:
			mark_vector(msg.vector);
			break;
	}
}

unsigned long CLocalAPIC ::read_register(unsigned long offset)
{
	assert(offset<(MAX_APIC_REG<<4));
	unsigned long data;
	
	data=registers[offset>>4];

	if(offset==CURR_CNT) registers[offset>>4]-=0x400;// timer current count
#if HIDE_APIC
	 data=0xffffffff;
#endif

#if LOG_APIC
	log("read local apic:%x,%x\n",offset,data);
#endif
	return data;
}
void CLocalAPIC ::write_register(unsigned long offset,unsigned long data)
{

#if LOG_APIC
	log("write local apic:%x,%x\n",offset,data);
#endif

	if(offset!=APIC_VER)
	if(offset<(MAX_APIC_REG<<4))
		registers[offset>>4]=data;

	if(offset==ICRLO) 
	{
		ICR().delivery_status=0;
		do_icr();
	}
	else if(offset==APIC_EOI)
	{

		// clear isr
		unmark_vector(isrv);
		isrv=0;
		//
		// make life simple send eoi to 8259
		eoi_8259(0);
		eoi_8259(1);
		// eoi;rescan int pending
		refresh_int();



		// send eoi to ioapic if tms not set
		return;

	}
	else if(offset==INIT_CNT)
	{
		TimerCurrentCount=data;

	}
	else if(offset==DIV_CNT)
	{
		//Div[3] Div[1¨C0] Resulting Timer Divide
		//	0 00 2
		//	0 01 4
		//	0 10 8
		//	0 11 16
		//	1 00 32
		//	1 01 64
		//	1 10 128
		//	1 11 1
		division_cfg=1<<(((data&3)+(data&8)/2+1)&7);
	}
	else if(offset==TPR_PRI)
	{
		refresh_int();
	}
	else if(offset==SPUR_INTR_VEC)
	{
		registers[offset>>4]|=0xf;//bits 0 through 3 are hardwired to logical ones

	}
	
}
//The state of local APIC registers and state machines after a power-up reset are as follows:
// The following registers are all reset to 0: the IRR, ISR, TMR, ICR, LDR, and TPR
//    registers; the holding registers; the timer initial count and timer current count registers; the
//    remote register; and the divide configuration register.
// The DFR register is reset to all 1s.
// The LVT register entries are reset to 0 except for the mask bits, which are set to 1s.
// The local APIC version register is not affected.
// The local APIC ID and Arb ID registers are loaded from processor input pins (the Arb ID
//    register is set to the APIC ID value for the local APIC).
// All internal state machines are reset.
// APIC is software disabled (that is, bit 8 of the SVR register is set to 0).
// The spurious-interrupt vector register is initialized to FFH.
void CLocalAPIC ::init( )
{
	memset((char *)&registers[3],0,sizeof(registers)-3*sizeof(registers[0]));
	registers[DEST_FORMAT>>4]=(unsigned long)-1;

	//The local APIC contains a hardwired version register, which software can use to 
	//identify the APIC version (refer to Figure 7-16). In addition, 
	//the version register specifies the size of LVT used in the specific implementation. 
	//The fields in the local APIC version register are as follows:
	//Version The version numbers of the local APIC or an external 82489DX
	//APIC controller:
	//1XH Local APIC.
	//0XH 82489DX.
	//20H through FFHReserved.
	//Max LVT Entry Shows the number of the highest order LVT entry. For the P6 family
	//processors, having 5 LVT entries, the Max LVT number is 4; for the Pentium processor, 
	//having 4 LVT entries, the Max LVT number is 3.

	registers[APIC_VER>>4]=0x30014;
	registers[SPUR_INTR_VEC>>4]=0xff;
	// mask off all interrupts
	registers[TIMER_LVT>>4]=0x10000;
	registers[PCNT_LVT>>4]=0x10000;
	registers[LINT0_LVT>>4]=0x10000;
	registers[LINT1_LVT>>4]=0x10000;
	registers[ERROR_LVT>>4]=0x10000;

	state=wait_for_SIPI;
	int_waiting=(false);
	division_cnt=(0);
	division_cfg=(1);
	irrv=(0);
}

void CLocalAPIC ::assert_lint0( )
{
#if LOG_APIC
	log0("lint0 asserted.\n");
#endif
	if(LVT(LINT0_LVT).mask==0)
		mark_vector(LVT(LINT0_LVT).vector);
	else {
		mark_vector(LVT(LINT0_LVT).vector);
	}
}
void CLocalAPIC ::deassert_lint0( )
{
#if LOG_APIC
	log0("lint0 deasserted.\n");
#endif

	if(LVT(LINT0_LVT).mask==0)
		unmark_vector(LVT(LINT0_LVT).vector);
	else {
		unmark_vector(LVT(LINT0_LVT).vector);
	}
}

//
// 00:index
// 01:data
//
unsigned long  CIoAPIC ::read_reg(unsigned long offset)
{
	unsigned long  data;
#if LOG_APIC
 log("read ioapic:%08x\n",offset);
#endif

 if(offset==IOREGSEL) data=index;
 else if(offset==IOWIN)
 {
	 if (index==IOAPICID)
	 {
		 data=registers[IOAPICID];
	 }
	 else if (index==IOAPICVER)
	 {
		 data=0x0020+((NUM_IOAPIC_REDIR-1)<<16);
	 }
	 else if (index==IOAPICARB)
	 {
		 data=registers[IOAPICARB];
	 }
	 else if(index>=0x10
		 &&index <=NUM_IOAPIC_DATA)
	 {
		 data=dw[index-0x10];
	 }
	 else 
		 data=0xffffffff;
#if HIDE_APIC
	 data=0xffffffff;
#endif
#if LOG_APIC
	 log("read ioapic reg %08x= %08x\n",index,data);
#endif
	 
 }
 else if(offset==IO30)
 {
	 data=0x20;//>0x1f to make xp happy
 }

 else 
	 data=0xffffffff;
 
	 //::step=true;
	return data;
}
void  CIoAPIC ::write_reg(unsigned long offset,unsigned long data)
{

#if LOG_APIC
 log("write ioapic:%08x:%08x\n",offset,data);
#endif

 if(offset==IOREGSEL) index=data;
 else if(offset==IOWIN)
 {
	 // skip read only  registerver
	 if(index==IOAPICID)
	 {
		registers[IOAPICARB]=
		registers[IOAPICID]=data;
	 }
	 else if (index==IOAPICARB)
	 {
		 registers[IOAPICARB]=data;
	 }
	 else if(index>=0x10
		 &&index <=NUM_IOAPIC_DATA)
	 {
		 dw[index-0x10]=data;
	 }
#if LOG_APIC
	 log("%08x->ioapic reg %08x \n",data,index);
#endif
 }
 else if(offset==PIN_ASSIGN)
 {
		 IRQPA=(unsigned char)data;
 }
 else if(offset==EOI)
 {
		//
 }


}

void CIoAPIC ::set_pin_status( int irq_no,bool asserted)
{
#if LOG_APIC	
	log0("IOAPIC accepts irq:%x",irq_no);
	if(irq_no<NUM_IOAPIC_REDIR)
		log0(" %smasked %x,%sasserted.\n",io_redir_tbl[irq_no].mask?"":"un",io_redir_tbl[irq_no].vector,asserted?"":"de");
	else log0("\n");
#endif
	//
	// if valid and not masked off
	//
	//Interrupt Vector (INTVEC)¡ªR/W: The vector field is an 8 bit field containing the interrupt
	//vector for this interrupt. Vector values range from 10h to FEh.
	if(irq_no<NUM_IOAPIC_REDIR
		&&io_redir_tbl[irq_no].mask==0
		&&io_redir_tbl[irq_no].vector>=0x10
		&&io_redir_tbl[irq_no].vector<=0xfe)
	{
		//
		// experimental
		//

		APIC_MSG msg;
		msg.delivery_mode= io_redir_tbl[irq_no].delivery_mode;
		msg.dst=io_redir_tbl[irq_no].dst;
		msg.vector=io_redir_tbl[irq_no].vector;
		msg.dst_mode=msg.dst==0xff?DST_ALL:DST_UNI;
		msg.logical_dest=io_redir_tbl[irq_no].destination_mode;
		msg.level=io_redir_tbl[irq_no].intpol;
		msg.src=id();

		apic_bus.transfer_msg(msg);
	}
}

void  CIoAPIC ::init()
{
	int i;
	memset((char *)registers,0,sizeof(registers));

	for(i=0;i<NUM_IOAPIC_REDIR;i++)
		io_redir_tbl[i].mask=1;

	index=0;

}
void CAPICBus::register_node(CLocalAPIC * p_node)
{
	assert(p_node);
	assert(n_nodes<CFG_MAX_LAPIC_NODES);

	nodes[n_nodes++]=p_node;
}


void  CLocalAPIC::decode_message(APIC_MSG & msg)
{
#if LOG_APIC
    log0("icr(%x):%08x:%08x\n",id(),registers[ICRLO>>4],registers[ICRHI>>4]);
#endif
	msg.vector=ICR().vector;
	msg.delivery_mode=ICR().delivery_mode;
	msg.dst=(unsigned char )(registers[ICRHI>>4]>>24);
	msg.logical_dest=ICR().destination_mode;
	msg.level=ICR().level;

	switch(ICR().destination_shorthand)
	{
	case DEST_SH_DEST:
		if(msg.dst==0xf) msg.dst_mode=DST_ALL;
		else msg.dst_mode=DST_UNI;
		break;
	case DEST_SH_SELF:
		if(msg.delivery_mode==DELIVERY_MODE_FIXED )
		{
			msg.dst=id();msg.logical_dest=0;
			msg.dst_mode=DST_UNI;
		}
		else
			msg.dst_mode=DST_NONE;
		break;
	case DEST_SH_ALL_INC_S:
		if(msg.delivery_mode==DELIVERY_MODE_FIXED )
			msg.dst_mode=DST_ALL;
		else
			msg.dst_mode=DST_NONE;

		break;
	case DEST_SH_ALL_EXC_S:
		if(ICR().trigger_mode==0
			|| (msg.delivery_mode!=DELIVERY_MODE_SMI
			&&msg.delivery_mode!=DELIVERY_MODE_STARTUP))
			msg.dst_mode=DST_ALLOTHERS;
		else
			msg.dst_mode=DST_NONE;
		break;
	}



	
	registers[0xc0>>4]^=3;
	//
	// delivered
	//
	ICR().delivery_status=0;

	msg.src=id();
}


void CLocalAPIC ::do_clock()
{
	if(0==TimerDivisionCount) return;
	//
	//if timer count== division, raise interrupt
	//
	if(TimerCurrentCount<=0x1000000)
	{
		if(division_cnt<=1)
		{
			division_cnt=division_cfg;


			if(LVT(TIMER_LVT).mask==0)	
				mark_vector(LVT(TIMER_LVT).vector);
			else
				mark_vector(SVR().vector);
			


	#if LOG_APIC
			log0("local apic(%d) timer counted to 0.  init:%08x ,division count: %08x, vecter:%x\n",id(),TimerInitCount,division_cfg,LVT(TIMER_LVT).vector);
	#endif
		}
		else
			division_cnt--;

		TimerCurrentCount=TimerInitCount;
	}
	else
	{	
		TimerCurrentCount-=0x1000000;


	}


}



void CLocalAPIC ::do_icr()
{
	//
	//
	//
	APIC_MSG msg;

	decode_message(msg);
	
	//
	// deliver message to destinations
	//
	apic_bus.transfer_msg(msg);
	//::step=true;
}


void CLocalAPIC ::set_bit(unsigned char * s, int bit)
{
	int byte=bit>>3;
	int b=bit&7;
	s[byte]|=(1<<b);
}
void CLocalAPIC ::reset_bit(unsigned char * s, int bit)
{
	int byte=bit>>3;
	int b=bit&7;
	s[byte]&=~(1<<b);
}
unsigned char CLocalAPIC ::get_bit(unsigned char * s)
{
	int i,j;

   int_waiting=false;

   for(i=0x1f;i>=0;i--)
   {
	   if(!s[i]) continue;

	   for(j=0;j<8;j++)
	   {
		   if(s[i]&(1<<j)) 
		   {
			   //clear irr
			   break;
		   }
	   }
   
	   // found
	   if(j!=8)
	   {
		   s[i]&=~(1<<j);
		   //
		   // set isr
		   //
		   isrv=i*8+j;

		   // set isr
		   //s=(unsigned char *)&registers[ISR_BEGIN>>4];
		   //s[i]|=(1<<j);
		   //
			//IF TPR[7:4] >= ISRV[7:4]
			//THEN
			//PPR[7:0] = TPR[7:0]
			//ELSE
			//PPR[7:4] = ISRV[7:4] AND PPR[3:0] = 0
			//Where ISRV is the vector of the highest priority ISR bit

		   if((tpr()>>4)>=(isrv>>4))
			   registers[PROC_PRI>>4]=registers[TPR_PRI>>4];
		   else
		   {
			   registers[PROC_PRI>>4]=isrv&0xf0;
		   }
		   return isrv;
	   }
   }

   //
   // return spurious vector
   //
   return SVR().vector;
}



void CLocalAPIC ::refresh_int()
{
	int i,j;

	unsigned char *s=(unsigned char *)&registers[IRR_BEGIN>>4];

   for (i=0x1f;i>=0;i--)
   {
	   if(!s[i]) continue;

	   for(j=0;j<8;j++)
	   {
		   if(s[i]&(1<<j)) 
		   {
				irrv=i*8+j;
			   int_waiting=true;
#if LOG_APIC
				log0("irrv=%x, tpr=%x\n",irrv,tpr());
#endif
			   return;
		   }
	   }

   }
   int_waiting=false;
}
void CLocalAPIC ::mark_vector(unsigned char vector)
{
#if LOG_APIC
	log0("new int request %x\n",vector);
#endif
	set_bit((unsigned char *)&registers[IRR_BEGIN>>4],vector);
	refresh_int();

}
void CLocalAPIC ::unmark_vector(unsigned char vector)
{
#if LOG_APIC
	log0("clear int request %x\n",vector);
#endif
	reset_bit((unsigned char *)&registers[IRR_BEGIN>>4],vector);
	refresh_int();

}
void CLocalAPIC ::calc_apr()
{
//
//IF (TPR[7:4] >= IRRV[7:4]) AND (TPR[7:4] > ISRV[7:4])
//THEN
//APR[7:0] = TPR[7:0]
//ELSE
//APR[7:4] = max(TPR[7:4] AND ISRV[7:4], IRRV[7:4]), APR[3:0]=0.
//Here, IRRV is the interrupt vector with the highest priority IRR bit set or cleared
}



void CLocalAPIC ::dump()
{

	log0("local apic (%x)registers:\n",id());
	for(int i=0;i<MAX_APIC_REG;i++)
	{
		log0("%x0:%x\n",i,registers[i]);
	}
}
void CIoAPIC ::dump()
{
	log0("IO REDIR table:\n");
	for(int i=0;i<NUM_IOAPIC_REDIR;i++)
	{
		log0("%x:%08x %08x\n",i,dw[i+i+1],dw[i+i]);
	}
}