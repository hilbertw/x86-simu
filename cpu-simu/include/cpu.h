#ifndef __CPU_H__
#define __CPU_H__

#include "types.h"
#include "common.h"
#include "exception.h"
#include "sysreg.h"
#include "regfile.h"
#include "mmu.h"
#include "mmu-ifetcher.h"
#include "msr.h"
#include "apic.h"

#include "fpu.h"
/*!
 *   \brief:
 *       system registers
 */

#define DESC_TMP1 MAX_SEG
#define DESC_TMP2 DESC_TMP1+1
#define DESC_TMP3 DESC_TMP1+2
#define DESC_TMP4 DESC_TMP1+3

class CCPU
{
public:


#include "sse-old.h"

	CFPU fpu87;
	CCPUException exception;
	DESCRIPTOR descriptors[MAX_SEG+3];


	SELECTOR ldt_seg;
	unsigned int ldt_base,gdt_base,idt_base;
	unsigned short  ldt_limit,gdt_limit,idt_limit;
	unsigned long mxcsr;

	SELECTOR   __TR__;
	DESCRIPTOR tr_desc;



	struct qword{	
		unsigned long lo;
		unsigned long hi;
	}msr1[MAX_MSR1],
	 msr2[MAX_MSR2],
	 msr3[MAX_MSR3];


	CREGFILE reg_file;

	unsigned int __NEXT_EIP__;

	//unsigned long tmpr32[8];//
	unsigned long cr[8],dr[8],tr[8];//
	unsigned long cr0_cache;
	unsigned int int_pin;
	bool intr;
	bool bsp;
	unsigned char id;


	//
	// smi 
	//
	bool smm;//SMIACT is asserteddeasserted along withsetting  smm
	bool halt;
	bool smi_pin;
	unsigned long smbase;



	//
	// debug 
	//
	unsigned char cpl;
	bool cr0_pending;
	//
	// state save boundary
	// Functional components
	//
	CLocalAPIC lapic;

	CMMU mmu;
	CIFetcherMMU fetcher;
	CMemoryInterface memory_interface;
	CBIU biu;

