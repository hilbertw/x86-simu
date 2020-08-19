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
void test_not8()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned char i,j,k1;
	unsigned char  k;


	printf("\nnot8:\n");
	for (i=0;i<255;i++)
	{
		k=(unsigned char )alu_not(cflag,i,8);

		_asm
		{
			xor ah,ah
			sahf;
			mov bh,i;
			not bh
			mov k1,bh;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("not8: %x,%x %x,%x (not %x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) //return;
			printf("\n");
		}
	//printf("\r%x",i);
	}
}
void test_not16()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned short i,j,k1;
	unsigned short  k;

	printf("\nnot16:\n");
	for (i=0;i<0xffff;i++)
	{
		cflag=0; 
		k=(unsigned short )alu_not(cflag,i,16);

		_asm
		{
			xor ah,ah
			sahf;
			mov cx,i;
			not cx;
			mov k1,cx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("not16: %x,%x %x,%x (not %x)--",k,k1,cflag,cflag1,i,j);
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

void test_not32()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned long i,j,k1;
	unsigned int k;

	printf("\nnot32:\n");
	srand(time(NULL));
	for (short k3=0;k3<0x1000;k3++)
	{
		i=rand();
		cflag=0;
		k=alu_not(cflag,i,32);

		_asm
		{
			xor ah,ah
			sahf;
			mov ecx,i;
			not ecx;
			mov k1,ecx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("not32: %x,%x %x,%x (not %x)--",k,k1,cflag,cflag1,i,j);
			chk(O,err)
			chk(C,err)
			chk(Z,err)
			chk(S,err)
			chk(P,err)
			chk(A,err) //return;
			printf("\n");
		}

	//printf("\r%x",i);
	}
}
