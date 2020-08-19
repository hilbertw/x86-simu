#define _CRT_SECURE_NO_DEPRECATE 1
#include <stdio.h>
#include <conio.h>
#include "cpu.h"
#include "util.h"
#include "log.h"


 void vt_wr_msg(char*msg);
 bool vt_msg_loop_once();


void CCPU::update_reg(REG_CODE reg,unsigned long data)
{
	// need not  check reg_update_rec_num range
	 reg_update_table[reg_update_rec_num].reg=reg;
	 reg_update_table[reg_update_rec_num].data=data;
	reg_update_rec_num++;
}

void CCPU:: exec_insn(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	switch(p_insn->p_instr_desc->funit)
	{
	case alu: alu_insn_exec( p_insn,p_insn_exec_info);break;
	case lsq : lsq_insn_exec( p_insn,p_insn_exec_info);break;
	case branch : branch_insn_exec( p_insn,p_insn_exec_info);break;
	case fpu: fpu_insn_exec( p_insn,p_insn_exec_info);break;
	case sse: sse_insn_exec( p_insn,p_insn_exec_info);break;
	case mrom: mrom_insn_exec( p_insn,p_insn_exec_info);break;
	case mult: mult_insn_exec( p_insn,p_insn_exec_info);break;
	case bitop: bitop_insn_exec( p_insn,p_insn_exec_info);break;
	case disasm:
	case misc: 
		break;
	default:
		bug();
	}
}
extern bool step;
bool CCPU::step(/**/)
{
	bool mode_16=cs_desc.B==0;//__PE__ == 0 || (__PE__ == 1 && __VM__ == 1);
	//
	// halted. detecting ext  int,and return to next insn
	//
	if(halt)
	{
		if(__IF__==1)
		{
			char int_vector;

			if(intr)
			{
				 int_vector=biu.inta();
				 goto l_exit_hlt;
			}
			else if(lapic.int_pending())
			{
				int_vector=lapic.get_int();
	
l_exit_hlt:
				//log("resume from halt by ext int %x \n",int_vector);
				halt=false;
				int_call(int_vector,mode_16?16:32);
			}
		}
		else
		if(smi_pin&&!smm)
		{
			enter_smm();
			smi_pin=false;
			halt=false;
		}


		return true;
	}

	int tf_before_insn_retire=__TF__;

	int ex_num;

	exception.ClearException();


	int result =fetcher.fetch(&insn);

	if(FETCH_UD==result) 
	{
		log("UD:eip=%x\n",insn.eip);
		dump_mem(CS,insn.eip,0x100);
		__EIP__=insn.eip;
		UD_(0);
		goto scan_exception;

	}
	if(FETCH_SUCCESS!=result) 
	{
		__EIP__=insn.eip;

		log("Fetch  fault\n");
		PFT_(fetcher.fault());
		cr[2]=fetcher.fault_addr();
		goto scan_exception;
	}

	predecode_insn( &insn,&exec_info);

	if(insn.eip!=__NEXT_EIP__)
	{
		log("eip mismatch:%x %x\n",insn.eip,__NEXT_EIP__);
	}

	//if (instruction pointer not within code segment limits ) #GP(0);


	__NEXT_EIP__=insn.eip+insn.len;

#if 0
	if(insn.p_instr_desc->index==cmps)
	{

		if(mode_16)
		{
			dump_mem(___DS___,__ESI__,0x10);
			dump_mem(___ES___,__EDI__,0x10);
		}
		else
		{
			dump_mem(___DS___,__SI__,0x10);
			dump_mem(___ES___,__DI__,0x10);
		}
	}

	if(___CS___==0xc000&&exec_info.r_m.type==2)
	{
		unsigned long temp=0,temp1,temp2;
		gen_mem_addr(exec_info.r_m.operand_desc.mem,0x10,temp1,temp2);

		temp2+=BASE(descriptors[exec_info.r_m.operand_desc.mem.seg]);
		if( temp2>=0x22f40&& temp2<0x22f40+0xffff ) trace=true;

	}


	if(insn.code[insn.prefix_bytes]==0
		&&insn.code[insn.prefix_bytes+1]==0)
	{

			log("bad code.\n");
			return false;

	}
	if(*(unsigned short *)(insn.code+insn.prefix_bytes)==0x0fe74&&(__ZF__))
	{
			log("dead loop.\n");
			return false;
	}
#endif
	//
	// stop  endless loop: jmp $
	//
	if(*(unsigned short *)(insn.code+insn.prefix_bytes)==0x0feeb)
	{
			log("dead loop.halt\n");
			halt=true;
			return  true;
	}


	reg_update_rec_num=0;
	exec_insn(&insn,&exec_info);


	//
	// write back
	//
	int i;
	for(i=0;i<reg_update_rec_num;i++)
	{
		reg_file.write_reg(reg_update_table[i].reg,reg_update_table[i].data);
		//if(reg_update_table[i].reg.reg_cat==RC_REGG32
		//	&&reg_update_table[i].reg.reg_no==SP)
		//{

		//	if (last_esp!=reg_update_table[i].data)
		//	{
		//	log("%x %x\n",i,reg_update_table[i].data);
		//	last_esp=reg_update_table[i].data;
		//	}
		//}
	}


	//
	// process internal exception
	//


	if(exception.exceptions()!=0) goto scan_exception;
	//
	// update eip
	//
	__EIP__=__NEXT_EIP__;

	//
	// smi invocation,An SMI has a greater priority than debug exceptions and external interrupts.
	//
	if(smi_pin&&!smm)
	{
		enter_smm();
		smi_pin=false;
	}

	//
	// check apic status
	//

	else if((__IF__==1))
	{
		unsigned char int_vector;

		if(intr)
		{
			 int_vector=biu.inta();
			//log0("8259 ext int %x \n",int_vector);
			goto l_callint;
		}
		else if(lapic.int_pending())
		{
			int_vector=lapic.get_int();
			//log0("lapic ext int %x \n",int_vector);

l_callint:
			int_call(int_vector,mode_16?16:32);
		}




	}
	else if((tf_before_insn_retire==1))
	{
		//step=true;
		//__EIP__=insn.eip;
		int_call(1,mode_16?16:32);
	}
	__TSC__.lo++;
	if(__TSC__.lo==0) // lower dword overflow
		__TSC__.hi++;


	return true;
//
// debug 
//

scan_exception:
// with err code
//Invalid TSS                       10            Yes
//Segment not present               11            Yes
//Stack exception                   12            Yes
//General protection fault          13            Yes
//Page fault                        14            Yes

	if(exception.exceptions()&(1<<EX_DE))
	{
		ex_num=EX_DE;
		log("divided by zero.(%d) %s:%d\n",exception.exception_para[EX_DE],exception_file,exception_line);
		goto l_exception_handler;
	}

	else if(exception.exceptions()&(1<<EX_DB ))
	{
		//Some debug exceptions are traps and some are faults.  
		//The exception handler can determine which has occurred by examining DR6. 
		ex_num=EX_DB;
		log("Debug Exception.(%d) %s:%d\n",exception.exception_para[EX_DB],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_NMI ))
	{
		ex_num=EX_NMI;
		log("NMI.(%d) %s:%d\n",exception.exception_para[EX_NMI],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_BR))
	{
		ex_num=EX_BR;
		log("Debug Exception.(%d) %s:%d\n",exception.exception_para[EX_BR],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_OV))
	{
		ex_num=EX_OV;
		log("divided by zero.(%d) %s:%d\n",exception.exception_para[EX_OV],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_B  ))
	{
		ex_num=EX_B;
		log("Debug Exception.(%d) %s:%d\n",exception.exception_para[EX_B],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_UD))
	{
		ex_num=EX_UD;
		log("Unrecognized or unimplemented opcode.(%d) %s:%d\n",exception.exception_para[EX_UD],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_7 ))
	{
		ex_num=EX_7;
		log(" Coprocessor not available Exception.(%d) %s:%d\n",exception.exception_para[EX_7],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_8 ))
	{
		ex_num=EX_8;
		log(" System error (%d) %s:%d\n",exception.exception_para[EX_8],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_9   ))
	{
		ex_num=EX_9;
		log("Debug Exception.(%d) %s:%d\n",exception.exception_para[EX_9],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_TSS))
	{
		ex_num=EX_TSS;
		log("Invalid TSS Exception.(%d) %s:%d\n",exception.exception_para[EX_TSS],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_NP ))
	{
		ex_num=EX_NP;
		log("NP Exception.(%x,%d) %s:%d\n",cr[2],exception.exception_para[EX_NP],exception_file,exception_line);
		goto l_exception_handler;
	} 
	else if(exception.exceptions()&(1<<EX_SS ))
	{
		ex_num=EX_SS;
		log("SS Exception.(%d) %s:%d\n",exception.exception_para[EX_SS],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_GP ))
	{
		ex_num=EX_GP;
		//All GP faults are restartable. If the fault occurs while attempting to
		//vector to the handler for an external interrupt, the interrupted program is
		//restartable, but the interrupt may be lost.
		log("GP Exception.(%d) %s:%d\n",exception.exception_para[EX_GP],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_PF  ))
	{
		ex_num=EX_PF;
		//Any memory reference or code fetch
		log("Page fault Exception.(%x,%d) %s:%d\n",cr[2],exception.exception_para[EX_PF],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_387 ))
	{
		ex_num=EX_387;
		log("387 Exception.(%d) %s:%d\n",exception.exception_para[EX_387],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_AC ))
	{
		ex_num=EX_AC;
		log("Alignment  Exception.(%d) %s:%d\n",exception.exception_para[EX_387],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_MC ))
	{
		ex_num=EX_MC;
		log("Machine check Exception.(%d) %s:%d\n",exception.exception_para[EX_387],exception_file,exception_line);
		goto l_exception_handler;
	}
	else if(exception.exceptions()&(1<<EX_XM ))
	{
		ex_num=EX_XM;
		log("SIMD  Exception.(%d) %s:%d\n",exception.exception_para[EX_387],exception_file,exception_line);
		goto l_exception_handler;
	}
	else
	{
		log("Unknown %x- %s:%d\n",exception.exception_status,exception_file,exception_line);
		return true;
	}

l_exception_handler:
#if 0
	{

			char line[200];

			int pos=sprintf(line,"%04x:%08x ",___CS___,insn.eip);
			pos+=disasm_line(line+pos,&insn);	

		   dump_state();
		   log0("%s*\n",line);
	}
#endif
	if(exception.fault_mask&(1<<ex_num))
		__NEXT_EIP__=__EIP__;
	

	if(exception.para_mask&(1<<ex_num))
		ex_w_errcode(ex_num,exception.exception_para[ex_num],mode_16?16:32);
	else 
		exception_call(ex_num,mode_16?16:32);

	return true;

}

