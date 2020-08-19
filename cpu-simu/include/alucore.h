#ifndef  __ALUCORE_H__
#define  __ALUCORE_H__


//extern "C"
//{

unsigned int alu_add(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_adc(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);

unsigned int alu_inc(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int alu_dec(unsigned short & cflag, unsigned int data1, int op_size);

unsigned int alu_sub(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_subb(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_and(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_xor(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_or(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
//void alu_test(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
//void alu_cmp(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int  alu_neg(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int  alu_not(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int alu_sal(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_sar(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_shr(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_rcl(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_rcr(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_rol(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_ror(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
/*
unsigned int alu_rcl1(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int alu_rcr1(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int alu_sar1(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int   alu_ror1(unsigned short  &cflag, unsigned int src1, int op_size) ;
unsigned int   alu_rol1(unsigned short  &cflag, unsigned int src1, int op_size) ;
unsigned int alu_shr1(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int alu_sal1(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int alu_sar1(unsigned short & cflag, unsigned int data1, int op_size);
*/
unsigned int alu_shld(unsigned short & cflag, unsigned int rm,unsigned long reg, unsigned int count,int op_size);
unsigned int alu_shrd(unsigned short & cflag, unsigned int rm,unsigned long reg, unsigned int count,int op_size);
unsigned int alu_btr(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_bts(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);


unsigned int alu_btc(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);
unsigned int alu_bsr(unsigned short & cflag, unsigned int data1, int op_size);
unsigned int alu_bsf(unsigned short & cflag, unsigned int data1, int op_size);
void alu_bt(unsigned short & cflag, unsigned int data1,unsigned int data2, int op_size);


unsigned int alu_SETO(unsigned short cflag);
unsigned int alu_SETNO(unsigned short cflag);
unsigned int alu_SETB(unsigned short cflag);
unsigned int alu_SETNB(unsigned short cflag);
unsigned int alu_SETE(unsigned short cflag);
unsigned int alu_SETNE(unsigned short cflag);
unsigned int alu_SETNA(unsigned short cflag);
unsigned int alu_SETA(unsigned short cflag);
unsigned int alu_SETS(unsigned short cflag);
unsigned int alu_SETNS(unsigned short cflag);
unsigned int alu_SETPE(unsigned short cflag);
unsigned int alu_SETPO(unsigned short cflag);
unsigned int alu_SETL(unsigned short cflag);
unsigned int alu_SETNL(unsigned short cflag);
unsigned int alu_SETNG(unsigned short cflag);
unsigned int alu_SETG(unsigned short cflag);





void alu_cmc(unsigned short &cflag);
void alu_clc(unsigned short &cflag);
void alu_stc(unsigned short& cflag);
void alu_cld(unsigned short &cflag);
void alu_std_(unsigned short& cflag);
void alu_cli(unsigned short &cflag);
void alu_sti(unsigned short &cflag);
unsigned int alu_movsx(unsigned short  cflag, unsigned int data1, int op_size);
unsigned int alu_movzx(unsigned short  cflag, unsigned int data1, int op_size);

//}

#endif /* __ALUCORE_H__*/


