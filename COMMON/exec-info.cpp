#define   _CRT_SECURE_NO_DEPRECATE 1
#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <instr_db.h>
#include <common.h>
#include <assert.h>

#define make_reg(reg,no,cat) {reg.reg_cat=(cat);reg.reg_no=(no);}

//
// may be bug here. all w_mask is 1?
//
static char get_regg_cat(bool word,int data_size)
{
	return (!word)?RC_REGG8:
		(data_size==16)?RC_REGG16:
		(data_size==32)?RC_REGG32:
		(data_size==64)?RC_REGG64:		
		(data_size==128)?RC_REGG128:	
		RC_NOTHING;
}

char addr16_base[]={
	BX,BX,BP,BP,SI, DI,BP,BX } ;  

char addr16_index[]={
	 SI, DI, SI, DI } ;  

void  decode_addr16( unsigned char  *code, PMEM_DESC p_mem)
{
	 p_mem->scale=0;
     switch(code[1]>>6)
     {
      case 0:
          if((code[1]&7)==6)
		  {
			  p_mem->fmt=AT_DIRECT;
			  p_mem->disp=*( unsigned short*)(code+2);
			  return;
		  }
		  p_mem->disp=0;
		  break;

      case 1:
          p_mem->disp=(char)code[2];
		  break;
      case 2:
          p_mem->disp=*(short*)(code+2);
		  break;
      case 3:

          return ;            // reportr illegal instruction
     }

	 p_mem->fmt=(code[1]&7)>=sizeof(addr16_index)/sizeof(addr16_index[0])?AT_BASE:AT_FULL;
	 p_mem->base_reg_no=addr16_base[code[1]&7]&7;
	 p_mem->index_reg_no=addr16_index[code[1]&7]&7;
}

void  decode_addr32( unsigned char  *code, PMEM_DESC p_mem)
{
	bool sib;
	 //
	 //sib?
	 //
	sib=(4==(code[1]&7));
	if(sib)
	{
		p_mem->index_reg_no=(code[2]>>3)&7;
		p_mem->scale=((code[2]>>6)&7);

		if(0==(code[1]>>6)&&5==(code[2]&7))
		{
			p_mem->disp=*( int*)(code+3);

			//
			// scale==0? don't display *1
			//
		
			p_mem->fmt=AT_INDEX;
			return;
		}

		p_mem->base_reg_no=code[2]&7;

		if(((code[2]>>3)&7)==4)   // no index.
			p_mem->fmt=AT_BASE;
		else
			p_mem->fmt=AT_FULL;
	}
	else
	{
		p_mem->fmt=AT_BASE;
	    p_mem->base_reg_no=code[1]&7;
	}

     switch((code[1]>>6)&7)
     {

      case 0:
          if((code[1]&7)==5)
		  {
  			  p_mem->fmt=AT_DIRECT;
			  p_mem->disp=*( int*)(code+2);//[disp32]
			  return;
		  }
		  p_mem->disp=0;
		  break;

      case 1:
		  p_mem->disp=sib?(char)code[3]:(char)code[2];
		  break;
      case 2:
		  p_mem->disp=sib?*(int*)(code+3):*(int*)(code+2);
		  break;
      case 3:
          break ;            // reportr illegal instruction
     }
}

void set_dseg(int override_seg,POPERAND_R_M p_operand_r_m_i)
{
	if (override_seg!=NO_SEGOVR)
		p_operand_r_m_i->operand_desc.mem.seg=override_seg;
	else if ( (p_operand_r_m_i->operand_desc.mem.fmt&AT_BASE)
		&&(p_operand_r_m_i->operand_desc.mem.base_reg_no==4
		||p_operand_r_m_i->operand_desc.mem.base_reg_no==5))
		p_operand_r_m_i->operand_desc.mem.seg=SS;
	else if ( (p_operand_r_m_i->operand_desc.mem.fmt&AT_INDEX)
		&&(p_operand_r_m_i->operand_desc.mem.index_reg_no==4
		||p_operand_r_m_i->operand_desc.mem.index_reg_no==5))
		p_operand_r_m_i->operand_desc.mem.seg=SS;
	else
		p_operand_r_m_i->operand_desc.mem.seg=DS;

}
void decode_operand_Ev( 
	unsigned char * code,
	int addr_size,
	int data_size,
	bool word,
	int override_seg,
	POPERAND_R_M p_operand_r_m_i)
{
	if(((code[1]>>6)&3)==3)
	{
		p_operand_r_m_i->type=OT_REG;
		make_reg(p_operand_r_m_i->operand_desc.reg,code[1]&7,get_regg_cat(word,data_size));
	}
	else
	{
		p_operand_r_m_i->type=OT_MEM;
		if(addr_size==32)decode_addr32(code,&p_operand_r_m_i->operand_desc.mem);
		else decode_addr16(code,&p_operand_r_m_i->operand_desc.mem);

		set_dseg(override_seg,p_operand_r_m_i);
	}
}

