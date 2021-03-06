// alutest.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "alucore.h"

#define MASK_Z 0x40
#define MASK_A 0x10
#define MASK_P 0x4
#define MASK_O 0x800
#define MASK_C 0x1 
#define MASK_S 0x80

#define chk(F,x) if ((x&MASK_##F )!=0) printf( #F "F ");
void test_adc8()
{
	unsigned short cflag; 
	unsigned short cflag1;


	unsigned char i,j,k1;
	unsigned int k;

	printf("\nadc8:\n");
	for (i=0;i<0xff;i++)
	{
	for (j=0;j<0xff;j++)
	{
		cflag=MASK_C;
		k=alu_adc(cflag,i,j,8);

		_asm
		{
			stc;
			mov al,j;
			mov bl,i;
			adc al,bl
			mov k1,al;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("adc8: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) return;
			printf("\n");
			return;
		}
	}
	//printf("\r%x",i);
	}
}
void test_adc16()
{
	unsigned short cflag; 
	unsigned short cflag1;


	unsigned short i,j,k1;
	unsigned int k;

	printf("\nadc16:\n");
	for (i=0;i<0xffff;i++)
	{
	for (j=0;j<0xffff;j++)
	{
		cflag=MASK_C;
		k=alu_adc(cflag,i,j,16);

		_asm
		{
			stc;
			mov ax,j;
			mov cx,i;
			adc ax,cx;
			mov k1,ax;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("adc16: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) return;
			printf("\n");
			return;
		}
	}
	//printf("\r%x",i);
	}
}

void test_adc32()
{
	unsigned short cflag; 
	unsigned short cflag1;


	unsigned long i,j,k1;
	unsigned int k;

	printf("\nadc32:\n");
	for (short i1=0;i1<0x100;i1++)
	{
	for (short j1=0;j1<0x100;j1++)
	{
		srand(time(NULL));
		for (short k3=0;k3<0x1000;k3++)
		{
			i=rand();
			j=rand();

			i&=~0xf000000f;
			j&=~0xf000000f;

			i|=(i1&0xf);
			j|=(j1&0xf);

			i|=(i1&0xf0)<<24;
			j|=(j1&0xf0)<<24;

		cflag=MASK_C;
		k=alu_adc(cflag,i,j,32);

		_asm
		{
			stc;
			mov ebx,j;
			mov ecx,i;
			adc ebx,ecx;
			mov k1,ebx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("adc32: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) return;
			printf("\n");
			return;
		}
	}
	}
	//printf("\r%x",i);
	}
}
