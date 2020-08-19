#include <stdio.h>
#include <string.h>
#include "ifetcher.h"

int decode_length(int addr_size, int data_size,unsigned char  *code,bool pmode,bool &illegal,char prefix);
//
//
//
//return the length of prefixes
//      F0:lock
//      F2-repe/F-repne
//      0x66-data size
//      0x67-address size
//
//
//
int CFetcherBase::fetch_prefix(char & prefix,bool & illegal)
{
	 int i;

	 prefix=0;
	 illegal=false;

	for(i=0;i<bytes;i++)
	{
		switch( buffer[i])
		{
		case 0xf0:
				if ( prefix& PREFIX_LOCK) illegal=true;
				else prefix|= PREFIX_LOCK;
				break;
		case 0xf2:
				if ( prefix& PREFIX_REPZ) illegal=true;
				else prefix|= PREFIX_REPNZ;
				break;
		case 0xf3:
				if ( prefix& PREFIX_REPNZ) illegal=true;
				else prefix|= PREFIX_REPZ;
				break;
		case 0x26://ES
				if( ( prefix& PREFIX_SEGS)==NO_SEGOVR) illegal=true;
				else prefix|= 1;
				break;
		case 0x36://SS
				if( ( prefix& PREFIX_SEGS)==NO_SEGOVR) illegal=true;
				else prefix|= 2;
				break;
		case 0x2E://CS
				if( ( prefix& PREFIX_SEGS)==NO_SEGOVR) illegal=true;
				else prefix|= 3;
				break;
		case 0x3E://DS
				if( ( prefix& PREFIX_SEGS)==NO_SEGOVR) illegal=true;
				else prefix|= 4;
				break;
		case 0x64://FS
				if( ( prefix& PREFIX_SEGS)==NO_SEGOVR) illegal=true;
				else prefix|= 5;
				break;
		case 0x65://GS
				if( ( prefix& PREFIX_SEGS)==NO_SEGOVR) illegal=true;
				else prefix|= 6;
				break;
		case 0x66://DSIZE
				if ( prefix& PREFIX_DSIZE) illegal=true;
				else prefix|= PREFIX_DSIZE;
				break;
		case 0x67://ASIZE		
				if ( prefix& PREFIX_ASIZE) illegal=true;
				else prefix|= PREFIX_ASIZE;
				break;
		default:
				return i;

		}
	}

	return i;
}



void CFetcherBase::set_fetch_address(bool mode,unsigned int _base_,unsigned int _limit_,unsigned int _ip_, bool _user_)
{

	//
	// save pointer
	//
	address16_mode = mode;
	base = _base_;
	user = _user_;
	 ip = _ip_;
	 limit=_limit_;	 

	if (address16_mode)
	{
	ip = _ip_ & 0xffff;
	}

}

//
//  return
//        0: illegal instruction
//        1: success
//        2: end
//
int CFetcherBase::fetch( int &next_ip, unsigned char*&code,int & prefix_bytes,int &len,char & prefix, int& data_size,int& addr_size)
{
	bool illegal;

	if (!bytes ||!buffer) return 2; // no data
	//
	// prefix
	//
	prefix_bytes=fetch_prefix(prefix,illegal);

	if(illegal) return 0;// illegal instruction

	data_size=(address16_mode ^(0==(prefix&PREFIX_DSIZE)))?32:16;
	addr_size=(address16_mode ^(0==(prefix&PREFIX_ASIZE)))?32:16;

	int code_len = decode_length( addr_size, data_size,buffer+prefix_bytes,!address16_mode,illegal,prefix);

	
	if(illegal) 
		return 0;// illegal instruction

	len=prefix_bytes+code_len;

	if (bytes<len) return 2;

	code=buffer;
	next_ip=ip;
	ip+=len;
	buffer+=len;
	bytes-=len;

	return 1;

}


