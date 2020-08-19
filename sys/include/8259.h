#ifndef __8259_h__
#define __8259_h__

#pragma pack(push,1)
typedef struct icw1_8259_def
{
	unsigned char icw4:1;
	unsigned char sngl:1;//single, cascade mode?
	unsigned char adi:1;
	unsigned char ltim:1;
	unsigned char one:1;
	unsigned char a567:3;
}ICW1_8259;

typedef struct icw2_8259_def
{
	unsigned char a8910:3;
	unsigned char t3_7:4;
	unsigned char bit7:1;
}ICW2_8259;

typedef unsigned char ICW3_8259M; // master:has a slave on the pin?, slave:id:0-7

typedef struct icw3_8259_slave_def
{
	unsigned char id:3;
}ICW3_8259S;


typedef struct icw4_8259_def
{
	unsigned char upm:1;// 8086/8085 mode?
	unsigned char aeoi:1;// auto eoi
	unsigned char m_s:1;
	unsigned char buffered:1;
	unsigned char sfnum:1;// special function mofr
}ICW4_8259;

typedef  unsigned char OCW1_8259;

typedef struct ocw2_8259_def
{
	unsigned char l:3;
	unsigned char ocw_sel:2;  // 00
	unsigned char eoi:1;
	unsigned char sl:3;
	unsigned char r:1;
}OCW2_8259;

typedef struct ocw3_8259_def
{
	unsigned char ris:1;
	unsigned char rr:1;
	unsigned char p:1;
	unsigned char ocw_sel:2;  // 00
	unsigned char sm:1;
	unsigned char esm:1;
	unsigned char bit7:1;
}OCW3_8259;


typedef struct regs_8259
{
	union
	{
		struct regs
		{
			ICW1_8259 icw1;
			ICW2_8259 icw2;
			union {
				ICW3_8259M master;
				ICW3_8259S  slave;
			}icw3;
			ICW4_8259 icw4;
		} reg;
		unsigned char icw[4];
	};

	OCW1_8259 ocw1;
	OCW2_8259 ocw2;
	OCW3_8259 ocw3;
	unsigned char irr,isr;
	unsigned char state;
} REGS_8259; 

#pragma pack(pop)


void _8259_assert_irq(int controller, int irq_no);
void _8259_deassert_irq(int controller, int irq_no);

void eoi_8259(int controller_no);

#endif //__8259_h__
