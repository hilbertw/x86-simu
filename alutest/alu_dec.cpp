
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
void test_dec8()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned char i,j,k1;
	unsigned int k;


	printf("\ndec8:\n");
	for (i=0;i<255;i++)
	{
		cflag=0;
		k=alu_dec(cflag,i,8);

		_asm
		{
			xor ah,ah;
			sahf;
			mov bh,i;
			dec bh
			mov k1,bh;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("dec8: %x,%x %x,%x (dec %x)--",k,k1,cflag,cflag1,i,j);
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
void test_dec16()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned short i,j,k1;
	unsigned int k;

	printf("\ndec16:\n");
	for (i=0;i<0xffff;i++)
	{
		cflag=0;
		k=alu_dec(cflag,i,16);

		_asm
		{
			xor ah,ah;
			sahf;
			mov cx,i;
			dec cx;
			mov k1,cx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("dec16: %x,%x %x,%x (dec %x)--",k,k1,cflag,cflag1,i,j);
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

void test_dec32()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned long i,j,k1;
	unsigned int k;

	printf("\ndec32:\n");
	for (i=0;i<0xffffffff;i++)
	{
		cflag=0;
		k=alu_dec(cflag,i,32);

		_asm
		{
			xor ah,ah;
			sahf;
			mov ecx,i;
			dec ecx;
			mov k1,ecx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x202; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("dec32: %x,%x %x,%x (dec %x)--",k,k1,cflag,cflag1,i,j);
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
