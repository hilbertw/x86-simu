#ifndef __vgaemu_h__
#define __vgaemu_h__
class CVGAEmu
{

int page;
int mode;
BITMAP bm;
BITMAPINFOHEADER bi;
LPBITMAPINFOHEADER lpbi;
DWORD dwLen;
HANDLE hDIB;
HANDLE handle;
HDC hDC;
HPALETTE hPal;

};

#endif //__vgaemu_h__