void decode_operand_Mv( 
	unsigned char * code,
	int addr_size,
	int data_size,
	bool word,
	int override_seg,
	POPERAND_R_M p_operand_r_m_i)
{
	if(((code[1]>>6)&3)==3)
		assert(0);
	p_operand_r_m_i->type=OT_MEM;
	if(addr_size==32)decode_addr32(code,&p_operand_r_m_i->operand_desc.mem);
	else decode_addr16(code,&p_operand_r_m_i->operand_desc.mem);
	set_dseg(override_seg,p_operand_r_m_i);
}


void decode_operand_Ex( 
	unsigned char * code,
	int addr_size,
	int sse_reg_cat,
	int override_seg,
	POPERAND_R_M p_operand_r_m_i)
{
	if(((code[1]>>6)&3)==3)
	{
		p_operand_r_m_i->type=OT_REG;
		make_reg(p_operand_r_m_i->operand_desc.reg,code[1]&7,sse_reg_cat);
	}
	else
	{
		p_operand_r_m_i->type=OT_MEM;
		if(addr_size==32)decode_addr32(code,&p_operand_r_m_i->operand_desc.mem);
		else decode_addr16(code,&p_operand_r_m_i->operand_desc.mem);

		set_dseg(override_seg,p_operand_r_m_i);
	}
}

//
//
// flag:    1-- dir
//          2-- word
//			3-- sign extension
// 



char decode_flag(unsigned char *code,struct instr_rec *instr_desc);


