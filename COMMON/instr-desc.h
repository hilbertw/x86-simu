#ifndef __INSTR_DESC_H__
#define __INSTR_DESC_H__


typedef unsigned char uchar;
enum OP_FMT;
enum INSTRUTCION_INDEX;
enum FUNIT;



#undef INSTR_REC
#define INSTR_REC(x) x,

enum INSTRUTCION_INDEX {
#include "instr_id_def.h"
unused
};

#undef FUNIT_REC
#define FUNIT_REC(x) x,

enum FUNIT {
#include "funit_def.h"
misc
};

#undef OPER_FMT_REC
#define OPER_FMT_REC(x) x,


enum OP_FMT {
#include "oper_fmt_def.h"
fmt_none
};
#undef OPER_TYPE_REC
#define OPER_TYPE_REC(x) x,


enum OPERAND_TYPE {
#include "oper_type_def.h"
op_type_none
};

#define Monly 1
#define Ronly 2

typedef enum instr_struct_type { STRUCT_GENERAL,STRUCT_BASIC,STRUCT_FPU,STRUCT_SSE}  INSTR_STRUCT_TYPE;

struct instr_struct_g {
	INSTR_STRUCT_TYPE struct_type;
	char *mnemonics;
	uchar id1;
	uchar id2;        /* if mod xxx r/m            */
	uchar id1_mask;
	uchar id_2mask;
	OP_FMT op_fmt;
	bool RMConstraint;
	INSTRUTCION_INDEX index;	
	FUNIT funit;
};

struct instr_struct_basic {
	struct instr_struct_g  common;

	uchar sext_mask;  /* 0 means no sign-extension */
	uchar w_mask;     /* 0 means no oprand size    */
	uchar dir_mask;   /* 0 means no direction      */
	bool lockable;
	bool repeatable;
};


 
#define	P66  1
#define	PF2  2
#define	PF3  4
#define	PREX 8


struct instr_rec_sse {
	struct instr_struct_g  common;
	uchar dir_mask;   /* 0 means no direction      */
	unsigned char  aux_prefix,opt_prefix;

};
// operand type
//
//a Two one-word operands in memory or two double-word operands in memory, depending
//on operand-size attribute (used only by the BOUND instruction).
//b Byte, regardless of operand-size attribute.
//bsq Byte, sign-extended to 64 bits.
//bss Byte, sign-extended to the size of stack pointer.
//c Byte or word, depending on operand-size attribute.
//d Doubleword, regardless of operand-size attribute.
//dq Double-quadword, regardless of operand-size attribute.
//ds Doubleword, sign-extended to 64 bits.
//p 32-bit or 48-bit pointer, depending on operand-size attribute.
//pi Quadword MMX technology register (for example, mm0)
//pd 128-bit packed double-precision floating-point data
//ps 128-bit packed single-precision floating-point data.
//pt 80-bit far pointer.
//
//q Quadword, regardless of operand-size attribute.
//qp Quadword, promoted by REX.W.
//s 6-byte pseudo-descriptor.
//sd Scalar element of a 128-bit packed double-precision floating data.
//ss Scalar element of a 128-bit packed single-precision floating data.
//si Doubleword integer register (e.g., eax)
//t 10-byte far pointer.
//v Word or doubleword, depending on operand-size attribute.
//vq Quadword (default) or word if 66H is used.
//vs Word or doubleword sign extended to the width of the stack pointer.
//w Word, regardless of operand-size attribute.
//
#endif // __INSTR_DESC_H__