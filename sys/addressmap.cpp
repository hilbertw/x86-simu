#include "addressmap.h"
TAddressMap<unsigned int>;

TAddressMap<unsigned short>;
template<class T>
TAddressMap<T>::~TAddressMap()
{
	ADDRESS_RANGE *p = pAddressRange;
	while(p)
	{
		pAddressRange = pAddressRange->pNext;

		p->pfnHandler((void *)(p+1),ADDR_RANGE_DEL,0,0,0);

		delete (p);

		p = pAddressRange;
	}
}

template<class T>
void *TAddressMap<T>::AllocAddressRangeBlock(int dwContextSize)
{
	ADDRESS_RANGE *p = (ADDRESS_RANGE *) new char[sizeof (ADDRESS_RANGE) + dwContextSize];

	if(p)
	{
		p->pNext = pAddressRange;
		pAddressRange = p;
	}

	return p;
}

template<class T>
void *TAddressMap<T>::LocateAddressBlock( T dwAddress)
{
	ADDRESS_RANGE *p=pAddressRange;

	while(p)
	{
		if (p->dwStart + p->dwSize > dwAddress && p->dwStart<=dwAddress)
			break;

		p = p->pNext;
	}

	return (void *)p;
}

template<class T>
void * TAddressMap<T>::RegisterAddressRange( 
		ADDR_RANGE_HANDLER f, 
		T dwStart,
		T dwBytes,
		int dwContextSize )
{

	// already in the list ?

	// Allocate, link it, and initialize it.
	ADDRESS_RANGE *p = (ADDRESS_RANGE*)AllocAddressRangeBlock(dwContextSize);
	if (p)
	{
		p->dwStart = dwStart;
		p->dwSize = dwBytes;
		p->pfnHandler = f;
	}

	return (void *)(p+1);
}

template<class T>
bool TAddressMap<T>::BusOp(ADDR_RANGE_CMD op,T dwAddress, char *pszData, int dwBytes)
{
	ADDRESS_RANGE *p = (ADDRESS_RANGE *)LocateAddressBlock (dwAddress);
	if (p)
	{
		p->pfnHandler ((void *)(p+1), op, dwAddress, pszData, dwBytes);
		return TRUE;
	}
	return FALSE;
}
template<class T>
char TAddressMap<T>::ReadByte(T dwAddress)
{
	char cData;
	if (!BusOp(ADDR_RANGE_READ,dwAddress,&cData,1)) cData = (char)0xff;
		
	return cData;
}

template<class T>
short TAddressMap<T>::ReadWord(T dwAddress)
{
	short wData;
	if (!BusOp(ADDR_RANGE_READ,dwAddress,(char *)&wData,2)) wData = (short)0xffff;
		
	return wData;
}
template<class T>
int TAddressMap<T>::ReadDWord(T dwAddress)
{
	int dwData;
	if (!BusOp(ADDR_RANGE_READ,dwAddress,(char *)&dwData,4)) dwData = (int)0xffffffff;
		
	return dwData;
}

template<class T>
bool  TAddressMap<T>::ReadString(T dwAddress,char *pszBuff,int dwCount)
{
	return BusOp(ADDR_RANGE_READ,dwAddress,pszBuff,dwCount);

}

template<class T>
bool TAddressMap<T>::WriteByte(T dwAddress,char bData) 
{
	return BusOp(ADDR_RANGE_WRITE,dwAddress,&bData,1); 
}

template<class T>
bool TAddressMap<T>::WriteWord(T dwAddress,short wData)
{
	return BusOp(ADDR_RANGE_WRITE,dwAddress,(char *)&wData,2);
}

template<class T>
bool TAddressMap<T>::WriteDWord(T dwAddress,int dwData)
{
	return BusOp(ADDR_RANGE_WRITE,dwAddress,(char *)&dwData,4);
}

template<class T>
bool TAddressMap<T>::WriteString(T dwAddress,char *pszBuff,int dwCount)
{
	return BusOp(ADDR_RANGE_WRITE,dwAddress,pszBuff,dwCount);
}
template<class T>
bool TAddressMap<T>::ReadByte(T dwAddress,char *bData) 
{
	return BusOp(ADDR_RANGE_READ,dwAddress,bData,1); 
}

template<class T>
bool TAddressMap<T>::ReadWord(T dwAddress,short *wData)
{
	return BusOp(ADDR_RANGE_READ,dwAddress,(char *)wData,2);
}

template<class T>
bool TAddressMap<T>::ReadDWord(T dwAddress,int *dwData)
{
	return BusOp(ADDR_RANGE_READ,dwAddress,(char *)dwData,4);
}

