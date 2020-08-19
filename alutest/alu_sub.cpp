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
void test_sub8()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	printf("\nsub8:\n");
	unsigned char i,j,k1;
	unsigned int k;

	for (i=0;i<255;i++)
	{
	for (j=0;j<255;j++)
	{
		k=alu_sub(cflag,i,j,8);

		_asm
		{
			mov bh,i;
			mov bl,j;
			sub bh,bl
			mov k1,bh;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("sub8: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) return;
			printf("\n");
		}
	}
	//printf("\r%x",i);
	}
}
void test_sub16()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned short i,j,k1;
	unsigned int k;

	printf("\nsub16:\n");
	for (i=0;i<0xffff;i++)
	{
	for (j=0;j<0xffff;j++)
	{
		k=alu_sub(cflag,i,j,16);

		_asm
		{
			mov bx,i;
			mov cx,j;
			sub bx,cx;
			mov k1,bx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("sub16: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) return;
			printf("\n");
		}
	}
	//printf("\r%x",i);
	}
}

void test_sub32()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned long i,j,k1;
	unsigned int k;

	printf("\nsub32:\n");
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

			cflag=0;
			k=alu_sub(cflag,i,j,32);

			_asm
			{
				mov ebx,i;
				mov ecx,j;
				sub ebx,ecx;
				mov k1,ebx;
				pushf;
				pop cflag1;
			}

			cflag1&=~0x202; // 

			if ((k!=k1)
				||(cflag!=cflag1))
			{
				short err=(cflag^cflag1);


				printf("sub32: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
				chk(O,err)
				chk(C,err)
				chk(Z,err)
				chk(S,err)
				chk(P,err)
				chk(A,err) return;
				printf("\n");
			}
		}
	}
	//printf("\r%x",i);
	}
}
