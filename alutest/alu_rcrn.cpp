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
void test_rcrn8()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned char i,j,k1;
	unsigned char k;


	printf("\nrcr8:\n");
	for (i=0;i<255;i++)
	{
	for (j=0;j<10;j++)
	{
		cflag=0; 
		k=(unsigned char)alu_rcr(cflag,i,j,8);

		_asm
		{
			xor ah,ah
			sahf;
			mov cl,j;
			mov bh,i;
			rcr bh,cl
			mov k1,bh;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr8: %x,%x %x,%x (%x>>%x)--",k,k1,cflag,cflag1,i,j);
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
void test_rcrn16()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned short i,k1;
	char j;
	unsigned int k;

	printf("\nrcr16:\n");
	for (i=0;i<0xffff;i++)
	{
	for (j=0;j<20;j++)
	{
		cflag=0; 
		k=alu_rcr(cflag,i,j,16);

		_asm
		{
			xor ah,ah
			sahf;
			mov bx,i;
			mov cl,j;
			rcr bx,cl;
			mov k1,bx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr16: %x,%x %x,%x (%x>>%x)--",k,k1,cflag,cflag1,i,j);
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

void test_rcrn32()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned long i,k1;
	char j;
	unsigned int k;

	printf("\nrcr32:\n");
	srand(time(NULL));
	for (int i1=0;i1<0x1000;i1++)
	{
		i=rand();
	for (j=0;j<40;j++)
	{
		cflag=0; 
		k=alu_rcr(cflag,i,j,32);

		_asm
		{
			xor ah,ah
			sahf;
			mov ebx,i;
			mov cl,j;
			rcr ebx,cl;
			mov k1,ebx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr32: %x,%x %x,%x (%x>>%x)--",k,k1,cflag,cflag1,i,j);
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
	printf("\nrcr32-1:\n");
	srand(time(NULL));
	for (int i1=0;i1<0x1000;i1++)
	{
		i=rand();
	for (j=0;j<40;j++)
	{
		cflag=MASK_C; 
		k=alu_rcr(cflag,i,j,32);

		_asm
		{
			xor ah,ah
			sahf;
			stc;
			mov ebx,i;
			mov cl,j;
			rcr ebx,cl;
			mov k1,ebx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr32: %x,%x %x,%x (%x>>%x)--",k,k1,cflag,cflag1,i,j);
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
