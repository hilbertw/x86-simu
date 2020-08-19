#ifndef __BIU_H__
#define __BIU_H__


#include "addressmap.h"

class CBIU
{
	TAddressMap<unsigned short> &IO;
	TAddressMap<unsigned int> &mem;
	long f000_mapped;

public:
	CBIU(TAddressMap<unsigned short> &io,TAddressMap<unsigned int> & m) : IO(io),mem(m),f000_mapped(0xf000) {}

	char io_read_byte(unsigned short address) {return IO.ReadByte(address);}
	short io_read_short(unsigned short address){return IO.ReadWord(address);}
	int io_read_int(unsigned short address){return IO.ReadDWord(address);}

	void io_read_string(unsigned short address,char *buff,int count){IO.ReadString(address,buff,count);}

	void io_write_byte(unsigned short address,char dat) {IO.WriteByte(address,dat);}
	void io_write_short(unsigned short address,short dat){IO.WriteWord(address,dat);}
	void io_write_int(unsigned short address,int dat){IO.WriteDWord(address,dat);}

	void io_write_string(unsigned short address,char *buff,int count){IO.WriteString(address,buff,count);}

	char mem_read_byte(unsigned int address){return mem.ReadByte(address);}
	short mem_read_short(unsigned int address){return mem.ReadWord(address);}
	int mem_read_int(unsigned int address){return mem.ReadDWord(address);}

	void mem_read_string(unsigned int address,char *buff,int count)
	{
		if(address<0x100000)
		{
		if( f000_mapped&(1<<(0xf&(address>>16))))  { address |=0xfff00000;}
		}
		mem.ReadString(address,buff,count);
	}

	void mem_write_byte(unsigned int address,char dat){mem.WriteByte(address,dat);}
	void mem_write_short(unsigned int address,short dat){mem.WriteWord(address,dat);}
	void mem_write_int(unsigned int address,int dat){mem.WriteDWord(address,dat);}

	void mem_write_string(unsigned int address,char *buff,int count)
	{

		 if(address<0x100000)
		{
		if( f000_mapped&(1<<(0xf&(address>>16)))) { f000_mapped&=~(1<<(0xf&(address>>16)));}
		}

		mem.WriteString(address,buff,count);
	}
};



#endif   /*    __BIU_H__   */
   