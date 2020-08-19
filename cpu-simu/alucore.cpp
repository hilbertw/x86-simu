#include "alucore.h"
#include "condition.h"
#include "sysreg.h"
//Instruction            OF   SF   ZF   AF   PF   CF
//AAA                    --   --   --   TM   --   M
//AAS                    --   --   --   TM   --   M
//AAD                    --   M    M    --   M    --
//AAM                    --   M    M    --   M    --
//DAA                    --   M    M    TM   M    TM
//DAS                    --   M    M    TM   M    TM

//ADC                    M    M    M    M    M    TM
//ADD                    M    M    M    M    M    M
//SBB                    M    M    M    M    M    TM
//SUB                    M    M    M    M    M    M

//CMP                    M    M    M    M    M    M
//CMPS                   M    M    M    M    M    M
//SCAS                   M    M    M    M    M    M
//NEG                    M    M    M    M    M    M
//DEC                    M    M    M    M    M
//INC                    M    M    M    M    M
//IMUL                   M    --   --   --   --   M
//MUL                    M    --   --   --   --   M

//RCL/RCR 1              M                        TM
//RCL/RCR count          --                       TM
//ROL/ROR 1              M                        M
//ROL/ROR count          --                       M
//SAL/SAR/SHL/SHR 1      M    M    M    --   M    M
//SAL/SAR/SHL/SHR count  --   M    M    --   M    M
//SHLD/SHRD              --   M    M    --   M    M
//BSF/BSR                --   --   M    --   --   --
//BT/BTS/BTR/BTC         --   --   --   --   --   M
//AND                    0    M    M    --   M    0
//OR                     0    M    M    --   M    0
//TEST                   0    M    M    --   M    0
//XOR                    0    M    M    --   M    0


typedef unsigned int ALU_RESULT;

bool AF,CF,PF,OF,SF,ZF;


#define MASK_ZF 0x40
#define MASK_AF 0x10
#define MASK_PF 0x4
#define MASK_OF 0x800
#define MASK_CF 0x1 
#define MASK_SF 0x80

#define SET_FLAG(x)  {if (x) cflag|=MASK_##x;else cflag&=~ MASK_##x;}

//#define ZF_MASK 0x40
//#define AF_MASK 0x10
//#define PF_MASK 0x4
//#define OF_MASK 0x800
//#define MASK_CF 0x1 
//#define SF_MASK 0x80
//

#define MOD(x,y) (x-((x<y?x:(x+1-y))/y)*y)

 //PF     Parity Flag -- Set if low-order eight bits of DEST contain
 //       an even number of 1 bits; cleared otherwise.

bool calc_pf(unsigned long DEST)
{
	unsigned char c1,c2;
	bool pf;

	// only lower 8 bits
	c1=(unsigned char)DEST&0xf;
	c2=(unsigned char)(DEST>>4)&0xf;
	c1^=c2;
	
	c2=(c1>>2)&3;
	c1&=3;
	c1^=c2;
	c2=(c1>>1)&1;
	c1&=1;
	pf=(c1^c2)!=1;

	return pf;
}

__inline bool calc_sf(unsigned int DEST, int op_size)
{
	return (DEST&(1<<(op_size-1)))!=0;
}

__inline bool calc_zf(unsigned int DEST)
{
	return DEST==0;
}



   //IF instruction is SAL or SHL
   //THEN CF := high-order bit of r/m;
   //FI;
   //IF instruction is SAR or SHR
   //THEN CF := low-order bit of r/m;
   //FI;



//(* Determine overflow for the various instructions *)
//   IF instruction is SAL or SHL
//   THEN OF := high-order bit of r/m <> (CF);
//   FI;
//   IF instruction is SAR
//   THEN OF := 0;
//   FI;
//   IF instruction is SHR
//   THEN OF := high-order bit of operand;
//   FI;
__inline ALU_RESULT  alu_shr1( unsigned int src1, int op_size) 
{
	ALU_RESULT  DEST;

	if(op_size==8)
	{
	unsigned char c1;

	c1=(unsigned char )src1;
	DEST=(unsigned char )(c1>>1);

	}
	else 	if(op_size==16)
	{

	unsigned short s1;

	s1=(unsigned short )src1;
	DEST=(unsigned short )(s1>>1);

	}else if(op_size==32)
	{
	DEST=src1>>1;
	}

	return DEST;

} 
//(* Determine overflow for the various instructions *)
//   IF instruction is SAL or SHL
//   THEN OF := high-order bit of r/m <> (CF);
//   FI;

//OF is set to 0 if the high bit of the answer is the same as the DEST of the carry flag 
//(i.e., the top two bits of the original operand were the same); OF is set to 1 if they are different
__inline ALU_RESULT  alu_shl1( unsigned int src1, int op_size) 
{
	ALU_RESULT  DEST;


	if(op_size==8)
	{
	unsigned char c1;
	c1=(unsigned char )src1;
	DEST=(unsigned char )(c1<<1);

	}
	else 	if(op_size==16)
	{

	unsigned short s1;

	s1=(unsigned short )src1;
	DEST=(unsigned short )(s1<<1);

	}else if(op_size==32)
	{
	DEST=src1<<1;
	}


	return DEST;
}
   //THEN OF := 0;
__inline ALU_RESULT  alu_sar1(unsigned int src1, int op_size) 
{
	ALU_RESULT  DEST;

	if(op_size==8)
	{
	 unsigned  char  c1;

	c1=( unsigned  char )src1;
	DEST=c1>>1;

	DEST|=(src1 &0x80);
	DEST=( unsigned  char )DEST;
	}
	else 	if(op_size==16)
	{

	unsigned  short  s1;

	s1=(unsigned  short )src1;
	DEST=s1>>1;
	DEST|=(src1 &0x8000);
	DEST=( unsigned  short )DEST;
	}else if(op_size==32)
	{

	
	DEST=src1>>1;
	DEST|=(src1 &0x80000000);
	}

	return DEST;
}
   //IF (high-order bit of r/m) <> (bit next to high-order bit of r/m)
   //THEN OF := 1;
   //ELSE OF := 0;
   //FI;

