#ifndef __apic_h__
#define __apic_h__

#include <assert.h>
#include "config.h"

typedef unsigned long APIC_DWORD;
//
//irect registers
//
//IOREGSEL〞I/O REGISTER SELECT REGISTER:FEC0 xy00h (xy=See APICBASE Register in the PIIX3)
//IOWIN〞I/O WINDOW REGISTER:FEC0 xy10h (xy=See APICBASE Register in PIIX3)
//FEC0_0000h Index Register 8 bits R/W
//FEC0_0010h Data Register 32 bits R/W
//FECO_0020h IRQ Pin Assertion Register 8 bits WO
//FECO_0040h EOI Register 8 bits WO
#define IOREGSEL 0x00
#define IOWIN    0x10
#define PIN_ASSIGN 0x20
#define IO30     0x30
#define EOI      0x40
#define MAX_IOAPIC_DREG 4
// indirect registers
//0h ID 32 bits R/W
//01h Version 32 bits RO
//02h Arbitration ID 32 bits RO
//03h Boot Configuration 32 bits R/W
//03每0Fh Reserved RO
//10每11h Redirection Table 0 64 bits R/W, RO
//12每13h Redirection Table 1 64 bits R/W, RO
//... ... ... ...
//3E每3Fh Redirection Table 23 64 bits R/W, RO

#define  IOAPICID  00 //00IOAPIC IDENTIFICATION REGISTER
#define  IOAPICVER 01 //01 IOAPIC VERSION REGISTER
#define  IOAPICARB  02 //02 IOAPIC ARBITRATION REGISTER
#define  BOOT_CFG   03
#define  MAX_IOAPIC_REG 4

#define   NUM_IOAPIC_REDIR 0x18
#define   NUM_IOAPIC_DATA ((NUM_IOAPIC_REDIR*2)+0x10)
//
//10-11h (IOREDTBL0) 
//12-13h (IOREDTBL1) 
//14-15h (IOREDTBL2) 
//16-17h (IOREDTBL3) 
//18-19h (IOREDTBL4) 
//1A-1Bh (IOREDTBL5) 
//1C-1Dh (IOREDTBL6) 
//1E-1Fh (IOREDTBL7) 
//20-21h (IOREDTBL8) 
//22-23h (IOREDTBL9) 
//24-25h (IOREDTBL10)
//26-27h (IOREDTBL11)
//28-29h (IOREDTBL12)
//2A-2Bh (IOREDTBL13)
//2C-2Dh (IOREDTBL14)
//2E-2Fh (IOREDTBL15)
//30-31h (IOREDTBL16)
//32-33Fh (IOREDTBL17)
//34-35h (IOREDTBL18)
//36-37h (IOREDTBL19)
//38-39h (IOREDTBL20)
//3A-3Bh (IOREDTBL21)
// 3C-3Dh (IOREDTBL22)
// 3E-3Fh (IOREDTBL23)
#pragma pack(push,1)
typedef struct int_redir_table_def
{
	unsigned char vector;
	unsigned char delivery_mode:3;
	unsigned char destination_mode:1;
	unsigned char delivery_status:1;
	unsigned char intpol:1;      //Interrupt Input Pin Polarity
	unsigned char remote_irr:1;  //Remote IRR
	unsigned char trigger_mode:1;// Trigger Mode
	unsigned char mask:1;        //Interrupt Mask
	unsigned char reserved:7;

	unsigned char reserved2[4];
	unsigned char dst;  // 56:63
}IOREDTBL;

