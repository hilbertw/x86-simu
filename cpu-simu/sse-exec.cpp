#include <stdio.h>
#include "cpu.h"

void CCPU:: sse_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	switch(p_insn->p_instr_desc->index)
	{
	case PMINUB:
	 PMINUB_(p_insn_exec_info);
	break;
	case PREFETCHT0:
	 PREFETCHT0_(p_insn_exec_info);
	break;
	case PREFETCHT1:
	 PREFETCHT1_(p_insn_exec_info);
	break;
	case PREFETCHT2:
	 PREFETCHT2_(p_insn_exec_info);
	break;
	case PREFETCHTA:
	 PREFETCHTA_(p_insn_exec_info);
	break;
	case addpd:
	 addpd_(p_insn_exec_info);
	break;
	case addps:
	 addps_(p_insn_exec_info);
	break;
	case addsd:
	 addsd_(p_insn_exec_info);
	break;
	case addss:
	 addss_(p_insn_exec_info);
	break;
	case addsubpd:
	 addsubpd_(p_insn_exec_info);
	break;
	case addsubps:
	 addsubps_(p_insn_exec_info);
	break;
	case andnpd:
	 andnpd_(p_insn_exec_info);
	break;
	case andnps:
	 andnps_(p_insn_exec_info);
	break;
	case andpd:
	 andpd_(p_insn_exec_info);
	break;
	case andps:
	 andps_(p_insn_exec_info);
	break;
	case blendpd:
	 blendpd_(p_insn_exec_info);
	break;
	case blendps:
	 blendps_(p_insn_exec_info);
	break;
	case blendvpd:
	 blendvpd_(p_insn_exec_info);
	break;
	case blendvps:
	 blendvps_(p_insn_exec_info);
	break;
	case cmppd:
	 cmppd_(p_insn_exec_info);
	break;
	case cmpps:
	 cmpps_(p_insn_exec_info);
	break;
	case comisd:
	 comisd_(p_insn_exec_info);
	break;
	case comiss:
	 comiss_(p_insn_exec_info);
	break;
	case cvtdq2ps:
	 cvtdq2ps_(p_insn_exec_info);
	break;
	case cvtpd2dq:
	 cvtpd2dq_(p_insn_exec_info);
	break;
	case cvtpd2pi:
	 cvtpd2pi_(p_insn_exec_info);
	break;
	case cvtpd2ps:
	 cvtpd2ps_(p_insn_exec_info);
	break;
	case cvtpi2pd:
	 cvtpi2pd_(p_insn_exec_info);
	break;
	case cvtpi2ps:
	 cvtpi2ps_(p_insn_exec_info);
	break;
	case cvtps2dq:
	 cvtps2dq_(p_insn_exec_info);
	break;
	case cvtps2pd:
	 cvtps2pd_(p_insn_exec_info);
	break;
	case cvtps2pi:
	 cvtps2pi_(p_insn_exec_info);
	break;
	case cvtsd2si:
	 cvtsd2si_(p_insn_exec_info);
	break;
	case cvtsd2ss:
	 cvtsd2ss_(p_insn_exec_info);
	break;
	case cvtsi2sd:
	 cvtsi2sd_(p_insn_exec_info);
	break;
	case cvtsi2ss:
	 cvtsi2ss_(p_insn_exec_info);
	break;
	case cvtss2sd:
	 cvtss2sd_(p_insn_exec_info);
	break;
	case cvtss2si:
	 cvtss2si_(p_insn_exec_info);
	break;
	case cvttpd2dq:
	 cvttpd2dq_(p_insn_exec_info);
	break;
	case cvttpd2pi:
	 cvttpd2pi_(p_insn_exec_info);
	break;
	case cvttps2dq:
	 cvttps2dq_(p_insn_exec_info);
	break;
	case cvttps2pi:
	 cvttps2pi_(p_insn_exec_info);
	break;
	case cvttsd2si:
	 cvttsd2si_(p_insn_exec_info);
	break;
	case cvttss2si:
	 cvttss2si_(p_insn_exec_info);
	break;
	case divpd:
	 divpd_(p_insn_exec_info);
	break;
	case divps:
	 divps_(p_insn_exec_info);
	break;
	case divsd:
	 divsd_(p_insn_exec_info);
	break;
	case divss:
	 divss_(p_insn_exec_info);
	break;
	case dppd:
	 dppd_(p_insn_exec_info);
	break;
	case dpps:
	 dpps_(p_insn_exec_info);
	break;
	case extractps:
	 extractps_(p_insn_exec_info);
	break;
	case haddpd:
	 haddpd_(p_insn_exec_info);
	break;
	case haddps:
	 haddps_(p_insn_exec_info);
	break;
	case hsubpd:
	 hsubpd_(p_insn_exec_info);
	break;
	case hsubps:
	 hsubps_(p_insn_exec_info);
	break;
	case insertps:
	 insertps_(p_insn_exec_info);
	break;
	case lddqu:
	 lddqu_(p_insn_exec_info);
	break;
	case maskmovdqu:
	 maskmovdqu_(p_insn_exec_info);
	break;
	case maskmovq:
	 maskmovq_(p_insn_exec_info);
	break;
	case maxpd:
	 maxpd_(p_insn_exec_info);
	break;
	case maxps:
	 maxps_(p_insn_exec_info);
	break;
	case maxsd:
	 maxsd_(p_insn_exec_info);
	break;
	case maxss:
	 maxss_(p_insn_exec_info);
	break;
	case minpd:
	 minpd_(p_insn_exec_info);
	break;
	case minps:
	 minps_(p_insn_exec_info);
	break;
	case minsd:
	 minsd_(p_insn_exec_info);
	break;
	case minss:
	 minss_(p_insn_exec_info);
	break;
	case movapd:
	 movapd_(p_insn_exec_info);
	break;
	case movaps:
	 movaps_(p_insn_exec_info);
	break;
	case movd:
	 movd_(p_insn_exec_info);
	break;
	case movddup:
	 movddup_(p_insn_exec_info);
	break;
	case movdq2q:
	 movdq2q_(p_insn_exec_info);
	break;
	case movdqa:
	 movdqa_(p_insn_exec_info);
	break;
	case movdqu:
	 movdqu_(p_insn_exec_info);
	break;
	case movhlps:
	 movhlps_(p_insn_exec_info);
	break;
	case movhpd:
	 movhpd_(p_insn_exec_info);
	break;
	case movhps:
	 movhps_(p_insn_exec_info);
	break;
	case movlhpd:
	 movlhpd_(p_insn_exec_info);
	break;
	case movlhps:
	 movlhps_(p_insn_exec_info);
	break;
	case movlpd:
	 movlpd_(p_insn_exec_info);
	break;
	case movlps:
	 movlps_(p_insn_exec_info);
	break;
	case movmskpd:
	 movmskpd_(p_insn_exec_info);
	break;
	case movmskps:
	 movmskps_(p_insn_exec_info);
	break;
	case movndq:
	 movndq_(p_insn_exec_info);
	break;
	case movntdqa:
	 movntdqa_(p_insn_exec_info);
	break;
	case movnti:
	 movnti_(p_insn_exec_info);
	break;
	case movntpd:
	 movntpd_(p_insn_exec_info);
	break;
	case movntps:
	 movntps_(p_insn_exec_info);
	break;
	case movntq:
	 movntq_(p_insn_exec_info);
	break;
	case movq:
	 movq_(p_insn_exec_info);
	break;
	case movq2dq:
	 movq2dq_(p_insn_exec_info);
	break;
	case movsd:
	 movsd_(p_insn_exec_info);
	break;
	case movshdup:
	 movshdup_(p_insn_exec_info);
	break;
	case movsldup:
	 movsldup_(p_insn_exec_info);
	break;
	case movss:
	 movss_(p_insn_exec_info);
	break;
	case movupd:
	 movupd_(p_insn_exec_info);
	break;
	case movups:
	 movups_(p_insn_exec_info);
	break;
	case mpsadbw:
	 mpsadbw_(p_insn_exec_info);
	break;
	case mulpd:
	 mulpd_(p_insn_exec_info);
	break;
	case mulps:
	 mulps_(p_insn_exec_info);
	break;
	case mulsd:
	 mulsd_(p_insn_exec_info);
	break;
	case mulss:
	 mulss_(p_insn_exec_info);
	break;
	case orpd:
	 orpd_(p_insn_exec_info);
	break;
	case orps:
	 orps_(p_insn_exec_info);
	break;
	case packssdw:
	 packssdw_(p_insn_exec_info);
	break;
	case packsswb:
	 packsswb_(p_insn_exec_info);
	break;
	case packuswb:
	 packuswb_(p_insn_exec_info);
	break;
	case paddb:
	 paddb_(p_insn_exec_info);
	break;
	case paddd:
	 paddd_(p_insn_exec_info);
	break;
	case paddq:
	 paddq_(p_insn_exec_info);
	break;
	case paddsb:
	 paddsb_(p_insn_exec_info);
	break;
	case paddsw:
	 paddsw_(p_insn_exec_info);
	break;
	case paddusb:
	 paddusb_(p_insn_exec_info);
	break;
	case paddusw:
	 paddusw_(p_insn_exec_info);
	break;
	case paddw:
	 paddw_(p_insn_exec_info);
	break;
	case pand:
	 pand_(p_insn_exec_info);
	break;
	case pandn:
	 pandn_(p_insn_exec_info);
	break;
	case pavgb:
	 pavgb_(p_insn_exec_info);
	break;
	case pavgw:
	 pavgw_(p_insn_exec_info);
	break;
	case pcmpeqb:
	 pcmpeqb_(p_insn_exec_info);
	break;
	case pcmpeqd:
	 pcmpeqd_(p_insn_exec_info);
	break;
	case pcmpeqw:
	 pcmpeqw_(p_insn_exec_info);
	break;
	case pcmpgtb:
	 pcmpgtb_(p_insn_exec_info);
	break;
	case pcmpgtd:
	 pcmpgtd_(p_insn_exec_info);
	break;
	case pcmpgtw:
	 pcmpgtw_(p_insn_exec_info);
	break;
	case pextrw:
	 pextrw_(p_insn_exec_info);
	break;
	case pinsrw:
	 pinsrw_(p_insn_exec_info);
	break;
	case pmaddwd:
	 pmaddwd_(p_insn_exec_info);
	break;
	case pmaxsw:
	 pmaxsw_(p_insn_exec_info);
	break;
	case pmaxub:
	 pmaxub_(p_insn_exec_info);
	break;
	case pminsw:
	 pminsw_(p_insn_exec_info);
	break;
	case pmovmskb:
	 pmovmskb_(p_insn_exec_info);
	break;
	case pmulhuw:
	 pmulhuw_(p_insn_exec_info);
	break;
	case pmullhw:
	 pmullhw_(p_insn_exec_info);
	break;
	case pmulludq:
	 pmulludq_(p_insn_exec_info);
	break;
	case pmullw:
	 pmullw_(p_insn_exec_info);
	break;
	case por:
	 por_(p_insn_exec_info);
	break;
	case ppushd:
	 ppushd_(p_insn_exec_info);
	break;
	case psadbw:
	 psadbw_(p_insn_exec_info);
	break;
	case pshufhw:
	 pshufhw_(p_insn_exec_info);
	break;
	case pshuflw:
	 pshuflw_(p_insn_exec_info);
	break;
	case pshufw:
	 pshufw_(p_insn_exec_info);
	break;
	case pslld:
	 pslld_(p_insn_exec_info);
	break;
	case pslldq:
	 pslldq_(p_insn_exec_info);
	break;
	case psllq:
	 psllq_(p_insn_exec_info);
	break;
	case psllw:
	 psllw_(p_insn_exec_info);
	break;
	case psrad:
	 psrad_(p_insn_exec_info);
	break;
	case psraw:
	 psraw_(p_insn_exec_info);
	break;
	case psrldq:
	 psrldq_(p_insn_exec_info);
	break;
	case psubb:
	 psubb_(p_insn_exec_info);
	break;
	case psubd:
	 psubd_(p_insn_exec_info);
	break;
	case psubq:
	 psubq_(p_insn_exec_info);
	break;
	case psubsb:
	 psubsb_(p_insn_exec_info);
	break;
	case psubsw:
	 psubsw_(p_insn_exec_info);
	break;
	case psubusb:
	 psubusb_(p_insn_exec_info);
	break;
	case psubusw:
	 psubusw_(p_insn_exec_info);
	break;
	case psubw:
	 psubw_(p_insn_exec_info);
	break;
	case punpckhbw:
	 punpckhbw_(p_insn_exec_info);
	break;
	case punpckhdq:
	 punpckhdq_(p_insn_exec_info);
	break;
	case punpckhqdq:
	 punpckhqdq_(p_insn_exec_info);
	break;
	case punpckhwd:
	 punpckhwd_(p_insn_exec_info);
	break;
	case punpcklbw:
	 punpcklbw_(p_insn_exec_info);
	break;
	case punpckldq:
	 punpckldq_(p_insn_exec_info);
	break;
	case punpcklqdq:
	 punpcklqdq_(p_insn_exec_info);
	break;
	case punpcklwd:
	 punpcklwd_(p_insn_exec_info);
	break;
	case pxor:
	 pxor_(p_insn_exec_info);
	break;
	case rcpps:
	 rcpps_(p_insn_exec_info);
	break;
	case rcpss:
	 rcpss_(p_insn_exec_info);
	break;
	case rsqrtps:
	 rsqrtps_(p_insn_exec_info);
	break;
	case rsqrtss:
	 rsqrtss_(p_insn_exec_info);
	break;
	case shufpd:
	 shufpd_(p_insn_exec_info);
	break;
	case shufps:
	 shufps_(p_insn_exec_info);
	break;
	case sqrtps:
	 sqrtps_(p_insn_exec_info);
	break;
	case sqrtsd:
	 sqrtsd_(p_insn_exec_info);
	break;
	case sqrtss:
	 sqrtss_(p_insn_exec_info);
	break;
	case subpd:
	 subpd_(p_insn_exec_info);
	break;
	case subps:
	 subps_(p_insn_exec_info);
	break;
	case subsd:
	 subsd_(p_insn_exec_info);
	break;
	case subss:
	 subss_(p_insn_exec_info);
	break;
	case ucomisd:
	 ucomisd_(p_insn_exec_info);
	break;
	case ucomiss:
	 ucomiss_(p_insn_exec_info);
	break;
	case unpckhpd:
	 unpckhpd_(p_insn_exec_info);
	break;
	case unpckhps:
	 unpckhps_(p_insn_exec_info);
	break;
	case unpcklpd:
	 unpcklpd_(p_insn_exec_info);
	break;
	case unpcklps:
	 unpcklps_(p_insn_exec_info);
	break;
	case xorpd:
	 xorpd_(p_insn_exec_info);
	break;
	case xorps:
	 xorps_(p_insn_exec_info);
	break;

	}
}
