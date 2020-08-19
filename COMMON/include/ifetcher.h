#ifndef __FECTHBASE_H__
#define __FECTHBASE_H__


#include <windows.h>

#include "common.h"

class CFetcherBase
{
public:
	CFetcherBase():address16_mode(true),bytes(0),buffer(NULL),ip(0)
	{}
	unsigned char * get_buffer() { return buffer;}
	unsigned int get_fetch_eip() { return ip;}
	void set_fetch_address(bool mode,unsigned int ip);
	void set_mode( bool bit16_mode) {address16_mode=bit16_mode;}
//	int fetch( unsigned int& old_ip, unsigned  char*&code,int & prefix_bytes,int &len,char & prefix, int& data_size,int& addr_size);

	int fetch(PINSN_BASIC_INFO p_insn);

private:
	unsigned int decode_length(unsigned char  *code,int addr_size,unsigned  int data_size,prefix_t prefix,struct instr_rec* p_instr_desc);
	unsigned int fetch_prefix(prefix_t & prefix,bool & illegal);

protected:
	unsigned char *buffer;
	unsigned int  bytes;

	bool address16_mode;

	unsigned int ip;               // ip to decode the next instruction
};

#define FETCH_SUCCESS 0
#define FETCH_UD      -1
#define FETCH_EOF     -2

class CIFetcherMemory: public CFetcherBase
{
protected:
	unsigned char *lpszData;
	unsigned int   starting_address;
	unsigned int   dwSize;

public:
	CIFetcherMemory(){}

	void set_buffer(unsigned char * data,unsigned int nbytes,unsigned int dwStart)
	{
		buffer=lpszData=data;
		bytes=dwSize=nbytes;
		starting_address=dwStart;

	}
	bool set_fetch_address(bool mode, unsigned  int ip,unsigned short seg=0);

};

class CIFetcherFileMap: public CIFetcherMemory
{
private:
	HANDLE hFile;
	HANDLE hFileMapping;

public:

	CIFetcherFileMap():hFileMapping(NULL),hFile(NULL) {}

	int Open( 
		LPCTSTR wscFileName,   // file to map
		unsigned int dwOffset,         // bytes to skip from the beginning of the file
		unsigned int dwBytes,unsigned int dwStart);         // bytes to map
	

	void Close();

};

#endif 