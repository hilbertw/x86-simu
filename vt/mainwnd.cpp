#include "windows.h"
#include "vt.h"
#include "vga.h"
#ifndef MAPVK_VK_TO_VSC
#define MAPVK_VK_TO_VSC 0
#endif
#ifndef MAPVK_VSC_TO_VK
#define MAPVK_VSC_TO_VK 1
#endif
#ifndef MAPVK_VK_TO_CHAR
#define MAPVK_VK_TO_CHAR 2
#endif
#ifndef MAPVK_VSC_TO_VK_EX
#define MAPVK_VSC_TO_VK_EX 3
#endif

#define MAX_LOADSTRING 100

// Global Variables:
extern HINSTANCE hInstance;								// current instance
extern HWND hDlg,hWnd;
extern HDC hScrDC;
//extern unsigned char ascii_to_scancode_table[];
extern unsigned char scancode_to_ascii_table[];
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass();
BOOL				InitInstance( int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Debug(HWND , UINT , WPARAM , LPARAM );
INT_PTR CALLBACK	FloppyDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	HDDDlgProc(HWND, UINT, WPARAM, LPARAM);

bool create_bitmap_16color(HDC hDC);
void key_hit( char scan_code);
void reset(int i);
void add_redraw_rect(RECT *rect);
void post_main_thread_msg(char msg);

// Message handler for about box.
INT_PTR CALLBACK Debug(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	}
	return (INT_PTR)FALSE;
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent,scancode;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_FILE_LOAD:
			post_main_thread_msg('l');
			break;
		case ID_FILE_SAVE:
			post_main_thread_msg('s');
			break;
		case ID_HACK_INVERTTRACE:
			post_main_thread_msg('t');
			break;
		case ID_HACK_INVERTSTEP:
			post_main_thread_msg('p');
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		case ID_SYSTEM_RESET:
			reset(3);
			break;
		case ID_FLOPPY:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_MOUNT_FLOPPY), hWnd, FloppyDlgProc);
			break;
		case ID_HDD:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_MOUNT_HDD), hWnd, HDDDlgProc);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SYSKEYUP:
//		if((unsigned char)wParam!=0x79)	break;
		scancode=(unsigned char)(lParam>>16);
		key_hit(scancode|0x80);
		if((lParam&0x2000000)!=0)
			key_hit(0xb8);
		break;
		//
		//F10 fall through
		//
	case WM_KEYUP:
		scancode=(unsigned char)(lParam>>16);
		key_hit(scancode|0x80);
		break;
	case WM_SYSKEYDOWN:
		//if((unsigned char)wParam!=0x79)	
		//{
		//
		// ALT pressed?
		//
		if((lParam&0x2000000)!=0)
			key_hit(0x38);
		//	break;
		//}
		//
		//F10 fall through
		//
	case WM_KEYDOWN:

		scancode=(unsigned char)(lParam>>16);
		key_hit(scancode);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//// TODO: Add any drawing code here...
		//BitBlt(hdc,
		//	ps.rcPaint.left,ps.rcPaint.top,
		//	ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top,
		//	hScrDC,ps.rcPaint.left,ps.rcPaint.top,
		//	SRCCOPY);
		add_redraw_rect(&ps.rcPaint);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

