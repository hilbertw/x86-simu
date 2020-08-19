#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "cpu.h"
#include "log.h"
#pragma warning (disable :4018)
extern bool step;
void CCPU::dump_mem(unsigned short seg,unsigned long off,unsigned long _size)
{
	unsigned char _start[16];
	char szBuffer[1024];


	long j,k,x,address;
	unsigned char c;


	x=off&0xf;
	off-=x;

	if(real_v86()) address=seg*16+off;
	else address=off;


	while(_size)
	{
		char *s=szBuffer;

		s +=sprintf(s, "%04x:%04x ",seg,off );

		// x: first byte to display
		// k: last bytes to display in this line
		if(_size<(16-x)) k=x+_size; else k=16;	
		
		if(mmu.ld(address,0,(char *)_start,16)) 
		{
			s+=sprintf(s,"Page fault");
			_size=0;
			goto l1;
		}
		for( j=0;j<x ;j++ ) s+=sprintf(s,"   ");

		for( ;j<k;j++) {

			s+=sprintf(s,"%02x%c",((unsigned char *)_start)[j],(j==7)?'-':' ');
			
		}

		for( ;j<16 ;j++ ) s+=sprintf(s,"   ");

		*s++=' ';
		for( j=0;j<x ;j++ ) *s++=' ';
		for( ;j<k;j++) 
		{
			c=((unsigned char *)_start)[j];
			s+=sprintf(s,"%c",c<' '?'.':(c<0x80?c:'.'));
		}



		k-=x;
		_size-=k;
		address+=16;
		off+=16;
		x=0;

l1:			
		log0( "%s\n",szBuffer );

	} 
}

void CCPU::dump_stack()
{
	unsigned short seg=___SS___;
	unsigned long off=BIG_SEGMENT(descriptors[SS])?__ESP__:__SP__;


	dump_mem(seg,off,0x80);
}

void CCPU::dump_int(unsigned int  n)
{
#if 0
	if (__PE__ == 0 || __VM__ == 1)
	{

		if (n != 0x8 && n != 0x1c && n != 0x16 && n != 0x2a && n != 0x28)
		{
			long w = MEM_READL(n * 4);


			log("INT %x->%x( AX=%x BX=%x CX=%x DX=%x)\n", n, w, __AX__, __BX__, __CX__, __DX__);
		}

		if (n == 0x21)
		{

			if (__AH__ == 9 || __AH__ == 0x3c || __AH__ == 0x3d)
			{
				log("%x:%x INT %x/AH=%x\n", ___CS___, __EIP__, n, __AH__);
				dump_mem(___DS___, __DX__, 0x20);
			}
			//else if(__AH__==2)
			//	log("%x:%x INT %x/AH=%x  DL=%x(%c):\n",___CS___,__EIP__,n,__AH__,__DL__,__DL__);
			//else

		}

		//else if(n==0x13)
		//{
		//	if(__AH__==2)
		//log("%x:%x INT %x/AH=%x AL=%x ES=%x BX=%x CX=%x DX=%x\n",___CS___,__EIP__,n,__AH__,__AL__,___ES___,__BX__,__CX__,__DX__);
		//}
		//else

		if (n == 0x10)
		{
			log("INT %x/AH=%x  AL=%x(%c):\n", n, __AH__, __AL__, __AL__ >= ' ' ? __AL__ : '.');
		}


		//else
		// if(/*n==0x13||n==0x10&&__AX__==0x12*/)
	{
		::step = true;
	}

	//}else 

	if (__PE__ == 1 && __VM__ == 0)
	{
		unsigned long esp0;
		mmu.lddw(BASE(tr_desc) + OFFSET(TSS, esp0), 0, &esp0);
		load_descriptor(idt_base, &tmp_desc1, n);
		log("INT %x: %x %x->%x:%x,EAX=%x esp0=%x esp=%x\n", n, DWORD0_OF(tmp_desc1),
			DWORD1_OF(tmp_desc1),
			WORD1_OF(tmp_desc1), GATE_OFFSET(tmp_desc1), reg_file.reg32[0], esp0, __ESP__);

	}
	else
	{
		log("INT %x \n", n);
		//::step=true;
	}
	}
#endif

}

void CCPU::dump_state()
{
	char line[500];


	bool mode_16=cs_desc.B==0;//__PE__ == 0 || (__PE__ == 1 && __VM__ == 1);
	reg_file.print(line,0);
	log0("%s",line);  // eip from insn.eip
}