#pragma pack(pop)
//
// local apic registers
//
#define APIC_ID       0x20   //APIC ID Register
#define APIC_VER      0x30   //APIC Version Register
#define TPR_PRI       0x80   //Task Priority Register
#define ARB_PRI       0x90   //Arbitration Priority Register
#define PROC_PRI      0xA0   //Processor Priority Register
#define APIC_EOI      0xB0   //End Of Interrupt Register
#define LOG_DEST      0xD0   //Logical Destination Register
#define DEST_FORMAT   0xE0   //Destination Format Register
#define SPUR_INTR_VEC 0xF0   //Spurious Interrupt Vector Register
#define ISR_BEGIN     0x100  //In-Service Registers
#define ISR_END       0x170  //In-Service Registers
#define TMR_BEGIN     0x180  //Trigger Mode Registers
#define TMR_END       0x1F0  //Trigger Mode Registers
#define IRR_BEGIN     0x200  //Interrupt Request Registers
#define IRR_END       0x270  //Interrupt Request Registers
#define ERR_STAT      0x280  //Error Status Register
#define ICRLO         0x300  //Interrupt Command Register Low (bits 31每0)
#define ICRHI         0x310  //Interrupt Command Register High (bits 63每32)
#define TIMER_LVT     0x320  //Timer Local Vector Table Entry
#define PCNT_LVT      0x340  //Performance Counter Local Vector Table Entry
#define LINT0_LVT     0x350  //Local Interrupt 0 Local Vector Table Entry
#define LINT1_LVT     0x360  //Local Interrupt 1 Local Vector Table Entry
#define ERROR_LVT     0x370  //Error Local Vector Table Entry
#define INIT_CNT      0x380  //Timer Initial Count Register
#define CURR_CNT      0x390  //Timer Current Count Register
#define DIV_CNT       0x3E0  //Timer Divide Configuration Register

#define MAX_APIC_REG  0x40   // enough for both local and io apic
#pragma pack(push,1)
typedef struct icr_def
{
	unsigned char vector;
	unsigned char delivery_mode:3;
	unsigned char destination_mode:1;
	unsigned char delivery_status:1;
	unsigned char reserved:1;
	unsigned char level:1;
	unsigned char trigger_mode:1;
	unsigned short reserved2:2;
	unsigned short destination_shorthand:2;
}_ICR;

typedef struct _lvt_def
{
	unsigned long vector:8;//	Vector (Vector)〞Bits 7每0. This field contains the vector that will be sent for this interrupt source.
	unsigned long resrved0:4;
	unsigned long status:1;	//Delivery Status (DlvryStat)〞Bit 12. This bit is set to indicate that the interrupt has not yet been
							//accepted by the CPU.
	unsigned long resrved1:3;
	unsigned long mask:1;//Mask (Mask)〞Bit 16. If this bit is set, this LVT Entry will not generate interrupts.
	unsigned long mode:1;//Mode (Mode)〞Bit 17. This bit is 0 for One-shot and 1 for Periodic.
} _LVT;

typedef struct _svr_def
{
	unsigned char vector;//	Vector (Vector)〞Bits 7每0. This field contains the vector that will be sent for this interrupt source.
	                     // bits 0-3 mudt be 0xf
	unsigned char APIC_Enabled:1;
	unsigned char Focus_Processor_Checking:1;	//Delivery Status (DlvryStat)〞Bit 12. This bit is set to indicate that the interrupt has not yet been
							//accepted by the CPU.
	unsigned short resrved;

} _SVR;

#pragma pack(pop)

#define DELIVERY_MODE_FIXED     0
#define DELIVERY_MODE_LOWEST_PRIORITY 1
#define DELIVERY_MODE_SMI       2
#define DELIVERY_MODE_RESERVED  3
#define DELIVERY_MODE_NMI       4
#define DELIVERY_MODE_INIT      5
#define DELIVERY_MODE_STARTUP   6
#define DELIVERY_MODE_EXTINT    7

#define DEST_SH_DEST        0
#define DEST_SH_SELF        1
#define DEST_SH_ALL_INC_S   2
#define DEST_SH_ALL_EXC_S   3


#define ICR() (*(_ICR*)&(registers[ICRLO>>4]))
#define LVT(i)(*(_LVT*)&(registers[i>>4]))
#define SVR()(*(_SVR*)&(registers[SPUR_INTR_VEC>>4]))

unsigned int  apic_read( unsigned int dwAddress);           // start dwAddress on 32bit bus
void apic_write( unsigned int dwAddress,unsigned int  data);// start dwAddress on 32bit bus

class CAPICNode;



typedef struct _apic_msg
{
	unsigned char dst,src;//ID or broadcast/
	unsigned char vector;
	unsigned char logical_dest:1;
	unsigned char level:1; // asserted/deasserted
	unsigned char delivery_mode:3;
	unsigned char dst_mode:3;
} APIC_MSG;

