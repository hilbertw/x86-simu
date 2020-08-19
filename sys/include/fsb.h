#ifndef __FSB_H__
#define __FSB_H__

#include "config.h"
#include "mch.h"
//
// front system bus
//

unsigned char inta_8259();
extern CMCH mch;
//
// cache has not been implemented. fsb will do cache snoop among cpu-s.
//

class CFrontSystemBus
{
	static unsigned long the_lock;

public:
	CFrontSystemBus()  {}


	//
	// buslock 
	//
	void release_lock(){ the_lock=0;}
	void acquire_lock()
	{
		__asm 
		{
		l_wait:
			lock bts the_lock,1;
			jb l_wait;
		}
	}
	bool try_lock()
	{
		char c;
		__asm 
		{
			bt the_lock,1;
			setc c;
		}
		return c==0;
	}
	//
	//call to mch
	//
	char io_read_byte(unsigned short address) {return mch.io_read_byte(address);}
	short io_read_short(unsigned short address){return  mch.io_read_short(address);}
	int io_read_int(unsigned short address){return  mch.io_read_int(address);}


	void io_write_byte(unsigned short address,char dat) { mch.io_write_byte(address,dat);}
	void io_write_short(unsigned short address,short dat){ mch.io_write_short(address,dat);}
	void io_write_int(unsigned short address,int dat){ mch.io_write_int(address,dat);}

	char mem_read_byte(unsigned int address){return  mch.mem_read_byte(address);}
	short mem_read_short(unsigned int address){return  mch.mem_read_short(address);}
	int mem_read_int(unsigned int address){return  mch.mem_read_int(address);}

	void mem_read_string(unsigned int address,char *buff,int count){ mch.mem_read_string(address,buff,count);}

	void mem_write_byte(unsigned int address,char dat){ mch.mem_write_byte(address,dat);}
	void mem_write_short(unsigned int address,short dat){ mch.mem_write_short(address,dat);}
	void mem_write_int(unsigned int address,int dat){ mch.mem_write_int(address,dat);}

	void mem_write_string(unsigned int address,char *buff,int count){ mch.mem_write_string(address,buff,count);}

	unsigned char inta(){return inta_8259();}
	void assert_smiact(bool a){mch.assert_smiact(a);}


};

#endif   /*    __BIU_H__   */
   