__inline ALU_RESULT  alu_ror1( unsigned int src1, int op_size) 
{
	ALU_RESULT  DEST=src1;

	if(op_size==8)
	{

	unsigned char s1;

	s1=(unsigned char)src1;


	DEST=s1>>1;
	if (src1&1) DEST|= 0x80;  //temp cf
	DEST=( unsigned  char )DEST;
	}
	else 	if(op_size==16)
	{

	unsigned short s1;

	s1=(unsigned short )src1;

	DEST=s1>>1;
	if (src1&1) DEST|= 0x8000;
	DEST=( unsigned  short )DEST;
	}else if(op_size==32)
	{
	DEST=src1>>1;
	if (src1&1) DEST|= 0x80000000;
	}


	return DEST;

}
//   IF high-order bit of r/m <> CF
//   THEN OF := 1;
//   ELSE OF := 0;
//   FI;
//
ALU_RESULT  alu_rol1( unsigned int src1, int op_size) 
{

	ALU_RESULT  DEST;

	if(op_size==8)
	{

		unsigned char s1;
		s1=(unsigned char)src1;
		DEST=s1<<1;
		if (src1&0x80) DEST|=1 ;  //temp cf
	    DEST=( unsigned  char )DEST;
	}
	else 	if(op_size==16)
	{
		unsigned short s1;
		s1=(unsigned short )src1;
		DEST=s1<<1;
		if (src1&0x8000) DEST|=1 ;
		DEST=( unsigned  short )DEST;
	}else if(op_size==32)
	{
		DEST=src1<<1;
		if (src1&0x80000000) DEST|=1 ;
	}


	return DEST;

}


__inline ALU_RESULT  alu_rcr1( unsigned int src1, int op_size) 
{
	ALU_RESULT  DEST=src1;

	if(op_size==8)
	{

	unsigned char s1;

	s1=(unsigned char)src1;


	DEST=s1>>1;
	if (CF) DEST|= 0x80;  //temp cf
	DEST=( unsigned  char )DEST;
	}
	else 	if(op_size==16)
	{

	unsigned short s1;

	s1=(unsigned short )src1;

	DEST=s1>>1;
	if (CF) DEST|= 0x8000;
	DEST=( unsigned  short )DEST;
	}else if(op_size==32)
	{
	DEST=src1>>1;
	if (CF) DEST|= 0x80000000;
	}
	return DEST;

}
//   IF high-order bit of r/m <> CF
//   THEN OF := 1;
//   ELSE OF := 0;
//   FI;
//
ALU_RESULT  alu_rcl1(unsigned int src1, int op_size) 
{
	ALU_RESULT  DEST;


	if(op_size==8)
	{

	unsigned char s1;

	s1=(unsigned char)src1;


	DEST=s1<<1;
	if (CF) DEST|= 1;  //temp cf
	CF= (src1&0x80)!=0;
	DEST=( unsigned  char )DEST;
	}
	else 	if(op_size==16)
	{

	unsigned short s1;

	s1=(unsigned short )src1;

	DEST=s1<<1;
	if (CF) DEST|= 1;
	CF=(src1&0x8000)!=0;
	DEST=( unsigned  short )DEST;
	}else if(op_size==32)
	{
	DEST=src1<<1;
	if (CF) DEST|=1;
	CF=(src1&0x80000000)!=0;
	}

	return DEST;

}

//r/m := NOT r/m;

ALU_RESULT  alu_not(unsigned short  &cflag, unsigned int src1,int op_size) 
{
	ALU_RESULT DEST=src1^(-1);

	return DEST;

}
//IF r/m = 0 THEN CF := 0 ELSE CF := 1; FI;
//r/m := - r/m;

ALU_RESULT  alu_neg(unsigned short  &cflag, unsigned int src1, int op_size) 
{
	ALU_RESULT  DEST;

	if(op_size==8)
	{

	unsigned char s1;

	s1=src1;

	DEST=1+(0xff^s1);
	DEST=( unsigned  char )DEST;
	//
	//SF:
	//
	SF=(DEST&0x80)!=0;
	//
	//CF:
	//
	CF=s1!=0;
	//
	// OV: 
	//
	OF=s1==0x80;

	}
	else 	if(op_size==16)
	{

	unsigned short s1;

	s1=(unsigned short )src1;

	DEST=1+(0xffff^s1);

	DEST=( unsigned  short )DEST;
	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;
	//
	//
	//CF:
	//
	CF=s1!=0;
	//
	// OV: sold==snew?
	//
	//OF=(unsigned short)src1==(unsigned short)0x8000;
	OF=src1==0x8000;

	}else if(op_size==32)
	{
	DEST=1+(0xffffffff^src1);

	//
	//CF:
	//
	CF=(src1)!=0;
	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;
	//
	// OV: sold==snew?
	//
	//OF=(unsigned long)src1==(unsigned long)0x80000000;
	OF=src1==0x80000000;
	}

	//
	//PF:
	//
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;

	SET_FLAG(ZF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)

	return DEST;


}
//DEST := LeftSRC XOR RightSRC
//CF := 0
//OF := 0


ALU_RESULT  alu_xor(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{

	ALU_RESULT DEST;
	unsigned char c1,c2;
	if(op_size==8)
	{



	c1=(unsigned char )src1;
	c2=(unsigned char )src2;

	DEST=c1^c2;
	DEST=( unsigned  char )DEST;
	//
	//SF:
	//
	SF=(DEST&0x80)!=0;
	}
	else 	if(op_size==16)
	{

	unsigned short s1,s2;

	s1=(unsigned short )src1;
	s2=(unsigned short )src2;

	DEST=s1^s2;
	DEST=( unsigned  short )DEST;
	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;

	}else if(op_size==32)
	{

	DEST=src1^src2;

	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	}

	//OF := 0, CF := 0; 
	OF=CF=false;

	//
	//PF:
	//
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;

	SET_FLAG(ZF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)


	return DEST;


}
//DEST := DEST OR SRC;
//CF := 0;
//OF := 0


ALU_RESULT  alu_or (unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	ALU_RESULT DEST;
	unsigned char c1,c2;
	if(op_size==8)
	{



	c1=(unsigned char )src1;
	c2=(unsigned char )src2;

	DEST=c1|c2;
	DEST=( unsigned  char )DEST;
	//
	//SF:
	//
	SF=(DEST&0x80)!=0;

	}
	else 	if(op_size==16)
	{

	unsigned short s1,s2;

	s1=(unsigned short )src1;
	s2=(unsigned short )src2;

	DEST=s1|s2;
	DEST=( unsigned  short )DEST;
	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;

	}else if(op_size==32)
	{

	DEST=src1|src2;

	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	}

	//OF := 0, CF := 0; 
	OF=CF=false;

	//
	//PF:
	//
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;

	SET_FLAG(ZF)

	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)


	return DEST;


}
//DEST := DEST AND SRC;
//CF := 0;
//OF := 0;