void predecode_insn(
	PINSN_BASIC_INFO p_insn,
	PINSN_EXEC_INFO p_insn_exec_info)
{

	char addr_size=p_insn->addr_size;
	char data_size=p_insn->data_size;
	char override_seg=p_insn->prefix&7;
	char sse_reg_cat =RC_NOTHING;
	unsigned int next_ip = p_insn->eip+p_insn->len;
	unsigned char *code=p_insn->code+p_insn->prefix_bytes;

	//p_insn_exec_info->p_instr_desc=get_instr_desc(code);

	assert(p_insn->p_instr_desc);
	char flag=decode_flag(code,p_insn->p_instr_desc);
	enum OP_FMT &fmt=p_insn->p_instr_desc->op_fmt;

	p_insn_exec_info->flag=flag;
	p_insn_exec_info->imm=0x55aa;
	p_insn_exec_info->reg.reg_cat=RC_NOTHING;	
	p_insn_exec_info->secondary_reg.reg.reg_cat=RC_NOTHING;
	p_insn_exec_info->op_presence=0;

	if(code[0]==0x0f) code++;
	switch(fmt)
	{
	case acc_dx	   :// in al,dx
	p_insn_exec_info->flag |=EXEC_F_DIR;  // to mark al will be written

	case dx_acc	   :// out dx,al	p_insn_exec_info->reg.reg_cat=RC_REGG16;
	p_insn_exec_info->secondary_reg.reg.reg_cat=RC_REGG16;
	p_insn_exec_info->secondary_reg.reg.reg_no=DX;
	p_insn_exec_info->reg.reg_cat=get_regg_cat(0!=(flag&EXEC_F_WORD),data_size);
	p_insn_exec_info->reg.reg_no=0;//AL
	p_insn_exec_info->type.reg_present=1;
	p_insn_exec_info->type.s_reg_present=1;
	break; 

case accEv	   :
	break; 
case accIb	   ://in al,im8
	p_insn_exec_info->flag |=EXEC_F_DIR;  // to mark al will be written, fall through

case Ibacc	   :// out imm8,acc
	p_insn_exec_info->imm=code[1];
	p_insn_exec_info->reg.reg_cat=get_regg_cat(0!=(flag&EXEC_F_WORD),data_size);
	p_insn_exec_info->reg.reg_no=0;//AL
	p_insn_exec_info->imm=code[1];
	p_insn_exec_info->type.reg_present=1;
	p_insn_exec_info->type.imm_present=1;
	break; 

case accIv 	   :
	p_insn_exec_info->type.reg_present=1;
	p_insn_exec_info->type.imm_present=1;
	p_insn_exec_info->reg.reg_cat=get_regg_cat(0!=(flag&EXEC_F_WORD),data_size);
	p_insn_exec_info->reg.reg_no=0;//AL
	p_insn_exec_info->flag |=EXEC_F_DIR;  // =1, reg will be written
	if( 0!=(flag&EXEC_F_WORD)) {// imm
		if(data_size==32)
		{
			p_insn_exec_info->reg.reg_cat=RC_REGG32;
			p_insn_exec_info->imm=*(unsigned long *)(code+1);
		}else
		{
			p_insn_exec_info->reg.reg_cat=RC_REGG16;
			p_insn_exec_info->imm=*(unsigned short *)(code+1);
		}
	}else
	{
		p_insn_exec_info->reg.reg_cat=RC_REGG8;
		p_insn_exec_info->imm=code[1];
	}
	break;   
case accReg	   :

	//
	// only xchg acc,r16/32,word mask is 1
	//
	p_insn_exec_info->type.reg_present=1;
	p_insn_exec_info->type.s_reg_present=1;
	p_insn_exec_info->reg.reg_no=AL;
	p_insn_exec_info->secondary_reg.reg.reg_no=code[0]&7;

	if(data_size==32) 
	{
		p_insn_exec_info->reg.reg_cat=RC_REGG32;
		p_insn_exec_info->secondary_reg.reg.reg_cat=RC_REGG32;
	}else
	{
		p_insn_exec_info->reg.reg_cat=RC_REGG16;
		p_insn_exec_info->secondary_reg.reg.reg_cat=RC_REGG16;
	}
	break;    
case accXv     :// lodsb, no operands
	break;  
case Ap	       :
	p_insn_exec_info->type.imm_present=1;
	p_insn_exec_info->type.s_imm_present=1;
	if(data_size==32) 
	{
		p_insn_exec_info->secondary_imm.imm=*(unsigned short *)(code+5);
		p_insn_exec_info->imm=*(unsigned long *)(code+1);
	}
	else 
	{
		p_insn_exec_info->secondary_imm.imm=*(unsigned short *)(code+3);
		p_insn_exec_info->imm=*(unsigned short *)(code+1);
	}

	break;     
case DXXv      :// ins, no operands
	break;    
case Eb		   :
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		false,          //byte
		 override_seg,
		 &p_insn_exec_info->r_m);
	break;    
case Ev1	   :
	p_insn_exec_info->type.imm_present=1;
	p_insn_exec_info->imm=1;
	goto decode_Ev;
case EvCL	   :
	p_insn_exec_info->type.s_reg_present=1;
	make_reg(p_insn_exec_info->secondary_reg.reg,CL,RC_REGG8);
	// fall through
case Ep		   :
case Ev		   :
decode_Ev:
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);
	break;   
case EvGvCL	   ://shrd  
	p_insn_exec_info->type.s_reg_present=1;
	make_reg(p_insn_exec_info->secondary_reg.reg,CL,RC_REGG8);
	// fall through
case EvGv	   :

	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);
	p_insn_exec_info->type.reg_present=1;
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,get_regg_cat(0!=(flag&EXEC_F_WORD),data_size));

	break; 
case EwGw	   :

	 decode_operand_Ev(
		 code, 
		 addr_size,
		 0x10,
		 true,          //16 bit word
		 override_seg,
		 &p_insn_exec_info->r_m);
	p_insn_exec_info->type.reg_present=1;
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,get_regg_cat(true,0x10));// 16 bit word

	break;
