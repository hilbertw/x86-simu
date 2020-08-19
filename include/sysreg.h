#ifndef __SYSREG_H__
#define __SYSREG_H__

/*!
 *   \brief:
 *       General Descriptor Format
 */
//struct SegDescr{
//    uint16 limit_1;   // limit, bits 0..15
//    uint16 base_1;    // base, bits 0..15
//    uint8 base_2;     // base, bits 16..23
//    uint8 type_attr;  // type_attr
//    uint8 lim_attr;
//      //^ bits 0..3: limit, bits 16..19
//      //^ bits 4..7: additional data/code attributes
//    uint8 base_3;     // base, bits 24..31
//};

typedef struct descriptor_def {
	unsigned short limit_lo;
	unsigned short base_lo;
	unsigned char  base_mi;
	// bit:40~47
	unsigned char  type     : 5;

	unsigned char  dpl      : 2;
	unsigned char  present  : 1;
	// bit:48~55	
	unsigned char  limit_hi     : 4;
	unsigned char  available    : 1;
	unsigned char  O            : 1;
	unsigned char  B            : 1;   /* BIG for code or DEFAULT for data */ 
	unsigned char  granularity  : 1;   /* unit of limit is byte or 4KB     */

	// bit:56~63
	unsigned char  base_hi;

} DESCRIPTOR;

#define CODE_SEG(a)    (((a).type & 0x18) == 0x18)
#define DATA_SEG(a)    (((a).type & 0x18) == 0x10)
#define TASKSTATE_SEG(a)(((a).type & 0x1d) == 9)
#define WRITEABLE(a)   ((a).type & 0x2)
#define READABLE(a)    ((a).type & 0x2)
#define CONFORMING(a)  ((a).type & 0x4)
#define EXPAND_DOWN(a) ((a).type & 0x4)
#define GRANULARITY(a) ((a).granularity)
#define BIG_SEGMENT(a) ((a).B)
#define PRESENT(a)     ((a).present)
#define DPL(a)         ((a).dpl)
#define AVAILABLE(a)   ((a).available)
#define LIMIT_V(a)     ((a).limit_lo | ((a).limit_hi<<16)) 
#define LIMIT(a)       ((a).granularity ? (LIMIT_V (a) << 12) | (unsigned)0xfff : LIMIT_V (a))
#define BASE(a)        ((a).base_lo | ((a).base_mi<<16)| ((a).base_hi<<24)) 


#define DWORD0_OF(s)  ((unsigned long *)&(s))[0]
#define DWORD1_OF(s)  ((unsigned long *)&(s))[1]
#define DWORD2_OF(s)  ((unsigned long *)&(s))[2]
#define DWORD3_OF(s)  ((unsigned long *)&(s))[3]
#define DWORD4_OF(s)  ((unsigned long *)&(s))[4]

#define WORD0_OF(s)  ((unsigned short *)&(s))[0]
#define WORD1_OF(s)  ((unsigned short *)&(s))[1]
#define WORD2_OF(s)  ((unsigned short *)&(s))[2]
#define WORD3_OF(s)  ((unsigned short *)&(s))[3]
#define WORD4_OF(s)  ((unsigned short *)&(s))[4]
#define WORD5_OF(s)  ((unsigned short *)&(s))[5]
#define WORD6_OF(s)  ((unsigned short *)&(s))[6]
#define WORD7_OF(s)  ((unsigned short *)&(s))[7]
#define WORD8_OF(s)  ((unsigned short *)&(s))[8]
#define WORD9_OF(s)  ((unsigned short *)&(s))[9]

#define BYTE0_OF(s)  ((unsigned char *)&(s))[0]
#define BYTE1_OF(s)  ((unsigned char *)&(s))[1]
#define BYTE2_OF(s)  ((unsigned char *)&(s))[2]
#define BYTE3_OF(s)  ((unsigned char *)&(s))[3]
#define BYTE4_OF(s)  ((unsigned char *)&(s))[4]
#define BYTE5_OF(s)  ((unsigned char *)&(s))[5]
#define BYTE6_OF(s)  ((unsigned char *)&(s))[6]
#define BYTE7_OF(s)  ((unsigned char *)&(s))[7]
#define BYTE8_OF(s)  ((unsigned char *)&(s))[8]
#define BYTE9_OF(s)  ((unsigned char *)&(s))[9]
#define BYTE10_OF(s)  ((unsigned char *)&(s))[10]
#define BYTE11_OF(s)  ((unsigned char *)&(s))[11]
#define BYTE12_OF(s)  ((unsigned char *)&(s))[12]
#define BYTE13_OF(s)  ((unsigned char *)&(s))[13]
#define BYTE14_OF(s)  ((unsigned char *)&(s))[14]
#define BYTE15_OF(s)  ((unsigned char *)&(s))[15]
#define BYTE16_OF(s)  ((unsigned char *)&(s))[16]
#define BYTE17_OF(s)  ((unsigned char *)&(s))[17]
#define BYTE18_OF(s)  ((unsigned char *)&(s))[18]
#define BYTE19_OF(s)  ((unsigned char *)&(s))[19]
/*!
 *   \brief:
 *       Selector Format
 */

 typedef struct selector_def {
	 unsigned short rpl    : 2;
	 unsigned short ti     : 1;  /* in GDT or LDT */
	 unsigned short index  : 13;
 } SELECTOR;

