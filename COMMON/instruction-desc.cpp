#include <string.h>
#include <instr_db.h>
#include <common.h>
#include <assert.h>
#include "instr-dat.h"

static int lookup_instr_desc_fast( unsigned  char * opcode,unsigned int bytes,struct instr_rec * table, int n_elements, prefix_t prefix,int start)
{
	int j;
	for (j=start; j<n_elements; j++) {
		if (opcode[0]<table[j].id1) break;
			if ((opcode[0] & table[j].id1mask) == table[j].id1)
			{
				if(table[j].id2mask!=0&& bytes<2) return -2;

				if( (opcode[1] & table[j].id2mask) == table[j].id2
				&& ((table[j].rm_constraints==Monly&&(opcode[1]&0xc0)!=0xc0)||(table[j].rm_constraints==Ronly&&(opcode[1]&0xc0)==0xc0)||table[j].rm_constraints==0)
				&& (!TEST_PREFIX(prefix,PREFIX_LOCK)|| (table[j].lockable==LOCKABLE_IF_M&&(opcode[1]&0xc0)!=0xc0)|| (table[j].lockable==LOCKABLE_IF_DIR0&&(opcode[1]&0xc0)!=0xc0)&&(opcode[0]&table[j].dir_mask)==0)
				&& (!TEST_PREFIX(prefix,PREFIX_REPZNZ)|| table[j].repeatable)) 
	
				return j;
			
			}
	}
	return -1;
}

static int lookup_instr_desc_ex_fast( unsigned  char * opcode,unsigned int bytes,struct instr_rec * table, int n_elements, prefix_t prefix,int start)
{
	int j;

	if (PREFIX_SEG(prefix) == NO_SEGOVR) prefix &= ~PREFIX_SEGS;

	for (j=start-1; j<n_elements; j++) {
		if (opcode[0]<table[j].id1) break;
			if ((opcode[0] & table[j].id1mask) == table[j].id1)
			{
				if(table[j].id2mask!=0&& bytes<2) return -2;

				if( (opcode[1] & table[j].id2mask) == table[j].id2
				&& (table[j].rm_constraints!=Monly||(opcode[1]&0xc0)!=0xc0)	
				&& (table[j].aux_prefix&prefix)==table[j].aux_prefix
				&& (prefix&(~table[j].opt_prefix )&( PREFIX_67 | PREFIX_SEGS)) == 0)
				 

				return j;
			}
	}
	return -1;
}

static int lookup_instr_desc( unsigned  char * opcode,unsigned int bytes,struct instr_rec * table, int n_elements, prefix_t prefix)
{
	int j;
	for (j=0; j<n_elements; j++) {
			if ((opcode[0] & table[j].id1mask) == table[j].id1)
			{
				if(table[j].id2mask!=0&& bytes<2) return -2;
				if( (opcode[1] & table[j].id2mask) == table[j].id2
				&& (table[j].rm_constraints!=Monly||(opcode[1]&0xc0)!=0xc0)
				&& (!TEST(prefix,PREFIX_LOCK)|| table[j].lockable)
				&& (!TEST(prefix,PREFIX_REPZNZ)|| table[j].repeatable)) 

				return j;
			}
	}
	return -1;
}


static int lookup_instr_desc_ex( unsigned  char * opcode,unsigned int bytes,struct instr_rec * table, int n_elements, prefix_t prefix)
{
	int j;
	if (PREFIX_SEG(prefix) == NO_SEGOVR) prefix &= ~PREFIX_SEGS;

	for (j=0; j<n_elements; j++) {
			if ((opcode[0] & table[j].id1mask) == table[j].id1)
			{
				if(table[j].id2mask!=0&& bytes<2) return -2;
				if( (opcode[1] & table[j].id2mask) == table[j].id2
				&& (table[j].rm_constraints!=Monly||(opcode[1]&0xc0)!=0xc0)	
				&& (table[j].aux_prefix&prefix)==table[j].aux_prefix
				&& (prefix&(~table[j].opt_prefix)&(PREFIX_67|PREFIX_SEGS))==0
				) 

				return j;
			}
	}
	return -1;
}
//
// processor does not recognize, but disassembler must reognize it.
// otherwise disassembler will choke
//
struct instr_rec  esc87=
{ "esc",  0xd8,  0xF8,  0x00,  0x00,     0,     0,        0,     fmt_esc,     esc,     disasm,   false,     false, 0x40,    0x00  };
	  
struct instr_rec *  get_instr_desc( unsigned char * code, unsigned int bytes,prefix_t prefix, bool &eof)
{
		struct instr_rec * p=NULL;
		int i;

		eof=false;

		if(code[0]==0x0f)
		{
			code ++;
			bytes--;
			if((i=lookup_instr_desc_fast(code, bytes,ext_instr_recs,
				sizeof (ext_instr_recs)/sizeof (ext_instr_recs[0]),prefix,ext_instr_index[code[0]]))>=0) 
				p=&ext_instr_recs[i];
			else 
			if((i=lookup_instr_desc_ex_fast(code, bytes,sse_instr_recs,
				sizeof (sse_instr_recs)/sizeof (sse_instr_recs[0]),prefix,sse_instr_index[code[0]]))>=0) 
				p=&sse_instr_recs[i];
		}
		else
			//
			// FWAIT is in 1byte table, it is not an esc insn
			//
		if((code[0]&0xf8)==0xd8)
		{
			if((i=lookup_instr_desc(code, bytes,x87_instr_recs,
				sizeof (x87_instr_recs)/sizeof (x87_instr_recs[0]),prefix))>=0) 
				p=&x87_instr_recs[i];
			else p=&esc87;
		}
		else 
		if((i=lookup_instr_desc_fast(code, bytes,instr_recs,
			sizeof (instr_recs)/sizeof (instr_recs[0]),prefix,instr_index[code[0]]))>=0) 
		{
			p=&instr_recs[i];
		}
		if(i==-2) eof=true;

		return p;
}
