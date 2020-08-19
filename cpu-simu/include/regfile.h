#ifndef __REGFILE_H__
#define __REGFILE_H__

#include "common.h"

class CREGFILE {
public:
	unsigned int  reg32[8];
	unsigned short sreg[8];
	unsigned int  eip,eflags;

	union {unsigned char b[16];unsigned int l[4];unsigned short s[8]; float f[8];double d[4];}mmx_reg[8];

	union {unsigned char b[16];unsigned int l[4];unsigned short s[8]; float f[8];double d[4];}xmm_reg[8];

	CREGFILE(){}

	unsigned int read_reg(REG_CODE reg);
	void write_reg(REG_CODE reg,unsigned int data);
	void print(char buffer[],bool mode);

	unsigned char get_reg8(int regno);	
	unsigned short get_reg16(int regno);
	unsigned int get_reg32(int regno);
	void set_reg8(int regno,unsigned char data);		
	void set_reg16(int regno,unsigned short data);
	void set_reg32(int regno,unsigned int data);

};

#endif   /*    __REGFILE_H__   */