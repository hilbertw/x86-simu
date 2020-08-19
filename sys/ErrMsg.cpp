// 
//  @(#) errmsg.cpp   1998.1.8
//
// 
//
#include <windows.h>

void LastErrorMsg()
{
	LPVOID lpMsgBuf;
 
	FormatMessage( 
	    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
	    NULL,
	    GetLastError(),
	    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	    (LPTSTR) &lpMsgBuf,
	    0,
	    NULL 
	);

	// Display the string.
	MessageBox( NULL, (LPTSTR)lpMsgBuf, L"GetLastError", MB_OK|MB_ICONINFORMATION );

	// Free the buffer.
	LocalFree( lpMsgBuf );
}

