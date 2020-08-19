#ifndef __ALU_H__
#define __ALU_H__

class CALU
{
public:
	typedef enum alu_op_def 
	{
		ALU_ADD,
		ALU_ADC,
		ALU_SUB,
		ALU_SBB,
		ALU_SHR,
		ALU_SHL,
		ALU_SAR,
		ALU_SAL,
		ALU_ROR,
		ALU_ROL,
		ALU_RCR,
		ALU_RCL,
		ALU_NOT,
		ALU_NEG,
		ALU_XOR,
		ALU_OR,
		ALU_AND,
		ALU_CMP,
		ALU_CMPI,
		ALU_TEST,
		ALU_BT,
		ALU_BTS,
		ALU_BTR,
		ALU_BTC,
		ALU_BSF,
		ALU_BSR,
		ALU_INC,
		ALU_DEC,
		ALU_TOTAL
	} ALU_OP;

    
	
	static void Function (ALU_OP op, char &cflag, int src1,int src2, int &dst,int op_size);

private:
	static void (*pfnFunc[ALU_TOTAL])(char &cflag, int src1,int src2, int &dst,int op_size);

	static void ADD(char &cflag, int src1,int src2, int &dst,int op_size);
	static void ADC(char &cflag, int src1,int src2, int &dst,int op_size);
	static void SUB(char &cflag, int src1,int src2, int &dst,int op_size);
	static void SBB(char &cflag, int src1,int src2, int &dst,int op_size);
	static void SHR(char &cflag, int src1,int src2, int &dst,int op_size);
	static void SHL(char &cflag, int src1,int src2, int &dst,int op_size);
	static void SAR(char &cflag, int src1,int src2, int &dst,int op_size);
	static void SAL(char &cflag, int src1,int src2, int &dst,int op_size);
	static void ROR(char &cflag, int src1,int src2, int &dst,int op_size);
	static void ROL(char &cflag, int src1,int src2, int &dst,int op_size);
	static void RCR(char &cflag, int src1,int src2, int &dst,int op_size);
	static void RCL(char &cflag, int src1,int src2, int &dst,int op_size);
	static void NOT(char &cflag, int src1,int src2, int &dst,int op_size);
	static void NEG(char &cflag, int src1,int src2, int &dst,int op_size);
	static void XOR(char &cflag, int src1,int src2, int &dst,int op_size);
	static void OR (char &cflag, int src1,int src2, int &dst,int op_size);
	static void AND(char &cflag, int src1,int src2, int &dst,int op_size);
	static void CMP(char &cflag, int src1,int src2, int &dst,int op_size);
	static void CMPI(char &cflag, int src1,int src2, int &dst,int op_size);
	static void TEST(char &cflag, int src1,int src2, int &dst,int op_size);
	static void BT  (char &cflag, int src1,int src2, int &dst,int op_size);
	static void BTS (char &cflag, int src1,int src2, int &dst,int op_size);
	static void BTR (char &cflag, int src1,int src2, int &dst,int op_size);
	static void BTC (char &cflag, int src1,int src2, int &dst,int op_size);
	static void BSF (char &cflag, int src1,int src2, int &dst,int op_size);
	static void BSR (char &cflag, int src1,int src2, int &dst,int op_size);
	static void INC (char &cflag, int src1,int src2, int &dst,int op_size);
	static void DEC (char &cflag, int src1,int src2, int &dst,int op_size);

};


#endif   /*  __ALU_H__ */