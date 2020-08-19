#include "cpu.h"
#include "log.h"

#define STEPPING 5
extern bool step;

typedef unsigned int address_t;

void CCPU::load_ss (SELECTOR selector)
{
	unsigned int dt_base;
	unsigned short dt_limit;

	dt_base  = IN_GDT(selector) ? gdt_base : ldt_base;
	dt_limit = IN_GDT(selector) ? gdt_limit: ldt_limit;
	
	if (selector.index == 0) GP(0)
	
	else if ( SELECTOR_TOP(selector) > dt_limit || ( selector.rpl != __CPL__) )
	{
			GP(MK_SHORT(selector))
	}
	else {
			load_descriptor(dt_base,&tmp_desc1,selector.index);
		  //AR byte must indicate a writable data segment ELSE #GP(selector);
		  // DPL in the AR byte must equal CPL ELSE #GP(selector);
			if ( WRITEABLE (tmp_desc1)==0 || DPL(tmp_desc1) != __CPL__ )
				GP(MK_SHORT(selector))
		
			else if (!PRESENT (tmp_desc1)) 
				SS(MK_SHORT(selector));
	
			__SS__  = selector;
			ss_desc = tmp_desc1;
	}
}

#define RESET_DESCRIPTOR(a,ar)                       \
	WORD0_OF  (a) = 0xffff;      /* limit = 0xffff */\
    WORD1_OF  (a) =              /* base = 0x0000  */\
    BYTE4_OF  (a) = 0;             /* base_mi  */\
    BYTE5_OF  (a) = (ar);         /*ar        */ \
	BYTE6_OF  (a) = 0x10;        /*available*/ \
    BYTE7_OF  (a) = 0;             /* base_hi... */\
/*!
 *    \brief:
 *         System register initialization
 */


//The processor resets the value in its internal SMBASE register to 30000H on a RESET, 
 //but does not change it on an INIT.)
CCPU::CCPU() :smbase(0x30000)

{}

void CCPU::setup(int _id ,bool _bsp)
{   
	//Software must ensure that all APICs are assigned unique APIC IDs. 
	//When both ApicExtId and ApicExtBrdCst in the HyperTransport.
	//Transaction Control Register are set, all 8 bits of APIC ID are used. When either ApicExtID
	//or ApicExtBrdCst is clear, only bits 3¨C0 of APIC ID are used, and bits 7¨C4 are reserved. 
	id=_id;
	bsp=_bsp;
	mmu.cr=cr;
	mmu.p_memory=&memory_interface;
	memory_interface.p_biu=&biu;
	memory_interface.p_lapic=&lapic;
	fetcher.p_mmu=&mmu;


	lapic.init();
	//??Node ID is initially 00h if this is the boot strap processor or 07h for all other nodes.
	lapic.id(bsp?0:7);
	lapic.p_cpu=this;
	reset();
	if(!bsp) halt=true;
}

