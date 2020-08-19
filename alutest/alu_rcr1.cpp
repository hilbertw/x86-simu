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
void test_rcr18()
{
	unsigned short cflag; 
	unsigned short cflag1;


	unsigned char i,j,k1;
	unsigned int k;


	printf("\nrcr18-1:\n");
	for (i=0;i<255;i++)
	{
		cflag=MASK_C;
		k=alu_rcr(cflag,i,1,8);

		_asm
		{
			xor ah,ah
			sahf;
			stc;
			mov bh,i;
			rcr bh,1
			mov k1,bh;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr18: %x,%x %x,%x (%x>>1)--",k,k1,cflag,cflag1,i,j);
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

printf("\nrcr18:\n");
	for (i=0;i<255;i++)
	{
		cflag=0;
		k=alu_rcr(cflag,i,1,8);

		_asm
		{
			xor ah,ah
			sahf;
			mov bh,i;
			rcr bh,1
			mov k1,bh;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr18: %x,%x %x,%x (%x>>1)--",k,k1,cflag,cflag1,i,j);
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
void test_rcr116()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned short i,j,k1;
	unsigned int k;

	printf("\nrcr116:\n");
	for (i=0;i<0xffff;i++)
	{
		cflag=0; 
		k=alu_rcr(cflag,i,1,16);

		_asm
		{
			xor ah,ah
			sahf;
			mov bx,i;
			rcr bx,1;
			mov k1,bx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr116: %x,%x %x,%x (%x>>1)--",k,k1,cflag,cflag1,i,j);
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

	printf("\nrcr116-1:\n");
	for (i=0;i<0xffff;i++)
	{
		cflag=MASK_C;
		k=alu_rcr(cflag,i,1,16);

		_asm
		{
			xor ah,ah
			sahf;
			stc;
			mov bx,i;
			rcr bx,1;
			mov k1,bx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr116: %x,%x %x,%x (%x>>1)--",k,k1,cflag,cflag1,i,j);
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

void test_rcr132()
{
	unsigned short cflag=0; 
	unsigned short cflag1;


	unsigned long i,j,k1;
	unsigned int k;

	printf("\nrcr132:\n");
	for (i=0;i<0xffffffff;i++)
	{
		cflag=0;
		k=alu_rcr(cflag,i,1,32);

		_asm
		{
			xor ah,ah
			sahf;
			mov ebx,i;
			rcr ebx,1;
			mov k1,ebx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr132: %x,%x %x,%x (%x>>1)--",k,k1,cflag,cflag1,i,j);
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

	printf("\nrcr132-1:\n");
	for (i=0;i<0xffffffff;i++)
	{
		cflag=MASK_C;
		k=alu_rcr(cflag,i,1,32);

		_asm
		{
			xor ah,ah
			sahf;
			stc;
			mov ebx,i;
			rcr ebx,1;
			mov k1,ebx;
			pushf;
			pop cflag1;
		}

		cflag1&=~0x212; // 

		if ((k!=k1)
			||(cflag!=cflag1))
		{
			short err=(cflag^cflag1);


			printf("rcr132: %x,%x %x,%x (%x>>1)--",k,k1,cflag,cflag1,i,j);
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