	//CCacheInterface cache;


#define  __CR0__	(*(CTRL_REG0*)&cr[0])   
#define  __CR3__	(*(CTRL_REG3*)&cr[3])   
#define  __CR4__	(*(CTRL_REG4*)&cr[3])   

#define ___CR0___ cr[0]
#define ___CR3___ cr[3]
#define ___CR1___ cr[1]
#define ___CR2___ cr[2]
#define ___CR4___ cr[4]

#define __CPL__ cpl//(__CS__.rpl)
#define __TSS__ ((TSS*)BASE (tr_desc))

#define __GDT__ ((DESCRIPTOR *)gdt_base)
#define __IDT__ ((DESCRIPTOR *)idt_base)
#define __LDT__ ((DESCRIPTOR *)ldt_base)

#define __EAX__ (reg_file.reg32[0])
#define __ECX__ (reg_file.reg32[1])
#define __EDX__ (reg_file.reg32[2])
#define __EBX__ (reg_file.reg32[3])
#define __ESP__ (reg_file.reg32[4])
#define __EBP__ (reg_file.reg32[5])
#define __ESI__ (reg_file.reg32[6])
#define __EDI__ (reg_file.reg32[7])

#define __AX__ (*(unsigned short *)&reg_file.reg32[0])
#define __CX__ (*(unsigned short *)&reg_file.reg32[1])
#define __DX__ (*(unsigned short *)&reg_file.reg32[2])
#define __BX__ (*(unsigned short *)&reg_file.reg32[3])
#define __SP__ (*(unsigned short *)&reg_file.reg32[4])
#define __BP__ (*(unsigned short *)&reg_file.reg32[5])
#define __SI__ (*(unsigned short *)&reg_file.reg32[6])
#define __DI__ (*(unsigned short *)&reg_file.reg32[7])

#define __TSC__ msr1[0x10]

#define __EIP__ reg_file.eip
#define __IP__ (*(unsigned short *)&__EIP__)


#define __AL__ (*(unsigned char *)&reg_file.reg32[0])
#define __CL__ (*(unsigned char *)&reg_file.reg32[1])
#define __DL__ (*(unsigned char *)&reg_file.reg32[2])
#define __BL__ (*(unsigned char *)&reg_file.reg32[3])
#define __AH__ (*((unsigned char *)&reg_file.reg32[0]+1))
#define __CH__ (*((unsigned char *)&reg_file.reg32[1]+1))
#define __DH__ (*((unsigned char *)&reg_file.reg32[2]+1))
#define __BH__ (*((unsigned char *)&reg_file.reg32[3]+1))

#define ___AL___ *(unsigned char *)&reg_file.reg32[0]
#define ___CL___ *(unsigned char *)&reg_file.reg32[1]
#define ___DL___ *(unsigned char *)&reg_file.reg32[2]
#define ___BL___ *(unsigned char *)&reg_file.reg32[3]
#define ___AH___ *((unsigned char *)&reg_file.reg32[0]+1)
#define ___CH___ *((unsigned char *)&reg_file.reg32[1]+1)
#define ___DH___ *((unsigned char *)&reg_file.reg32[2]+1)
#define ___BH___ *((unsigned char *)&reg_file.reg32[3]+1)

#define ___CS___ reg_file.sreg[CS]
#define ___DS___ reg_file.sreg[DS]
#define ___ES___ reg_file.sreg[ES]
#define ___SS___ reg_file.sreg[SS]
#define ___FS___ reg_file.sreg[FS]
#define ___GS___ reg_file.sreg[GS]

#define __CS__ (*(SELECTOR *)&___CS___)
#define __DS__ (*(SELECTOR *)&___DS___)
#define __ES__ (*(SELECTOR *)&___ES___)
#define __SS__ (*(SELECTOR *)&___SS___)
#define __FS__ (*(SELECTOR *)&___FS___)
#define __GS__ (*(SELECTOR *)&___GS___)

#define ___LDT___  *((unsigned short *)&ldt_seg)
#define ___TR___   *((unsigned short *)&__TR__)

#define __PE__ (__CR0__.pe)
#define __PG__ (__CR0__.pg)

#define __DR7__   (*(struct _dr7_*)&dr[7])
#define __DR6__   (*(struct _dr6_*)&dr[6])

#define ueflag (*(UEFLAGS*)&reg_file.eflags)



#define __EFLAGS__ (ueflag.dw)
#define __CFLAGS__ (*(unsigned short *)&ueflag)

#define __CF__   (ueflag.s.CF)
#define __PF__   (ueflag.s.PF)    
#define __AF__   (ueflag.s.AF)    
#define __ZF__   (ueflag.s.ZF)    
#define __SF__   (ueflag.s.SF)    
#define __TF__   (ueflag.s.TF)    
#define __IF__   (ueflag.s.IF)    
#define __DF__   (ueflag.s.DF)    
#define __OF__   (ueflag.s.OF)    
#define __IOPL__ (ueflag.s.IOPL)  
#define __NT__   (ueflag.s.NT)    
#define __RF__   (ueflag.s.RF)    
#define __VM__   (ueflag.s.VM)    
#define __AC__   (ueflag.s.AC)    
#define __VIF__  (ueflag.s.VIF)   
#define __VIP__  (ueflag.s.VIP)   
#define __ID__   (ueflag.s.ID)  

#define  cs_desc descriptors[CS]
#define  ds_desc descriptors[DS]
#define  es_desc descriptors[ES]
#define  ss_desc descriptors[SS]
#define  fs_desc descriptors[FS]
#define  gs_desc descriptors[GS]

#define  tmp_desc1  descriptors[DESC_TMP1]
#define  tmp_desc2  descriptors[DESC_TMP2]
#define  tmp_desc3  descriptors[DESC_TMP3]
#define  tmp_desc4  descriptors[DESC_TMP4]


#define  CS_DESC  descriptors[DESC_TMP1]
#define  SS_DESC  descriptors[DESC_TMP2]
#define  TSS_DESC descriptors[DESC_TMP3]
#define  CALLGATE_DESC descriptors[DESC_TMP4]
#define  INTGATE_DESC descriptors[DESC_TMP4]

#define  CS_DESC_NO DESC_TMP1
#define  SS_DESC_NO DESC_TMP2
#define  TSS_DESC_NO DESC_TMP3
#define  CALLGATE_DESC_NO DESC_TMP4
#define  INTGATE_DESC_NO DESC_TMP4


#define  selector(x) (*(SELECTOR*)(&reg_file.sreg[x]))

#define GP(a)  {exception.RaiseException ( EX_GP , (unsigned)(a) ) ;exception_file=__FILE__;exception_line=__LINE__;return;}
#define NP(a)  {exception.RaiseException ( EX_NP , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;return;}
#define TS(a)  {exception.RaiseException ( EX_TSS , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;return;}
#define TS(a)  {exception.RaiseException ( EX_TSS , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;return;}
#define SS(a)  {exception.RaiseException ( EX_SS , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;return;}
#define PFT(a)  {exception.RaiseException ( EX_PF , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;return;}


#define GP_(a)  {exception.RaiseException ( EX_GP , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}
#define NP_(a)  {exception.RaiseException ( EX_NP , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}
#define SS_(a)  {exception.RaiseException ( EX_SS , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}
#define TS_(a)  {exception.RaiseException ( EX_TSS , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}
#define PFT_(a)  {exception.RaiseException ( EX_PF , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}

#define BR_(a)  {exception.RaiseException ( EX_BR , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}
#define UD_(a)  {exception.RaiseException ( EX_UD , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}
#define DE_(a)  {exception.RaiseException ( EX_DE , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}
#define DB_(a)  {exception.RaiseException ( EX_DB , (unsigned)(a));exception_file=__FILE__;exception_line=__LINE__;}

#define real_v86() (__PE__ == 0 ||  __VM__ == 1)

public:
	CCPU();
	