void CCPU::reset()
{
	smi_pin=
	intr=false;

	cr0_pending=0;

	__EFLAGS__ = 2;



	__EDX__ = CPU_SIG;  /* processor arch */
  //Pentium   4 00000FxxH   
  //P6   Family 000006xxH   
  //Pentium 000005xxH   

	__EAX__ =
	__ECX__ =

	__EBX__ =
	__ESI__ =
	__EDI__ =
	__EBP__ =
	__ESP__ = 0;


	
	/* limit = 0xffff 
       base = 0x0000  
       AR: P | R/W | A 
	 */

	___CS___  = 
	___DS___  =
	___ES___  =
	___SS___  =
	___FS___  =
	___GS___  = 0;
 	__NEXT_EIP__ =
 	__EIP__ = 0xfff0;    /* cs:ip = 0xffff0 */

    RESET_DESCRIPTOR (cs_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (ds_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (es_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (ss_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (fs_desc, 0x83); /*AR: P | R/W | A */	
    RESET_DESCRIPTOR (gs_desc, 0x83); /*AR: P | R/W | A */

	cpl=0;//real mode 
	___CS___  = 0xf000;
	cs_desc.base_mi=0xff;// cs base:ffff0000
	cs_desc.base_lo=0x0;
	cs_desc.base_hi=0xff;
	/* ldtr, gdtr, idtr */
	___LDT___ = 0;

	ldt_base  = 0;
	ldt_limit = 0xffff;

	gdt_base  = 0;
	gdt_limit = 0xffff;

	idt_base  = 0;
	idt_limit = 0x3ff;

    /*  tss */
	___TR___ = 0;
    RESET_DESCRIPTOR (tr_desc, 0x82);   /*AR: P | R/W  */

	/* cr 0/1/2/3/4 */
	___CR0___ = 0x60000010;//60000010H
	___CR1___ =
	___CR2___ =
	___CR3___ =
	___CR4___ = 0;

	/* dr 0/1/2/3 */
	dr[0] = 
	dr[1] = 
	dr[2] = 
	dr[3] = 
	dr[4] = 
	dr[5] = 0;

	/* dr6 = 0xffff0ff0 */
	/* dr7 = 0x00000400 */
	dr[6] = 0xffff0ff0;
	dr[7] = 0x00000400;

	/* Time Stamp Counter = 0*/

    /* Control and Event Select = 0 */

	/* tr12 = 0 */

	/* msr's  */
	init_msr();


	/* CW = 0x40 */

	/* SW = 0 */

	/* TW = 0x5555 */

	/* FIP/FEA/FCS/FDS/FOP = 0 */

	/* FSTACK = 0 */

	/* SMBASE = 0x3000 */
	mxcsr=0x1F80;//FINIT/FNINIT: Unchanged

	/* DCache and ICache */
	dcache_invalid ();
	icache_incalid ();

	/* ITLB and DTLB, BTB, SDC */
	itlb_invalid ();
	dtlb_invalid ();


	// initialize MMU



	// initialize fetcher
	sync_fetch_eip(__EIP__);
}



void   CCPU::copy_cs_desc (SELECTOR  selector,int desc_no)
		
{
	copy_sreg_desc(CS,selector,desc_no);
	cpl=(__VM__==1)?3:RPL(selector);
}

void    CCPU::copy_sreg_desc (unsigned char seg, SELECTOR  selector,int desc_no)
{
	descriptors[seg]=descriptors[desc_no];
	reg_file.sreg[seg]=MK_SHORT(selector);
}

void    CCPU::load_seg_real_or_v86(unsigned short seg,unsigned short data)
{
	reg_file.sreg[seg]= data;

	descriptors[seg].base_lo=(data<<4)&0xffff;
	descriptors[seg].base_mi=(data>>12)&0x0f;
	descriptors[seg].base_hi=0;
}

void CCPU::load_dseg (SEGREG sreg, SELECTOR selector)
{
	unsigned int dt_base;
	unsigned short dt_limit;
	DESCRIPTOR  desc;
	dt_base  = IN_GDT(selector) ? gdt_base : ldt_base;
	dt_limit = IN_GDT(selector) ? gdt_limit: ldt_limit;
	//	IF DS, ES, FS or GS is loaded with a null selector;
	//THEN
	//   Load segment register with selector;
	//   Clear descriptor valid bit;
	//FI;

	if (selector.index == 0) 
	{
		if(sreg<NO_SEGOVR)
		{
				descriptors[sreg].available=0;
				reg_file.sreg[sreg]=0;
		}
		//if (sreg==ES)       {__ES__=selector;es_desc.available=0;}
		//else if (sreg==DS)  {__DS__=selector;ds_desc.available=0;}
		//else if (sreg==FS)  {__FS__=selector;fs_desc.available=0;}
		//else if (sreg==GS)  {__GS__=selector;gs_desc.available=0;}
		else {
			bug();
		}
	}
	else
   //Selector index must be within its descriptor table limits
   //   else #GP(selector);
	if ( SELECTOR_TOP(selector) > dt_limit  )
		GP(MK_SHORT(selector))
	else {
		load_descriptor(dt_base,&desc,selector.index);


	   //AR byte must indicate data or readable code segment else
	   //#GP(selector);
		if ( DATA_SEG (desc) ||(READABLE(desc)&&CODE_SEG (desc)))
		{
		}
		else
			GP(MK_SHORT(selector));
	   //IF data or nonconforming code segment
		if ( DATA_SEG (desc) ||(CONFORMING(desc)&&CODE_SEG (desc)))
		{
			
			//THEN both the RPL and the CPL must be less than or equal to DPL in
			//AR byte;
			//ELSE #GP(selector)			
			if( DPL(desc) < __CPL__ || DPL(desc) <RPL(selector))
				GP(MK_SHORT(selector));
		}

	   //Segment must be marked present else #NP(selector);
		if (!PRESENT (desc)) 
			NP(MK_SHORT(selector));
	   //Load segment register with selector;
	   //Load segment register with descriptor;

		if(sreg<NO_SEGOVR)
		{
			descriptors[sreg]=desc;
			reg_file.sreg[sreg]=MK_SHORT(selector);
		}

		//if (sreg==ES)       {__ES__=selector;es_desc=desc;}
		//else if (sreg==DS)  {__DS__=selector;ds_desc=desc;}
		//else if (sreg==FS)  {__FS__=selector;fs_desc=desc;}
		//else if (sreg==GS)  {__GS__=selector;gs_desc=desc;}
		else {
			bug();
		}
	
	}
}
void CCPU::set_sreg( int regno,unsigned short data)
{
	if(__PE__ == 0 ||  __VM__ == 1) load_seg_real_or_v86(regno,data);
	else
	{
	
		if (regno==SS) load_ss(*(SELECTOR*)&data);
		else if (regno==CS)bug();
		else if (regno==ES) load_dseg((SEGREG)regno,*(SELECTOR*)&data);
		else if (regno==DS) load_dseg((SEGREG)regno,*(SELECTOR*)&data);
		else if (regno==FS) load_dseg((SEGREG)regno,*(SELECTOR*)&data);
		else if (regno==GS) load_dseg((SEGREG)regno,*(SELECTOR*)&data);
		else
		{
			bug();
		}
	}
}


void CCPU::sync_fetch_eip(unsigned int new_eip)
{
	//update_cr0();

	//printf("%x\n",new_eip);
	fetcher.set_fetch_address (
		(__PE__ == 0)|| (__VM__ == 1)||(0==cs_desc.B), 
		BASE(cs_desc),
		LIMIT(cs_desc),
		new_eip,
		((__PE__ == 0)|| (__VM__ == 1)?0:(__CPL__==3))
		); //real mode

	//if(new_eip==0x40000600)
	//	::step=true;
}

unsigned long CCPU::get_special_reg(REG_CODE reg)
{
	unsigned long data;
	switch(reg.reg_cat)
	{
	case RC_REGCR:
		data= cr[reg.reg_no&7];
		break;
	case RC_REGDR:
		data= dr[reg.reg_no&7];
		break;
		break;
	case RC_REGTR:
		data= tr[reg.reg_no&7];
		break;
	default:
		bug();
	}
	//log("rd %cr[%d]:%x\n",reg.reg_cat==RC_REGCR?'c':
	//	reg.reg_cat==RC_REGDR?'d':'t',reg.reg_no,data);
	return data;
}

void CCPU::set_special_reg(REG_CODE reg,unsigned long data)
{
	//log("%cr[%d]=%x\n",reg.reg_cat==RC_REGCR?'c':
		//reg.reg_cat==RC_REGDR?'d':'t',reg.reg_no,data);
	
	

	switch(reg.reg_cat)
	{
	case RC_REGCR:
		
		 if(reg.reg_no==3)
			 mmu.invalid_all_tlb(true);
		 else  if(reg.reg_no==4
			 && ((cr[4]^data)&0xb0)!=0)  //pge,pae,pse
			 mmu.invalid_all_tlb(false);
		 else  if(reg.reg_no==0
			 && ((cr[0]^data)&0x80000001)!=0)  //pg,pe
			 mmu.invalid_all_tlb(false);

		 //
		 // cr0 changes. sync fetcher
		 //
		 if (reg.reg_no==0
			 &&(data&1)!=(cr[0]&1)) 
		 {
			 //log("cr0 cache:%x\n",data);
			 cr0_cache=data;
			 //entering/leaving real mode,
			 cr0_pending=true;
		 }
		 //
		 // cr3, flush none-global tlb
		 //
		 else cr[reg.reg_no]=data;

		break;
	case RC_REGDR:
		
		log("dr[%d]=%x\n",reg.reg_no,data);
		dr[reg.reg_no&7]=data;
		//
		// 
		//
		if(__DR7__.GD==1)
		{
			__DR6__.BD=1;
			DB_(0);
		}
		break;
		break;
	case RC_REGTR:
		log("tr[%d]=%x\n",reg.reg_no,data);
		 tr[reg.reg_no&7]=data;
		break;
	default:
		bug();
	}
}
void CCPU::update_cr0()
{

 ////log("cr0 cache:%x %x\n",cr0_cache,cr[0]);
cr[0]=cr0_cache;
cr0_pending=false;

if(__PE__==0)
	cpl=0;
}


bool CCPU::load_descriptor(unsigned long dt_base,void * dst,  unsigned short index)
{
	mmu.lddw(dt_base + (unsigned int)&((DESCRIPTOR*)NULL)[index],0,(unsigned long  *)dst);
	mmu.lddw(dt_base + (unsigned int)&((DESCRIPTOR*)NULL)[index]+4,0,((unsigned long *)dst)+1);
	return true;
}



#define DSEG_DESC tmp_desc2

void CCPU::null_invalid_sreg_selector(SELECTOR &SegmentSelector)
{
		unsigned int dt_base  = IN_GDT(SegmentSelector) ? gdt_base : ldt_base;
		unsigned int dt_limit = IN_GDT(SegmentSelector) ? gdt_limit: ldt_limit;

		load_descriptor(dt_base,&DSEG_DESC,SegmentSelector.index);

		//if (segment selector index is not within descriptor table limits
		//  OR segment descriptor indicates the segment is not a data or readable code segment
		//  OR if (the segment is a data or non-conforming code segment and the segment
		//     descriptor¡¯s DPL < CPL or RPL of code segment¡¯s segment selector
		if( SegmentSelector.index > dt_limit
			|| !DATA_SEG( DSEG_DESC )
			|| (CODE_SEG( DSEG_DESC) && !READABLE (DSEG_DESC) )
			|| ( DATA_SEG(DSEG_DESC)&& (( DPL(DSEG_DESC) <__CPL__ )|| DPL(DSEG_DESC)<RPL(SegmentSelector)))
			|| (!CONFORMING(DSEG_DESC) && (( DPL(DSEG_DESC) <__CPL__ )|| DPL(DSEG_DESC)<RPL(SegmentSelector))))   

		{
			/* segment register invalid */
			 SegmentSelector = NULLSELECTOR; /* null segment selector */
		}

}


void CCPU::int3_(int OperandSize)
{


	int_call(3,OperandSize);

}




void CCPU::ipi_init()// INIT IPI
{
	reset();
	halt=false;
	log0("AP %x INIT\n",lapic.id());
}
void CCPU::ipi_startup(unsigned char vector)// STARTUP IPI
{
	halt=false;
	cr0_pending=false;

	/* ldtr, gdtr, idtr */
	___LDT___ = 0;

	ldt_base  = 0;
	ldt_limit = 0xffff;

	gdt_base  = 0;
	gdt_limit = 0xffff;

	idt_base  = 0;
	idt_limit = 0x3ff;

    /*  tss */
	___TR___ = 0;
    RESET_DESCRIPTOR (tr_desc, 0x82);   /*AR: P | R/W  */

	/* cr 0/1/2/3/4 */
	___CR0___ = 0x60000010;//60000010H
	___CR1___ =
	___CR2___ =
	___CR3___ =
	___CR4___ = 0;

	/* dr 0/1/2/3 */
	dr[0] = 
	dr[1] = 
	dr[2] = 
	dr[3] = 
	dr[4] = 
	dr[5] = 0;

	/* dr6 = 0xffff0ff0 */
	/* dr7 = 0x00000400 */
	dr[6] = 0xffff0ff0;
	dr[7] = 0x00000400;
    RESET_DESCRIPTOR (cs_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (ds_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (es_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (ss_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (fs_desc, 0x83); /*AR: P | R/W | A */	
    RESET_DESCRIPTOR (gs_desc, 0x83); /*AR: P | R/W | A */

	cpl=0;//real mode 
	__NEXT_EIP__=
	__EIP__ =0;
	load_seg_real_or_v86(CS,vector<<8);
	__EAX__=vector<<8;
	__ECX__ =
	__EBX__ =
	__ESI__ =
	__EDI__ =
	__EBP__ =
	__ESP__ = 0;
	___DS___  =
	___ES___  =
	___SS___  =
	___FS___  =
	___GS___  = 0;
	sync_fetch_eip(0);

	log0("AP %x STARTUP, jump to %x:0000\n",lapic.id(),___CS___);
}

void CCPU::ipi_smi()// SMI IPI
{
	enter_smm();
	halt=false;
	log0("AP %x SMI\n",lapic.id());
}