case EvvGv:
	//
	// movzx/movsx
	//
	p_insn_exec_info->type.reg_present=1;
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,get_regg_cat(true,data_size));

	p_insn_exec_info->flag |=EXEC_F_DIR;
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 16,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);
	break;

case EvGvIb    :
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);
	p_insn_exec_info->type.reg_present=1;
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,get_regg_cat(EXEC_F_WORD==(flag&EXEC_F_WORD),data_size));

	goto decode_operand_Ib_after_Ev;

	break;    
case EvGvIv	   :
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		 true, //always word,EXEC_F_WORD==(flag&EXEC_F_WORD) just for imm         
		 override_seg,
		 &p_insn_exec_info->r_m);

	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,get_regg_cat(true,data_size));//always word
	p_insn_exec_info->type.reg_present=1;

	goto decode_operand_Iv_after_Ev;
	break;    
case EvIb	   :
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);

decode_operand_Ib_after_Ev:
	{
		unsigned char * imm=code +((addr_size==32)?decode_len_mm_noimm_32(code+1):(addr_size==16)?decode_len_mm_noimm_16(code+1):0);

		p_insn_exec_info->imm=imm[0];
		p_insn_exec_info->type.imm_present=1;
	}
	break;   
case EvIv	   :
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);

decode_operand_Iv_after_Ev:
	{
		unsigned char * imm=code +((addr_size==32)?decode_len_mm_noimm_32(code+1):(addr_size==16)?decode_len_mm_noimm_16(code+1):0);
		switch(flag&(EXEC_F_WORD|EXEC_F_SEXT))
		{
		case EXEC_F_WORD:
		// imm
			if(data_size==32)
				p_insn_exec_info->imm=*(unsigned long *)imm;
			else
				p_insn_exec_info->imm=*(unsigned short *)imm;
			break;
		
		case EXEC_F_SEXT|EXEC_F_WORD:
			if(data_size==32)
				p_insn_exec_info->imm=( long )(char)imm[0];
			else
				p_insn_exec_info->imm=(unsigned short )(char)imm[0];
			break;

		default:
			p_insn_exec_info->imm=imm[0]; // unsined char 0-exted to 32 bit
		}

		p_insn_exec_info->type.imm_present=1;

	}
	break;    
case Ew		   :

	 decode_operand_Ev(
		 code, 
		 addr_size,
		 16,
		true,          //word
		 override_seg,
		 &p_insn_exec_info->r_m);

	break;    
//case EwRw	   :
//	break;    
case FSGS	   :
	//
	// only push/pop fs/gs0xa1/pop fs 0xa9/pop gs
	//
	make_reg(p_insn_exec_info->reg,(code[0]&0x8)?GS:FS,RC_REGSEG);
	p_insn_exec_info->type.reg_present=1;
	break;
case Fv		   :
	break;    
case GvEw      :

	break;    

	break;    
case GvMv      :
case GvMa	   :
case GvMp	   :
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,get_regg_cat(true,data_size));
	p_insn_exec_info->type.reg_present=1;
	 decode_operand_Mv(
		 code, 
		 addr_size,
		 data_size,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);
	 break;
case Mp		   :

case mem10d	:
case mem10r	:
case mem14	:
case mem2i	:
case mem4i	:
case mem4r	:
case mem8i	:
case mem8r	:
case mem108	:
case Ms		   :
	 decode_operand_Mv(
		 code, 
		 addr_size,
		 data_size,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);

	break;    
case Ib		   :// int xx
	p_insn_exec_info->imm=code[1];
	p_insn_exec_info->type.imm_present=1;
	break;    
  
case fmt_int3	   :
	p_insn_exec_info->imm=3;
	p_insn_exec_info->type.imm_present=1;
	break;    
case Iv		   ://push Iv
	//
	// if word
	//
	if( 0!=(flag&EXEC_F_WORD))
	{
	if(data_size==32)
		p_insn_exec_info->imm=*(long *)(code+1);
	else
		p_insn_exec_info->imm=*(short *)(code+1);
	}
	else
		p_insn_exec_info->imm=(long)(char)code[1];
	p_insn_exec_info->type.imm_present=1;
	break;    