ALU_RESULT  alu_and(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{

	ALU_RESULT DEST;
	unsigned char c1,c2;
	if(op_size==8)
	{

	c1=(unsigned char )src1;
	c2=(unsigned char )src2;

	DEST=c1&c2;
	DEST=( unsigned  char )DEST;
	//
	//SF:
	//
	SF=(DEST&0x80)!=0;

	}
	else 	if(op_size==16)
	{

	unsigned short s1,s2;

	s1=(unsigned short )src1;
	s2=(unsigned short )src2;

	DEST=s1&s2;
	DEST=( unsigned  short )DEST;
	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;
	DEST&=0xffff;

	}else if(op_size==32)
	{

	DEST=src1&src2;

	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	}

	//OF := 0, CF := 0; 
	OF=CF=false;

	//
	//PF:
	//
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;

	SET_FLAG(ZF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)


	return DEST;


}

//CF := BIT[LeftSRC, RightSRC];

void  alu_bt  (unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size)
{
	int mask= 1<<src2;

	CF = (src1&mask)!=0; 


	SET_FLAG(CF)

}

//CF := BIT[LeftSRC, RightSRC];
//BIT[LeftSRC, RightSRC] := 1;

ALU_RESULT  alu_bts (unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size)
{
	int mask= 1<<src2;

	ALU_RESULT  DEST=src1 |mask;

	CF = (src1&mask)!=0; 


	SET_FLAG(CF)

	return DEST;


}

//CF := BIT[LeftSRC, RightSRC];
//BIT[LeftSRC, RightSRC] := 0;

ALU_RESULT  alu_btr (unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	int mask= 1<<src2;

	ALU_RESULT  DEST=src1 &~mask;

	CF = (src1&mask)!=0; 


	SET_FLAG(CF)

	return DEST;

}

//CF := BIT[LeftSRC, RightSRC];
//BIT[LeftSRC, RightSRC] := NOT BIT[LeftSRC, RightSRC];

ALU_RESULT  alu_btc (unsigned short  &cflag, unsigned int src1, unsigned int src2,  int op_size) 
{
	int mask= 1<<src2;

	ALU_RESULT  DEST=src1 ^mask;

	CF = (src1&mask)!=0; 


	SET_FLAG(CF)

	return DEST;

}
//is the number of leading zero bits. 
//Operation
//IF r/m = 0
//THEN
//   ZF := 1;
//   register := UNDEFINED;
//ELSE
//   temp := 0;
//   ZF := 0;
//   WHILE BIT[r/m, temp = 0]
//   DO
//      temp := temp + 1;
//      register := temp;
//   OD;
//FI;


ALU_RESULT  alu_bsf (unsigned short  &cflag, unsigned int src1, int op_size) 
{
	ALU_RESULT  DEST=0;
	if (op_size==8) src1&=0xff;
	else if (op_size==16) src1&=0xffff;

	

	if (!src1 ) 
		ZF=true;
	else
	{
		ZF=false;
		unsigned int bit=0;



		while ((src1 &(1<<bit))==0) bit ++;

		DEST=bit;
	}

	SET_FLAG(ZF)

	return DEST;


}
//IF r/m = 0
//THEN
//   ZF := 1;
//   register := UNDEFINED;
//ELSE
//   temp := OperandSize - 1;
//   ZF := 0;
//   WHILE BIT[r/m, temp] = 0
//   DO
//      temp := temp - 1;
//      register := temp;
//   OD;
//FI;



ALU_RESULT  alu_bsr (unsigned short  &cflag, unsigned int src1, int op_size) 
{
		ALU_RESULT  DEST=0;
	if (op_size==8) src1&=0xff;
	else if (op_size==16) src1&=0xffff;


	if (!src1 ) 
		ZF=true;
	else
	{
		ZF=false;
		unsigned int bit=op_size-1;

		while ((src1 &(1<<bit))==0) bit --;

		DEST=bit;
	}

	SET_FLAG(ZF)

	return DEST;

}



   //IF instruction is SAL or SHL
   //THEN CF := high-order bit of r/m;
   //FI;
   //IF instruction is SAR or SHR
   //THEN CF := low-order bit of r/m;
   //FI;



//(* Determine overflow for the various instructions *)
//IF COUNT = 1
//THEN

//   IF instruction is SAL or SHL
//   THEN OF := high-order bit of r/m <> (CF);
//   FI;
//   IF instruction is SAR
//   THEN OF := 0;
//   FI;
//   IF instruction is SHR
//   THEN OF := high-order bit of operand;
//   FI;
//ELSE OF := undefined;

ALU_RESULT  alu_shr(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	//the 80386 does not allow rotation counts greater than 31. If a rotation count greater than 31 is attempted, 
	//only the bottom five bits of the rotation are used. The 8086 does not mask rotation counts. 
	unsigned char cnt=0x1f&(unsigned short )src2,temp_count;  

	unsigned long DEST=src1;
	// all flags unchanged
	if(cnt==0)
		goto l3;

	temp_count=cnt;
	while(temp_count)
	{

		CF=DEST&1;
		DEST=alu_shr1(DEST,op_size);

		temp_count--;
	}

	//
	// calc MSB(DEST)
	//

	bool msb;

	if(op_size==8)
	{
		msb=(DEST &0x80)!=0;
	}
	else 	if(op_size==16)
	{
		msb=(DEST &0x8000)!=0;

	}else if(op_size==32)
	{
		msb=(DEST &0x80000000)!=0;
	}

	//
	// OF
	//
	if(cnt==1)
	{
		// OF=MSB(temp dest)
		if(op_size==8)
		{
			OF=(src1 &0x80)!=0;
		}
		else 	if(op_size==16)
		{
			OF=(src1 &0x8000)!=0;
		}else if(op_size==32)
		{
			OF=(src1 &0x80000000)!=0;
		}
		SET_FLAG(OF)
	}
	// else undefined 

	SET_FLAG(CF)

	SF=msb;
	SET_FLAG(SF)

	PF=calc_pf(DEST);
	SET_FLAG(PF)

	ZF =DEST==0;
	SET_FLAG(ZF)

l3:
	return DEST;
}
//OF := undefined;
//IF instruction is SAL or SHL
//THEN CF := high-order bit of r/m;


ALU_RESULT  alu_shl(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	//the 80386 does not allow rotation counts greater than 31. If a rotation count greater than 31 is attempted, 
	//only the bottom five bits of the rotation are used. The 8086 does not mask rotation counts. 
	unsigned char cnt=0x1f&(unsigned short )src2,temp_count;  

	unsigned long DEST=src1;
	// all flags unchanged
	if(cnt==0)
		goto l3;

	temp_count=cnt;
	while(temp_count)
	{
		CF=(DEST &(1<<(op_size-1)))!=0;

		DEST=alu_shl1(DEST,op_size);

		temp_count--;
	}

	//
	// calc MSB(DEST)
	//

	bool msb;

	if(op_size==8)
	{

		msb=(DEST &0x80)!=0;
	}
	else 	if(op_size==16)
	{
		msb=(DEST &0x8000)!=0;

	}else if(op_size==32)
	{
		msb=(DEST &0x80000000)!=0;
	}

	//
	// OF
	//
	if(cnt==1)
	{
		// OF=MSB(dest) ^CF
		OF=msb^CF;
		SET_FLAG(OF)
	}
	// else undefined 

	SET_FLAG(CF)

	SF=msb;
	SET_FLAG(SF)

	PF=calc_pf(DEST);
	SET_FLAG(PF)

	ZF =DEST==0;
	SET_FLAG(ZF)

l3:
	return DEST;

}

ALU_RESULT  alu_sal(unsigned short  &cflag, unsigned int src1,unsigned int src2,int op_size) 
{
	return alu_shl(cflag, src1,src2, op_size);

}
//OF := undefined;
// CF := low-order bit of r/m;

ALU_RESULT  alu_sar(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	//the 80386 does not allow rotation counts greater than 31. If a rotation count greater than 31 is attempted, 
	//only the bottom five bits of the rotation are used. The 8086 does not mask rotation counts. 
	unsigned char cnt=0x1f&(unsigned short )src2,temp_count;  

	unsigned long DEST=src1;
	// all flags unchanged
	if(cnt==0)
		goto l3;

	temp_count=cnt;
	while(temp_count)
	{

		CF=DEST &1;
		DEST=alu_sar1(DEST,op_size);

		temp_count--;
	}

	//
	// calc MSB(DEST)
	//

	bool msb;

	if(op_size==8)
	{

		msb=(DEST &0x80)!=0;
	}
	else 	if(op_size==16)
	{
		msb=(DEST &0x8000)!=0;

	}else if(op_size==32)
	{
		msb=(DEST &0x80000000)!=0;
	}

	//
	// OF
	//
	if(cnt==1)
	{
		// OF=0
		OF=0;
		SET_FLAG(OF)
	}
	// else undefined 

	SET_FLAG(CF)

	SF=msb;
	SET_FLAG(SF)

	PF=calc_pf(DEST);
	SET_FLAG(PF)

	ZF =DEST==0;
	SET_FLAG(ZF)

l3:
	return DEST;

}



//OF := undefined

//temp := COUNT;
//WHILE (temp <> 0 )
//DO
//   tmpcf := low-order bit of (r/m);
//   r/m := r/m / 2 + (tmpcf * 2^(width(r/m)));
//   temp := temp - 1;
//DO;
//IF COUNT = 1
//THEN
//   IF (high-order bit of r/m) <> (bit next to high-order bit of r/m)
//   THEN OF := 1;
//   ELSE OF := 0;
//   FI;
//ELSE OF := undefined;

ALU_RESULT  alu_ror(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	//the 80386 does not allow rotation counts greater than 31. If a rotation count greater than 31 is attempted, 
	//only the bottom five bits of the rotation are used. The 8086 does not mask rotation counts. 
	unsigned char cnt=0x1f&(unsigned short )src2,temp_count;  
	ALU_RESULT DEST=src1;

    if(op_size==8) temp_count=MOD(cnt,8);
	else if(op_size==16) temp_count=MOD(cnt,16);
	else if(op_size==32) temp_count=MOD(cnt,32);
	else temp_count=MOD(cnt,64);  // 64


	// all flags unchanged
	if(temp_count==0) goto l3;

	while(temp_count)
	{

		bool temp_CF=DEST&1;
		DEST=alu_ror1(DEST,op_size);
		CF=temp_CF;

		temp_count--;
	}

	//
	// calc MSB(DEST)
	//

	bool msb;

	if(op_size==8)
	{
		msb=(DEST &0x80)!=0;
	}
	else 	if(op_size==16)
	{
		msb=(DEST &0x8000)!=0;
	}else if(op_size==32)
	{
		msb=(DEST &0x80000000)!=0;
	}
	//
	// CF= MSB(dest)
	//
	CF=msb;
	SET_FLAG(CF)
	//
	// OF
	//
	if(cnt==1)
	{
		bool msb_1;
		// OF=MSB(temp dest)^MSB-1(temp dest)
		if(op_size==8)
		{
			msb_1=(DEST &0x40)!=0;
		}
		else 	if(op_size==16)
		{
			msb_1=(DEST &0x4000)!=0;
		}else if(op_size==32)
		{
			msb_1=(DEST &0x40000000)!=0;
		}
		OF=msb ^msb_1;
		SET_FLAG(OF)
	}
	// else  OF undefined 




	// SF, ZF,PF, not affected.

l3:
	return DEST;

}
//OF is undefined for multi-bit rotates; 
//temp := COUNT;
//WHILE (temp <> 0)
//DO
//   tmpcf := high-order bit of (r/m);
//   r/m := r/m * 2 + (tmpcf);
//   temp := temp - 1;
//OD;
//IF COUNT = 1
//THEN
//   IF high-order bit of r/m <> CF
//   THEN OF := 1;
//   ELSE OF := 0;
//   FI;
//ELSE OF := undefined;

ALU_RESULT  alu_rol(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	//the 80386 does not allow rotation counts greater than 31. If a rotation count greater than 31 is attempted, 
	//only the bottom five bits of the rotation are used. The 8086 does not mask rotation counts. 
	unsigned char cnt=0x1f&(unsigned short )src2,temp_count;  
	unsigned short   cf=cflag;
	ALU_RESULT DEST=src1;



    if(op_size==8) temp_count=MOD(cnt,8);
	else if(op_size==16) temp_count=MOD(cnt,16);
	else if(op_size==32) temp_count=MOD(cnt,32);

	// all flags unchanged
	if(temp_count==0)	goto l3;

	while(temp_count)
	{

		DEST=alu_rol1(DEST,op_size);

		temp_count--;
	}

	//
	// calc MSB(DEST),LSB(DEST)
	//

	bool msb,lsb;

	if(op_size==8)
	{
		msb=(DEST &0x80)!=0;
	}
	else 	if(op_size==16)
	{
		msb=(DEST &0x8000)!=0;
	}else if(op_size==32)
	{
		msb=(DEST &0x80000000)!=0;
	}

	lsb=DEST &1;
	//
	// CF= MSB(dest)
	//
	CF=lsb;
	SET_FLAG(CF)
	//
	// OF
	//
	if(cnt==1)
	{
		// OF=MSB( dest)^LSB( dest)
		OF=msb ^lsb;
		SET_FLAG(OF)
	}
	// else  OF undefined 
	// SF, ZF,PF, not affected.

l3:
	return DEST;
}

//OF := undefined

//temp := COUNT;
//WHILE (temp <> 0 )
//DO
//   tmpcf := low-order bit of (r/m);
//   r/m := r/m / 2 + (tmpcf * 2^(width(r/m)));
//   temp := temp - 1;
//DO;
//IF COUNT = 1
//THEN
//   IF (high-order bit of r/m) <> (bit next to high-order bit of r/m)
//   THEN OF := 1;
//   ELSE OF := 0;
//   FI;
//ELSE OF := undefined;

ALU_RESULT  alu_rcr(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	//the 80386 does not allow rotation counts greater than 31. If a rotation count greater than 31 is attempted, 
	//only the bottom five bits of the rotation are used. The 8086 does not mask rotation counts. 
	unsigned char cnt=(unsigned short )src2&0x1f,temp_count;

	ALU_RESULT DEST=src1;

    if(op_size==8) temp_count=MOD(cnt,9);
	else if(op_size==16) temp_count=MOD(cnt,17);
	else temp_count=cnt;


	if(temp_count!=0)
	{
		//
		// calc MSB(DEST)
		//

		bool msb;

		if(op_size==8)
		{
			msb=(DEST &0x80)!=0;
		}
		else 	if(op_size==16)
		{
			msb=(DEST &0x8000)!=0;
		}else if(op_size==32)
		{
			msb=(DEST &0x80000000)!=0;
		}

		bool   temp_CF=cflag&MASK_CF;
		//
		// OF=MSB^CF
		//
		if(cnt==1)
		{
			OF=msb^temp_CF;
			SET_FLAG(OF)
		}
		//else  OF undefined 
		//
		// 
		//

		while(temp_count)
		{
			temp_CF=DEST&1;
			DEST=alu_rcr1(DEST,op_size);
			CF=temp_CF;

			temp_count--;
		}

		SET_FLAG(CF)

		// SF, ZF,PF, not affected.
	}


	return DEST;

}
//OF is undefined for multi-bit rotates; 
//temp := COUNT;
//WHILE (temp <> 0)
//DO
//   tmpcf := high-order bit of (r/m);
//   r/m := r/m * 2 + (tmpcf);
//   temp := temp - 1;
//OD;
//IF COUNT = 1
//THEN
//   IF high-order bit of r/m <> CF
//   THEN OF := 1;
//   ELSE OF := 0;
//   FI;
//ELSE OF := undefined;
//Instruction            OF   SF   ZF   AF   PF   CF
//RCL/RCR 1              M                        TM
//RCL/RCR count          --                       TM
//ROL/ROR 1              M                        M
//ROL/ROR count          --                       M

ALU_RESULT  alu_rcl(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	//the 80386 does not allow rotation counts greater than 31. If a rotation count greater than 31 is attempted, 
	//only the bottom five bits of the rotation are used. The 8086 does not mask rotation counts. 
	unsigned char cnt=(unsigned short )src2&0x1f,temp_count;  
	unsigned long DEST=src1;


    if(op_size==8) temp_count=MOD(cnt,9);
	else if(op_size==16) temp_count=MOD(cnt,17);
	else if(op_size==32) temp_count=MOD(cnt,0x1f);
	else temp_count=MOD(cnt,0x3f);

	if(temp_count!=0) 
	{

		bool   temp_CF=(cflag&MASK_CF)!=0;

		//
		// 
		//

		CF=temp_CF;
		while(temp_count)
		{
			temp_CF=(DEST&(1<<(op_size-1)))!=0;
			DEST=alu_rcl1(DEST,op_size);
			CF=temp_CF;

			temp_count--;
		}

		SET_FLAG(CF)


		//
		// calc MSB(DEST)
		//

		bool msb;

		if(op_size==8)
		{
			msb=(DEST &0x80)!=0;
		}
		else 	if(op_size==16)
		{
			msb=(DEST &0x8000)!=0;
		}else if(op_size==32)
		{
			msb=(DEST &0x80000000)!=0;
		}
		//
		// OF=MSB^CF
		//
		if(cnt==1)
		{
			OF=msb^CF;
			SET_FLAG(OF)
		}
		//else  OF undefined 




		// SF, ZF,PF, not affected.
	}


	return DEST;


}

void alu_cmc(unsigned short &cflags)
{
(*(UEFLAGS*)&cflags).s.CF=~(*(UEFLAGS*)&cflags).s.CF;
}
void alu_clc(unsigned short& cflags)
{
(*(UEFLAGS*)&cflags).s.CF=0;
}
void alu_stc(unsigned short& cflags)
{
(*(UEFLAGS*)&cflags).s.CF=1;
}
void alu_cld(unsigned short &cflags)
{
(*(UEFLAGS*)&cflags).s.DF=0;
}
void alu_std_(unsigned short& cflags)
{
(*(UEFLAGS*)&cflags).s.DF=1;
}
void alu_cli(unsigned short& cflags)
{
(*(UEFLAGS*)&cflags).s.IF=0;
}
void alu_sti(unsigned short& cflags)
{
(*(UEFLAGS*)&cflags).s.IF=1;
}
unsigned int alu_movsx(unsigned short  cflag, unsigned int data1, int op_size)
{
	long DEST;
	if(op_size==8)// src size
	 DEST=(char)data1;
	else DEST=(short)data1;
	return DEST;
}
unsigned int alu_movzx(unsigned short  cflag, unsigned int data1, int op_size)
{
	long DEST;
	if(op_size==8)
	DEST=(unsigned char)data1;
	else DEST=(unsigned short)data1;
	return DEST;
}


unsigned int alu_SETO(unsigned short cflag) 
{
 	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_O,cflag)?1:0;
}
unsigned int alu_SETNO(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_NO,cflag)?1:0;
}
unsigned int alu_SETB(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_B,cflag)?1:0;
}
unsigned int alu_SETNB(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_NB,cflag)?1:0;
}
unsigned int alu_SETE(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_E,cflag)?1:0;
}
unsigned int alu_SETNE(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_NE,cflag)?1:0;
}
unsigned int alu_SETNA(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_NA,cflag)?1:0;
}
unsigned int alu_SETA(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_A,cflag)?1:0;
}
unsigned int alu_SETS(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_S,cflag)?1:0;
}
unsigned int alu_SETNS(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_NS,cflag)?1:0;
}
unsigned int alu_SETPE(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_PE,cflag)?1:0;
}
unsigned int alu_SETPO(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_PO,cflag)?1:0;
}
unsigned int alu_SETL(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_L,cflag)?1:0;
}
unsigned int alu_SETNL(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_NL,cflag)?1:0;
}
unsigned int alu_SETNG(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_NG,cflag)?1:0;
}
unsigned int alu_SETG(unsigned short cflag) 
{
 
	//if condition  r/m8 := 1;else r/m8 := 0; 
	return eval_cond(cond_G,cflag)?1:0;
}

