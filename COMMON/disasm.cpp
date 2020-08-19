#define   _CRT_SECURE_NO_DEPRECATE 1
#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>

#include <assert.h>
#include "common.h"
//#define instr_recs  instr_recs1
//#define ext_instr_recs  ext_instr_recs1
//#define x87_instr_recs  x87_instr_recs1
//
//#include "instr_dat.h"
//extern  struct instr_rec p_instr_desc;
//
//int get_ext_instr_desc( unsigned  char * opcode);
//int get_instr_desc( unsigned  char * opcode);
//int get_x87_instr_desc( unsigned  char * opcode);

#undef INSTR_REC
#define INSTR_REC(x) #x,



const char * mnemonics[]={
#include <instr_id_def.h>
};

#define TAB0 30       // column width for instrction code 
#define TAB1 16       // column width for instrction mnemonics 

extern char * segment_name[];
extern char *reg16_name[];
extern char *reg8_name[];
//
// may be bug here. all w_mask is 1?
//
static const char * get_acc(bool word,int data_size)
{
	return word?(data_size==32?"EAX":"AX"):"AL";
}
static int display_regIv(char * buffer,int reg, bool word, unsigned char * code, int data_size)
{
	int len;

	if(word) {// imm
	if(data_size==32)
		len=sprintf(buffer,"E%s,%08x",reg16_name[reg],*(unsigned long *)(code+1));
	else
		len=sprintf(buffer,"%s,%04x",reg16_name[reg],*(unsigned short *)(code+1));
	}
	else
	len=sprintf(buffer,"%s,%02x",reg8_name[reg],code[1]);


	return len;
}
//
// MOV -- Move to/from Special Registers
//
static int display_cr_r(char * buffer, bool dir, unsigned char * code, char c)
{
	int len;
	if(dir)
		len=sprintf (buffer,"%CR%d,e%s",c,(code[1]>>3)&7,reg16_name[(code[1])&7]);
	else 
		len=sprintf (buffer,"E%s,%CR%d",reg16_name[code[1]&7],c,(code[1]>>3)&7);
	return len;
}
//
//
//
// flag:    1-- dir
//          2-- word
//			3-- sign extension
// 


