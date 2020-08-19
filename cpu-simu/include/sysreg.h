#ifndef __SYSREG_H__
#define __SYSREG_H__

/*!
 *   \brief:
 *       General Descriptor Format
 */

#pragma pack(push,1)

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
	unsigned char  B            : 1;   /*  DEFAULTfor code or BIG for data */ 
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
#define DEFAULT(a)     ((a).B)
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
#define SELECTOR_TOP(a)  (((a).index+7))
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

#define INTERRUPT_GATE(a)   (((a).type &7)>=6) //== DESCRIPTOR_INT32 ||(a).type == DESCRIPTOR_INT16)
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

typedef struct pae_cr3_def {
	 unsigned int reserved0 : 3;
	 unsigned int pwt       : 1;
	 unsigned int pcd       : 1;
	 unsigned int pdptbr : 27;  //PAGE DIRECTORY pointer table BASE REGISTER 
} PAE_CR3;

 typedef struct cr4_def {
	unsigned int  vme:1 ;//0 VME Virtual 8086 mode extensions Like the virtual interrupt flag; enabled if set 
	unsigned int  pvi:1 ;//1 PVI Protected Mode Virtual Interrupts Enables hardware support of virtual interrupt flag (VIF) in protected mode 
	unsigned int  tsd:1 ;//2 TSD Time Stamp Disable If set, RDTSC instruction can only be executed when in ring 0, otherwise RDTSC can be used at any privilege level 
	unsigned int  de :1 ;//3 DE Debugging Extensions 
	unsigned int  pse:1 ;//4 PSE Page Size Extensions If unset page size is 4 KiB, else it is increased to 4 MiB, or with PAE, 2 MiB 
	unsigned int  pae:1 ;//5 PAE Physical Address Extension Changes page table layout to translate 32-bit virtual address into extended 36-bit physical address 
	unsigned int  mce:1 ;//6 MCE Machine Check Exception Enables machine check interrupts to occur if set
	unsigned int  pge:1 ;//7 PGE Page Global Enabled 
	unsigned int  pce:1 ;//8 PCE Performance-Monitoring Counter enable If set, RDPMC can be executed at any privilege level, else it can only be used in kernel mode (ring 0) 
	unsigned int  fxsr:1 ;//9 OSFXSR Operating system support for FXSAVE and FXSTOR instructions Must be set to use SSE instructions 
	unsigned int  xmmxcpt:1 ;//10 OSXMMEXCPT Operating System Support for Unmasked SIMD Floating-Point Exceptions  
	unsigned int  und0:2 ;//
	unsigned int  vmxe:1 ;// 13 VMXE VMX Enable 
} CTRL_REG4;

