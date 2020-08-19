#define  _CRT_SECURE_NO_WARNINGS 1

#include <conio.h>
#include <stdio.h>
#include "cpu.h"
#include "util.h"
#include "history.h"
#include "log.h"
#include "8253.h"
#include "pci.h"
#include "irq-wire.h"



//#define DBG_LINUX
#define DBG_NT

void dump_acpi();
void dump_apic();
void dump_mp();
void dump_pci();
void dump_irq_rout();

void pre_warm_boot_8042();

#undef INSTR_REC
#define INSTR_REC(x) #x ,

char * instr_id[]=
{
#include "instr_id_def.h"
};
unsigned int state[sizeof(instr_id)/sizeof(instr_id[0])];

extern CMCH mch;
extern STATE_8253 timer[3];
extern CCPU cpu[CFG_N_CPUS];
int cpu_no; // used in  log.cpp,mch.cpp.current bus owner

bool vt_msg_loop_once();
bool sys_load_snapshot();
bool sys_save_snapshot();

#define MAX_CBP 10
#define MAX_DBP 10

#define PTR16(cs,off) (((cs)<<4)+(off&0xffff))
unsigned long code_bp[MAX_CBP] = {0x102};
unsigned long data_bp[MAX_DBP] = {PTR16(0x1210,0x140)};
int num_cbp = 0;
int num_dbp = 0;

int cpu_no_chk;

CIFetcherMMU disasm_fetcher;

CHistory history;
void dump_frame_list();
void dump_rq(unsigned long rq);
void dump_tsk( unsigned long addr,int level);
extern char * linux_scall_fmt[256];
void display_disasm(unsigned short seg,unsigned long base,unsigned long eip, int lines,bool dump_m=false)
{
	bool mode_16=(cpu[cpu_no].descriptors[CS].B==0||(cpu[cpu_no].cr[0]&1)==0||(cpu[cpu_no].reg_file.eflags&0x20000)==0x20000);


	disasm_fetcher.p_mmu=&cpu[cpu_no].mmu;
	disasm_fetcher.set_fetch_address(mode_16,base,0xffffffff,eip,false);
	for(int i=0;i<lines;i++)
	{
		char line[200];
		INSN_BASIC_INFO insn;

		if(disasm_fetcher.fetch(&insn)!=FETCH_SUCCESS) break;
		int pos=sprintf(line,(mode_16==false)?"%x:%08x ":"%x:%04x ",seg,insn.eip);
		pos+=disasm_line(line+pos,&insn);

		if(dump_m)
		{
			INSN_EXEC_INFO exec_info;
			predecode_insn( &insn,&exec_info);
			if(exec_info.r_m.type==2)
			{
				unsigned long temp=0,temp1,temp2;
				int data_size=(exec_info.flag&EXEC_F_WORD)?insn.data_size:8;
				cpu[cpu_no].gen_mem_addr(exec_info.r_m.operand_desc.mem,0x20,temp1,temp2);
				if(cpu[cpu_no].simulator_read_mem(exec_info.r_m.operand_desc.mem,temp,data_size,insn.addr_size))
					sprintf(line+pos-1,";(%x,%x)",temp2,temp);
			}
		}
		log0("%s\n",line);

	}
}
bool step=false,trace=false;
unsigned int next_stop_eip=-1;
unsigned int next_stop_cs;
int n_call,n_file;

