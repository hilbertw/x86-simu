#include "cpu.h"
#include "log.h"


#pragma warning (disable :4244)
#define RESET_DESCRIPTOR(a,ar)                       \
	WORD0_OF  (a) = 0xffff;      /* limit = 0xffff */\
    WORD1_OF  (a) =0;              /* base = 0x0000  */\
    BYTE4_OF  (a) = 0;             /* base_mi  */\
    BYTE5_OF  (a) = (ar);         /*ar        */ \
	BYTE6_OF  (a) = 0xdf;        /*4klimit, big data available,limit_hi*/ \
    BYTE7_OF  (a) = 0;             /* base_hi... */\

#define SAVEL(a,b) memory_interface.mem_write_int(a,b)//MEM_WRITEL(a,b)
#define SAVEW(a,b) memory_interface.mem_write_short(a,b)//MEM_WRITEW(a,b)
//
// enter smm
//
void CCPU::enter_smm()
{
	dump_state();
	log("entering SMM, save state to %x\n",smbase);
	smm=true;
	biu.assert_smiact(true);
	//
	// save state
	//
	SAVEL(smbase+0x8000+0x7ffc,cr[0]);
	SAVEL(smbase+0x8000+0x7ff8,cr[3]);
	SAVEL(smbase+0x8000+0x7ff4,__EFLAGS__);
	SAVEL(smbase+0x8000+0x7ff0,__EIP__);
	SAVEL(smbase+0x8000+0x7fec,__EDI__);
	SAVEL(smbase+0x8000+0x7fe8,__ESI__);
	SAVEL(smbase+0x8000+0x7fe4,__EBP__);
	SAVEL(smbase+0x8000+0x7fe0,__ESP__);
	SAVEL(smbase+0x8000+0x7fdc,__EBX__);
	SAVEL(smbase+0x8000+0x7fd8,__EDX__);
	SAVEL(smbase+0x8000+0x7fd4,__ECX__);
	SAVEL(smbase+0x8000+0x7fd0,__EAX__);
	SAVEL(smbase+0x8000+0x7fcc,dr[6]);
	SAVEL(smbase+0x8000+0x7fc8,dr[7]);
	SAVEL(smbase+0x8000+0x7fc4,___TR___);
	SAVEL(smbase+0x8000+0x7fc0,ldt_base);
	SAVEL(smbase+0x8000+0x7fbc,___GS___);
	SAVEL(smbase+0x8000+0x7fb8,___FS___);
	SAVEL(smbase+0x8000+0x7fb4,___DS___);
	SAVEL(smbase+0x8000+0x7fb0,___SS___);
	SAVEL(smbase+0x8000+0x7fac,___CS___);
	SAVEL(smbase+0x8000+0x7fa8,___ES___);
	SAVEL(smbase+0x8000+0x7fa4,*(DWORD *)&es_desc);
	SAVEL(smbase+0x8000+0x7fa0,*((DWORD *)&es_desc+1));
	SAVEL(smbase+0x8000+0x7f9c,*(DWORD *)&cs_desc);
	SAVEL(smbase+0x8000+0x7f98,*((DWORD *)&cs_desc+1));
	SAVEL(smbase+0x8000+0x7f94,*(DWORD *)&ss_desc);
	SAVEL(smbase+0x8000+0x7f90,*((DWORD *)&ss_desc+1));
	SAVEL(smbase+0x8000+0x7f8c,*(DWORD *)&ds_desc);
	SAVEL(smbase+0x8000+0x7f88,*((DWORD *)&ds_desc+1));
	SAVEL(smbase+0x8000+0x7f84,*(DWORD *)&fs_desc);
	SAVEL(smbase+0x8000+0x7f80,*((DWORD *)&fs_desc+1));
	SAVEL(smbase+0x8000+0x7f7c,*(DWORD *)&gs_desc);
	SAVEL(smbase+0x8000+0x7f78,*((DWORD *)&gs_desc+1));
	//
	// AUTO HLT
	//
	SAVEW(smbase+0x8000+0x7f02,halt?1:0);
	SAVEW(smbase+0x8000+0x7f00,0);
	SAVEL(smbase+0x8000+0x7efc,0x30004);// supports restart i/o,smbase relocation,at least 4
	SAVEL(smbase+0x8000+0x7ef8,smbase);
	//
	// init smm
	//
	//General-purpose registers Undefined
	//EFLAGS 00000002H
	//EIP 00008000H
	//CS selector SMM Base shifted right 4 bits (default 3000H)
	//CS base SMM Base (default 30000H)
	//DS, ES, FS, GS, SS Selectors 0000H
	//DS, ES, FS, GS, SS Bases 000000000H
	//DS, ES, FS, GS, SS Limits 0FFFFFFFFH
	//CR0 PE, EM, TS and PG flags set to 0; others unmodified
	//DR6 Undefined
	//DR7 00000400H
	intr=false;
	cr0_pending=0;

	__EFLAGS__ = 2;

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
 	__EIP__ = 0x8000;    /* cs:ip = 0xffff0 */

    RESET_DESCRIPTOR (cs_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (ds_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (es_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (ss_desc, 0x83); /*AR: P | R/W | A */
    RESET_DESCRIPTOR (fs_desc, 0x83); /*AR: P | R/W | A */	
    RESET_DESCRIPTOR (gs_desc, 0x83); /*AR: P | R/W | A */

	___CS___  = smbase>>4;
	cs_desc.base_mi=smbase>>16;// cs base:ffff0000
	cs_desc.base_lo=0x0;
	cs_desc.base_hi=0x00;

	cs_desc.B=0;
	cr[0] &= 0x7ffffff2;//60000010H
	cr[4]=0;



	/* dr7 = 0x00000400 */
	dr[7] = 0x00000400;


	// initialize fetcher
	sync_fetch_eip(__NEXT_EIP__=__EIP__);
}

#define RESTOREL(a,b) b=MEM_READL((a))
#define RESTOREW(a,b) b=MEM_READW((a))
//
// exiting smm
//	
void CCPU::leave_smm()
{
	long tr,gs,fs,ds,cs,ss,es,smm_revision;
	short restart_halt,restart_io;

	dump_state();
	log("leaving SMM, restote state from %x\n",smbase);
	//
	// restore state
	//
	RESTOREL(smbase+0x8000+0x7ffc,cr[0]);
	RESTOREL(smbase+0x8000+0x7ff8,cr[3]);
	RESTOREL(smbase+0x8000+0x7ff4,__EFLAGS__);
	RESTOREL(smbase+0x8000+0x7ff0,__NEXT_EIP__);//__EIP__=__NEXT_EIP__;
	RESTOREL(smbase+0x8000+0x7fec,__EDI__);
	RESTOREL(smbase+0x8000+0x7fe8,__ESI__);
	RESTOREL(smbase+0x8000+0x7fe4,__EBP__);
	RESTOREL(smbase+0x8000+0x7fe0,__ESP__);
	RESTOREL(smbase+0x8000+0x7fdc,__EBX__);
	RESTOREL(smbase+0x8000+0x7fd8,__EDX__);
	RESTOREL(smbase+0x8000+0x7fd4,__ECX__);
	RESTOREL(smbase+0x8000+0x7fd0,__EAX__);
	RESTOREL(smbase+0x8000+0x7fcc,dr[6]);
	RESTOREL(smbase+0x8000+0x7fc8,dr[7]);
	RESTOREL(smbase+0x8000+0x7fc4,tr);
	RESTOREL(smbase+0x8000+0x7fc0,ldt_base);
	RESTOREL(smbase+0x8000+0x7fbc,gs);
	RESTOREL(smbase+0x8000+0x7fb8,fs);
	RESTOREL(smbase+0x8000+0x7fb4,ds);
	RESTOREL(smbase+0x8000+0x7fb0,ss);
	RESTOREL(smbase+0x8000+0x7fac,cs);
	RESTOREL(smbase+0x8000+0x7fa8,es);
	RESTOREL(smbase+0x8000+0x7fa4,*(DWORD *)&es_desc);
	RESTOREL(smbase+0x8000+0x7fa0,*((DWORD *)&es_desc+1));
	RESTOREL(smbase+0x8000+0x7f9c,*(DWORD *)&cs_desc);
	RESTOREL(smbase+0x8000+0x7f98,*((DWORD *)&cs_desc+1));
	RESTOREL(smbase+0x8000+0x7f94,*(DWORD *)&ss_desc);
	RESTOREL(smbase+0x8000+0x7f90,*((DWORD *)&ss_desc+1));
	RESTOREL(smbase+0x8000+0x7f8c,*(DWORD *)&ds_desc);
	RESTOREL(smbase+0x8000+0x7f88,*((DWORD *)&ds_desc+1));
	RESTOREL(smbase+0x8000+0x7f84,*(DWORD *)&fs_desc);
	RESTOREL(smbase+0x8000+0x7f80,*((DWORD *)&fs_desc+1));
	RESTOREL(smbase+0x8000+0x7f7c,*(DWORD *)&gs_desc);
	RESTOREL(smbase+0x8000+0x7f78,*((DWORD *)&gs_desc+1));

	//
	//
	//
	//load_tr(MK_SELECTOR(tr));
	//load_ldt(MK_SELECTOR(ldt));

	//
	//load hidden part
	//
	___CS___=cs;
	set_sreg(GS,gs);
	set_sreg(FS,fs);
	set_sreg(DS,ds);
	set_sreg(SS,ss);
	set_sreg(ES,es);
	//
	// AUTO HLT
	//
	RESTOREW(smbase+0x8000+0x7f02,restart_halt);
	RESTOREW(smbase+0x8000+0x7f00,restart_io);
	RESTOREL(smbase+0x8000+0x7efc,smm_revision);
	RESTOREL(smbase+0x8000+0x7ef8,smbase);
	log("smbase @ rsm:%x\n",smbase);
	smbase=0xb0000;
	//
	//the SMBASE values must be aligned on a 32-KByte boundary or the processor will 
	// enter shutdown state during the execution of a RSMinstruction.)
	//
//The following registers are saved (but not readable) and restored upon exiting SMM:
// Control register CR4 (CR4 is set to ¡°0¡± while in the SMM handler).
// The hidden segment descriptor information stored in segment registers CS, DS, ES, FS,GS, SS.
	// how to do it?
	intr=false;
	cr0_pending=0;

	smm=false;
	biu.assert_smiact(false);

	// initialize fetcher
	sync_fetch_eip(__NEXT_EIP__);

	dump_state();
}