#define NULL_SELECTOR(a) ((a).index==0)
#define RPL(a)           ((a).rpl)
#define IN_LDT(a)        ((a).ti == 1)
#define IN_GDT(a)        ((a).ti == 0)

inline SELECTOR null_selector() 
{
	SELECTOR x; 
	
	*(unsigned short *)&x = 0; 
	return x;
}

#define NULLSELECTOR null_selector() 
/*!
 *   \brief:
 *       System Descriptor Format
 */

 typedef struct gate_descriptor_def {
	unsigned short offset_lo;
	SELECTOR selector;
	// bit:32~39
	unsigned char  unused   : 5;
	unsigned char  zero     : 3;
	// bit:40~47	
	unsigned char  type     : 5; 
	unsigned char  dpl      : 2;
	unsigned char  present  : 1;   
	// bit:48~63
	unsigned short  offset_hi;
} GATE_DESCRIPTOR;


#define GATE_SELECTOR(a)   (((GATE_DESCRIPTOR *)&a)->selector)
#define GATE_OFFSET(a)     (((GATE_DESCRIPTOR *)&a)->offset_hi << 16) \
                           |((GATE_DESCRIPTOR *)&a)->offset_lo
#define GATE_PARAMCOUNT(a) (((GATE_DESCRIPTOR *)&a)->unused)
#define GATE_BUSYTSS(a)    ((a).type & 2)
/*!
 *   \brief:
 *       System-Descriptor type
 *       0       -reserved
 *       1       Available 286 TSS (16 bit)
 *       2       LDT
 *       3       Busy 286 TSS (16 bit)
 *       4       Call Gate (16 bit)
 *       5       Task Gate
 *       6       286 Interrupt Gate (16 bit)
 *       7       286 Trap Gate (16 bit)
 *       8       -reserved
 *       9       Available 386 TSS
 *       A       -reserved
 *       B       Busy 386 TSS
 *       C       386 Call Gate
 *       D       -reserved
 *       E       386 Interrupt Gate
 *       F       386 Trap Gate
 */
#define DESCRIPTOR_RESERVED     0
#define DESCRIPTOR_FREETSS16    1
#define DESCRIPTOR_LDT          2
#define DESCRIPTOR_BUSYTSS16    3
#define DESCRIPTOR_CALLGATE16   4
#define DESCRIPTOR_TASKGATE     5
#define DESCRIPTOR_INT16        6
#define DESCRIPTOR_TRAP16       7
#define DESCRIPTOR_RESERVED1    8
#define DESCRIPTOR_FREETSS32    9
#define DESCRIPTOR_RESERVED2    0xA
#define DESCRIPTOR_BUSYTSS32    0xB
#define DESCRIPTOR_CALLGATE32   0xC
#define DESCRIPTOR_RESERVED3    0xD
#define DESCRIPTOR_INT32        0xE
#define DESCRIPTOR_TRAP32       0xF

#define INTERRUPT_GATE(a)   ((a).type == DESCRIPTOR_INT32 ||(a).type == DESCRIPTOR_INT16)
#define TASK_GATE(a)  ((a).type == DESCRIPTOR_TASKGATE )
#define TSS32(a)      ((a).type == DESCRIPTOR_FREETSS32 ||(a).type == DESCRIPTOR_BUSYTSS32)
#define TSS16(a)      ((a).type == DESCRIPTOR_FREETSS16 ||(a).type == DESCRIPTOR_BUSYTSS16)
#define GATE32(a)     ((a).type > DESCRIPTOR_RESERVED1 )
#define TSS_GATE(a)   (TSS32(a) || TSS16(a))
#define CALL_GATE(a)  ((a).type == DESCRIPTOR_CALLGATE32 ||(a).type == DESCRIPTOR_CALLGATE16)

typedef struct cr3_def {
	 unsigned int reserved0 : 3;
	 unsigned int pwt       : 1;
	 unsigned int pcd       : 1;
	 unsigned int reserved  : 7;
	 unsigned int pdbr      : 20;  //PAGE DIRECTORY BASE REGISTER
} CTRL_REG3;