/* count is an unsigned integer corresponding to the last operand of the
instruction, either an immediate byte or the byte in register CL */

//ShiftAmt := count MOD 32;
//inBits := register; (* Allow overlapped operands *)
//IF ShiftAmt = 0
//THEN no operation
//ELSE
//   IF ShiftAmt >= OperandSize
//   THEN (* Bad parameters *)
//      r/m := UNDEFINED;
//      CF, OF, SF, ZF, AF, PF := UNDEFINED;
//   ELSE (* Perform the shift *)
//      CF := BIT[Base, OperandSize - ShiftAmt];
//         (* Last bit shifted out on exit *)
//   FOR i := OperandSize - 1 DOWNTO ShiftAmt
//   DO
//      BIT[Base, i] := BIT[Base, i - ShiftAmt];
//   OF;
//   FOR i := ShiftAmt - 1 DOWNTO 0
//   DO
//      BIT[Base, i] := BIT[inBits, i - ShiftAmt + OperandSize];
//   OD;
//   Set SF, ZF, PF (r/m);
//      (* SF, ZF, PF are set according to the value of the DEST *)
//   AF := UNDEFINED;
//   FI;
//FI;



unsigned int alu_shld(unsigned short & cflag, unsigned int rm,unsigned long reg, unsigned int count,int op_size)
{
	int ShiftAmt=count&0x1f;
	unsigned int DEST=rm;

	if(ShiftAmt==0)
	{
	}
	else if(ShiftAmt>=op_size)// ShiftAmt >= OperandSize
	{
	}
	else
	{
		CF=(reg&(1<<(op_size-ShiftAmt)))==0?0:1;
		//
		// higher ShiftAmt bits of reg goes to lower ShiftAmt bits of rm
		//
		DEST=(reg>>(op_size-ShiftAmt))|(rm<<ShiftAmt);

		//reg=/*(rm>>(op_size-ShiftAmt))|*/(reg<<ShiftAmt);

		PF=calc_pf(DEST);
		SF=calc_sf(DEST,op_size);
		ZF=calc_zf(DEST);

		SET_FLAG(ZF)
		SET_FLAG(PF)
		SET_FLAG(SF)
	}
	return DEST;
}
/* count is an unsigned integer corresponding to the last operand of the
instruction, either an immediate byte or the byte in register CL *)
ShiftAmt := count MOD 32;
inBits := register; (* Allow overlapped operands */
//
//IF ShiftAmt = 0
//THEN no operation
//ELSE
//   IF ShiftAmt >= OperandSize
//   THEN (* Bad parameters *)
//      r/m := UNDEFINED;
//      CF, OF, SF, ZF, AF, PF := UNDEFINED;
//   ELSE (* Perform the shift *)
//      CF := BIT[r/m, ShiftAmt - 1]; (* last bit shifted out on exit *)
//      FOR i := 0 TO OperandSize - 1 - ShiftAmt
//      DO
//         BIT[r/m, i] := BIT[r/m, i - ShiftAmt];
//      OD;
//      FOR i := OperandSize - ShiftAmt TO OperandSize - 1
//      DO
//         BIT[r/m,i] := BIT[inBits,i+ShiftAmt - OperandSize];
//      OD;
//      Set SF, ZF, PF (r/m);
//         (* SF, ZF, PF are set according to the value of the DEST *)
//      Set SF, ZF, PF (r/m);
//      AF := UNDEFINED;
//   FI;
//FI;

