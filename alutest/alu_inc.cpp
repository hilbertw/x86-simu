// alutest.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include "alucore.h"

#define MASK_Z 0x40
#define MASK_A 0x10
#define MASK_P 0x4
#define MASK_O 0x800
#define MASK_C 0x1 
#define MASK_S 0x80

#define chk(F,x) if ((x&MASK_##F )!=0) printf( #F "F ");
void test_inc8()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned char i,j,k1;
	unsigned int k;


	printf("\ninc8:\n");
	for (i=0;i<255;i++)
	{
		k=alu_inc(cflag,i,8);

		_asm
		{
			mov bh,i;
			inc bh
			mov k1,bh;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("inc8: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) return;
			printf("\n");
		}

	//printf("\r%x",i);
	}
}
void test_inc16()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned short i,j,k1;
	unsigned int k;

	printf("\ninc16:\n");
	for (i=0;i<0xffff;i++)
	{
		k=alu_inc(cflag,i,16);

		_asm
		{
			mov cx,i;
			inc cx;
			mov k1,cx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("inc16: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) return;
			printf("\n");
		}
	
	//printf("\r%x",i);
	}
}

void test_inc32()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned long i,j,k1;
	unsigned int k;

	printf("\ninc32:\n");
	for (i=0;i<0xffffffff;i++)
	{
		k=alu_inc(cflag,i,32);

		_asm
		{
			mov ecx,i;
			inc ecx;
			mov k1,ecx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("inc32: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) return;
			printf("\n");
		}
	
	//printf("\r%x",i);
	}
}