	void setup (int id,bool bsp);


	void enable_a20() {mmu.set_a20_mask((DWORD)-1);}
	void disable_a20() {mmu.set_a20_mask((DWORD)~0x100000);}


	//void run(bool (*)());
	bool step(/*bool trace*/);

	void reset();
	void init_msr();
	void assert_intr(){intr=true;}
	void deassert_intr(){intr=false;}
	//
	// descriptor manipulation
	//
	void    copy_sreg_desc (unsigned char seg, SELECTOR  selector,int desc_no);
	void    copy_cs_desc (SELECTOR  selector,int desc_no);
	void    load_ss (SELECTOR  selector);


    void    null_invalid_sreg_selector(SELECTOR &SegmentSelector);
	//void    load_cs_real_or_v86(unsigned short seg);
	void    load_seg_real_or_v86(unsigned short seg,unsigned short data);
	void	set_sreg( int regno,unsigned short data);
	void    load_dseg (SEGREG sreg, SELECTOR selector);
	void    load_tr (SELECTOR selector);
	void    load_ldt (SELECTOR selector);

	unsigned long get_special_reg(REG_CODE reg);

	void set_special_reg(REG_CODE reg,unsigned long data);

	bool    lgdt_ (MEM_DESC &m_info, int OperandSize,int addr_size);
	bool    lidt_ (MEM_DESC &m_info, int OperandSize,int addr_size);
	bool    sgdt_ (MEM_DESC &m_info, int OperandSize,int addr_size);
	bool    sidt_ (MEM_DESC &m_info, int OperandSize,int addr_size);

	unsigned short store_tr() {return MK_SHORT(__TR__);}
	//
	// branch
	//
	void    near_rel_call (int offset,int OperandSize) ;
	void    near_abs_call (address_t __eip__ ,int OperandSize);
	void    far_call86 (unsigned short  seg, unsigned int off32, int OperandSize);
	void    far_call (unsigned short seg, unsigned int off32,int OperandSize);

	void    iret_real (  int  OperandSize);
	void    iret_protected (  int  OperandSize);
	void    iret_(int OperandSize);
	void    iret_to_v86(SELECTOR  TempCS,unsigned long eip,unsigned long  eflags,int OperandSize);
	void    iret_from_v86(int OperandSize);
	void    iret_task(int OperandSize);
	void    iret_same_level(SELECTOR  TempCS,unsigned long eip,unsigned long  eflags,int OperandSize);
	void    iret_to_outer_level(SELECTOR  TempCS,unsigned long eip,unsigned long  eflags,int OperandSize);

	void    call_to_code_seg( SELECTOR cs, address_t off32,int OperandSize);
	void    call_to_call_gate( SELECTOR cs);
	void    switch_task (SELECTOR tss_selector,int desc_cache_line_no,bool nesting=false);

	bool    int_redirected(unsigned char int_no);

	void    exception_call(unsigned char exception_no,int OperandSize);
	void    int_real86 (unsigned int vector_number);
	void    int_protected (unsigned int vector_number,unsigned char INTn, 
		unsigned int error_code, unsigned with_error_code,  int  OperandSize);
	void    int_call (unsigned char vector,int OperandSize);
	void    int_inter_previlege(SELECTOR  cs,int error_code,int with_error_code,int OperandSize);
	void    int_v86(SELECTOR  cs,int error_code,int with_error_code,int OperandSize);
	void    int_intra_privilege(SELECTOR cs,int error_code,int with_error_code,int OperandSize);
	void    int_task_gate(int error_code, unsigned with_error_code);
	void    int3_(int OperandSize);

