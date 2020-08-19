#ifndef __log_h__
#define __log_h__
#include <stdio.h>
bool log_init();
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

#define LOG_CMOS   1
#define LOG_PCI    1
#define LOG_VGA    0
#define LOG_CGA    0
#define LOG_MDA    0
#define LOG_FLOPPY 0
#define LOG_IDE    0
#define LOG_SIO    1
#define LOG_SMBUS  1
#define LOG_GPIO   1
#define LOG_INT    0
#define LOG_8237   0
#define LOG_8253   0
#define LOG_8042   0
#define LOG_8259   0
#define LOG_DMA    0
#define LOG_ACPI   0
#define LOG_APIC   0 
#define LOG_PORT80 0
#define LOG_IO     0
#define LOG_USB    1
#define LOG_APM    0
#define LOG_PORT92 0
#define LOG_8042_OPORT 0
#define LOG_A20     0
#define LOG_ATAPI   0
#define LOG_ISAPNP  0
#define LOG_MSR     0
#define LOG_TLB     0
#endif //__log_h__