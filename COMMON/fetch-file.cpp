#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "ifetcher.h"
int CIFetcherFileMap::Open( 
		LPCTSTR wscFileName,   // file to map
		unsigned int dwOffset,         // bytes to skip from the beginning of the file
		unsigned int dwBytes,          // bytes to map
		unsigned int dwStart)          // linear address mapped to 1st byte

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

	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwBytes, NULL);
	if ( hFileMapping == 0 )
	{
		CloseHandle(hFile);
		OutputDebugString(L"Couldn't open file mapping with CreateFileMapping()\n");
		return 2;
	}

	lpszData =( unsigned char*) MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, dwOffset, dwBytes);
	if ( lpszData == 0 )
	{
		Close();
		OutputDebugString(L"Couldn't map view of file with MapViewOfFile()\n");
		return 3;
	}
	
	starting_address=dwStart;
	dwSize = dwBytes;
	/*Adjust();*/

	return 0;

}
void CIFetcherFileMap::Close()
{
	if(hFileMapping) CloseHandle(hFileMapping);
	if(hFile) CloseHandle(hFile);
}

//void CIFetcherFileMap::Adjust()
//{
//
//		//
//		// linear address=physical address
//		//
//			int current_a=(address16_mode)?(ip&0xffff):ip;
//
//		if (current_a<starting_address||starting_address+dwSize<current_a)
//		{
//			buffer=NULL;  // current ip not mapped.
//			bytes=0;
//		}
//		else
//		{
//			buffer=lpszData+(current_a-starting_address);
//			bytes=dwSize-(current_a-starting_address);
//		}
//}

//void CIFetcherFileMap::set_fetch_address(bool mode, unsigned int _ip_)
//{
//
//	CFetcherBase::set_fetch_address( mode,_ip_);
//	
//	Adjust();
//}
bool CIFetcherMemory::set_fetch_address(bool mode, unsigned int _ip_,unsigned short seg)
{

	CFetcherBase::set_fetch_address( mode,    _ip_);
		//
		// linear address=physical address
		//
		unsigned int current_a=(address16_mode)?(seg*16+ip):_ip_;

		if (current_a<starting_address||starting_address+dwSize<current_a)
		{
			buffer=NULL;  // current ip not mapped.
			bytes=0;
		}
		else
		{
			buffer=lpszData+(current_a-starting_address);
			bytes=dwSize-(current_a-starting_address);
		}
		return buffer!=NULL;
}


//
//void CIFetcherFileMap::Close()
//{
//	if(hFileMapping) CloseHandle(hFileMapping);
//	if(hFile) CloseHandle(hFile);
//}
//
////void CIFetcherFileMap::Adjust()
////{
////
////		//
////		// linear address=physical address
////		//
////			int current_a=(address16_mode)?(ip&0xffff):ip;
////
////		if (current_a<starting_address||starting_address+dwSize<current_a)
////		{
////			buffer=NULL;  // current ip not mapped.
////			bytes=0;
////		}
////		else
////		{
////			buffer=lpszData+(current_a-starting_address);
////			bytes=dwSize-(current_a-starting_address);
////		}
////}
//
////void CIFetcherFileMap::set_fetch_address(bool mode, unsigned int _ip_)
////{
////
////	CFetcherBase::set_fetch_address( mode,_ip_);
////	
////	Adjust();
////}
//bool CIFetcherMemory::set_fetch_address(bool mode, unsigned int _ip_,unsigned short seg)
//{
//
//	CFetcherBase::set_fetch_address( mode,    _ip_);
//		//
//		// linear address=physical address
//		//
//		unsigned int current_a=(address16_mode)?(seg*16+ip):_ip_;
//
//		if (current_a<starting_address||starting_address+dwSize<current_a)
//		{
//			buffer=NULL;  // current ip not mapped.
//			bytes=0;
//		}
//		else
//		{
//			buffer=lpszData+(current_a-starting_address);
//			bytes=dwSize-(current_a-starting_address);
//		}
//		return buffer!=NULL;
//}