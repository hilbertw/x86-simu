// vt.cpp : Defines the entry point for the application.
//
#include "windows.h"
#include "tchar.h"
#include "vt.h"
#include "vga.h"

//
// bug om vs 2005
//
#if _MSC_VER==1400
#define MAPVK_VK_TO_VSC 0
#define MAPVK_VSC_TO_VK 1
#define MAPVK_VK_TO_CHAR 2
#define MAPVK_VSC_TO_VK_EX 3
#endif

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInstance;								// current instance
HWND hDlg,hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
extern HDC hScrDC;
//extern unsigned char ascii_to_scancode_table[];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass();
BOOL				InitInstance( int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Debug(HWND , UINT , WPARAM , LPARAM );

bool create_bitmap_16color(HDC hDC);
void key_hit( char scan_code, char ascii);
void reset(int i);
int APIENTRY _tWinMain(HINSTANCE hInst,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInst, IDC_VT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass();

	hInstance=hInst;

	// Perform application initialization:
	if (!InitInstance ( nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_VT));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW|CS_NOCLOSE;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_VT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));



	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(int nCmdShow)
{
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED,
      CW_USEDEFAULT, CW_USEDEFAULT, 
	  640+GetSystemMetrics(SM_CXFIXEDFRAME), 
	  480+GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYMENU)+GetSystemMetrics(SM_CYCAPTION), 
	  NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   hDlg=CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DEBUG),hWnd,Debug);
   if (!hDlg)
   {
      return FALSE;
   }
   HDC hDC=GetDC(hWnd);

   if(!create_bitmap_16color(hDC))
   {
	   ReleaseDC(hWnd,hDC);
	   DestroyWindow(hWnd);
      return FALSE;
   }
   ReleaseDC(hWnd,hDC);

   ShowWindow(hDlg, nCmdShow);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}




void ErrorMsg(); 
HACCEL hAccelTable;
bool  vt_init(HINSTANCE hInst )
{
	WNDCLASSEX wcex;

	hInstance=hInst;

	wcex.cbSize = sizeof(WNDCLASSEX);
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	wsprintf(szWindowClass,_T("vt-%x"),hInstance);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_VT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);



// Perform application initialization:
	if (!InitInstance (SW_SHOWNORMAL))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VT));


	//
	// initialize vga
	//
	write_vga_reg(3,1);// 3c3
	write_vga_reg(2,(char)0xc3);// 3c2,480 lines, color,


	

	return TRUE;


}
//
// return false if wm_exit;
//
bool __cdecl vt_msg_loop_once()
{
	MSG msg;
	// run  message loop once:
	if (PeekMessage(&msg, NULL, 0, 0,false))
	{
		if(GetMessage(&msg, NULL, 0, 0))
		{

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else 
			return false;
	}
		return true;
}




void vt_speaker(bool on )
{
	if(on)
	MessageBeep(0);
}


DWORD WINAPI init_vt( HINSTANCE hInst )
{

	WNDCLASSEX wcex;

	hInstance=hInst;

	wcex.cbSize = sizeof(WNDCLASSEX);
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	wsprintf(szWindowClass,_T("vt-%x"),hInstance);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_VT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	int i=RegisterClassEx(&wcex);



// Perform application initialization:
	if (!InitInstance (SW_SHOWNORMAL))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VT));


	//
	// initialize vga
	//
	write_vga_reg(3,1);// 3c3
	write_vga_reg(2,(char)0xc3);// 3c2,480 lines, color,
	//extern unsigned char text[];

	return true;
}


DWORD WINAPI msg_thread( LPVOID lpParam )
{
	MSG msg;
	if(!vt_init((HINSTANCE)lpParam))
	{
		return 0;
	}


	while(GetMessage(&msg, NULL, 0, 0)!=0)
	{

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	 return msg.wParam;
}