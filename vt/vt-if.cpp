#define  _CRT_SECURE_NO_WARNINGS 1

#include "windows.h"
#include "tchar.h"
#include "vt.h"

extern HWND hDlg;

void __cdecl vt_wr_port_80(unsigned char dat)
{
	TCHAR buffer[10];
    wsprintf(buffer,_T("%02X"),dat);
	SendDlgItemMessage(hDlg,IDC_port80,WM_SETTEXT,2,(LPARAM)buffer);

}

//void ErrorMsg() ;
//void __cdecl vt_wr_msg(char*msg)
//{
//	HWND hEdit = GetDlgItem (hDlg, IDC_msg);
//   int ndx = GetWindowTextLength (hEdit);
//
//    SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
//    SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM)msg);
//
//}
static TCHAR *szOn=_T("ON");
static TCHAR *szOff=_T("OFF");

void __cdecl vt_show_a20(bool on)
{
	SendDlgItemMessage(hDlg,IDC_A20,WM_SETTEXT,3,(LPARAM)(on?szOn:szOff));
}

void __cdecl vt_set_led(char led)
{
	SendDlgItemMessage(hDlg,IDC_CAPS,WM_SETTEXT,3,(LPARAM)((led&4)?szOn:szOff));
	SendDlgItemMessage(hDlg,IDC_NUM,WM_SETTEXT,3,(LPARAM)((led&2)?szOn:szOff));
	SendDlgItemMessage(hDlg,IDC_SCROLL,WM_SETTEXT,3,(LPARAM)((led&1)?szOn:szOff));
}
void __cdecl vt_show_smram(bool opened, bool enabled)
{
 	TCHAR buffer[20];
	;
	SendDlgItemMessage(hDlg,
		IDC_smram,
		WM_SETTEXT,
		wsprintf(buffer,_T("%s,%s"),opened?_T("Opened"):_T("Closed"),enabled?_T("Enabled"):_T("Disabled")),
		(LPARAM)buffer);
}