void CIFetcherMMU::fill_buff()
{

	if (buffer_id == 2) 
	{
		// starting building the circular buffer
		mmu.fetch(base + line_offset,user, (char *)lines[1],2*FETCH_BUFF_SIZE);
		bytes=2*FETCH_BUFF_SIZE;
		buffer=&lines[1][0];
		buffer_id=0;  // next time,fill buffer0
	}
	else
	{
		mmu.fetch(base + line_offset,user, (char *)lines[buffer_id],FETCH_BUFF_SIZE);
		
		if (buffer_id == 1)  // copy buffer 0 to buffer 2 to emulate a circular address
		{
			memcpy(lines[2], lines[0],FETCH_BUFF_SIZE);
			buffer-=2*FETCH_BUFF_SIZE;

			buffer_id=0;
		}
		else
			buffer_id=1;

			bytes+=FETCH_BUFF_SIZE;

			line_offset += FETCH_BUFF_SIZE;
			if( address16_mode ) line_offset &= 0xffff;
		}

	}


void CIFetcherMMU::set_fetch_address(bool mode, unsigned int _base_,unsigned int _limit_,unsigned  int _ip_, bool _user_)
{

	CFetcherBase::set_fetch_address( mode,  _base_,_limit_,  _ip_,  _user_);
	
	line_offset = ip & ~(FETCH_BUFF_SIZE-1);
	if( address16_mode ) line_offset &= 0xffff;

    buffer_id = 2;   // initialize.

	fill_buff();

	int start=ip & (FETCH_BUFF_SIZE-1);

	bytes-=start;
	buffer+=start;

	
}

int CIFetcherMMU::fetch( int &next_ip, unsigned  char*&code,int& prefix_bytes,int& len,char & prefix, int& data_size,int& addr_size)
{

	//
	//need fill the next buffer?
	//
	if( bytes <FETCH_BUFF_SIZE) fill_buff();
	//
	// fetch from buffer
	//
	return CFetcherBase::fetch(next_ip,code,prefix_bytes,len,prefix,data_size,addr_size);
}

LONG CIFetcherFileMap::Open( 
		LPCWSTR wscFileName,   // file to map
		LONG dwOffset,         // bytes to skip from the beginning of the file
		LONG dwBytes,LONG dwStart)          // bytes to map

{
if (NULL!=hFile||NULL!=hFileMapping)
	{
		OutputDebugString(L"Couldn't open file with CreateFile()\n");
		return 5;
	}
	hFile = CreateFile(wscFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
					
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		OutputDebugString(L"Couldn't open file with CreateFile()\n");
		return 1;
	}

	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if ( hFileMapping == 0 )
	{
		CloseHandle(hFile);
		OutputDebugString(L"Couldn't open file mapping with CreateFileMapping()\n");
		return 2;
	}

	lpFileBase =( unsigned char*) MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, dwOffset, dwBytes);
	if ( lpFileBase == 0 )
	{
		Close();
		OutputDebugString(L"Couldn't map view of file with MapViewOfFile()\n");
		return 3;
	}
	
	starting_address=dwStart;
	dwSize = dwBytes;
	Adjust();

	return 0;

}
void CIFetcherFileMap::Close()
{
	if(hFileMapping) CloseHandle(hFileMapping);
	if(hFile) CloseHandle(hFile);
}

void CIFetcherFileMap::Adjust()
{

		//
		// linear address=physical address
		//
			int current_a=base+(address16_mode)?(ip&0xffff):ip;

		if (current_a<starting_address||starting_address+dwSize<current_a)
		{
			buffer=NULL;  // current ip not mapped.
			bytes=0;
		}
		else
		{
			buffer=lpFileBase+(current_a-starting_address);
			bytes=dwSize-(current_a-starting_address);
		}
}

void CIFetcherFileMap::set_fetch_address(bool mode,unsigned  int _base_,unsigned int _limit_, unsigned int _ip_, bool _user_)
{

	CFetcherBase::set_fetch_address( mode,  _base_,_limit_,  _ip_,  _user_);
	
	Adjust();
}

	