unsigned int alu_shrd(unsigned short & cflag, unsigned int  rm,unsigned long reg, unsigned int count,int op_size)
{

	int ShiftAmt=count&0x1f;
	unsigned int DEST=rm;

	if(ShiftAmt==0)
	{
	}
	else if(ShiftAmt>=op_size)// ShiftAmt >= OperandSize
	{
	}
	else
	{
		CF=(rm&(1<<(ShiftAmt-1)))==0?0:1;
		//
		// lower ShiftAmt bits of reg goes to higher ShiftAmt bits of rm
		//
		DEST=(reg<<(op_size-ShiftAmt))|(rm>>ShiftAmt);

		//reg=/*(rm<<(op_size-ShiftAmt))|*/(reg>>ShiftAmt);
		PF=calc_pf(DEST);
		SF=calc_sf(DEST,op_size);
		ZF=calc_zf(DEST);

		SET_FLAG(ZF)
		SET_FLAG(PF)
		SET_FLAG(SF)
	}
	return DEST;
}

#if 1 
ALU_RESULT  alu_adder(unsigned short  &cflag, 
					 unsigned int src1,
					 unsigned int src2, 
					 unsigned int c,
					 int op_size) 
{
	ALU_RESULT  DEST;
	//
	//lowest 4 bits to calc AF
	//

	unsigned char c1,c2;

	c1=(unsigned char )src1&0xf;
	c2=(unsigned char )src2&0xf;

	c1+=c2+c;
	AF=(c1&0x10)!=0;

	if(op_size==8)
	{

	unsigned char s1,s2;

	s1=(unsigned char)src1;
	s2=(unsigned char)src2;


	DEST=s1+s2+c;

	//
	//SF:
	//
	SF=(DEST&0x80)!=0;
	//
	//CF:
	//
	CF=(DEST&0x100)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2)&0x100))!=(0==(DEST&0x80));
	OF=((s1^s2^(s1+s2+c)^(DEST>>1))&0x80)!=0;

	DEST=(unsigned char)DEST;

	}
	else 	if(op_size==16)
	{

	unsigned short s1,s2;

	s1=(unsigned short )src1;
	s2=(unsigned short )src2;

	DEST=s1+s2+c;

	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;
	//
	//CF:
	//
	CF=(DEST&0x10000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2)&0x10000))!=(0==(DEST&0x8000));
	OF=((s1^s2^(s1+s2+c)^(DEST>>1))&0x8000)!=0;
	DEST=(unsigned short)DEST;

	}else if(op_size==32)
	{

	unsigned int s1,s2;
	s1=((unsigned int )src1)>>4;
	s2=((unsigned int )src2)>>4;

	DEST=(s1&0xfffffff)+(s2&0xfffffff)+(AF?1:0);
	//
	//CF:
	//
	CF=(DEST&0x10000000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2+(AF?1:0))&0x10000000))!=(0==(DEST&0x8000000));
	OF=((s1^s2^(s1+s2+(AF?1:0))^(DEST>>1))&0x8000000)!=0;
	DEST <<=4;
	DEST|=(c1&0xf);

	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	}

	//
	//PF:
	//
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;
	//ADD                    M    M    M    M    M    M
	SET_FLAG(ZF)
	SET_FLAG(AF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)

	return DEST;

}



