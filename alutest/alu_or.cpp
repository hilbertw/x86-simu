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
void test_or8()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned char i,j,k1;
	unsigned int k;

	printf("\nor8:\n");
	for (i=0;i<255;i++)
	{
	for (j=0;j<255;j++)
	{
		k=alu_or(cflag,i,j,8);

		_asm
		{
			mov bh,j;
			mov bl,i;
			or bh,bl
			mov k1,bh;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("or8: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
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
void test_or16()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned short i,j,k1;
	unsigned int k;

	printf("\nor16:\n");
	for (i=0;i<0xffff;i++)
	{
	for (j=0;j<0xffff;j++)
	{
		k=alu_or(cflag,i,j,16);

		_asm
		{
			mov bx,j;
			mov cx,i;
			or bx,cx;
			mov k1,bx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("or16: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
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

void test_or32()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned long i,j,k1;
	unsigned int k;

	printf("\nor32:\n");
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

			i&=~0xff000000;
			j&=~0xff00000;

			i|=(i1)<<24;
			j|=(j1)<<24;

			cflag=0;
			k=alu_or(cflag,i,j,32);

			_asm
			{
				mov ebx,j;
				mov ecx,i;
				or ebx,ecx;
				mov k1,ebx;
				pushf;
				pop cflag1;
			}

			cflag1&=~0x212; // 

			if ((k!=k1)
				||(cflag!=cflag1))
			{
				short err=(cflag^cflag1);


				printf("or32: %x,%x %x,%x (%x+%x)--",k,k1,cflag,cflag1,i,j);
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