case Iw		   :// rern xx
	p_insn_exec_info->imm=*(unsigned short *)(code+1);
	p_insn_exec_info->type.imm_present=1;
	break;    
case IwIb	   :
	p_insn_exec_info->imm=*(unsigned short *)(code+1),
	p_insn_exec_info->secondary_imm.imm=*(unsigned char *)(code+3);
	p_insn_exec_info->type.imm_present=1;
	p_insn_exec_info->type.s_imm_present=1;
	break;    
case Jb		   :
	if(data_size==32)
	p_insn_exec_info->imm=(char)code[1];//next_ip
	else
	p_insn_exec_info->imm=((char )code[1])&0xffff;//next_ip
	p_insn_exec_info->type.imm_present=1;

	break;    
case Jv		   :
	if(data_size==32)
	p_insn_exec_info->imm=*(long *)(code+1);//next_ip
	else
	p_insn_exec_info->imm=(*(short *)(code+1))&0xffff;//next_ip
	p_insn_exec_info->type.imm_present=1;

	break;    
   
case Ovacc	   :
	p_insn_exec_info->type.reg_present=1;
	make_reg(p_insn_exec_info->reg,0,get_regg_cat(0!=(flag&EXEC_F_WORD),data_size)); //acc
	p_insn_exec_info->r_m.type=OT_MEM;
	p_insn_exec_info->r_m.operand_desc.mem.fmt=AT_DIRECT; // direct addressing
	if(addr_size==32) p_insn_exec_info->r_m.operand_desc.mem.disp=*(unsigned long *)(code+1);
	else p_insn_exec_info->r_m.operand_desc.mem.disp=*(unsigned short *)(code+1);

	p_insn_exec_info->r_m.operand_desc.mem.seg=(override_seg==7)?DS:override_seg;
	p_insn_exec_info->flag^=EXEC_F_DIR;
	break;    
case RdCd	   :
	make_reg(p_insn_exec_info->reg,code[1]&7,get_regg_cat(true,32));
	p_insn_exec_info->op_presence=OP_PRESENCE_RM_R|OP_PRESENCE_REG;
	make_reg(p_insn_exec_info->r_m.operand_desc.reg,(code[1]>>3)&7,RC_REGCR);
	break;    
case RdDd	   :
	make_reg(p_insn_exec_info->reg,code[1]&7,get_regg_cat(true,32));
	p_insn_exec_info->op_presence=OP_PRESENCE_RM_R|OP_PRESENCE_REG;
	make_reg(p_insn_exec_info->r_m.operand_desc.reg,(code[1]>>3)&7,RC_REGDR);
	break;    
case RdTd	   :
	make_reg(p_insn_exec_info->reg,code[1]&7,get_regg_cat(true,32));
	p_insn_exec_info->op_presence=OP_PRESENCE_RM_R|OP_PRESENCE_REG;
	make_reg(p_insn_exec_info->r_m.operand_desc.reg,(code[1]>>3)&7,RC_REGTR);
	break;    
case fmt_REG	   :
	//
	// dec/inc/push/pop r16/32
	//
	p_insn_exec_info->type.reg_present=1;
	make_reg(p_insn_exec_info->reg,code[0]&7,get_regg_cat(true,data_size));
	break;    
case regIv	   :
	p_insn_exec_info->type.reg_present=1;
	p_insn_exec_info->type.imm_present=1;
	make_reg(p_insn_exec_info->reg,code[0]&7,get_regg_cat(0!=(flag&EXEC_F_WORD),data_size));
	p_insn_exec_info->flag |=EXEC_F_DIR;  // =1, reg will be writeen
	if( 0!=(flag&EXEC_F_WORD)) {// imm
	if(data_size==32)
		p_insn_exec_info->imm=*(unsigned long *)(code+1);
	else
		p_insn_exec_info->imm=*(unsigned short *)(code+1);
	}
	else
		p_insn_exec_info->imm=code[1];

	break;    
case fmt_SREG	   :
	//
	//only in push/pop,bit3/4
	make_reg(p_insn_exec_info->reg,(code[0]>>3)&3,RC_REGSEG);
	p_insn_exec_info->type.reg_present=1;
	break;    
case SwEw      :// reg16<->sreg
	p_insn_exec_info->type.reg_present=1;
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,RC_REGSEG);
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 16,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);
		  
	break;    
