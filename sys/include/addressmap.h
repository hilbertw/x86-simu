#ifndef __ADDRESSMAP_H__
#define __ADDRESSMAP_H__

#include <windows.h>

typedef enum address_range_cmd_def { 
	ADDR_RANGE_READ,            // Bus read
	ADDR_RANGE_WRITE,           // Bus write
	ADDR_RANGE_DEL              // delete an address range
} ADDR_RANGE_CMD;



typedef void (*ADDR_RANGE_HANDLER)(
	  void *lpContext,
	  ADDR_RANGE_CMD op, 
	  unsigned int dwAddress,           // start dwAddress on 32bit bus
	  char *pszBuff,           // optional 
	  unsigned int nBytes );            // in or out buffer



typedef struct file_map_def {
	HANDLE hFile;
	HANDLE hFileMapping;
	LPBYTE lpFileBase;
	unsigned long startingAddress;
	unsigned long dwBytes;
	bool dwReadOnly;
} FILE_MAP;


typedef struct alias_map_def {
	void * alias;
	unsigned long dwMask;
} ALIAS_MAP;
template<class T >
class TAddressMap
{
	template <class Y>
	struct address_range_def
	{
		 struct address_range_def *pNext;

		Y dwStart;
		Y dwSize;
		bool bReadOnly;
			
		ADDR_RANGE_HANDLER pfnHandler;

	};
	
	typedef address_range_def<T> ADDRESS_RANGE;

	ADDRESS_RANGE *pAddressRange;

public:
	TAddressMap() : pAddressRange(NULL) {}
	~TAddressMap();

	void *RegisterAddressRange( 
		ADDR_RANGE_HANDLER f, 
		T dwStart,
		T dwBytes,
		int dwContextSize );    // extra bytes allocated for the handler.

	char ReadByte(T dwAddress);
	short ReadWord(T dwAddress);
	int ReadDWord(T dwAddress);

	bool ReadString(T dwAddress,char *pzBuff,int dwCount);

	bool WriteByte(T dwAddress,char bData) ;
	bool WriteWord(T dwAddress,short wData);
	bool WriteDWord(T dwAddress,int dwData);

	bool WriteString(T dwAddress,char *pszBuff,int dwCount);


	bool ReadByte(T dwAddress,char * dddat);
	bool ReadWord(T dwAddress,short *dddat);
	bool ReadDWord(T dwAddress,int *dddat);

	LONG RegisterAddressMapToFile( 
		LPCWSTR wscFileName,   // file to map
		LONG dwOffset,         // bytes to skip from the beginning of the file
		LONG dwBytes,          // bytes to map
		LONG dwStart);         // starting address of the address range

	LONG RegisterAddressMapToFileRW( 
		LPCWSTR wscFileName,   // file to map
		LONG dwOffset,         // bytes to skip from the beginning of the file
		LONG dwBytes,          // bytes to map
		LONG dwStart);         // starting address of the address range

	LONG RegisterAddressAlias( 
		LONG dwStart,          // bytes to map
		LONG dwBytes,         // starting address of the address range
		LONG dwMask );         // mask address to form the alias address
	static void FileMapHandler (
		void *lpContext,          // registration context  
		ADDR_RANGE_CMD op,              // operation 
		unsigned int dwAddress,           // start dwAddress in a 32bit bus
		char *pszBuff,           // optional 
		unsigned int nBytes);             // in or out buffer
	static void AliasMapHandler (
		void *lpContext,          // registration context  
		ADDR_RANGE_CMD op,              // operation 
		unsigned int dwAddress,           // start dwAddress in a 32bit bus
		char *pszBuff,           // optional 
		unsigned int nBytes);             // in or out buffer
protected:
	bool BusOp(ADDR_RANGE_CMD op,T dwAddress, char *pszData, int dwBytes);
	void *LocateAddressBlock( T dwAddress);
	void *AllocAddressRangeBlock(int dwContextSize);
};

#endif   /*   __ADDRESSMAP_H__  */