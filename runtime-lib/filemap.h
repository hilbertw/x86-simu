#ifndef __filemap__
#define __filemap__

#include <windows.h>
#include <assert.h>
class CFileMap
{
public:
HANDLE hFile;
HANDLE hFileMapping;
LPVOID lpFileBase;

CFileMap():hFile(NULL),hFileMapping(NULL),lpFileBase(NULL)
{}

~CFileMap() 
{
	if(hFileMapping) CloseHandle(hFileMapping);
	if(hFile) CloseHandle(hFile);
}

void Close ()
{
	CloseHandle(hFileMapping);
	CloseHandle(hFile);
	lpFileBase=NULL; 
	hFile=hFileMapping=NULL;
}

int LoadFile(LPCTSTR filename,DWORD dwoffsetHi, DWORD dwoffsetLo,DWORD dwSize, bool read_only );
	operator  char *(){return ( char *)lpFileBase;}
};

class CFileMapEx: public CFileMap
{
public:
	unsigned int max_pos;


	CFileMapEx():max_pos(0){}
	void close (){

		CFileMap::Close();
		max_pos=0;
	}


	int load_file(LPCTSTR filename)
	{
		int retcd=LoadFile( filename,0,0,0,true);
		max_pos=GetFileSize(hFile,NULL);

		return retcd;
	}
	char operator [](unsigned int pos)
	{
		assert(lpFileBase!=NULL);
		return pos>=max_pos?0x7f:(( char *)lpFileBase)[pos];
	}

	int eof(unsigned int pos){return pos>=max_pos;}

	unsigned int size() {return max_pos;}
};

#endif