void sys_invert(int what)
{
	if (what == 0) trace = !trace;
	else step = !step;
}
bool do_cpu()
{
	if((cpu[cpu_no_chk].halt==false)
		//&&cpu[cpu_no_chk].eip()>>16>0xf9ce
		)
	history.add_w_dup(cpu[cpu_no_chk].reg_file.sreg[CS],cpu[cpu_no_chk].eip());

	if(cpu[cpu_no_chk].eip()==next_stop_eip)
	{
		step=true;
	}
	/*
	if (cpu[cpu_no_chk].reg_file.sreg[CS] == 0x600
		&& cpu[cpu_no_chk].eip() == 0x102)
	{
		step = true;
	}
	*/
	//
	// code breakpoint
	//
	for(int k=0;k<num_cbp;k++)
		if(cpu[cpu_no_chk].eip()==code_bp[k])
			step=true;


	//if(cpu[cpu_no_chk].eip()==dbg_eip)
	//{
	//	char buffer[0x8000];
	//	sprintf(buffer,"f_%d_%d.bin",n_call,n_file);
	//	
	//	n_file ++;
	//	FILE *fp=fopen(buffer,"wb");
	//	if(fp)
	//	{
	//		cpu[cpu_no_chk].mmu.ld(dbg_mem,0,buffer,0x8000);
	//		fwrite(buffer,1,0x8000,fp);
	//		fclose(fp);
	//	}
	//	dbg_eip=0;

	//}
	//if(cpu[cpu_no_chk].eip()==0x8048080)
	//{
	//	step=true;
	//}
	//if(step==false&&(cpu[cpu_no_chk].eip()>>24)!=0)
	//{
	//	step=true;

	//	//history.add_w_dup(cpu[cpu_no_chk].reg_file.sreg[CS],cpu[cpu_no_chk].eip());
	//}

	if(_kbhit()&&_getch()==' ') step= true;

#if 0
	if(*(long *)((char *)mch.ram_map+0x101000)==0)
	{
		step=true;
	}

	if(cpu[cpu_no_chk].reg_file.sreg[CS]==0x60&&((cpu[cpu_no_chk].eip()>>12)==0xc0134))
	{
		//if((unsigned short )cpu[cpu_no_chk].reg_file.reg32[DI&7]>=0x310
		//	&& (unsigned short )cpu[cpu_no_chk].reg_file.reg32[DI&7]<0x400)

		step= true;
	}

	if(cpu[cpu_no_chk].reg_file.sreg[CS]==0xb8c //0x10df
		&&(cpu[cpu_no_chk].eip()==0x0583)//||cpu[cpu_no_chk].eip()==0x82f5||cpu[cpu_no_chk].eip()==0x7a6a)
		)
	{
		//if((unsigned short )cpu[cpu_no_chk].reg_file.reg32[DI&7]>=0x310
		//	&& (unsigned short )cpu[cpu_no_chk].reg_file.reg32[DI&7]<0x400)

		step= true;
	}

/*	else
		step=false;
	*/	
#endif

	if(trace||step)		
	{
l1:

		for(cpu_no=0;cpu_no<CFG_N_CPUS;cpu_no++)
		{
			if(cpu[cpu_no].halt) continue;

			log0("CPU%d:\n",cpu_no);
			cpu[cpu_no].dump_state();
			display_disasm(cpu[cpu_no].reg_file.sreg[CS],
				BASE(cpu[cpu_no].descriptors[CS]),
				cpu[cpu_no].eip(),1,true);
		}

	}
	cpu_no=cpu_no_chk;
	while(step)
	{
		char line [100];

		printf("\n-");

		gets_s(line,sizeof(line));

		if(line[0]=='t'||line[0]=='\0')	
		{
			break;//return true;
		}
		else if(line[0]=='z')	cpu[cpu_no_chk].revert_zf();
		else if(((short*)line)[0]==('g'|(','<<8)))
		{
			if(!htoiA(line+2,'\x0',8,next_stop_eip))
			{
				printf("\xa%x",next_stop_eip);
				continue;
			}
			step=false;
			break;
		}
		else if(((short*)line)[0]==('g'|('='<<8)))
		{

			if(!htoiA(line+2,'\x0',8,next_stop_eip))
			{
				printf("\xa%x",next_stop_eip);
				continue;;
			}
			int op_size=((cpu[cpu_no_chk].cr[0]&1)==0||(cpu[cpu_no_chk].reg_file.eflags&0x20000)!=0)?16:32;
			cpu[cpu_no_chk].near_abs_jump(next_stop_eip,op_size);
			//step=true;
			break;
		}
		else if(line[0]=='-') cpu[cpu_no_chk].revert_sf();
		else if(line[0]=='c') cpu[cpu_no_chk].revert_cf();
		else if(line[0]=='r') {trace=!trace;wprintf(L"trace:%s\n",trace?L"on":L"off");}
		else if(line[0]=='H'&&line[1]<'9'&&line[1]>='0') {cpu[line[1]-'0'].halt=true;}
		else if(line[0]=='A'&&line[1]<'9'&&line[1]>='0') {cpu[line[1]-'0'].halt=false;}
		else if(line[0]=='C'&&line[1]<'9'&&line[1]>='0') {cpu_no_chk=line[1]-'0';if(cpu_no_chk>10)cpu_no_chk-=7;}
		else if(line[0]=='B') {
			cpu[line[1]-'0'].halt=false;
			pre_warm_boot_8042();
			cpu[line[1]-'0'].lapic.init();
			cpu[line[1]-'0'].reset();
		}
		else if(line[0]=='n')
		{
			next_stop_eip=disasm_fetcher.get_fetch_eip();//cpu[cpu_no_chk].next_eip();
			step=false;
			break;
		}
		else if(line[0]=='e')
		{
			unsigned int  v,offset;
			int l;

	
			l=htoiA(line+1,' ',8,offset);
			if(l==0) continue;
			
	
			l=htoiA(line+l+1,'\0',8,v);
			if(l==0) continue;;

			cpu[cpu_no_chk].mmu.stdw(offset,0,v);
			continue;
		}
		else if(line[0]=='d')
		{
			unsigned int  seg,offset;
			int l;

			if(((cpu[cpu_no_chk].cr[0]&1)==0||(cpu[cpu_no_chk].reg_file.eflags&0x20000)!=0))
			{
				l=htoiA(line+1,':',4,seg);
				if(l==0) continue;;
			}
			else {seg=0; l=0;}

			l=htoiA(line+l+1,'\0',8,offset);
			if(l==0) continue;;

			cpu[cpu_no_chk].dump_mem(seg,offset,0x80);
			continue;;
		}
		else if(line[0]=='m')
		{
			unsigned int  offset;
			int l;

			l=htoiA(line+1,'\0',8,offset);
			if(l==0) continue;;

			print_mem32((char *)mch.ram_map+offset,offset,0x80);
			continue;;
		}
		else if(line[0]=='w')
		{
			unsigned int  offset,count;
			FILE * fp;
			char buffer[0x400];
			int l;

			l=htoiA(line+3,'\0',8,offset);
			if(l==0) continue;
			l=htoiA(line+4+l,'\0',8,count);
			if(l==0) continue;
			sprintf(line+2,".bin");

			fp=fopen(line,"wb");

			if(!fp) continue;
			for(;count>0;count-=0x400,offset+=0x400)
			{
				l=count>0x400?0x400:count;
				cpu[cpu_no_chk].mmu.ld(offset,0,buffer,l);
				fwrite(buffer,1,l,fp);

			}
			fclose(fp);
			continue;;
		}
		else if(line[0]=='v')
		{
			unsigned int  offset,a;
			int l;

			l=htoiA(line+1,'\0',8,offset);
			if(l==0) continue;;
			a=offset >>22;
			a<<=2;
			a+=(cpu[cpu_no_chk].cr[3]&~0xfff);
			log0("pde:%x %x\n",*(long*)((char *)mch.ram_map+a),a);
			a=*(long*)((char *)mch.ram_map+a);
			a&=~0xfff;
			a+=((offset>>12)&0x3ff)*4;

		   log0("pte:%x %x\n",*(long*)((char *)mch.ram_map+a),a);
			continue;;
		}
		else if(line[0]=='u')
		{
			unsigned int  base,offset,seg;
			int l;
			bool mode_16=((cpu[cpu_no_chk].cr[0]&1)==0||(cpu[cpu_no_chk].reg_file.eflags&0x20000)!=0);

			if(mode_16)
			{
				l=htoiA(line+1,':',4,seg);
				if(l==0) continue;;
				base=seg <<4;
			}
			else 
			{
				base=BASE(cpu[cpu_no_chk].descriptors[CS]);
				seg=cpu[cpu_no_chk].reg_file.sreg[CS];
				l=0;
			}

			l=htoiA(line+l+1,'\0',8,offset);
			if(l==0) continue;

			display_disasm(seg,base,offset,8);
		}
		else if(line[0]=='h') 
		{
			history.dump();
		}
		else if(line[0]=='o') 
		{
			int l;
			unsigned int j;
			l=htoiA(line+1,'\0',8,j);
			if(l==0) continue;

			if(j==0)
			{
				for( int i=0;i<sizeof(instr_id)/sizeof(instr_id[0]);i++)state[i]=0;
			}
			else
			{
				for( int i=0;i<sizeof(instr_id)/sizeof(instr_id[0]);i++)
					if(state[i]&&state[i]<j)log0("%s,%d\n",instr_id[i],state[i]);
			}
		}
		else if(line[0]=='p') {goto l1;}
		else if(line[0]=='s') {cpu[cpu_no_chk].dump_stack();}
		else if(line[0]=='1') 
		{
			unsigned int offset;
			int l=htoiA(line+1,'\0',8,offset);

			if(!l) goto l1;
			dump_tsk(offset,0);

		}
		else if(line[0]=='q') return false;
		else if(line[0]=='S') {
			if(sys_save_snapshot())
			{
				wprintf(L"Save state failed.\n");
			}
		}
		else if(line[0]=='L') {
			if(sys_load_snapshot())
			{
				wprintf(L"Reload state failed.\n");
			}
			else
			{
				for(cpu_no=0;cpu_no<CFG_N_CPUS;cpu_no++)
				cpu[cpu_no].sync_fetch_eip(cpu[cpu_no].eip());
			}
		}
		else if(line[0]=='T') 
		{
			timer[0].run=line[1]-'0';
			wprintf(L"Timer 0 :%s.\n",timer[0].run?L"On":L"Off");
            if(timer[0].run) assert_irq(0);
		}

		else if(line[0]=='D') 
		{
			if(line[1]=='g') cpu[cpu_no_chk].dump_gdt();
			else if(line[1]=='l') cpu[cpu_no_chk].dump_ldt();
			else if(line[1]=='i') cpu[cpu_no_chk].dump_idt();
			else if(line[1]=='t') cpu[cpu_no_chk].dump_tss();
			else if(line[1]=='a') dump_acpi();
			else if(line[1]=='A') dump_apic();
			else if(line[1]=='m') dump_mp();
			else if(line[1]=='p') {dump_pci();dump_irq_rout();}
			else if(line[1]=='u') dump_frame_list();
		}
		else if(line[0]=='O') 
		{
			unsigned int port,dat;
			int l=htoiA(line+1,' ',4,port);

			if(!l) goto l1;
			l=htoiA(line+1+l,'\0',2,dat);

			if(!l) goto l1;
			cpu[cpu_no_chk].biu.io_write_byte(port,dat);
			log0("%x:%x\n",port,(unsigned char)dat);
		}
		else if(line[0]=='I') 
		{
			unsigned int port,dat;
			int l=htoiA(line+1,'\0',4,port);

			if(!l) goto l1;
			dat=cpu[cpu_no_chk].biu.io_read_byte(port);
			log0("%x:%x\n",port,(unsigned char)dat);
		}
		else if(line[0]=='i') 
		{
			unsigned int dat;
			int l=htoiA(line+1,'\0',2,dat);

			if(!l) goto l1;
			assert_irq(dat);
			log0("assert irq:%x\n",(unsigned char)dat);
		}
		else if(line[0]=='b') 
		{
			unsigned int bp;
			int l=htoiA(line+2,'\0',8,bp);
			
			if(!l) goto l1;

			if( line[1]=='c')
			{
				if(num_cbp==MAX_CBP)
					log0("too many  code bp");
				else 
				log0("code bp %d:%x\n",num_cbp,bp);
				code_bp[num_cbp++]=bp;
			}
			else
			if( line[1]=='d')
			{	
				if(num_dbp==MAX_DBP)
					log0("too many  data bp");
				else 
				log0("data bp %d:%x\n",num_dbp,bp);
				data_bp[num_dbp++]=bp;
			}
		}
	}

	for(cpu_no=0;cpu_no<CFG_N_CPUS;cpu_no++)
	cpu[cpu_no].step();

	cpu_no=cpu_no_chk;
	state[cpu[cpu_no_chk].insn.p_instr_desc->index]++;

	//if(cpu[cpu_no_chk].insn.p_instr_desc->funit==fpu) 
	// 0xf9e41300,0xf9e43a22 f9f31300,f9f325b3

	if(0)//cpu[cpu_no_chk].eip()>>24>0xf9)
	{


			char line[200];

			int pos=sprintf(line,"%04x:%08x ",cpu[cpu_no_chk].reg_file.sreg[CS],cpu[cpu_no_chk].insn.eip);
			pos+=disasm_line(line+pos,&cpu[cpu_no_chk].insn);	

		   cpu[cpu_no_chk].dump_state();
		   log0("%s\n",line);
			if(cpu[cpu_no_chk].exec_info.r_m.type==2)
			{
				unsigned long temp1,temp2;
				cpu[cpu_no_chk].gen_mem_addr(cpu[cpu_no_chk].exec_info.r_m.operand_desc.mem,0x20,temp1,temp2);
				
				temp2+=BASE(cpu[cpu_no_chk].descriptors[cpu[cpu_no_chk].exec_info.r_m.operand_desc.mem.seg]);
				   cpu[cpu_no_chk].dump_mem(0,temp2,0x10);
			}
			log0("%x\n",(cpu[cpu_no_chk].fpu87.FPUStatusWord>>11)&7);



	}	

	//if(cpu[cpu_no_chk].insn.eip==0xC023ffc6&&cpu[cpu_no_chk].insn.code[0]==0x39)
	//{

	//	cpu[cpu_no_chk].reg_file.eflags&=~1;
	//	//cpu[cpu_no_chk].revert_sf();
	//}


	//if(cpu[cpu_no_chk].insn.eip==0xC023fff0
	//	||cpu[cpu_no_chk].insn.eip==0xc010ac59)
	//{

	//	cpu[cpu_no_chk].reg_file.eflags|=0x80;
	//}
	
	//if(cpu[0].insn.eip==0xC0106ff2
	//	&& cpu[0].insn.code[0]==0xf4
	//	&& cpu[0].intr==false)
	//{

	//	void assert_irq( int irq_no);
	//	assert_irq(0);
	//	//unsigned long x;
	//	//cpu[cpu_no_chk].mmu.lddw(0xc036cb24,0,&x);
	//	//x+=0x80;

	//	//cpu[cpu_no_chk].mmu.stdw(0xc036cb24,0,x);
	//}

	//if(cpu[cpu_no_chk].insn.eip==0xc01ab6d3&&cpu[cpu_no_chk].insn.code[0]==0x83&&*(unsigned short *)&cpu[cpu_no_chk].insn.code[1]==0xfffb)
	//{
	//	cpu[cpu_no_chk].revert_zf();
	//}


	//if (cpu[cpu_no_chk].eip()==0xc012434c)
	//{
	//	unsigned long p_timer=cpu[cpu_no_chk].MEM_READL(cpu[cpu_no_chk].reg_file.reg32[ESP]+4);
	//	unsigned long t=cpu[cpu_no_chk].MEM_READL(p_timer+8);
	//	unsigned long d=cpu[cpu_no_chk].MEM_READL(p_timer+0xc);
	//	unsigned long f=cpu[cpu_no_chk].MEM_READL(p_timer+0x10);
	//	log0("add tmer:%x %x (%x)-now:%x\n",t,f,d,cpu[cpu_no_chk].MEM_READL(0xc036cb24));


	//}
	//if (cpu[cpu_no_chk].eip()==0xc016a05c
	//	||cpu[cpu_no_chk].eip()==0xc0167b18)
	//{
	//	unsigned long name=cpu[cpu_no_chk].MEM_READL(cpu[cpu_no_chk].reg_file.reg32[ESP]+4);
	//	log0("proc entry\n");
	//	cpu[cpu_no_chk].dump_mem(0,name,0x20);
	//}
	

    if(cpu[cpu_no_chk].insn.p_instr_desc->index==intx)
	{




#ifdef DBG_LINUX
		if(0x80==cpu[cpu_no_chk].exec_info.imm)
		{
			unsigned char i=cpu[cpu_no_chk].reg_file.reg32[AX];
			char *s=linux_scall_fmt[i];

			log("%08x INT %02x (eax=%x,esp=%x)\n",
				cpu[cpu_no_chk].insn.eip,
				cpu[cpu_no_chk].exec_info.imm,
				cpu[cpu_no_chk].reg_file.reg32[EAX],
				cpu[cpu_no_chk].reg_file.reg32[ESP]);

			//if(i==0xb) step=true;

			if(s)
			log(s,
			
			cpu[cpu_no_chk].reg_file.reg32[EBX],
			cpu[cpu_no_chk].reg_file.reg32[ECX],
			cpu[cpu_no_chk].reg_file.reg32[EDX],
			cpu[cpu_no_chk].reg_file.reg32[ESI],
			cpu[cpu_no_chk].reg_file.reg32[EDI]
			);
#if 0
			// sys read(stdout)
			if(3==(char)cpu[cpu_no_chk].reg_file.reg32[0])
			{
			   //dbg_len=cpu[cpu_no_chk].reg_file.reg32[EDX];
				//dbg_mem=cpu[cpu_no_chk].reg_file.reg32[ECX];
				//dbg_eip=cpu[cpu_no_chk].insn.eip+2;
			}

#endif
			// sys write(stdout)
			if(4==(char)cpu[cpu_no_chk].reg_file.reg32[0])
			{
				if(cpu[cpu_no_chk].reg_file.reg32[EDX]<0x200)
					cpu[cpu_no_chk].dump_mem(0,cpu[cpu_no_chk].reg_file.reg32[ECX],cpu[cpu_no_chk].reg_file.reg32[EDX]);
			}

			
			if(5==(unsigned char)cpu[cpu_no_chk].reg_file.reg32[0]// sys open
			||0x7f==(unsigned char)cpu[cpu_no_chk].reg_file.reg32[0]// create_module/init_module
			||0x80==(unsigned char)cpu[cpu_no_chk].reg_file.reg32[0]
			||0x0b==(unsigned char)cpu[cpu_no_chk].reg_file.reg32[0] // sys_execv
			||0xc3==(unsigned char)cpu[cpu_no_chk].reg_file.reg32[0])// stats64
			{
				cpu[cpu_no_chk].dump_mem(0,cpu[cpu_no_chk].reg_file.reg32[EBX],0x20);
			}

			//dump_tsk(cpu[cpu_no_chk].reg_file.reg32[ESP],false);
			// sys_waitpid
		//dump_tsk(cpu[cpu_no_chk].reg_file.reg32[ESP],0);
		}
#else
		if(0x20<cpu[cpu_no_chk].exec_info.imm
			&&0x30>cpu[cpu_no_chk].exec_info.imm)
		{
			log("%08x INT %02x (eax=%x,esp=%x)\n",
				cpu[cpu_no_chk].insn.eip,
				cpu[cpu_no_chk].exec_info.imm,
				cpu[cpu_no_chk].reg_file.reg32[EAX],
				cpu[cpu_no_chk].reg_file.reg32[ESP]);
		}

#endif
	}

#ifdef DBG_NT
	if (cpu[cpu_no_chk].eip()==0x8089F093)
	{
		log0("unicode string:\n");
		cpu[cpu_no_chk].dump_mem(0,cpu[cpu_no_chk].reg_file.reg32[EDX],0x80);
	}

	if (cpu[cpu_no_chk].eip()==0x808A2503)
	{
		log0("CreateSystemThread(%x): %x,%x\n",cpu[cpu_no_chk].reg_file.reg32[0],
			cpu[cpu_no_chk].MEM_READL(cpu[cpu_no_chk].reg_file.reg32[EBP]+0x2C),
			cpu[cpu_no_chk].MEM_READL(cpu[cpu_no_chk].reg_file.reg32[EBP]+0x30));

	}

	//if(cpu[cpu_no_chk].reg_file.sreg[CS]==0x2000)
	//	step=true;
	//if(cpu[cpu_no_chk].reg_file.eflags&0x20000)
	//	step=true;

	//if(cpu[cpu_no_chk].eip()==0x80809BDF
	//	||cpu[cpu_no_chk].eip()==0x804dcb37)
	//{
	//	cpu[cpu_no_chk].lapic.mark_vector(0xd1);
	//}

	//
	//80810556:ZwCreateFile
	//
	//FileHandle      = dword ptr  4
	//DesiredAccess   = dword ptr  8
	//ObjectAttributes= dword ptr  0Ch
	//IoStatusBlock   = dword ptr  10h
	//AllocationSize  = dword ptr  14h
	//FileAttributes  = dword ptr  18h
	//ShareAccess     = dword ptr  1Ch
	//CreateDisposition= dword ptr  20h
	//CreateOptions   = dword ptr  24h
	//EaBuffer        = dword ptr  28h
	//EaLength        = dword ptr  2Ch

	if (cpu[cpu_no_chk].eip()==0x80810556)
	{
		log0("ZwCreateFile\n");

		 unsigned long ObjectAttributes=cpu[cpu_no_chk].MEM_READL(cpu[cpu_no_chk].reg_file.reg32[ESP]+0x2C);
         unsigned long ObjectName=cpu[cpu_no_chk].MEM_READL(ObjectAttributes+0xc);
		cpu[cpu_no_chk].dump_mem(0,ObjectName,0x80);

	}

#endif

	//if(cpu[cpu_no_chk].insn.eip==0xf9ce4f71)
	//	step=true;

	//
	// raise irq0 to pull back from hlt
	//
    if(cpu[cpu_no_chk].insn.p_instr_desc->index==hlt
		&& !cpu[cpu_no_chk].intr) 
	{
		assert_irq(0);
	}

#if 0
	//
	// data breakpoint
	//
	if(cpu[cpu_no_chk].exec_info.r_m.type==2)
	{
		unsigned long temp1,temp2;
		cpu[cpu_no_chk].gen_mem_addr(cpu[cpu_no_chk].exec_info.r_m.operand_desc.mem,0x20,temp1,temp2);
		
		temp2+=BASE(cpu[cpu_no_chk].descriptors[cpu[cpu_no_chk].exec_info.r_m.operand_desc.mem.seg]);
		int k;

		for(k=0;k<num_dbp;k++)
		if( temp2>=data_bp[k]&&temp2 <data_bp[k]+cpu[cpu_no_chk].insn.data_size/8)
			break;

		if(k!=num_dbp)
		{
			char line[200];

			int pos=sprintf(line,"%08x ",cpu[cpu_no_chk].insn.eip);
			pos+=disasm_line(line+pos,&cpu[cpu_no_chk].insn);	
			if(cpu[cpu_no_chk].simulator_read_mem(cpu[cpu_no_chk].exec_info.r_m.operand_desc.mem,temp1,0x20,cpu[cpu_no_chk].insn.addr_size))
				sprintf(line+pos-1,";(%x,%x)",temp2,temp1);

		   cpu[cpu_no_chk].dump_state();
		   log0("%s*\n",line);
			step=true;

		}
	}
	if (cpu[cpu_no_chk].insn.p_instr_desc->index==movs)
	{
		unsigned long temp1, temp2, temp3;
		temp2 = cpu[cpu_no_chk].insn.addr_size==0x20?cpu[cpu_no_chk].reg_file.reg32[ESI]:
			cpu[cpu_no_chk].reg_file.reg32[ESI]&0xffff;
		temp1 = cpu[cpu_no_chk].insn.addr_size == 0x20 ? cpu[cpu_no_chk].reg_file.reg32[EDI] :
			cpu[cpu_no_chk].reg_file.reg32[EDI] & 0xffff;
		if (cpu[cpu_no_chk].insn.prefix&PREFIX_REPZNZ)
			temp3 = cpu[cpu_no_chk].insn.addr_size == 0x20 ? cpu[cpu_no_chk].reg_file.reg32[ECX] :
			cpu[cpu_no_chk].reg_file.reg32[ECX] & 0xffff;
		else
			temp3 = 1;

		temp1 += BASE(cpu[cpu_no_chk].descriptors[ES]);
		temp2 += BASE(cpu[cpu_no_chk].descriptors[DS]);
		int k;

		for (k = 0; k < num_dbp; k++)
		{
			if (temp1 >= data_bp[k] && temp1 < data_bp[k] + temp3)
				break;
			if (temp2 >= data_bp[k] && temp2 < data_bp[k] + temp3)
				break;
		}

		if (k != num_dbp)
		{
			cpu[cpu_no_chk].dump_state();

			step = true;
		}

	}
#endif
	return true;
}
