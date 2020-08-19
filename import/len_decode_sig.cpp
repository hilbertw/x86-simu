
#include <assert.h>
#include <string.h>

#include "windows.h"
#undef OPER_FMT_REC
#define OPER_FMT_REC(x) x,
//
// If new oper type is to be defined, go to oper_type.h
//
enum id_fmt {
	
#include "oper_fmt_def.h"
N_FMT
};

#define ImmV   0x10      // db dw or dd , checking sext bit
#define Imm    0x20      // dw , don't check sext, add 2 if data size is 32
#define RorM   0x40
#define AddrA  0x80      // add 2 if addr size is 32,e.g.direct mem addressing,
#define AddrD  Imm    // address  depending on data size,e.g. jump ptr16:16/32


unsigned char get_len_decode_sig( int id)
{
	 char  flag =0;  // inducates presence of iv ib, rm
	 char  fix_bytes=1; 
	
	switch(id)
	{
case Ap	       :  // jump ptr16:16/32
	flag |=AddrD; // depending on data size
	fix_bytes+=4;
	break;    
case accEv	   :
case Eb		   :
case Ep		   :
case Ev		   :    
case Ev1	   :
case EvCL	   :
case EvGv	   :
case EvvGv	   :
case EvGvCL	   :
case Ew		   :
case EwGw	   :
case GvEw      :
case GvMa	   :
case GvMv	   :
case GvMp	   :
case Mv		   :
case Mp		   :
case Ms		   :
case mem10d	:
case mem10r	:
case mem14	:
case mem2i	:
case mem4i	:
case mem4r	:
case mem8i	:
case mem8r	:
//case mem94	:
case mem108	:
//case 	mem512  :
case 	mem32   :

case 	VW	    :
case 	VPG	    :
case 	PBVM    :
case 	GPW	    :
case 	GdV  	:
case 	GdPV	:
case 	EdPV	:
case 	VM   	:
case 	MG	    :
case  m64_mm:
case  m128_xmm:
//case  m32_r32:
case  mm_mmm64:
case  mm_rm32:
case  mm_rm64:
case  mm_xm64:
case  r32_xm32:
case  r32_xmm:
case  r32_xm64:
case  xm64_xmm:
case  xmm_mmm64:
case  xmm_rm32:
case  xmm_rm64:
case  xmm_xm128:
case  xmm_xm128v:
case  xmm_xm32:
case  xmm_xm64:
case mm_mm64:
case SwEw       :
case RdCd	   :
case RdDd	   :
case RdTd	   :
	flag |=RorM;
	fix_bytes=0;// because old code has counted in opcode bytes
	break;   
case  xmm_xm64Ib:
case  xmm_xm32Ib:
case  xmm_xm128Ib:
case EvIb	   :
case EvGvIb    :
case VWIb	:
    flag |=RorM;
	break;    
case EvGvIv	   :
case EvIv	   :
	flag |=RorM|ImmV;
	fix_bytes=0;// because old code has counted in opcode bytes
	break;     
case Ib		   :
case accIb	   :
case Jb		   :
case Ibacc	   :fix_bytes++;// in imm8,acc
	break;    
case Iw		   :
	fix_bytes=3;
	break;    
case IwIb	   :
	fix_bytes=4;
	break;     
case Jv		   :flag |=Imm;
	fix_bytes=3;
	break;    
  
case Ovacc	   :
	flag |=AddrA;//address size
	fix_bytes+=2;
	break;    
case Iv		   :
case regIv	   :
case accIv 	   :
	flag |=ImmV;
	break; 
case acc_dx:
case dx_acc	   :
case accReg	   :
case accXv     :
case DXXv      :
case Fv		   :
case FSGS	   :
case fmt_REG   :
case fmt_SREG :
case XvYv     :
case Yvacc    :
case YvDX     :
case fmt_AX	:
//case ST0    :
case ST0i	:
//case ST1    :
//case ST2    :
case STi    :
case STi0	:
case fmt_none:
case fmt_xlat:
case fmt_int3:
	break;
default:
	return -1;// indicates unknown format

	}

	return flag| fix_bytes;
}