void CCPU::dump_disasm()
{
	int pos;
	char line[500];
	bool mode_16=cs_desc.B==0;//__PE__ == 0 || (__PE__ == 1 && __VM__ == 1);
	if(mode_16)
		pos=sprintf(line,"%04X:%04X %08X ",___CS___,insn.eip,insn.eip+BASE(cs_desc));
	else
		pos=sprintf(line,"%04X:%08X %08X ",___CS___,insn.eip,insn.eip+BASE(cs_desc));

	pos+=disasm_line(line+pos,&insn);

	if(exec_info.r_m.type==2)
	{
		unsigned long temp=0,temp1,temp2;
		int data_size=(exec_info.flag&EXEC_F_WORD)?insn.data_size:8;

		gen_mem_addr(exec_info.r_m.operand_desc.mem,0x20,temp1,temp2);

		if(simulator_read_mem(exec_info.r_m.operand_desc.mem,temp,data_size,insn.addr_size))
			sprintf(line+pos-1,";(%x,%x)",temp2,temp);
	}
	log0("%s\n",line);
	//printf("%s\n",line);
	
}


void CCPU::dump_gdt()
{
	unsigned short s;

	log0("Gdt:%x,%x\n",gdt_base,gdt_limit);

	for(s=0;s<(gdt_limit+1)/8;s++)
	{
		load_descriptor(gdt_base,&tmp_desc1,s);
		log0("%x %x %x- %x:%x (%x,%x)\n",
			s*8,
			DWORD0_OF(tmp_desc1),
			DWORD1_OF(tmp_desc1),
			BASE(tmp_desc1),
			LIMIT(tmp_desc1),
			BYTE5_OF(tmp_desc1),
			BYTE6_OF(tmp_desc1)>>4
			);
	}

}
void CCPU::dump_ldt()
{
	unsigned short s;

	log0("Ldt %x:%x,%x\n",ldt_seg,ldt_base,ldt_limit);

	for(s=0;s<(ldt_limit+1)/8;s++)
	{
		load_descriptor(ldt_base,&tmp_desc1,s);
		log0("%x %x %x- %x:%x (%x,%x)\n",
			s*8,
			DWORD0_OF(tmp_desc1),
			DWORD1_OF(tmp_desc1),
			BASE(tmp_desc1),
			LIMIT(tmp_desc1),
			BYTE5_OF(tmp_desc1),
			BYTE6_OF(tmp_desc1)>>4
			);
	}
}
void CCPU::dump_idt()
{
	unsigned short s;

	log0("Idt:%x,%x\n",idt_base,idt_limit);

	for(s=0;s<(idt_limit+1)/8;s++)
	{
		load_descriptor(idt_base,&tmp_desc1,s);
		log0("%x %x %x - %x:%x\n",
			s,
			DWORD0_OF(tmp_desc1),
			DWORD1_OF(tmp_desc1),
			GATE_SELECTOR(tmp_desc1),
			GATE_OFFSET(tmp_desc1)
			);
	}
}

void CCPU::dump_tss()
{
	unsigned int tss_base=BASE(tr_desc);
	unsigned int tss_limit=LIMIT(tr_desc);

	log0("Tss:%x,%x\n",tss_base,tss_limit);
	log0("I/O Map Base Address:%x\n",MEM_READW(0x66+tss_base));
	log0("LDT Segment Selector:%x\n",MEM_READW(96+tss_base));
	log0("GS:%x\n",MEM_READW(92+tss_base));
	log0("FS:%x\n",MEM_READW(88+tss_base));
	log0("DS:%x\n",MEM_READW(84+tss_base));
	log0("SS:%x\n",MEM_READW(80+tss_base));
	log0("CS:%x\n",MEM_READW(76+tss_base));
	log0("ES:%x\n",MEM_READW(72+tss_base));
	log0("EDI:%x\n",MEM_READL(68+tss_base));
	log0("ESI:%x\n",MEM_READL(64+tss_base));
	log0("EBP:%x\n",MEM_READL(60+tss_base));
	log0("ESP:%x\n",MEM_READL(56+tss_base));
	log0("EBX:%x\n",MEM_READL(52+tss_base));
	log0("EDX:%x\n",MEM_READL(48+tss_base));
	log0("ECX:%x\n",MEM_READL(44+tss_base));
	log0("EAX:%x\n",MEM_READL(40+tss_base));
	log0("EFLAGS:%x\n",MEM_READL(36+tss_base));
	log0("EIP:%x\n",MEM_READL(32+tss_base));
	log0("CR3 (PDBR):%x\n",MEM_READL(28+tss_base));
	log0("SS2:%x\n",MEM_READW(24+tss_base));
	log0("ESP2:%x\n",MEM_READL(20+tss_base));
	log0("SS1:%x\n",MEM_READW(16+tss_base));
	log0("ESP1:%x\n",MEM_READL(12+tss_base));
	log0("SS0:%x\n",MEM_READW(8+tss_base));
	log0("ESP0:%x\n",MEM_READL(4+tss_base));
	log0("Previous Task Link:%x\n",MEM_READW(0+tss_base));
}