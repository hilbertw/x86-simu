#ifndef __BIU_H__
#define __BIU_H__


#include "fsb.h"

extern CFrontSystemBus fsb;
//
// a biu will lock fsb and do read and write
//
class CBIU
{
	int dummy;
public:
	CBIU() {}

	char io_read_byte(unsigned short address) {return fsb.io_read_byte(address);}
	short io_read_short(unsigned short address){return fsb.io_read_short(address);}
	int io_read_int(unsigned short address){return fsb.io_read_int(address);}

	void io_write_byte(unsigned short address,char dat) {fsb.io_write_byte(address,dat);}
	void io_write_short(unsigned short address,short dat){fsb.io_write_short(address,dat);}
	void io_write_int(unsigned short address,int dat){fsb.io_write_int(address,dat);}


	char mem_read_byte(unsigned int address){return fsb.mem_read_byte(address);}
	short mem_read_short(unsigned int address){return fsb.mem_read_short(address);}
	int mem_read_int(unsigned int address){return fsb.mem_read_int(address);}

	void mem_read_string(unsigned int address,char *buff,int count)
	{fsb.mem_read_string(address,buff,count);}

	void mem_write_byte(unsigned int address,char dat){fsb.mem_write_byte(address,dat);}
	void mem_write_short(unsigned int address,short dat){fsb.mem_write_short(address,dat);}
	void mem_write_int(unsigned int address,int dat){fsb.mem_write_int(address,dat);}

	void mem_write_string(unsigned int address,char *buff,int count)
	{fsb.mem_write_string(address,buff,count);}

	unsigned char inta(){return fsb.inta();}
	void assert_smiact(bool a){fsb.assert_smiact(a);}
};



#endif   /*    __BIU_H__   */
   