typedef struct cr0_def {
	 unsigned int pe  :1 ;//0 PE Protected Mode Enable If 1, system is in protected mode, else system is in real mode 
	 unsigned int mp  :1; //1 MP Monitor co-processor Controls interaction of WAIT/FWAIT instructions with TS flag in CR0  
	 unsigned int em  :1; //2 EM Emulation If set, no x87 floating point unit present, if clear, x87 FPU present 
	 unsigned int ts  :1; //3 TS Task switched Allows saving x87 task context only after x87 instruction used after task switch
	 unsigned int et  :1; //4 ET Extension type 
	 unsigned int ne  :1; //5 NE Numeric error Enable internal x87 floating point error reporting when set, else enables PC style x87 error detection 
	 unsigned int und0:10;// reserved
	 unsigned int wp  :1; //16 WP Write protect
	 unsigned int und1:1; //17 reserved
	 unsigned int am  :1; //18 AM Alignment mask Alignment check enabled if AM set, 
	                      //   AC flag set (in EFLAGS register, and privilege level is 3 
	 unsigned int und2:10;// reserved
	 unsigned int nw  :1 ;//29 NW Not-write through
	 unsigned int cd  :1 ;//30 CD Cache disable
	 unsigned int pg  :1 ;//31 PG Paging If 1, enable paging and use the CR3 register, else disable paging 
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
	
	unsigned int eip,eflags,eax,ecx,edx,ebx,esp,ebp,esi,edi;

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

#define MK_SELECTOR(s) *(SELECTOR*)(&s)

//3:0 - Stepping 
//7:4 - Model 
//11:8 - Family 
//13:12 - Processor Type 
//19:16 - Extended Model 
//27:20 - Extended Family 
typedef struct cpu_signature
{
	unsigned long stepping:4;
	unsigned long model:4;
	unsigned long family:4;
	unsigned long processor_type:2;
	unsigned long reserved:2;
	unsigned long extended_model:4;
	unsigned long extended_family:8;
	unsigned long unused:4;
} CPU_SIGNATURE;

//Processor Type Field
#define PROCESSOR_OEM      0 //Original OEM processor 00B
#define PROCESSOR_OV       1 //Intel(R) OverDrive(R) processor 01B
#define PROCESSOR_DUAL     2 //Dual processor*  10B 
#define PROCESSOR_RESERVED 3 //Intel(R) reserved 11B

typedef struct feature_info
{
	unsigned long fpu:1;//0 FPU  Floating Point Unit On-Chip. The processor contains an x87 FPU.
	unsigned long vme:1;//1 VME  Virtual 8086 Mode Enhancements. Virtual 8086 mode enhancements, 
	                    //  including CR4.VME for controlling the feature, CR4.PVI for protected 
	                    //  mode virtual interrupts, software interrupt indirection, expansion 
	                    //  of the TSS with the software indirection bitmap, and EFLAGS.VIF and EFLAGS.VIP flags.
	unsigned long de: 1;//2 DE Debugging Extensions. Support for I/O breakpoints, including CR4.DE for controlling 
	                    //  the feature, and optional trapping of accesses to DR4 and DR5.
	unsigned long pse:1;//3 PSE  Page Size Extension. Large pages of size 4Mbyte are supported, 
	                    //  including CR4.PSE for controlling the feature, the defined dirty bit in 
	                    //  PDE (Page Directory Entries), optional reserved bit trapping in CR3, PDEs, and PTEs.
	unsigned long tsc:1;//4 TSC Time Stamp Counter. The RDTSC instruction is supported, including CR4.TSD for controlling privilege.
 	unsigned long msr:1;//5 MSR  Model Specific Registers RDMSR and WRMSR Instructions. The RDMSR and WRMSR instructions are supported. 
	                    //  Some of the MSRs are implementation dependent.
	unsigned long pae:1;//6 PAE  Physical Address Extension. Physical addresses greater than 32 bits are supported: extended page 
	                    //  table entry formats, an extra level in the page translation tables is defined, 2 Mbyte pages are supported instead of 
	                    // 4 Mbyte pages if PAE bit is 1. The actual number of address bits beyond 32 is not defined, and is implementation specific.
	unsigned long mce:1;//7 MCE Machine Check Exception. Exception 18 is defined for Machine Checks, including CR4.MCE for controlling the feature. 
	                    //  This feature does not define the model-specific implementations of machine-check error logging, reporting, 
	                    //  and processor shutdowns. Machine Check exception handlers may have to depend on processor version to do model specific 
	                    //  processing of the exception, or test for the presence of the Machine Check feature.
	unsigned long cx8:1;//8 CX8 CMPXCHG8B Instruction. The compare-and-exchange 8 bytes (64 bits)instruction is supported (implicitly locked and atomic).
	unsigned long apic:1;//9 APIC APIC On-Chip. The processor contains an Advanced Programmable Interrupt Controller (APIC), responding to memory mapped commands 
	                    //   in the physical address range FFFE0000H to FFFE0FFFH (by default - some processors permit the APIC to be relocated).
	unsigned long red1:1;//10Reserved Reserved 
	unsigned long sep:1; //11 SEP SYSENTER and SYSEXIT Instructions. The SYSENTER and SYSEXIT and associated MSRs are supported.
	unsigned long mtrr:1;//12 MTRR  Memory Type Range Registers. MTRRs are supported. The MTRRcap MSR contains feature bits that describe what memory types are supported, 
	                     //         how many variable MTRRs are supported, and whether fixed MTRRs are supported.
	unsigned long pge:1 ;//13 PGE PTE Global Bit. The global bit in page directory entries (PDEs) and page table entries (PTEs) is supported, indicating 
	                     //   TLB entries that are common to different processes and need not be flushed. The CR4.PGE bit controls this feature.
	unsigned long mca:1; //14 MCA Machine Check Architecture. The Machine Check Architecture, which provides a compatible mechanism for error reporting in P6 family,P4, and Xeon is supported. 
	                     //   The MCG_CAP MSR contains feature bits describing how many banks of error reporting MSRs are supported.
	unsigned long cmov:1;//15 CMOV Conditional Move Instructions. The conditional move instruction CMOV is supported. In addition, if x87 FPU is present as indicated by the CPUID.FPU feature bit, 
	                     //         then the FCOMI and FCMOV instructions are supported.
	unsigned long  pat:1;//16 PAT Page Attribute Table. Page Attribute Table is supported. This feature augments the Memory Type Range Registers (MTRRs), allowing an operating system to specify 
	                     //            attributes of memory on a 4K granularity through a linear address.
	unsigned long  pse36:1; //17 PSE-36 32-Bit Page Size Extension. Extended 4-MByte pages that are capable of addressing physical memory beyond 4 GBytes are supported. This feature indicates 
	                        //         that the upper four bits of the physical address of the 4-MByte page is encoded by bits 13-16 of the page directory entry.
	unsigned long psn:1;    //18 PSN Processor Serial Number. The processor supports the 96-bit processor identification number feature and the feature is enabled.
	unsigned long clflush:1;//19 CLFLSH CLFLUSH Instruction. CLFLUSH Instruction is supported.
	unsigned long  rsed:1;//20 Reserved. Reserved
	unsigned long  ds:1;  //21 DS Debug Store. The processor supports the ability to write debug information into a memory resident buffer. This feature is used by the branch trace store 
	                      //  (BTS) and precise event-based sampling (PEBS) facilities 
	unsigned long  acpi:1;//22 ACPI Thermal Monitor and Software Controlled Clock Facilities. The processor implements internal MSRs that allow processor temperature to be monitored and
                          //   processor performance to be modulated in predefined duty cycles under software control.
	unsigned long mmx:1;  //23 MMX Intel MMX™ Technology. The processor supports the Intel MMX technology.
	unsigned long  fxsr:1;//24 FXSR FXSAVE and FXRSTOR Instructions. The FXSAVE and FXRSTOR instructions are supported for fast save and restore of the floating point context. 
	                      //   Presence ofthis bit also indicates that CR4.OSFXSR is available for an operating system to indicate that it supports the FXSAVE and FXRSTOR instructions.
	unsigned long sse:1;  //25 SSE SSE. The processor supports the SSE extensions.
	unsigned long  sse2:1;//26 SSE2 SSE2. The processor supports the SSE2 extensions.
	unsigned long  ss:1;  //27 SS Self Snoop. The processor supports the management of conflicting memory types by performing a snoop of its own cache structure for transactions issued to the bus.
	unsigned long htt:1;  //28 HTT  Hyper-Threading Technology. The processor implements Hyper-Threading technology.
	unsigned long tm:1;   //29 TM Thermal Monitor. The processor implements the thermal monitor automatic thermal control circuitry (TCC).
	unsigned long rsed2:1; //30 Reserved. Reserved
	unsigned long pbe:1;  //31 PBE Pending Break Enable. The processor supports the use of the FERR#/PBE# pin when the processor is in the stop-clock state 
	                      //   (STPCLK# is asserted) to signal the processor that an interrupt is pending and that the processor should return to normal operation to handle the interrupt. 
	                      //   Bit 10 (PBE enable) in the IA32_MISC_ENABLE MSR enables this capability.
} FEATURE_INFORMATION;
 






#define MSR_SYSENTER_CS				0x174
#define MSR_SYSENTER_ESP			0x175
#define MSR_SYSENTER_EIP			0x176
#define MSR_MTRRCAP					0xFE
#define MSR_MTRR_DEF_TYPE			0x2FF
#define MSR_MTRR_PHYSBASE0			0x200
#define MSR_MTRR_PHYSMASK0			0x201
#define MSR_MTRR_PHYSBASE1			0x202
#define MSR_MTRR_PHYSMASK1			0x203
#define MSR_MTRR_PHYSBASE2			0x204
#define MSR_MTRR_PHYSMASK2			0x205
#define MSR_MTRR_PHYSBASE3			0x206
#define MSR_MTRR_PHYSMASK3			0x207
#define MSR_MTRR_PHYSBASE4			0x208
#define MSR_MTRR_PHYSMASK4			0x209
#define MSR_MTRR_PHYSBASE5			0x20A
#define MSR_MTRR_PHYSMASK5			0x20B
#define MSR_MTRR_PHYSBASE6			0x20C
#define MSR_MTRR_PHYSMASK6			0x20D
#define MSR_MTRR_PHYSBASE7			0x20E
#define MSR_MTRR_PHYSMASK7			0x20F
#define MSR_PAT						0x277
#define MTRR_MASK_VALID				0x800

#define MEM_UNCACHEABLE				0
#define MEM_WRITE_COMBINING			1
#define MEM_WRITE_THROUGH			4
#define MEM_WRITE_PROTECTED			5
#define MEM_WRITEBACK				6
#define MEM_UNCACHED				7


typedef struct _dr7_
{
	unsigned long  L0:1;
	unsigned long  G0:1;
	unsigned long  L1:1;
	unsigned long  G1:1;
	unsigned long  L2:1;
	unsigned long  G2:1;
	unsigned long  L3:1;
	unsigned long  G3:1;
	unsigned long  LE:1;
	unsigned long  GE:1;
	unsigned long  RV:1;
	unsigned long  Z0:2;
	unsigned long  GD:1;
	unsigned long  Z1:2;

	unsigned long  RW0:2;
	unsigned long  lEN0:2;
	unsigned long  RW1:2;
	unsigned long  lEN1:2;
	unsigned long  RW2:2;
	unsigned long  lEN2:2;
	unsigned long  RW3:2;
	unsigned long  lEN3:2;
}DR7;

typedef struct _dr6_
{
	unsigned long  B0:1;
	unsigned long  B1:1;
	unsigned long  B2:1;
	unsigned long  B3:1;
	unsigned long  ff0:9;
	unsigned long  BD:1;
	unsigned long  BS:1;
	unsigned long  BT:1;
	unsigned long  ff1:16;
}DR6;

#pragma pack(pop)

#endif   /*    __SYSREG_H__  */