#define DST_NONE      0
#define DST_UNI       1
#define DST_ALL       2    
#define DST_ALLOTHERS 3

class CCPU; // local apic will init/startup/shutdown ap

typedef enum apic_state
{
	wait_for_SIPI, // power-on,INIT#, or INIT IPI
	SIPI_recved,   // INIT will be ignored
	SIPI_twice,   // INIT will be ignored
}APIC_STATE;

class CLocalAPIC 
{
	int state;
	bool int_waiting;
	unsigned long registers[MAX_APIC_REG];
	unsigned short division_cnt;
	unsigned short division_cfg;
	unsigned char  isrv;
	unsigned char irrv;
public:
	CLocalAPIC()
	{
		init();
	}
	void do_recv(APIC_MSG & msg);
	unsigned long read_register(unsigned long offset);
	void write_register(unsigned long offset,unsigned long data);
	void init();// after powered up or init ipi

	void id(unsigned char apic_id){registers[APIC_ID>>4]=(apic_id&0xf)<<24;}
	unsigned char id() {return (unsigned char )(registers[APIC_ID>>4]>>24);}
	unsigned char logical_dest() {return (unsigned char )(registers[LOG_DEST>>4]>>24);}
	unsigned char  ppr(){return (unsigned char )(registers[PROC_PRI>>4]);}
	unsigned char  tpr(){return (unsigned char )(registers[TPR_PRI>>4]);}
	unsigned char  abr(){return (unsigned char )(registers[ARB_PRI>>4]);}

	void decode_message(APIC_MSG & msg);
	void do_icr();
	void do_clock();
	void refresh_int();
	void calc_apr();
	void mark_vector(unsigned char vector);
	void unmark_vector(unsigned char vector);
	bool int_pending()
	{	
		return int_waiting
			&&SVR().APIC_Enabled==1
			&&(tpr()>>4)!=0xf      //
			&&irrv>tpr();
	}
	unsigned char get_int(){return get_bit((unsigned char *)&registers[IRR_BEGIN>>4]);}
	void transfer_msg(APIC_MSG & msg);
	void set_bit(unsigned char * s, int bit);
	void reset_bit(unsigned char * s, int bit);
	unsigned char  get_bit(unsigned char * s);

	void assert_lint0();
	void deassert_lint0();
	void dump();
	friend class CAPICBus;
public:
	unsigned char pin_lint0,pin_lint1;
	CCPU * p_cpu;
};

#define TimerInitCount     registers[INIT_CNT>>4]
#define TimerCurrentCount  registers[CURR_CNT>>4]
#define TimerDivisionCount registers[DIV_CNT>>4]
#define TimerLvt           registers[TIMER_LVT>>4]
#define LINT0Lvt           registers[LINT0_LVT>>4]
#define LINT1Lvt           registers[LINT1_LVT>>4]

class CIoAPIC 
{
	unsigned char  IRQPA;
	APIC_DWORD  index; // offset 0
//current_int;
	unsigned long registers[MAX_IOAPIC_REG];
	union
	{
		IOREDTBL      io_redir_tbl[NUM_IOAPIC_REDIR];
		APIC_DWORD   dw[1];
	};

public:
	CIoAPIC() {init();}

	unsigned char id() {return (unsigned char)(registers[IOAPICID]>>24);}
	unsigned long read_reg(unsigned long offset);
	void write_reg(unsigned long offset,unsigned long data);

	int redir_index(int index) {return (index -0x10)>>3;}
	void set_pin_status(int irq_no,bool asserted);
	void assert_irq(int irq_no){set_pin_status(irq_no,true);}
	void deassert_irq(int irq_no){set_pin_status(irq_no,false);}

	void init();
	
	bool get_message(APIC_MSG & msg);

	void dump();

	friend class CAPICBus;

};



class CAPICBus
{
	CLocalAPIC * nodes[CFG_MAX_LAPIC_NODES];
	int n_nodes;
	int last_send_node;
	int num_delay;

public:
	CAPICBus():n_nodes(0),num_delay(0)
	{
	}

	void register_node(CLocalAPIC * p_node);
	void transfer_msg( APIC_MSG & msg);

	void do_clock();
	void dump_msg(APIC_MSG &msg);
};
#endif