typedef struct cr0_def {
	 unsigned int pe   : 1; // PE (Protection Enable, bit 0) 
	 unsigned int mp   : 1; // MP (Math Present, bit 1) 
	 unsigned int em   : 1; // EM (Emulation, bit 2) 
	 unsigned int ts   : 1; // TS (Task Switched, bit 3)
	 unsigned int et   : 1; // ET (Extension Type, bit 4) 
	 unsigned int zero0:11; // reserved
	 unsigned int wp   : 1; // Write Protection
	 unsigned int zero1:14; // reserved
	 unsigned int pg   : 1; // PG (Paging, bit 31) 
 } CTRL_REG0;

typedef int CTRL_REG2;

typedef struct tss_def {
	unsigned short link;
	unsigned short zero0;
	unsigned int   esp0;
	SELECTOR ss0;
	unsigned short zero1;
	unsigned int   esp1;
	SELECTOR ss1;
	unsigned short zero2;
	unsigned int   esp2;
	SELECTOR ss2;
	unsigned short zero3;
	
	CTRL_REG3 cr3;
	
	unsigned int eip,eax,ecx,edx,ebx,esp,ebp,esi,edi;

	SELECTOR es;
	
	unsigned short zero4;
	
	SELECTOR cs;
	
	unsigned short zero5;
	
	SELECTOR ss;
	
	unsigned short zero6;
	
	SELECTOR ds;
	
	unsigned short zero7;
	
	SELECTOR fs;
	
	unsigned short zero8;
	
	SELECTOR gs;

	unsigned short zero9;

	unsigned short ldt;  /* task's ldt */

	unsigned short zero10;

	//The T-bit (debug trap bit) which causes the processor to raise 
	//a debug exception when a task switch occurs 
	unsigned short t      :1;//
	unsigned short zero11 :15;

	unsigned short iomap;   /* I/O MAP base */
} TSS;

/*!
 *   \brief:
 *       TSS-Descriptor Format
 */

 typedef struct tss_descriptor_def {
	unsigned short limit_lo;
	unsigned short base_lo;
	unsigned char  base_mi;
	// bit:40~47
	unsigned char  type     : 5;    // 9 or 10: 10---busy TSS
	unsigned char  dpl      : 2;
	unsigned char  present  : 1;
	// bit:48~55	
	unsigned char  limit_hi     : 4;
	unsigned char  available    : 1;
	unsigned char  zero1        : 1;
	unsigned char  zero2        : 1;
	unsigned char  granularity  : 1;   /* unit of limit is byte or 4KB */
	// bit:56~63
	unsigned char  base_hi;
} TSS_DESCRIPTOR;



//S Indicates a Status Flag
//C Indicates a Control Flag
//X Indicates a System Flag
typedef union uflags_def {
	struct eflags_def {
		unsigned int CF    :1; //S Carry Flag (CF)
		unsigned int one   :1; //reserved
		unsigned int PF    :1; //S Parity Flag (PF)
		unsigned int zero1 :1; //reserved
		unsigned int AF    :1; //S Auxiliary Carry Flag (AF)
		unsigned int zero2 :1; //reserved
		unsigned int ZF    :1; //S Zero Flag (ZF)
		unsigned int SF    :1; //S Sign Flag (SF)
		unsigned int TF    :1; //X Trap Flag (TF)
		

		unsigned int IF    :1; //X Interrupt Enable Flag (IF)
		unsigned int DF    :1; //X Direction Flag (DF)
		unsigned int OF    :1; //X Overflow Flag (OF)
		unsigned int IOPL  :2; //X I/O Privilege Level (IOPL)

		unsigned int NT    :1; //X Nested Task (NT)
		unsigned int zero3 :1; //reserved
		unsigned int RF    :1; //X Resume Flag (RF)
		unsigned int VM    :1; //X Virtual 8086 Mode (VM)
		unsigned int AC    :1; //X Alignment Check (AC)
		
		unsigned int VIF   :1; //X Virtual Interrupt Flag (VIF)
		unsigned int VIP   :1; //X Virtual Interrupt Pending (VIP)
		unsigned int ID    :1; //X ID Flag (ID)
		unsigned int zero4 :10;//reserved
	} s;

	unsigned long dw;
} UEFLAGS;

#define CF(a)       (a.s.CF  )
#define PF(a)	   (a.s.PF  )
#define AF(a)	   (a.s.AF  )
#define ZF(a)	   (a.s.ZF  )
#define SF(a)	   (a.s.SF  )
#define TF(a)	   (a.s.TF  )
#define IF(a)	   (a.s.IF  )
#define DF(a)	   (a.s.DF  )
#define OF(a)	   (a.s.OF  )
#define IOPL(a)	   (a.s.IOPL)
#define NT(a)	   (a.s.NT  )
#define RF(a)	   (a.s.RF  )
#define VM(a)	   (a.s.VM  )
#define AC(a)	   (a.s.AC  )
#define VIF(a)	   (a.s.VIF )
#define VIP(a)	   (a.s.VIP )
#define ID(a)	   (a.s.ID  )


#endif   /*    __SYSREG_H__  */