#ifndef __ICH_H__
#define __ICH_H__


#include "addressmap.h"
#include "config.h"
#include "filemap.h"

class CICH
{
public:
	TAddressMap<unsigned short> IO;
	TAddressMap<unsigned int> mem;

public:
	//
	// rd only bios file map
	//
	CFileMap bios_map;
	//CFileMap nvs_map;
	//
	// readonly bios rom file map
	//

	CICH() {}

	char io_read_byte(unsigned short address);
	short io_read_short(unsigned short address);
	int io_read_int(unsigned short address);
	//void io_read_string(unsigned short address,char *buff,DWORD count);

	void io_write_byte(unsigned short address,char dat);
	void io_write_short(unsigned short address,short dat);
	void io_write_int(unsigned short address,int dat);
	//void io_write_string(unsigned short address,char *buff,DWORD count);

	char mem_read_byte(unsigned int address);
	short mem_read_short(unsigned int address);
	int mem_read_int(unsigned int address);
	void mem_read_string(unsigned int address,char *buff,DWORD count);

	void mem_write_byte(unsigned int address,char dat);
	void mem_write_short(unsigned int address,short dat);
	void mem_write_int(unsigned int address,int dat);
	void mem_write_string(unsigned int address,char *buff,DWORD count);

};


#endif   /*    __ICH_H__   */
   