#ifndef __log_h__
#define __log_h__
#include <stdio.h>
bool log_init(const char *log);
void log_close();
void log1(char *s);
void log2(char *s,unsigned long x);
void log3(char *s,unsigned long x,unsigned long y);
void log4(char *s,char c,unsigned long x,unsigned long y);
void log5(char *s,char c,unsigned long x,unsigned long y,unsigned long z);
void log(char *s,...);
void log0(char *s,...);
extern FILE 	 * fp_log;
#define LOG1(x)    fprintf(fp_log,x)
#define LOG2(x,y)    fprintf(fp_log,x,y)
#define LOG3(x,y,z)    fprintf(fp_log,x,y,z)
#define LOG4(x,y,z,a)    fprintf(fp_log,x,y,z,a)
#define LOG5(x,y,z,a,b)    fprintf(fp_log,x,y,z,a,b)


#endif //__log_h__