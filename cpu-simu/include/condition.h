#ifndef __COND_H__
#define __COND_H__


enum COND{

 cond_O,
 cond_NO,
 cond_B,
 cond_NB,
 cond_E,
 cond_NE,
 cond_NA,
 cond_A,
 cond_S,
 cond_NS,
 cond_PE,
 cond_PO,
 cond_L,
 cond_NL,
 cond_NG,
 cond_G
};

#define MASK_ZF 0x40
#define MASK_AF 0x10
#define MASK_PF 0x4
#define MASK_OF 0x800
#define MASK_CF 0x1 
#define MASK_SF 0x80


#define EQU1(x) ((flags & MASK_##x )!=0)
#define EQU0(x) ((flags & MASK_##x )==0)


__inline bool eval_cond ( COND cond,unsigned short flags)
{
	bool val;

	switch(cond)
	{
//case cond_NBE:        //  if not below or equal(CF=0 and ZF=0)
case cond_A  :        //  if above (CF=0 andZF=0)
	val=EQU0(CF)&&EQU0(ZF);
	break;


//case cond_NAE:        //  if not above or equal(CF=1)
//case cond_C  :        //  if carry (CF=1)
case cond_B  :        //  if below (CF=1)
	val=EQU1(CF);
	break;




case cond_E  :        //  if equal (ZF=1)
//case cond_Z  :        //  if 0 (ZF=1)
	val=EQU1(ZF);
	break;
//case cond_NZ :        //  if not zero (ZF=0)
case cond_NE :        //  if not equal (ZF=0)
	val=EQU0(ZF);
	break;



case cond_NA :        //  if not above (CF=1 orZF=1)
//case cond_BE :        //  if below or equal(CF=1 or ZF=1)
	val=EQU1(CF)||EQU1(ZF);
	break;

//case cond_NLE:        // if not less or equal(ZF=0 and SF=OF)
case cond_G  :        // if greater (ZF=0 and SF=OF)
	val=EQU0(ZF)&&(EQU1(SF)==EQU1(OF));
	break;
//case cond_GE :        // if greater or equal(SF=OF)
case cond_NL :        // if not less (SF=OF)
	val=(EQU1(SF)==EQU1(OF));
	break;
case cond_L  :        // if less (SF<>OF)
//case cond_NGE:        // if not greater or equal (SF<>OF)
	val=(EQU1(SF)!=EQU1(OF));
	break;
//case cond_LE :        // if less or equal (ZF=1and SF<>OF)
case cond_NG :        // if not greater (ZF=1or SF<>OF)
	val=EQU1(ZF)||(EQU1(SF)!=EQU1(OF));
	break;

//case cond_AE :        // if above or equal(CF=0)
case cond_NB :        // if not below (CF=0)
//case cond_NC :        // if not carry (CF=0)
	val=EQU0(CF);
	break;
//case cond_NE :        // if not equal (ZF=0)
//case cond_NZ :        // if not zero (ZF=0)
	val=EQU0(ZF);
	break;
case cond_O :         // if overflow (OF=1)
	val=EQU1(OF);
	break;
case cond_NO :        // if not overflow (OF=0)
	val=EQU0(OF);
	break;
//case cond_P :         // if parity (PF=1)
case cond_PE:         // if parity even (PF=1)
	val=EQU1(PF);
	break;

//case cond_NP :        // if not parity (PF=0)
case cond_PO:         // if parity odd (PF=0)
	val=EQU0(PF);
	break;

case cond_S :         // if sign (SF=1)
	val=EQU1(SF);
	break;
case cond_NS :        //  if not sign (SF=0)
	val=EQU0(SF);
	break;
//case cond_Z  :        //  if zero (ZF = 1)
//case cond_E  :        // if equal (ZF=1)
	val=EQU1(ZF);
	break;
	}
	return val;
}


#endif // __COND_H__