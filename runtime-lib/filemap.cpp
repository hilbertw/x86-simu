
#include <tchar.h>
#include "filemap.h"



int CFileMap::LoadFile(LPCTSTR filename,DWORD dwOffsetHi, DWORD dwOffsetLo,DWORD dwSize, bool read_only )
{
	hFile = CreateFile(filename, read_only?GENERIC_READ:(GENERIC_READ|GENERIC_WRITE), FILE_SHARE_READ, NULL,
		read_only?OPEN_EXISTING:OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
					
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		OutputDebugString(_T("Couldn't open file with CreateFile()\n"));
		return 1;
	}

	
	hFileMapping = CreateFileMapping(hFile, NULL, read_only?PAGE_READONLY:PAGE_READWRITE, 0, dwSize, NULL);
	if ( hFileMapping == 0 )
	{
		CloseHandle(hFile);
		OutputDebugString(_T("Couldn't open file mapping with CreateFileMapping()\n"));
		hFile=NULL;
		return 2;
	}

	lpFileBase = MapViewOfFile(hFileMapping, read_only?FILE_MAP_READ:(FILE_MAP_READ|FILE_MAP_WRITE), dwOffsetHi, dwOffsetLo, dwSize);
	if ( lpFileBase == 0 )
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		OutputDebugString(_T("Couldn't map view of file with MapViewOfFile()\n"));
		hFileMapping=hFile=NULL;
		return 3;
	}
	return 0;
}