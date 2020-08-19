#ifndef MULT_H__
#define MULT_H__

class CMultiplier
{
public:
	typedef enum mul_op_def
	{
		MUL_MUL,
		MUL_IMUL,
		MUL_DIV,
		MUL_IDIV
	} MUL_OP;

	static void Function (ALU_OP op, char &cflag, int src1,int src2, int &dst1,int &dst2,int op_size);

private:
	static void MUL (char &cflag, int src1,int src2, int &dst1,int &dst2,int op_size);
	static void IMUL(char &cflag, int src1,int src2, int &dst1,int &dst2,int op_size);
	static void DIV (char &cflag, int src1,int src2, int &dst1,int &dst2,int op_size);
	static void IDIV(char &cflag, int src1,int src2, int &dst1,int &dst2,int op_size);
};


#endif  /* MULT_H__*/