	void    near_ret (unsigned short imm,  int  OperandSize) ;
	void    far_ret86 ( unsigned short imm,int OperandSize);
	void    far_ret_protected ( unsigned short imm,int OperandSize);
	void    far_ret (unsigned short imm,  int  OperandSize);
	void    near_rel_jump (int offset,int OperandSize );
	void    near_abs_jump (address_t __eip__,int OperandSize );
	void    far_jump86 (unsigned short seg, unsigned int off32,int OperandSize );
	void    far_jump (unsigned short seg, unsigned int off32,int OperandSize );

	void    ex_w_errcode (unsigned short vector,unsigned long errcode,int OperandSize);
	void    ex_w_errcode86 (unsigned int vector_number,unsigned short err_code);

	void    loop_nz(int rel, int OperandSize, int AddressSize);
	void    loop_z(int rel, int OperandSize, int AddressSize);
	void    loop_(int rel, int OperandSize, int AddressSize);
	void    jcxz_(int rel, int OperandSize, int AddressSize);

	void    leave_(int OperandSize );
	void    enter_ (int size,int level, int OperandSize);
	void	dcache_invalid () {}
	void	icache_incalid () {}
	void	itlb_invalid () {}
	void	dtlb_invalid () {}
	void    wbinvd_() {}
	void    Popfd();
	void    cpuid_();
	void    update_cr0();
	void    rdmsr_();
	void    wrmsr_();

	void    cdq_(int OperandSize);
	void    cbw_(int OperandSize);
	void    xlat_ (char seg,int  AddressSize);
	void    sahf_();
	void    lahf_();
	unsigned long lar_ (SELECTOR selector,int  OperandSize);
	//
	// stack
	//
	bool    Push (unsigned long SOURCE, int  OperandSize);
	bool    Pop (unsigned long &DEST,  int  OperandSize);
	void    Pushl (unsigned long x) {Push(x,32);}
	void    Pushw (unsigned short x) {Push(x,16);}
	unsigned long  Popl () {unsigned long x; if (Pop(x,32)) return x; else return 0;}
	unsigned short Popw () {unsigned long x; if (Pop(x,16)) return (unsigned short)x; else return 0;}
	bool    Pusha ( int  OperandSize);
	bool    Popa ( int  OperandSize);
	bool    PUSH (unsigned long SOURCE, int  OperandSize);
	bool    POP (unsigned long &DEST,  int  OperandSize);
	void    PUSHL (unsigned long x) {PUSH(x,32);}
	void    PUSHW (unsigned short x) {PUSH(x,16);}
	unsigned long  POPL () {unsigned long x; if (POP(x,32)) return x; else return 0;}
	unsigned short POPW () {unsigned long x; if (POP(x,16)) return (unsigned short)x; else return 0;}
	bool  check_stack_for_pop (int size); /* for push */
	bool  check_stack_for_push (int size);/* for pop */
	bool  CHECK_STACK2 (int lower,int upper);/* for fsr return */
    bool prev_checker_da(char sel);

	//
	// I/O
	//

