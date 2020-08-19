#define _CRT_SECURE_NO_WARNINGS   1
#include "config.h"
#include "log.h"
#include<stdio.h>
#include<assert.h>
#include<stdarg.h>

#include "cpu.h"
extern CCPU cpu[CFG_N_CPUS];
extern int cpu_no;

FILE 	 * fp_log;
extern unsigned int dbg_ip;

bool log_init()
{
	fp_log=fopen(CFG_LOG_FPATH,"wt");
	return NULL!=fp_log;
}
void log_close()
{
	assert(fp_log!=NULL);
	if(fp_log) fclose(fp_log);
}
void log_cs_ip()
{
	assert(fp_log!=NULL);
	fprintf(fp_log,cpu[cpu_no].descriptors[0].B?"CPU:%x %x:%08x ":"CPU:%x %x:%04x ",cpu_no,cpu[cpu_no].reg_file.sreg[CS],cpu[cpu_no].reg_file.eip);
}

void print_cs_ip()
{
	printf(cpu[cpu_no].descriptors[0].B?"CPU:%x %x:%08x ":"CPU:%x %x:%04x ",cpu_no,cpu[cpu_no].reg_file.sreg[CS],cpu[cpu_no].reg_file.eip);
}
void log1(char *s)
{
	assert(fp_log!=NULL);

	log_cs_ip();
	fprintf(fp_log,"%s\n",s);
}
void log2(char *s,unsigned long x)
{
	assert(fp_log!=NULL);
	log_cs_ip();
	fprintf(fp_log,"%s:%x\n",s,x);
}
void log3(char *s,unsigned long x,unsigned long y)
{
	assert(fp_log!=NULL);
	log_cs_ip();
	fprintf(fp_log,"%s:%x %x\n",s,x,y);
}
void log5(char *s,char c,unsigned long x,unsigned long y,unsigned long z)
{
	assert(fp_log!=NULL);
	log_cs_ip();
	fprintf(fp_log,"%s:(%c)%x %x dev:%x\n",s,c,x,y,z);
	print_cs_ip();
	printf("%s:(%c)%x %x dev:%x\n",s,c,x,y,z);
}

void log4(char *s,char c,unsigned long x,unsigned long y)
{
	assert(fp_log!=NULL);
	log_cs_ip();
	fprintf(fp_log,"%s:(%c)%x %x\n",s,c,x,y);
	print_cs_ip();
	printf("%s:(%c)%x %x\n",s,c,x,y);
}

void log(char *s,...)
{
	va_list va;
	va_start(va,s);
	assert(fp_log!=NULL);
	log_cs_ip();
	vfprintf(fp_log,s,va);
	print_cs_ip();
	vprintf(s,va);
	va_end(va);
}

void log0(char *s,...)
{
	va_list va;
	va_start(va,s);
	assert(fp_log!=NULL);
	vfprintf(fp_log,s,va);
	vprintf(s,va);
	va_end(va);
}