ALU_RESULT  alu_add(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	return alu_adder(cflag,src1,src2,0,op_size);
}
ALU_RESULT  alu_sub(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	ALU_RESULT DEST;

	//if(src2==0)
	//	DEST=alu_adder(cflag,src1,0,0,op_size);
	//else
	{
	DEST=alu_adder(cflag,src1,~src2,1,op_size);
	cflag^=(MASK_CF|MASK_AF);

	if(src2==(1<<(op_size-1)))cflag |=MASK_OF;
	}
	return DEST;
}
ALU_RESULT  alu_adc(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	return alu_adder(cflag,src1,src2,(cflag &MASK_CF)?1:0,op_size);
}
ALU_RESULT  alu_subb(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
	ALU_RESULT DEST;
	//if(src2==0)
	//	DEST=alu_adder(cflag,src1,(cflag &MASK_CF)?-1:0,0,op_size);
	//else
		DEST=alu_adder(cflag,src1,~src2,(cflag &MASK_CF)?0:1,op_size);

	cflag^=(MASK_CF|MASK_AF);
	
	return DEST;
}



//INC adds 1 to the operand. It does not change the carry flag. To affect the carry flag, 
//use the ADD instruction with a second operand of 1. 
ALU_RESULT  alu_inc (unsigned short  &cflag, unsigned int src1, int op_size)
{

	ALU_RESULT DEST;
	bool cf=(cflag &MASK_CF)!=0;
	DEST=alu_adder(cflag,src1,1,0,op_size);

	CF=cf;
	SET_FLAG(CF);

	return DEST;

}
//DEC subtracts 1 from the operand. DEC does not change the carry flag. To affect the carry flag, 
//use the SUB instruction with an immediate operand of 1. 
ALU_RESULT  alu_dec (unsigned short  &cflag, unsigned int src1, int op_size)
{

	ALU_RESULT DEST;
	bool cf=(cflag &MASK_CF)!=0;
	DEST=alu_adder(cflag,src1,-1,0,op_size);

	CF=cf;
	SET_FLAG(CF);
	AF=!AF;
	SET_FLAG(AF);
	return DEST;

}
#else