template<class T>
LONG TAddressMap<T>::RegisterAddressMapToFile( LPCWSTR wscFileName, LONG dwOffset,LONG dwBytes, LONG dwStart)
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


	FILE_MAP *q = (FILE_MAP *)RegisterAddressRange(FileMapHandler,dwStart, dwBytes, sizeof(FILE_MAP));
	if(!q)
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);

		OutputDebugString(L"Couldn't allocate memory for memory bloack\n");

		return 4;
	}

	q->lpFileBase=(LPBYTE)lpFileBase;
	q->hFile=hFile;
	q->hFileMapping=hFileMapping;
	q->startingAddress=dwStart;
	q->dwBytes=dwBytes;
	q->dwReadOnly=true;


	return 0;
}

template<class T>
LONG TAddressMap<T>::RegisterAddressMapToFileRW( LPCWSTR wscFileName, LONG dwOffset,LONG dwBytes, LONG dwStart)
{
	HANDLE hFile;
	HANDLE hFileMapping;
	LPVOID lpFileBase;
	
	hFile = CreateFile(wscFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
					
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		OutputDebugString(L"Couldn't open file with CreateFile()\n");
		return 1;
	}

	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, dwBytes, NULL);
	if ( hFileMapping == 0 )
	{
		CloseHandle(hFile);
		OutputDebugString(L"Couldn't open file mapping with CreateFileMapping()\n");
		return 2;
	}

	lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ|FILE_MAP_WRITE, 0, dwOffset, dwBytes);
	if ( lpFileBase == 0 )
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		OutputDebugString(L"Couldn't map view of file with MapViewOfFile()\n");
		return 3;
	}


	FILE_MAP *q = (FILE_MAP *)RegisterAddressRange(FileMapHandler,dwStart, dwBytes, sizeof(FILE_MAP));
	if(!q)
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);

		OutputDebugString(L"Couldn't allocate memory for memory bloack\n");

		return 4;
	}

	q->lpFileBase=(LPBYTE)lpFileBase;
	q->hFile=hFile;
	q->hFileMapping=hFileMapping;
	q->startingAddress=dwStart;
	q->dwBytes=dwBytes;
	q->dwReadOnly=false;


	return 0;
}
template<class T>
void TAddressMap<T>::FileMapHandler (
		 void *pContext,          // pointer to FILE_MAP object
         ADDR_RANGE_CMD op,              // operation 
         unsigned int dwAddress,           // start dwAddress
         char *pszBuff,           // optional 
         unsigned int nBytes)             // in or out buffer
{
	FILE_MAP *pFileMap = (FILE_MAP *)pContext;
	char *pszFileData = (char *)(pFileMap->lpFileBase + dwAddress-pFileMap->startingAddress);
	//
	// enough bytes available
	//
	if (nBytes+dwAddress> pFileMap->startingAddress+pFileMap->dwBytes)
	{
		assert(0);
	}

	if (op==ADDR_RANGE_READ)
	{
		memcpy (pszBuff, pszFileData, nBytes);
	}
	else if (op==ADDR_RANGE_WRITE)
	{
		
		if(!pFileMap->dwReadOnly)
		memcpy ( pszFileData, pszBuff, nBytes);
	}
}

//
// same mem obj are mapped to several address
//

template<class T>
LONG TAddressMap<T>::RegisterAddressAlias( 
	LONG dwStart,          // bytes to map
	LONG dwBytes,         // starting address of the address range
	LONG dwMask )         // mask address to form the alias address
{
	LONG alias_address=dwStart |dwMask;

	ADDRESS_RANGE *p = (ADDRESS_RANGE *)LocateAddressBlock (alias_address);
	if (!p) return 0;

	//
	// just a part of the range  is mapped, don't do it now
	//

	ALIAS_MAP *q = (ALIAS_MAP *)RegisterAddressRange(AliasMapHandler,dwStart, dwBytes, sizeof(ALIAS_MAP));
	if(!q)
	{
		OutputDebugString(L"Couldn't allocate memory for memory bloack\n");
		return 0;
	}

	q->alias=p;
	q->dwMask=dwMask;

	return 1;

}


template<class T>
void TAddressMap<T>::AliasMapHandler (
		 void *pContext,          // pointer to FILE_MAP object
         ADDR_RANGE_CMD op,              // operation 
         unsigned int dwAddress,           // start dwAddress
         char *pszBuff,           // optional 
         unsigned int dwBytes)             // in or out buffer
{
	ALIAS_MAP *pAliasMap = (ALIAS_MAP *)pContext;

	ADDRESS_RANGE *p=(ADDRESS_RANGE *)(pAliasMap->alias);

	p->pfnHandler ((void *)(p+1), op, dwAddress|pAliasMap->dwMask, pszBuff, dwBytes);
}