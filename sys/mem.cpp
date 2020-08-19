#include "mem.h"

CMemory::~CMemory()
{
	while(pMemory)
	{
		MEMORY_BLOCK *p = pMemory->pNext;

		CloseHandle(pMemory->hFileMapping);
		CloseHandle(pMemory->hFile);

		delete (pMemory);

		pMemory = p;
	}
}


CMemory::MEMORY_BLOCK *CMemory::AllocMemoryBlock()
{
	MEMORY_BLOCK *p = new MEMORY_BLOCK;

	if(p)
	{
		p->pNext = pMemory;
		pMemory = p;
	}

	return p;
}


LONG CMemory::RegisterFile( LPCWSTR wscFileName, LONG dwOffset,LONG dwBytes, LONG dwStart)
{
	HANDLE hFile;
	HANDLE hFileMapping;
	LPVOID lpFileBase;
	
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

	lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, dwOffset, dwBytes);
	if ( lpFileBase == 0 )
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		OutputDebugString(L"Couldn't map view of file with MapViewOfFile()\n");
		return 3;
	}

	MEMORY_BLOCK *p = AllocMemoryBlock();
	if(!p)
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		OutputDebugString(L"Couldn't allocate memory for memory bloack\n");
		return 4;
	}

	p->hFile = hFile;
	p->hFileMapping = hFileMapping;
	p->lpFileBase = (LPBYTE)lpFileBase;

	p->dwStart = dwStart;
	p->dwSize = dwBytes;

	return 0;
}


CMemory::MEMORY_BLOCK *CMemory::LocateMemoryBlock( LONG dwAddress)
{
	MEMORY_BLOCK *p=pMemory;

	while(p)
	{
		if (p->dwStart + p->dwSize > dwAddress && p->dwStart<dwAddress)
			break;

		p = p->pNext;
	}

	return p;
}


DWORD CMemory::ReadDword(LONG dwAddress)
{
	MEMORY_BLOCK *p = LocateMemoryBlock(dwAddress);

	return p ? *(DWORD *)(p->lpFileBase + dwAddress - p->dwStart) : 0xCCCCCCCC;
}
WORD CMemory::ReadWord(LONG dwAddress)
{
	MEMORY_BLOCK *p = LocateMemoryBlock(dwAddress);

	return p ? *(WORD *)(p->lpFileBase + dwAddress - p->dwStart) : 0xCCCC;
}
BYTE CMemory::ReadByte(LONG dwAddress)
{
	MEMORY_BLOCK *p = LocateMemoryBlock(dwAddress);

	return p ? *(BYTE *)(p->lpFileBase + dwAddress - p->dwStart) : 0xCC;
}

VOID CMemory::WriteDword(LONG dwAddress,DWORD value)
{
	MEMORY_BLOCK *p = LocateMemoryBlock(dwAddress);

	if(p) *(DWORD *)(p->lpFileBase + dwAddress - p->dwStart) = value;
}
VOID CMemory::WriteWord(LONG dwAddress,WORD value)
{
	MEMORY_BLOCK *p = LocateMemoryBlock(dwAddress);

	if(p) *(WORD *)(p->lpFileBase + dwAddress - p->dwStart) = value;
}
VOID CMemory::WriteByte(LONG dwAddress,BYTE value)
{
	MEMORY_BLOCK *p = LocateMemoryBlock(dwAddress);

	if(p) *(BYTE *)(p->lpFileBase + dwAddress - p->dwStart) = value;
}