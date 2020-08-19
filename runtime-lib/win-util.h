#ifndef __WINUTIL_H__
#define __WINUTIL_H__

#include <windows.h>
#ifdef __cplusplus
extern "C"{

#endif

void cdecl LastErrorMsg();


int getfilename (HWND hWnd, LPWSTR wcsFilter, LPWSTR wcsFile, int cchFile);
//
// 0:success, 1, err in src, -1, err in dst
// call getlasterror to get details
//
int copy_file(LPCWSTR  dst,LPCWSTR  src);


#ifdef __cplusplus
}
#endif 

#endif // __WINUTIL_H__