int display_operands(
	unsigned char  *code, // not including 0x0f if two bytes opcode instructions
	int next_ip,
	enum OP_FMT fmt,
	char * buffer,
	int addr_size,
	int data_size,
	prefix_t prefix,
	int flag)
{
	int len=0;
	int override_seg =PREFIX_SEG(prefix);

	switch(fmt)
	{
case acc_dx	   :len=sprintf(buffer,"%s,DX",get_acc(0!=(flag&EXEC_F_WORD),data_size));
	break; 
case dx_acc	   :len=sprintf(buffer,"DX,%s",get_acc(0!=(flag&EXEC_F_WORD),data_size));
	break; 
case accEv	   :
	break; 
case accIb	   ://in al,im8
	len=sprintf(buffer,"%s,%02x",get_acc(0!=(flag&EXEC_F_WORD),data_size),code[1]);
	break; 

case accIv 	   :
	if(flag&EXEC_F_WORD) {// imm
	if(data_size==32)
		len=sprintf(buffer,"EAX,%08x",*(unsigned long *)(code+1));
	else
		len=sprintf(buffer,"AX,%04x",*(unsigned short *)(code+1));
	}
	else
	len=sprintf(buffer,"AL,%02x",code[1]);
	break;   
case accReg	   :
	//
	// only xchg acc,r16/32
	//

	if(data_size==32) len = sprintf(buffer,"EAX,e%s",reg16_name[code[0]&7]);
	else  len = sprintf(buffer,"AX,%s",reg16_name[code[0]&7]);

	break;    
case accXv     :
	break;  
case Ap	       :
	if(data_size==32) len = sprintf(buffer,"%04X:%08X",*(unsigned short *)(code+5),*(unsigned long *)(code+1));
	else len = sprintf(buffer,"%04X:%04X",*(unsigned short *)(code+3),*(unsigned short *)(code+1));

	break;     
case DXXv      :
	break;    
case Eb		   :
	len= display_Ev(
		code, 
		buffer,
		 addr_size,
		data_size,     
		false,                // rm8
		 override_seg );
	   
	break;    
case Ep		   :

	len= display_Mv(
		code, 
		buffer,
		 addr_size,
		4,          // far ptr
		 override_seg );
	break;     
case Ev		   :

	len= display_Ev(
		code, 
		buffer,
		 addr_size,
		 data_size,
		0!=(flag&EXEC_F_WORD),          //word
		 override_seg,true );
	break;     
case Ev1	   :
	len= display_Ev(
		code, 
		buffer,
		 addr_size,
		 data_size,
		0!=(flag&EXEC_F_WORD),          //word
		 override_seg,true );

	buffer[len++]=',';
	buffer[len++]='1';
	break;    
case EvCL	   :

	len= display_Ev(
		code, 
		buffer,
		 addr_size,
		 data_size,
		0!=(flag&EXEC_F_WORD),          //word
		 override_seg ,true);
	buffer[len++]=',';
	buffer[len++]='c';
	buffer[len++]='l';


	break;    
case EvGv	   :

	len= display_EvGv(
		code, 
		buffer,
		addr_size,
		data_size,
		0!=(flag&EXEC_F_WORD),
		0!=(flag&EXEC_F_DIR),
		override_seg );

	break;  
case EwGw	   :

	len= display_EvGv(
		code, 
		buffer,
		addr_size,
		0x10,
		true,   // 16 bit word
		0!=(flag&EXEC_F_DIR),
		override_seg );

	break;
case EvvGv:
	//
	// movzx/movsx
	//
	len= display_Rv(
		(code[1]>>3)&7, 
		buffer,
		data_size,//(0!=(flag&EXEC_F_WORD))?32:16,
		true);    // always  word/dword

	buffer[len++]=',';

	len+= display_Ev(
		code, 
		buffer+len,
		 addr_size,
		  16,
		  0!=(flag&EXEC_F_WORD),
		 override_seg,
		 true );

break;


case EvGvCL	   ://shrd
	len= display_EvGv(
		code, 
		buffer,
		addr_size,
		data_size,
		0!=(flag&EXEC_F_WORD),
		0!=(flag&EXEC_F_DIR),
		override_seg );
	buffer[len++]=',';
	buffer[len++]='c';
	buffer[len++]='l';
	break;    
case EvGvIb    :
	len= display_EvGv(
		code, 
		buffer,
		addr_size,
		data_size,
		0!=(flag&EXEC_F_WORD),
		0!=(flag&EXEC_F_DIR),
		override_seg );

	goto display_Ib_after_Ev;

	break;    
case EvGvIv	   :// only for imul
	len= display_EvGv(
		code, 
		buffer,
		addr_size,
		data_size,
		true,    // always word. 
		true,    //always, r, r/m,imm
		override_seg );
	goto display_Iv_after_Ev;

	break;    
case EvIb	   :
	len= display_Ev(
		code, 
		buffer,
		addr_size,
		data_size,     
		0!=(flag&EXEC_F_WORD),                // rm16/32
		 override_seg,true );

display_Ib_after_Ev:
	{
		unsigned char * imm=code +((addr_size==32)?decode_len_mm_noimm_32(code+1):(addr_size==16)?decode_len_mm_noimm_16(code+1):0);

		len+=sprintf(buffer+len,",%02x",imm[0]);
	}
	break;   
case EvIv	   :
	len= display_Ev(
		code, 
		buffer,
		 addr_size,
		data_size,     
		0!=(flag&EXEC_F_WORD),                // rm16/32
		 override_seg,true );

display_Iv_after_Ev:
	{
		 unsigned char * imm=code +((addr_size==32)?decode_len_mm_noimm_32(code+1):(addr_size==16)?decode_len_mm_noimm_16(code+1):0);
		switch(flag&(EXEC_F_WORD|EXEC_F_SEXT))
		{
		case EXEC_F_WORD:
		// imm
			if(data_size==32)
				len+=sprintf(buffer+len,",%08x",*(unsigned long *)imm);
			else
				len+=sprintf(buffer+len,",%04x",*(unsigned short *)imm);
			break;
		
		case EXEC_F_SEXT|EXEC_F_WORD:
			if(data_size==32)
				len+=sprintf(buffer+len,",%08x",( long )(char)imm[0]);
			else
				len+=sprintf(buffer+len,",%04x",(unsigned short )(char)imm[0]);
			break;

		default:
			len+=sprintf(buffer+len,",%02x",imm[0]);
		}
	}
	break;    
case Ew		   :
	len= display_Ev(
		code, 
		buffer,
		 addr_size,
		16,     
		true,                // rm16
		 override_seg );
	   
	break;    
//case EwRw	   :
//	break;    
case FSGS	   :
	//
	// only push/pop fs/gs0xa0/push fs 0xa1/pop fs
	//
	strcpy(buffer,(code[0]&0x8)?"GS":"FS");
	len =2;
	break;    
case Fv		   :
	break;    
case GvEw      :
	break;    

	break;    
case GvMv      :
case GvMa	   :
case GvMp	   :

	len= display_EvGv(
		code, 
		buffer,
		addr_size,
		data_size,
		true,
		true,
		override_seg );
	break;    
case Ib		   :// int xx
	len=sprintf(buffer,"%02x",code[1]);
	break;    
case Ibacc	   :// out imm8,acc
	len=sprintf(buffer,"%02x,%s",code[1],get_acc(0!=(flag&EXEC_F_WORD),data_size));
	break;    
case fmt_int3	   :
	buffer[0]='3';len=1;
	break;    
case Iv		   ://push Iv
	//
	// if word
	//

	if(data_size==32)
		len = sprintf(buffer,"%08X",(flag &EXEC_F_WORD)?*(long *)(code+1):(unsigned long)(char)code[1]);
	else
		len = sprintf(buffer,"%04X",(flag &EXEC_F_WORD)?*(short *)(code+1):(unsigned short)(char)code[1]);

	break;    
case Iw		   :// rern xx
	len = sprintf(buffer,"%04X",*(unsigned short *)(code+1));
	break;    
case IwIb	   :
	len = sprintf(buffer,"%04X,%02X",*(unsigned short *)(code+1),*(unsigned char *)(code+3));
	break;    
case Jb		   :
	if(data_size==32)
	len = sprintf(buffer,"%08X",next_ip+(char)code[1]);
	else
	len = sprintf(buffer,"%04X",(next_ip+(char )code[1])&0xffff);
	break;    
case Jv		   :
	if(data_size==32)
	len = sprintf(buffer,"%08X",next_ip+*(long *)(code+1));
	else
	len = sprintf(buffer,"%04X",(next_ip+*(short *)(code+1))&0xffff);;
	break;    
case Mp		   :
	len= display_Mv(
		code, 
		buffer,
		 addr_size,
		4,          //far ptr
		 override_seg );
	break;    
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
	 len =display_Mv(
	 code, 
	 buffer,
	 addr_size,
	 -1, // don't display byte ptr etc.
	 override_seg );
	break;    
case Ovacc	   :

	if((flag &EXEC_F_DIR) !=0)
	{
		if(override_seg==DS||(override_seg==7))
		{
			if(addr_size==32) len = sprintf(buffer,"[%08X],%s",*(unsigned long *)(code+1),get_acc(0!=(flag&EXEC_F_WORD),data_size));
			else len = sprintf(buffer,"[%04X],%s",*(unsigned short *)(code+1),get_acc(0!=(flag&EXEC_F_WORD),data_size));
		}
		else
		{
			if(addr_size==32) len = sprintf(buffer,"%s:[%08X],%s",segment_name[override_seg],*(unsigned long *)(code+1),get_acc(0!=(flag&EXEC_F_WORD),data_size));
			else len = sprintf(buffer,"%s:[%04X],%s",segment_name[override_seg],*(unsigned short *)(code+1),get_acc(0!=(flag&EXEC_F_WORD),data_size));
		}
	}
	else
	{
		if(override_seg==DS||(override_seg==7))
		{
			if(addr_size==32) len = sprintf(buffer,"%s,[%08X]",get_acc(0!=(flag&EXEC_F_WORD),data_size),*(unsigned long *)(code+1));
			else len = sprintf(buffer,"%s,[%04X]",get_acc(0!=(flag&EXEC_F_WORD),data_size),*(unsigned short *)(code+1));
		}
		else
		{
			if(addr_size==32) len = sprintf(buffer,"%s,%s:[%08X]",get_acc(0!=(flag&EXEC_F_WORD),data_size),segment_name[override_seg],*(unsigned long *)(code+1));
			else len = sprintf(buffer,"%s,%s:[%04X]",get_acc(0!=(flag&EXEC_F_WORD),data_size),segment_name[override_seg],*(unsigned short *)(code+1));
		}
	}

	break;    
case RdCd	   :
	len = display_cr_r(buffer,0!=(flag &EXEC_F_DIR),code,'C'); 
	break;    
case RdDd	   :
	len = display_cr_r(buffer,0!=(flag &EXEC_F_DIR),code,'D'); 
	break;    
case RdTd	   :
	len = display_cr_r(buffer,0!=(flag &EXEC_F_DIR),code,'T'); 
	break;    
case fmt_REG	   :
	//
	// dec/inc/push/pop r16/32
	//
	if(data_size==32) len = sprintf(buffer,"E%s",reg16_name[code[0]&7]);
	else len = sprintf(buffer,"%s",reg16_name[code[0]&7]);
	break;    
case regIv	   :
	len = display_regIv(buffer,code[0]&7,0!=(flag&EXEC_F_WORD),code,data_size);
	break;    
case fmt_SREG	   :
	//
	//only in push/pop,bit3/4

	len =sprintf (buffer,"%s",segment_name[(code[0]>>3)&3]);
	break;    
case SwEw      :// reg16<->sreg
	if(flag &EXEC_F_DIR)
	{
		len=sprintf (buffer,"%s",segment_name[(code[1]>>3)&7]);
		buffer[len++]=',';
		len+= display_Ev(
			code, 
			buffer+len,
			 addr_size,
			16,     
			true,                // rm16
			 override_seg );
	}

	else 
	{
			len= display_Ev(
			code, 
			buffer,
			 addr_size,
			16,     
			true,                // rm16
			 override_seg );
		buffer[len++]=',';
		len+=sprintf( buffer+len,"%s",segment_name[(code[1]>>3)&7]);
	}
	break;    
case XvYv      :
	break;    
case Yvacc     :
	break;    
case YvDX      :
	break;    

case fmt_none:
	break;
case fmt_AX	:
	*(short *)(buffer+len)=0x5841;
	len +=2;
	break;
//case ST0	    :
//	len+=sprintf( buffer+len,"st(0)");
//	break;
case ST0i	:
	len+=sprintf( buffer+len,"st(0),st(%d)",code[1]&7);
	break;
//case ST1	    :
//	len+=sprintf( buffer+len,"st(1)");
//	break;
//case ST2	    :
//	len+=sprintf( buffer+len,"st(2)");
//	break;
case STi	    :
	len+=sprintf( buffer+len,"st(%d)",code[1]&7);
	break;
case STi0	:
	len+=sprintf( buffer+len,"st(%d),st(0)",code[1]&7);
	break;
case fmt_xlat:
	if(override_seg!=DS)
		len+=sprintf( buffer+len,"%s:",segment_name[override_seg]);
	if(addr_size==32)
	{
		len+=sprintf( buffer+len,"[EBX]");
	}
	else
	{
		len+=sprintf( buffer+len,"[BX]");
	}
	break;

case EdPV	:
	if(!(flag& EXEC_F_DIR))
	{
	len= display_R_sse(
		(code[1]>>3)&7, 
		buffer,
		0!=TEST(prefix,PREFIX_66) );

	buffer[len++]=',';
	len+= display_Ev(
		code, 
		buffer+len,
		 addr_size,
		32,     
		true,                // rm16
		 override_seg );
	}
	else
	{
	len= display_Ev(
		code, 
		buffer,
		 addr_size,
		32,     
		true,                // rm16
		 override_seg );


	buffer[len++]=',';

	len+= display_R_sse(
		(code[1]>>3)&7, 
		buffer+len,
		0!=TEST(prefix,PREFIX_66) );
	}
	break;

case VWIb	:
	break;
case VW	    :
case VPG	:
case PBVM   :
case GPW	:
case GdV  	:
case GdPV	:

case VM   	:
case MG	    :
	break;

case fmt_esc:// esc  fno,r/m

	len+=sprintf( buffer+len,"%02x",((code[0]&3)<<3)|((code[1]>>3)&7) );
	buffer[len++]=',';
	len+= display_Ev(
		code, 
		buffer+len,
		addr_size,
		data_size,     
		true,                // rm16
		override_seg );

	break;
//
// sse insns
//
case  m64_mm:
case  m128_xmm :
	 len =display_Mv(
	 code, 
	 buffer,
	 addr_size,
	 -1, // don't display byte ptr etc.
	 override_seg );
	buffer[len++]=',';

	len+= display_R_sse(
		(code[1]>>3)&7, 
		buffer+len,
		true);
	break;
case  mm_mmm64 :
	//len= display_R_sse(
	//	(code[1]>>3)&7, 
	//	buffer,
	//	false);
	//buffer[len++]=',';
	// len +=display_RorM_sse(
	// code, 
	// buffer+len,
	// addr_size,
	// override_seg,
	// false);
	len+=display_R_RorM_sse(	 
	code, 
	buffer+len,
	addr_size,
	(flag &EXEC_F_DIR)!=0, //.
	false,
	override_seg
	);
	break;

case  mm_rm32 :
case  mm_rm64 : // movd mm, rm32/64 movd xmm,rm32/64

	if(flag &EXEC_F_DIR)
	{
	len+= display_R_sse(
		(code[1]>>3)&7, 
		buffer+len,
		TEST_PREFIX(prefix,PREFIX_66));
	buffer[len++]=',';

	 len +=display_Ev(
	 code, 
	 buffer+len,
	 addr_size,
	 32,
	 true,
	override_seg
	  // don't display byte ptr etc.
	 );
	}
	else
	{

	 len +=display_Ev(
	 code, 
	 buffer+len,
	 addr_size,
	 32,
	 true,
	override_seg
	  // don't display byte ptr etc.
	 );
	buffer[len++]=',';
	len+= display_R_sse(
		(code[1]>>3)&7, 
		buffer+len,
		TEST_PREFIX(prefix ,PREFIX_66));
	}
	break;

case  mm_xm64 : //cvtps2pi

	len= display_R_sse(
		(code[1]>>3)&7, 
		buffer,
		false);
	buffer[len++]=',';	
	len+=display_R_RorM_sse(	 
	code, 
	buffer+len,
	addr_size,
	true, // don't display byte ptr etc.
	true,
	override_seg
	);



	break;
case  r32_xmm :
	len= display_Rv(
		(code[1]>>3)&7, 
		buffer,
		32,
		true);    // always  word/dword
	buffer[len++]=',';
	len+= display_R_sse(
		(code[1]>>3)&7, 
		buffer+len,
		true);
	break;

case  r32_xm32 : //cvtps2si
//case  r32_xm64 :
	len= display_Rv(
		(code[1]>>3)&7, 
		buffer,
		32,
		true);    // always  word/dword
display_xmm:
	buffer[len++]=',';
	len+=display_RorM_sse(	 
	code, 
	buffer+len,
	addr_size,
	override_seg,// don't display byte ptr etc.
	true
	);
	break;
case  xm64_xmm :
	len=display_R_RorM_sse(	 
	code, 
	buffer,
	addr_size,
	true, // don't display byte ptr etc.
	true,
	override_seg
	);
	buffer[len++]=',';
	len+= display_R_sse(
		(code[1]>>3)&7, 
		buffer+len,
		true );
	break;
case  xmm_mmm64 :
	len= display_R_sse(
		(code[1]>>3)&7, 
		buffer,
		true );
	goto display_xmm;

	break;
case  xmm_rm32 :
case  xmm_rm64 :
	len= display_R_sse(
		(code[1]>>3)&7, 
		buffer,
		true );
	buffer[len++]=',';
	len+= display_Ev(
			code, 
			buffer+len,
			 addr_size,
			 data_size,
			 true,
			 override_seg );
	break;
case  xmm1_m128 :
	len= display_R_sse(
		1, 
		buffer,
		true );
	buffer[len++]=',';
	len+= display_Mv(
			code, 
			buffer+len,
			 addr_size,
			 -1,
			 override_seg );
	break;

case  xmm_xm128v :
case  xmm_xm128 :
case  xmm_xm32 :
case  xmm_xm64 :
	len+= display_R_sse(
		(code[1]>>3)&7,  
		buffer+len,
		true );

	goto display_xmm;

case  xmm_xm128Ib :
case  xmm_xm32Ib :
case  xmm_xm64Ib :
	len= display_R_sse(
		(code[1]>>3)&7,  
		buffer,
		true );
	buffer[len++]=',';
	len+=display_R_RorM_sse(	 
	code, 
	buffer+len,
	addr_size,
	true, // don't display byte ptr etc.
	true,
	override_seg
	);
	goto display_Ib_after_Ev;
case mm_mm64:
	//len= display_R_sse(
	//	(code[1]>>3)&7,  
	//	buffer,
	//	false );
	//buffer[len++]=',';
	len+=display_R_RorM_sse(	 
	code, 
	buffer+len,
	addr_size,
	(flag &EXEC_F_DIR)==0, //.
	false,
	override_seg
	);
	break;
case mm_mm32:
	len += display_R_RorM_sse(
		code,
		buffer + len,
		addr_size,
		(flag &EXEC_F_DIR) == 0, //.
		false,
		override_seg
		);
	break;
default:
	assert(0);
	}

	return len;
}

