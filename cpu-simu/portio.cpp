
#include <stdio.h>
#include "cpu.h"
#include "log.h"
#define offset(t,f) (unsigned int)&((t*)NULL)->f
//Each bit in the map corresponds to an I/O port byte address; 
//for example, the bit for port 41 is found at I/O map base + 5, bit offset 1. 
//The processor tests all the bits that correspond to the I/O addresses spanned by an I/O operation; 
//for example, a doubleword operation tests four bits corresponding to four adjacent byte addresses. 
//If any tested bit is set, the processor signals a general protection exception. 
//If all the tested bits are zero, the I/O operation may proceed. 
//
//It is not necessary for the I/O permission map to represent all the I/O addresses. 
//I/O addresses not spanned by the map are treated as if they had one bits in the map. 
//For example, if TSS limit is equal to I/O map base + 31, the first 256 I/O ports are mapped; I/O operations on any port greater than 255 cause an exception. 
//

 bool CCPU::check_iopermission(unsigned short SRC,int OperandSize)
 {
	unsigned int tss_base=BASE(tr_desc);
	unsigned int tss_limit=LIMIT(tr_desc);
	//
	//bit(iomap [SRC>>3],SRC&7)?
	//
	unsigned short iomapbase= MEM_READW(offset(TSS,iomap)+tss_base);

	//If I/O map base is greater than or equal to TSS limit, 
	//the TSS segment has no I/O permission map, 
	//and all I/O instructions in the 80386 program cause exceptions when CPL > IOPL. 
	if (iomapbase>=tss_limit ) return false;

	//I/O addresses not spanned by the map are treated as if they had one bits in the map.
	unsigned int byte1_offset=iomapbase+(SRC>>3);
	if (byte1_offset>=tss_limit ) return false;

	// spans two bytes
	if ((SRC-1+OperandSize/8)/8>SRC/8)
		if (byte1_offset+1>=tss_limit ) return false;


	unsigned short bits= MEM_READW(byte1_offset+tss_base);
	unsigned short mask;
	bits >>= (SRC&7);

	if (OperandSize==8) mask=1;
	else if (OperandSize==16) mask=3;
	else if (OperandSize==32) mask=0xf;
	else mask=0;

	return (bits&mask)==0;

 }

/*!
     \brief:
      IN - Input Byte || Word From Port
 */

unsigned int  CCPU::inport(unsigned short SRC,int OperandSize)
{
	unsigned int  DEST;
	//The IN, INS, OUT, and OUTS
	//instructions, which are sensitive to IOPL in protected mode, 
	//are not sensitive in virtual-8086 mode
	//log("in %x.\n",SRC);
	if (((__PE__ == 1) && ((__CPL__ > __IOPL__) || (__VM__ == 1))))
	{ 
		/* Protected mode with __CPL__ > __IOPL__ or virtual-8086 mode */
		//Any I/O Permission Bit for I/O port being accessed == 1
		if (!check_iopermission(SRC,OperandSize))
		/* I/O operation is not allowed */
			GP_(0);
		
		/* I/O operation is allowed */
		/* DEST = SRC;Reads from selected I/O port */
	}
	/*Real Mode or Protected Mode with CPL <= IOPL */
	/* DEST = SRC; Reads from selected I/O port */

	if (OperandSize==8) DEST=biu.io_read_byte(SRC);
	else if (OperandSize==16) DEST=biu.io_read_short(SRC);
	else if (OperandSize==32) DEST=biu.io_read_int(SRC);

	return DEST;
}

/*!
     \brief:
      OUT - Output Byte || Word From Port
 */
void  CCPU::outport(unsigned short SRC,int dat,int OperandSize)
{
	//log("out %x.\n",SRC);
	//The IN, INS, OUT, and OUTS
	//instructions, which are sensitive to IOPL in protected mode, 
	//are not sensitive in virtual-8086 mode
	if (((__PE__ == 1) && ((__CPL__ > __IOPL__) || (__VM__ == 1))))
	{ /* Protected mode with __CPL__ > __IOPL__ or virtual-8086 mode */
		//Any I/O Permission Bit for I/O port being accessed == 1
		if (!check_iopermission(SRC,OperandSize))
		/* I/O operation is not allowed */
			GP(0);
		
		/* I/O operation is allowed */
		}
	else /*Real Mode or Protected Mode with CPL <= IOPL */

	if (OperandSize==8) biu.io_write_byte(SRC,dat&0xff);
	else if (OperandSize==16) biu.io_write_short(SRC,dat&0xffff);
	else if (OperandSize==32) biu.io_write_int(SRC,dat);
}