	bool check_iopermission(unsigned short SRC,int OperandSize);
	unsigned int  inport(unsigned short SRC,int OperandSize);
	void  outport(unsigned short SRC,int dat,int OperandSize);
	//
	//execute
	//
	void  lsq_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info);
	void  sse_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info);
	void  fpu_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info);
	void  branch_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info);
	void  alu_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info);
	void  mrom_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info);
	void  mult_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info);
	void  bitop_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info);
	void  exec_insn(PINSN_BASIC_INFO insn,PINSN_EXEC_INFO p_insn_exec_info);

	//
	// lsq
	//
	bool check_limit ( DESCRIPTOR *desc, unsigned int low, unsigned int high);
	bool  read_mem(MEM_DESC &m_info,unsigned long & data, int operand_size,int addr_size);
	bool  write_mem(MEM_DESC &m_info,unsigned long data, int operand_size,int addr_size);
	bool  read_string(MEM_DESC &m_info,char * data, int data_bytes,int addr_size);
	bool  write_string(MEM_DESC &m_info,char * data, int data_bytes,int addr_size);
	bool  read_mem_raw(char seg,unsigned long addr_hi,unsigned long addr_lo,unsigned long & data, int data_size,int addr_size);
	bool  write_mem_raw(char seg,unsigned long addr_hi,unsigned long addr_lo,unsigned long  data, int data_size,int addr_size);
	void  gen_mem_addr(MEM_DESC &m_info,int addr_size,unsigned long & hi, unsigned long & lo);
	void  gen_mem_addr_raw(unsigned long base,unsigned long disp,int addr_size,unsigned long & hi, unsigned long & lo);

	bool  simulator_read_mem(MEM_DESC &m_info,unsigned long & data, int operand_size,int addr_size);
	//
	// load and store by physical address
	//
	unsigned char MEM_READB (unsigned int addr)
	{
		unsigned char data;
		mmu.ldb(addr,false,(unsigned char *)&data);
		return data;
	}
	unsigned short MEM_READW (unsigned int addr)
	{
		unsigned short data;
		mmu.ldw(addr,false,(unsigned short *)&data);
		return data;
	}
	unsigned long  MEM_READL (unsigned int addr)
	{
		unsigned long data;
		mmu.lddw(addr,false,( unsigned long*)&data);
		return data;
	}
	void MEM_WRITEB (unsigned int addr,unsigned char data)
	{
		mmu.stb(addr,false,data);
	}
	void MEM_WRITEW (unsigned int addr,unsigned short data)
	{
		mmu.stw(addr,false,data);
	}
	void  MEM_WRITEL (unsigned int addr,unsigned long data)
	{
		mmu.stdw(addr,false,data);
	}
	//
	// insns
	//
	bool cmps_ (char override_seg, int OperandSize,int AddressSize);
	bool lods_ (char override_seg, int OperandSize,int AddressSize);
	bool stos_ (int OperandSize,int AddressSize);
	bool scas_ (int OperandSize,int AddressSize);
	bool movs_ (char override_seg, int OperandSize,int AddressSize);

	bool cmps_r (char override_seg, int OperandSize,int AddressSize);
	bool cmps_rnz (char override_seg, int OperandSize,int AddressSize);
	bool lods_r (char override_seg, int OperandSize,int AddressSize);
	bool stos_r (int OperandSize,int AddressSize);
	bool scas_r (int OperandSize,int AddressSize);
	bool scas_rnz (int OperandSize,int AddressSize);
	bool movs_r (char override_seg, int OperandSize,int AddressSize);
	
	bool outs_r (char override_seg,int OperandSize,int AddressSize);
	bool ins_r (int OperandSize ,int AddressSize);
	bool outs_ (char override_seg,int OperandSize,int AddressSize);
	bool ins_ (int OperandSize,int AddressSize );
	//
	// other instructions
	//
	void das_();
	void daa_();
	void aaa_();
	void aas_();
	void aad_(unsigned char imm8);
	void aam_(unsigned char imm8);
	void arpl_(SELECTOR src, SELECTOR &dst);
	void bound_(unsigned long LeftSRC,unsigned long RightSRC, int OperandSize);

	void rdtsc_();
	void rdpmc_();
	void lmsw_(unsigned short msw, int OperandSize);
	void cmpxchg8b_(MEM_DESC &m_info,int addr_size);
	void cmpxchg_(OPERAND_R_M &r_m,unsigned long  reg,int data_size,int addr_size);
	void sync_fetch_eip(unsigned int new_eip);



	bool load_descriptor( unsigned long dt_base,void * dst, unsigned short index);
	void update_reg(REG_CODE reg,unsigned long data);


//trace
//


	INSN_BASIC_INFO insn;
	INSN_EXEC_INFO exec_info;
	void revert_sf() {__SF__=!(__SF__);}
	void revert_zf() {__ZF__=!(__ZF__);}
	void revert_cf() {__CF__=!(__CF__);}
	unsigned long eip(){return __EIP__;}
	unsigned long next_eip(){return insn.eip+insn.len;}
	void dump_state();
	void dump_insn(INSN_BASIC_INFO *p_insn);
	void dump_int(unsigned int  n);
	void dump_mem(unsigned short seg,unsigned long offset,unsigned long _size);
	void dump_stack();
	void dump_disasm();
	void dump_tss();

	char * exception_file;
	int exception_line;



	//
	// smi
	//
	void assert_smi(){smi_pin=true;}
	//bool SMIACT(){return smm;}

	void enter_smm();
	void leave_smm();

	void bug() 
	{
#ifdef __GNUC__
	__asm__ __volatile__ ("int $3");
#else
	__asm int 3;
#endif
	}

	//
	// state dump
	//
	void dump_gdt();
	void dump_ldt();
	void dump_idt();
	//
	// ipi
	//
	void ipi_init();// INIT IPI
	void ipi_startup(unsigned char vector);// STARTUP IPI
	void ipi_smi();// SMI IPI
	//
	//
	//something like rob
	//
	struct reg_update_rec
	{
		REG_CODE reg;
		unsigned int data;
	} reg_update_table[10];
	int reg_update_rec_num;
};





#ifndef  OFFSET
#define OFFSET(t,m) ((unsigned long)&((t *)NULL)->m)
#endif
//
// each insn updates some registers.commit in wb  stage
//


#define CPU_STAE_SIZE  OFFSET(CCPU,lapic.p_cpu)


#define CPU_SIG 0xF29

#endif   /*   __CPU_H__  */