int display_line(	
	unsigned char * code,
	int next_ip,
	char * buffer,
	prefix_t prefix,
	int data_size,
	int addr_size,
	 struct instr_rec* p_instr_desc)
{
	int len=0;

	if(code[0]==0x0f) 
		code ++;

	//
	//print prefix andmnemonics
	//
	if ( prefix &PREFIX_LOCK)
	{
		strcpy( buffer,"lock ");
		len =5;
	}

	if ( prefix &PREFIX_REPZ)
	{
		strcpy( buffer+len,"repz ");
		len +=5;
	}
	else
	if ( prefix &PREFIX_REPNZ)
	{
		strcpy( buffer+len,"repnz ");
		len +=6;
	}
	//
	// mnem
	//
	if(p_instr_desc->index==cbw)
		len +=sprintf( buffer+len,(code[0]&1)?"cwd":"cbw");

	else if(p_instr_desc->index==jcxz)
	{

		len +=sprintf( buffer+len,(data_size==32)?"jecxz":"jcxz");

	}else{
		len +=sprintf( buffer+len,"%s",p_instr_desc->mnemonics);

		//
		//string instructions, append b/w/d
		//
		if(p_instr_desc->index >=stos
			&& p_instr_desc->index <=scas)
		{
			buffer[len++]=( code[0]&1)?(data_size==16?'w':'d'):'b';
		}
		//
		// pushad pushfd
		//
		else if(data_size==32 &&(
			p_instr_desc->index==pusha||p_instr_desc->index==pushf||
			p_instr_desc->index==popa||p_instr_desc->index==popf))
		{
			buffer[len++]='d';
		}
	}
	//
	// padding ' ' to TAB1
	//
	while ( len <TAB1)buffer[len++]=' ';
	//
	//prepare to print operands;
	//
	char flag=decode_flag(code,p_instr_desc);

#if 0
	int flag =0;
	// gen flag:1-- dir prersents
	//          2-- word prersent
	//			3-- sign extension prersents
	// 
	if( ( 0==p_instr_desc->dir_mask)
		||(0!=(code[0]&p_instr_desc->dir_mask))) 
		flag |=EXEC_F_DIR;


