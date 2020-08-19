#ifndef __UOP_H__
#define __UOP_H__


typedef int funit_t;
//
// functiom units:
//lsB:
//    rd/wr,
//    seg,
//    lock,
//    addr-reg
//    data-reg/imm
//
//
typedef struct  reg_value_def
{
	bool va;  // true if value is available
	union {
	int value;
	int reg;
	}u;
} RoV;

typedef union result_def
{
	int value;
	int reg;
} RandV;
//
// alu uop
//
typedef struct alu_uop_def {
	int dst_reg;
	RoV src1;
	RoV src2;
	RoV in_flag;
	int in_flag_reg;
} ALU_UOP;

//
// mult uop
//
typedef struct mult_uop_def {
	 //imul,idiv.mul,div
	int dst_hi_reg;
	int dst_lo_reg;
	RoV src1hi;
	RoV src1lo;
	RoV src2;
} MULT_UOP;
//
// addr uop
//
typedef struct addrg_uop_def {
	// 0~7-bit0/1/2 means src1/2/3 present or not
	int dst_reg;
	RoV src1;
	RoV src2;
	RoV src3;
} ADDRG_UOP;

//
// lsB uop
//
typedef struct lsB_uop_def {
	//ld/st
	RoV src_dst;
	RoV off;
	RoV seg;
} LSB_UOP;

//
// mrom uop
//
typedef struct mrom_uop_def {
	
	RoV src1;
	RoV src2;
	RoV src3;
} MROM_UOP;


typedef struct uop_def {

	int funit;
	int sub_op;
	int prefix;
	int op_size;//operand size
	RoV src[3];
	int dst;
	char desc[100];// dbg info
} UOP;




#endif   /*    __UOP_H__   */