ALU_RESULT  alu_add(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
ALU_RESULT  DEST;
	//
	//lowest 4 bits to calc AF
	//

	unsigned char c1,c2;

	c1=(unsigned char )src1&0xf;
	c2=(unsigned char )src2&0xf;

	c1+=c2;
	AF=(c1&0x10)!=0;

	if(op_size==8)
	{

	unsigned int s1,s2;

	s1=(unsigned char)src1;
	s2=(unsigned char)src2;


	DEST=(s1&0xff)+(s2&0xff);

	//
	//SF:
	//
	SF=(DEST&0x80)!=0;
	//
	//CF:
	//
	CF=(DEST&0x100)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2)&0x100))!=(0==(DEST&0x80));
	OF=((s1^s2^(s1+s2)^(DEST>>1))&0x80)!=0;

	DEST&=0xff;

	}
	else 	if(op_size==16)
	{

	unsigned int s1,s2;

	s1=(unsigned short )src1;
	s2=(unsigned short )src2;

	DEST=(s1&0xffff)+(s2&0xffff);

	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;
	//
	//CF:
	//
	CF=(DEST&0x10000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2)&0x10000))!=(0==(DEST&0x8000));
	OF=((s1^s2^(s1+s2)^(DEST>>1))&0x8000)!=0;
	DEST&=0xffff;

	}else if(op_size==32)
	{

	unsigned int s1,s2;
	s1=((unsigned int )src1)>>4;
	s2=((unsigned int )src2)>>4;

	DEST=(s1&0xfffffff)+(s2&0xfffffff)+(AF?1:0);
	//
	//CF:
	//
	CF=(DEST&0x10000000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2+(AF?1:0))&0x10000000))!=(0==(DEST&0x8000000));
	OF=((s1^s2^(s1+s2+(AF?1:0))^(DEST>>1))&0x8000000)!=0;
	DEST <<=4;
	DEST|=(c1&0xf);

	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	}

	//
	//PF:
	//
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;
	//ADD                    M    M    M    M    M    M
	SET_FLAG(ZF)
	SET_FLAG(AF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)

	return DEST;

}


ALU_RESULT  alu_adc(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
ALU_RESULT  DEST;
	//
	//lowest 4 bits to calc AF
	//

	unsigned char c1,c2;

	c1=(unsigned char )src1&0xf;
	c2=(unsigned char )src2&0xf;

	c1+=c2;
	if (0!=(cflag &MASK_CF))c1++;

	AF=(c1&0x10)!=0;

	if(op_size==8)
	{

	unsigned int s1,s2,c;

	s1=(unsigned char)src1;
	s2=(unsigned char)src2;


	DEST=(s1&0xff)+(s2&0xff);
	if (0!=(cflag &MASK_CF)) {DEST++;c=1;}else c=0;
	//
	//SF:
	//
	SF=(DEST&0x80)!=0;
	//
	//CF:
	//
	CF=(DEST&0x100)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2+c)&0x100))!=(0==(DEST&0x80));
	OF=((s1^s2^(s1+s2+c)^(DEST>>1))&0x80)!=0;
	DEST&=0xff;

	}
	else 	if(op_size==16)
	{

	unsigned int s1,s2,c;

	s1=(unsigned short )src1;
	s2=(unsigned short )src2;

	DEST=(s1&0xffff)+(s2&0xffff);
	if (0!=(cflag &MASK_CF)) {DEST++;c=1;}else c=0;
	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;
	//
	//CF:
	//
	CF=(DEST&0x10000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2+c)&0x10000))!=(0==(DEST&0x8000));
	OF=((s1^s2^(s1+s2+c)^(DEST>>1))&0x8000)!=0;
	DEST&=0xffff;

	}else if(op_size==32)
	{

	unsigned int s1,s2;
	s1=((unsigned int )src1)>>4;
	s2=((unsigned int )src2)>>4;

	DEST=(s1&0xfffffff)+(s2&0xfffffff)+(AF?1:0);
	//
	//CF:
	//
	CF=(DEST&0x10000000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	//OF= (0==((s1+s2+(AF?1:0))&0x10000000))!=(0==(DEST&0x8000000));
	OF=((s1^s2^(s1+s2+(AF?1:0))^(DEST>>1))&0x8000000)!=0;
	DEST <<=4;
	DEST|=(c1&0xf);

	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	}
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;
	//ADD                    M    M    M    M    M    M
	SET_FLAG(ZF)
	SET_FLAG(AF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)

	return DEST;

}