	if ((0==p_instr_desc->w_mask)
		||((2==p_instr_desc->w_mask)&&(0==(code[0]&2)))// if w_mask==2, it is for imm size, only imul and push
		||((2!=p_instr_desc->w_mask)&&(0!=(code[0]&p_instr_desc->w_mask))))

		flag |=EXEC_F_WORD;

	if (p_instr_desc->sext_mask&&
		(0!=(code[0]&p_instr_desc->sext_mask))
		)
		flag |=EXEC_F_SEXT;
#endif
	//
	//print operands;
	//
	len +=display_operands(
		code, 
		next_ip,
		p_instr_desc->op_fmt,
		buffer+len,
		addr_size,
		data_size,
		prefix,
		flag);

	return len;
}
//
// if len is more than 8 , the caller should use another line to print the rest bytes 
//
int disasm_line(char line[100],PINSN_BASIC_INFO p_insn)
{

	unsigned int ip=p_insn->eip;
	unsigned char *code=p_insn->code;
	int prefix_bytes=p_insn->prefix_bytes;
	int len=p_insn->len;
	prefix_t prefix=p_insn->prefix;
	int data_size=p_insn->data_size;
	int addr_size=p_insn->addr_size;
	unsigned char * code_no_prefix=code +prefix_bytes;
	struct instr_rec * p_instr_desc=p_insn->p_instr_desc;
	int len1=0;

		//
		// only print 8 bytes at most, the caller will print the rest bytes
		//
		for(int i=0;i<len&&i<8;i++) len1+=sprintf(line +len1,"%02X ",code[i]);

		//
		// padding to TAB0
		//
		while (len1<TAB0) line[len1++]=' ';

		len1+=display_line(	
		code_no_prefix,
		ip+len,    // next ip, for relative branch lines
		(char *)line+len1,
		prefix,
		data_size,
		addr_size,
		p_instr_desc );

		line[len1++]=0;
		return len1;
}



