#ifndef __INSTR_DB_H__
#define __INSTR_DB_H__





#define Monly 1
#define Ronly 2
#define LOCKABLE_NEVER   0
#define LOCKABLE_IF_M    1
#define LOCKABLE_IF_DIR0 2


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
fmt_dummy
};
#undef OPER_TYPE_REC
#define OPER_TYPE_REC(x) x,


enum OPERAND_TYPE {
#include "oper_type_def.h"
};


typedef unsigned char uchar;

struct instr_rec {
	const char *mnemonics;
	uchar id1;
	uchar id1mask;
	uchar id2;        /* if mod xxx r/m            */
	uchar id2mask;
	uchar w_mask;     /* 0 means no oprand size    */
	uchar dir_mask;   /* 0 means no direction      */
	uchar sext_mask;  /* 0 means no sign-extension */
	OP_FMT op_fmt;

	INSTRUTCION_INDEX index;	
	FUNIT funit;

	int lockable;
	bool repeatable;


	uchar len_decode_info;//fix bytes, rm?Iv?Ib?AdressV?
	uchar rm_constraints;
	int eflags_m;     // depending on flags
	int eflags_t;    // modify flags
	unsigned short   aux_prefix,opt_prefix;



};
#endif   /*    __INSTR_DB_H__ */