ALU_RESULT  alu_sub(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
ALU_RESULT  DEST;
	//
	//lowest 4 bits to calc AF
	//

	unsigned char c1,c2;

	c1=(unsigned char )src1&0xf;
	c2=(unsigned char )src2&0xf;

	c1-=c2;
	AF=(c1&0x10)!=0;

	if(op_size==8)
	{

	unsigned int s1,s2;

	s1=(unsigned char)src1;
	s2=(unsigned char)src2;


	DEST=(s1&0xff)-(s2&0xff);

	//
	//SF:
	//
	SF=(DEST&0x80)!=0;
	//
	//CF:
	//
	CF=(DEST&0x100)!=0;
	//
	// OV: Cs+1==Cs?
	//
	OF= (0==((s1-s2)&0x100))!=(0==(DEST&0x80));

	DEST&=0xff;

	}
	else 	if(op_size==16)
	{

	
	unsigned int s1,s2;

	s1=(unsigned short )src1;
	s2=(unsigned short )src2;

	DEST=(s1&0xffff)-(s2&0xffff);

	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;
	//
	//CF:
	//
	CF=(DEST&0x10000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	OF= (0==((s1-s2)&0x10000))!=(0==(DEST&0x8000));

	DEST&=0xffff;

	}else if(op_size==32)
	{

	unsigned int s1,s2;
	s1=((unsigned int )src1)>>4;
	s2=((unsigned int )src2)>>4;

	DEST=(s1&0xfffffff)-(s2&0xfffffff)-(AF?1:0);
	//
	//CF:
	//
	CF=(DEST&0x10000000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	OF= (0==((s1-s2-(AF?1:0))&0x10000000))!=(0==(DEST&0x8000000));

	DEST <<=4;
	DEST|=(c1&0xf);

	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	}

	//
	//PF:
	//

	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;
	//ADD                    M    M    M    M    M    M
	SET_FLAG(ZF)
	SET_FLAG(AF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)

	return DEST;

}


ALU_RESULT  alu_subb(unsigned short  &cflag, unsigned int src1,unsigned int src2, int op_size) 
{
ALU_RESULT  DEST;
	//
	//lowest 4 bits to calc AF
	//

	unsigned char c1,c2;

	c1=(unsigned char )src1&0xf;
	c2=(unsigned char )src2&0xf;

	c1-=c2;
	if (0!=(cflag &MASK_CF))c1--;

	AF=(c1&0x10)!=0;

	if(op_size==8)
	{

	unsigned int s1,s2,c;

	s1=(unsigned char)src1;
	s2=(unsigned char)src2;


	DEST=(s1&0xff)-(s2&0xff);
	if (0!=(cflag &MASK_CF)) {DEST--;c=1;}else c=0;
	//
	//SF:
	//
	SF=(DEST&0x80)!=0;
	//
	//CF:
	//
	CF=(DEST&0x100)!=0;
	//
	// OV: Cs+1==Cs?
	//
	OF= (0==((s1-s2-c)&0x100))!=(0==(DEST&0x80));

	DEST&=0xff;

	}
	else 	if(op_size==16)
	{

	unsigned int s1,s2,c;

	s1=(unsigned short )src1;
	s2=(unsigned short )src2;

	DEST=(s1&0xffff)-(s2&0xffff);
	if (0!=(cflag &MASK_CF)) {DEST--;c=1;}
	else c=0;
	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;
	//
	//CF:
	//
	CF=(DEST&0x10000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	OF= (((s1-s2-c)&0x10000)>>1)!=(DEST&0x8000);

	DEST&=0xffff;

	}else if(op_size==32)
	{

	
	unsigned int s1,s2;
	s1=((unsigned int )src1)>>4;
	s2=((unsigned int )src2)>>4;

	DEST=(s1&0xfffffff)-(s2&0xfffffff)-(AF?1:0);
	//
	//CF:
	//
	CF=(DEST&0x10000000)!=0;
	//
	// OV: Cs+1==Cs?
	//
	OF= ((((s1-s2-(AF?1:0))&0x10000000))>>1)!=(DEST&0x8000000);

	DEST <<=4;
	DEST|=(c1&0xf);

	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	}
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;
	//ADD                    M    M    M    M    M    M
	SET_FLAG(ZF)
	SET_FLAG(AF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(CF)
	SET_FLAG(OF)

	return DEST;

}

//INC adds 1 to the operand. It does not change the carry flag. To affect the carry flag, 
//use the ADD instruction with a second operand of 1. 
ALU_RESULT  alu_inc (unsigned short  &cflag, unsigned int src1, int op_size)
{
	ALU_RESULT  DEST;
	//
	//lowest 4 bits to calc AF
	//

	char c1,c2;

	c1=( char )src1&0xf;

	c1++;
	AF=(c1&0x10)!=0;

	if(op_size==8)
	{

	short s1;

	s1=( char)src1;
	DEST=s1+1;

	//
	//SF:
	//
	SF=(DEST&0x80)!=0;

	//
	// OV: Cs+1==Cs?
	//
	OF=(((s1+1)&0x100)>>1)!=(DEST&0x80);
	DEST&=0xff;

	}
	else 	if(op_size==16)
	{

	int  s1;

	s1=(short )src1;

	DEST=s1+1;

	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;

	//
	// OV: Cs+1==Cs?
	//
	OF=(((s1+1)&0x10000)>>1)!=(DEST&0x8000);

	DEST&=0xffff;

	}else if(op_size==32)
	{

	int s1;

	s1=(int )(src1>>4)&0xfffffff;

	DEST=s1+(AF?1:0);

	DEST <<=4;
	DEST|=(c1&0xf);
	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;

	//
	// OV: Cs+1==Cs?
	//
	OF=(((s1+(AF?1:0))&0x100000000)>>1)!=(DEST&0x80000000);


	}

	//
	//PF:
	//
	c1=DEST&0xf;
	c2=(DEST>>4)&0xf;
	c1^=c2;
	
	c2=(c1>>2)&3;
	c1&=3;
	c1^=c2;
	c2=(c1>>1)&1;
	c1&=1;
	PF=(c1^c2)==0;
	//
	// ZF
	//
	ZF=DEST==0;

	SET_FLAG(ZF)
	SET_FLAG(AF)
	SET_FLAG(PF)
	SET_FLAG(SF)

	SET_FLAG(OF)

	return DEST;

}

//DEC subtracts 1 from the operand. DEC does not change the carry flag. To affect the carry flag, 
//use the SUB instruction with an immediate operand of 1. 
ALU_RESULT  alu_dec (unsigned short  &cflag, unsigned int src1, int op_size)
{
	ALU_RESULT  DEST;
	//
	//lowest 4 bits to calc AF
	//

	char c1;

	c1=(unsigned char )src1&0xf;

	c1--;
	AF=(c1&0x10)!=0;

	if(op_size==8)
	{

	unsigned short s1;

	s1=( char )src1;
	DEST=s1-1;

	//
	//SF:
	//
	SF=(DEST&0x80)!=0;

	//
	// OV: Cs+1==Cs?
	//
	OF=(((s1-1)&0x100)>>1)!=(DEST&0x80);
	DEST&=0xff;

	}
	else 	if(op_size==16)
	{

	int s1;

	s1=(short )src1;

	DEST=s1-1;

	//
	//SF:
	//
	SF=(DEST&0x8000)!=0;

	//
	// OV: Cs+1==Cs?
	//
	OF=(((s1-1)&0x10000)>>1)!=(DEST&0x8000);

	DEST&=0xffff;

	}else if(op_size==32)
	{

	int s1;

	s1=(int )(src1>>4)&0xfffffff;

	DEST=s1-(AF?1:0);
	//
	// OV: Cs+1==Cs?
	//
	OF=(((s1-(AF?1:0))&0x10000000)>>1)!=(DEST&0x8000000);

	DEST <<=4;
	DEST|=(c1&0xf);
	//
	//SF:
	//
	SF=(DEST&0x80000000)!=0;


	}

	//
	//PF:
	//
	PF=calc_pf(DEST);
	//
	// ZF
	//
	ZF=DEST==0;

	SET_FLAG(ZF)
	SET_FLAG(AF)
	SET_FLAG(PF)
	SET_FLAG(SF)
	SET_FLAG(OF)

	return DEST;

}
#endif