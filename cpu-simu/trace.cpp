#define _CRT_SECURE_NO_DEPRECATE 1
#include <stdio.h>
#include <conio.h>
#include "cpu.h"
#include "util.h"

void CCPU::dump_insn(INSN_BASIC_INFO *p_insn)
{
	unsigned char line[400];


	reg_file.print((char *)line,0);
	printf("%s\n",line);

			//pos=sprintf(line,mode_16?"%04X:%04X ":"%04X:%08X ",___CS___,__EIP__);
	int pos=sprintf((char *)line,"%04X:%04X  %08X ",___CS___,p_insn->eip,p_insn->eip+BASE(cs_desc));

	disasm_line((char *)line+pos,p_insn);
	printf("%s\n",line);

	//
	// print the rest bytes
	//
	int i;
	for(i=8;i<p_insn->len+p_insn->prefix_bytes;i++)
	{
		pos+=sprintf((char *)line+pos," %02X",p_insn->code[i]);
	}
	if(i>8)
	printf("%s\n",line);
}