case XvYv      :// movs, no operands
	break;    
case Yvacc     ://stos, no operands
	break;    
case YvDX      :// outs, no operands
	break;   
case fmt_none:
	break;

case fmt_AX	:
	//p_insn_exec_info->type.reg_present=1;
	//p_insn_exec_info->reg.reg_cat=RC_REGG32;
	//p_insn_exec_info->reg.reg_no=0;
	break;
//case ST0	    :
//	p_insn_exec_info->type.reg_present=1;
//	p_insn_exec_info->reg.reg_cat=RC_REGFP;
//	p_insn_exec_info->reg.reg_no=0;
//	break;
case ST0i	:
	p_insn_exec_info->type.reg_present=1;
	p_insn_exec_info->reg.reg_cat=RC_REGFP;
	p_insn_exec_info->reg.reg_no=0;
	p_insn_exec_info->type.s_reg_present=1;
	p_insn_exec_info->secondary_reg.reg.reg_cat=RC_REGFP;
	p_insn_exec_info->secondary_reg.reg.reg_no=code[1]&7;
	break;
//case ST1	    :
//	p_insn_exec_info->type.reg_present=1;
//	p_insn_exec_info->reg.reg_cat=RC_REGFP;
//	p_insn_exec_info->reg.reg_no=1;
//	break;
//case ST2	    :
//	p_insn_exec_info->type.reg_present=1;
//	p_insn_exec_info->reg.reg_cat=RC_REGFP;
//	p_insn_exec_info->reg.reg_no=2;
//	break;
case STi	    :
	p_insn_exec_info->type.reg_present=1;
	p_insn_exec_info->reg.reg_cat=RC_REGFP;
	p_insn_exec_info->reg.reg_no=code[1]&7;
	break;
case STi0	:
	p_insn_exec_info->type.reg_present=1;
	p_insn_exec_info->reg.reg_cat=RC_REGFP;
	p_insn_exec_info->reg.reg_no=code[1]&7;
	p_insn_exec_info->type.s_reg_present=1;
	p_insn_exec_info->secondary_reg.reg.reg_cat=RC_REGFP;
	p_insn_exec_info->secondary_reg.reg.reg_no=0;
	break;
case fmt_xlat:
	break;


case EdPV	:
	sse_reg_cat=TEST(p_insn->prefix,PREFIX_66)?RC_REGMMX:RC_REGXMM;
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,sse_reg_cat);
	p_insn_exec_info->type.reg_present=1;
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		 true,
		 override_seg,
		 &p_insn_exec_info->r_m);
	 break;
case VW	    :
case VWIb	:
case VPG	:
case GPW	:
case GdV  	:
case GdPV	:

case PBVM   :
case VM   	:
case MG	    :
	break;
case fmt_esc:
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 data_size,
		 true,
		 override_seg,
		 &p_insn_exec_info->r_m);
	break;
case m128_xmm :	
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,RC_REGXMM);

decode_mv:
	p_insn_exec_info->type.reg_present=1;
	 decode_operand_Mv(
		 code, 
		 addr_size,
		 data_size,
		 0!=(flag&EXEC_F_WORD),          //word
		 override_seg,
		 &p_insn_exec_info->r_m);

	break;

case m64_mm  :
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,RC_REGXMM);
	p_insn_exec_info->type.reg_present=1;
	goto decode_mv;

	break;
case xmm_mmm64  :
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,RC_REGXMM);
	p_insn_exec_info->type.reg_present=1;
	goto decode_mv;

	 break;
case mm_mmm64  :
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,RC_REGMMX);

	p_insn_exec_info->type.reg_present=1;
	goto decode_mv;

	 break;
case mm_rm32  :
	sse_reg_cat=TEST_PREFIX(p_insn->prefix,PREFIX_66)?RC_REGMMX:RC_REGXMM;
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,sse_reg_cat);
	p_insn_exec_info->type.reg_present=1;
	 decode_operand_Ev(
		 code, 
		 addr_size,
		 32,
		 true,
		 override_seg,
		 &p_insn_exec_info->r_m);
	 break;

