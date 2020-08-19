#include <windows.h>
#include <stdio.h>

void DbgDumpMem(char * _start,long _size )
{
LONG i;

for( i=0;i<(LONG)_size;i+=16 ){
		LONG j,k;
		CHAR c;

		k=i+16;
		if( k>(LONG)_size ) k=(LONG)_size;
		for( j=i;j<k;j++) {
			printf("%02x ",((PUCHAR)_start)[j] );
		}
		
		if( k<i+16 )
		for( ;j<(LONG)i+16 ;j++ ) printf("   ");
		for( j=i;j<k;j++) 
			printf("%c",(c=((PUCHAR)_start)[j])<' '?'.':(c<0x80?c:'.') );
			
		printf("\n" );
}
}

void ErrorMsg() 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;

    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process


    MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);

}

