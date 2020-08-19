#define _CRT_SECURE_NO_WARNINGS   1

#include "log.h"
#include<stdio.h>
#include<assert.h>
#include<stdarg.h>


FILE 	 * fp_log;


bool log_init(const char * log)
{
	fp_log=fopen(log,"wt");
	return NULL!=fp_log;
}
void log_close()
{
	assert(fp_log!=NULL);
	if(fp_log) fclose(fp_log);
}

void log1(char *s)
{
	assert(fp_log!=NULL);
	fprintf(fp_log,"%s\n",s);
}
void log2(char *s,unsigned long x)
{
	assert(fp_log!=NULL);
	fprintf(fp_log,"%s:%x\n",s,x);
}
void log3(char *s,unsigned long x,unsigned long y)
{
	assert(fp_log!=NULL);
	fprintf(fp_log,"%s:%x %x\n",s,x,y);
}
void log5(char *s,char c,unsigned long x,unsigned long y,unsigned long z)
{
	assert(fp_log!=NULL);

	fprintf(fp_log,"%s:(%c)%x %x dev:%x\n",s,c,x,y,z);
//	printf("%s:(%c)%x %x dev:%x\n",s,c,x,y,z);
}

void log4(char *s,char c,unsigned long x,unsigned long y)
{
	assert(fp_log!=NULL);
;
	fprintf(fp_log,"%s:(%c)%x %x\n",s,c,x,y);
	//printf("%s:(%c)%x %x\n",s,c,x,y);
}

void log(char *s,...)
{
	va_list va;
	va_start(va,s);
	assert(fp_log!=NULL);

	vfprintf(fp_log,s,va);
	//vprintf(s,va);
	va_end(va);
}

void log0(char *s,...)
{
	va_list va;
	va_start(va,s);
	assert(fp_log!=NULL);
	vfprintf(fp_log,s,va);
	//vprintf(s,va);
	va_end(va);
}