case mm_rm64  :break;
case mm_xm64  :break;
case r32_xm32  :break;
case r32_xmm  :break;
case r32_xm64  :break;
case xmm_rm32  :break;
case xmm_rm64  :break;

case xmm_xm128Ib  :
case xmm_xm32Ib  :
case xmm_xm64Ib  :
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,RC_REGMMX);
	 decode_operand_Ex(
		 code, 
		 addr_size,
		 RC_REGMMX,
		 override_seg,
		 &p_insn_exec_info->r_m);
	goto decode_operand_Ib_after_Ev;

case xmm_xm128  :
case xmm_xm128v  :
case xmm_xm32  :
case xmm_xm64  :
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,RC_REGXMM);
	 decode_operand_Ex(
		 code, 
		 addr_size,
		 RC_REGXMM,
		 override_seg,
		 &p_insn_exec_info->r_m);
	break;

case xmm1_m128  :
	make_reg(p_insn_exec_info->reg,1,RC_REGMMX);
	goto decode_mv;
	break;
case mm_mm64  :
	p_insn_exec_info->type.reg_present=1;
	make_reg(p_insn_exec_info->reg,(code[1]>>3)&7,RC_REGMMX);
	 decode_operand_Ex(
		 code, 
		 addr_size,
		 RC_REGMMX,
		 override_seg,
		 &p_insn_exec_info->r_m);
	break;
default:
	assert(0);
	break;
	}
}

char decode_flag(unsigned char *code,struct instr_rec *instr_desc)
{
	char flag=0;

	if(code[0]==0x0f) 
		code ++;

	// gen flag:1-- dir prersents
	//          2-- word prersent
	//			3-- sign extension prersents
	// 
	if (0==instr_desc->dir_mask||
		(0!=(code[0]&instr_desc->dir_mask))
		)
		flag |=EXEC_F_DIR;

	if ((0==instr_desc->w_mask)
		||((2==instr_desc->w_mask)&&(0==(code[0]&2)))// if w_mask==2, it is for imm size, only imul and push
		||((2!=instr_desc->w_mask)&&(0!=(code[0]&instr_desc->w_mask))))
		flag |=EXEC_F_WORD;

	if (instr_desc->sext_mask&&
		(0!=(code[0]&instr_desc->sext_mask)))
		flag |=EXEC_F_SEXT;

	return flag;

}
extern char * segment_name[];
extern char *reg16_name[];
extern char *reg8_name[];

void dump_reg(REG_CODE &reg)
{

		switch(reg.reg_cat)
		{
			case RC_REGG8: printf("%s,", reg8_name[reg.reg_no]);break;
			case RC_REGG16: printf("%s,", reg16_name[reg.reg_no]);break;
			case RC_REGG32: printf("e%s,", reg16_name[reg.reg_no]);break;
			case RC_REGCR: printf("cr%d,", reg.reg_no);break;
			case RC_REGSEG: printf("%s,", segment_name[reg.reg_no]);break;

		}
}
void dump_mem(MEM_DESC &mem)
{
    printf("%s:",segment_name[mem.seg]);
	switch(mem.fmt)
		{
	case AT_BASE: printf("[%s+%x],", reg16_name[mem.base_reg_no],mem.disp);break;
	case AT_INDEX: printf("[%s*%d+%x],", reg16_name[mem.base_reg_no],1<<mem.scale,mem.disp);break;
	case AT_FULL:  printf("[e%s+e%s*%d+%x],", reg16_name[mem.base_reg_no],reg16_name[mem.index_reg_no],1<<mem.scale,mem.disp);break;
	case AT_DIRECT: printf("[%x],", mem.disp);break;
		}
}
void dump_operand(PINSN_EXEC_INFO p_info)
{
	dump_reg(p_info->reg);
	switch(p_info->r_m.type)
	{
	case OT_REG:dump_reg(p_info->r_m.operand_desc.reg);break;
	case OT_MEM:dump_mem(p_info->r_m.operand_desc.mem);break;
	}
	if(p_info->type.imm_present) printf("%x,",p_info->imm);
	if(p_info->type.s_imm_present) printf("%x,",p_info->secondary_imm.imm);
	if(p_info->type.s_reg_present) 	dump_reg(p_info->secondary_reg.reg);

}



