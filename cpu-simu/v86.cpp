
#include <stdio.h>
#include "cpu.h"
#include "log.h"

//I/O map base address field
//Contains a 16-bit offset from the base of the TSS to the I/O permission bit map
//and interrupt redirection bitmap. When present, these maps are stored in the
//TSS at higher addresses. The I/O map base address points to the beginning of
//the I/O permission bit map and the end of the interrupt redirection bit map.

bool  CCPU::int_redirected(unsigned char int_no)
{
	unsigned int tss_base=BASE(tr_desc);
	unsigned int tss_limit=LIMIT(tr_desc);

	unsigned short iomapbase= MEM_READW(OFFSET(TSS,iomap)+tss_base);

	unsigned int byte1_offset=iomapbase-32+(int_no>>3);
	//If I/O map base is greater than or equal to TSS limit, redirection table not present
	if (byte1_offset>=tss_limit ||byte1_offset<sizeof(TSS)) return false;

	unsigned short bits= MEM_READW(byte1_offset+tss_base);
	unsigned short mask=1<<(int_no&7);
	return (bits&mask)!=0;
}


