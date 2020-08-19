#include<assert.h>

#include "windows.h"
#include "win-util.h"
#include "resource.h"
#include "sys.h"
WCHAR wcsFloppyImage[2][MAX_PATH];	
WCHAR wcsHddImage[4][MAX_PATH];	
WORD dwFloppySel,dwHddSel;
/*
bool floppy_mount(wchar_t * wcsImage, int drive_no,bool write_protected);
void floppy_unmount(int drive_no);
bool ide_mount(wchar_t * wcsImage, int drive_no,int size, bool cdrom);
void ide_unmount( int drive_no);
*/
// Message handler for about box.
INT_PTR CALLBACK HDDDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);


	switch (message)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg,IDC_PriMaster,IDC_SecSlave,IDC_HDD_BASE+dwHddSel);
		CheckDlgButton(hDlg,IDC_CHECK1,1);
		SetDlgItemTextW(hDlg,IDC_EDIT1,wcsHddImage[dwHddSel]);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BROWSER )
		{
			OPENFILENAMEW ofn;       // common dialog box structure
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = (LPWSTR)wcsHddImage[dwHddSel];  // buffer for file name
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = L'\0';
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = L"Ide raw image\0*.raw\0cdrom image\0*.iso\0All files\0*.*\0\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = L"Open Hard disk/cdrom/dvd image";
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST|OFN_NOREADONLYRETURN;

			// Display the Open dialog box. 

			if(GetOpenFileNameW(&ofn))
			{
				SetDlgItemTextW(hDlg,IDC_EDIT1,wcsHddImage[dwHddSel]);
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_MOUNT)
		{

			ide_mount(wcsHddImage[dwHddSel],dwHddSel,0,false);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_UNMOUNT )
		{
			ide_unmount(dwHddSel);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam)>= IDC_PriMaster&& LOWORD(wParam) <= IDC_SecSlave )
		{
			dwHddSel=LOWORD(wParam)-IDC_PriMaster;
			SetDlgItemTextW(hDlg,IDC_EDIT1,wcsHddImage[dwHddSel]);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK FloppyDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);


	switch (message)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg,IDC_FLOPPYA,IDC_FLOPPYA,IDC_FLOPPY_BASE+dwFloppySel);
		CheckDlgButton(hDlg,IDC_CHECK1,1);
		SetDlgItemTextW(hDlg,IDC_EDIT1,wcsFloppyImage[dwFloppySel]);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BROWSER )
		{
			OPENFILENAMEW ofn;       // common dialog box structure
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = (LPWSTR)wcsFloppyImage[dwFloppySel];  // buffer for file name
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = L'\0';
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = L"floppy raw image\0*.img;*.ima;*.flp\0All files\0*.*\0\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = L"Open floppy disk image";
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST|OFN_NOREADONLYRETURN;

			// Display the Open dialog box. 

			if(GetOpenFileNameW(&ofn))
			{
				SetDlgItemTextW(hDlg,IDC_EDIT1,wcsFloppyImage[dwFloppySel]);
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_MOUNT)
		{
			bool write_protected=IsDlgButtonChecked(hDlg,IDC_WRITE_PROTECTED);

			floppy_mount(wcsFloppyImage[dwFloppySel],dwFloppySel,write_protected);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_UNMOUNT )
		{
			floppy_unmount(dwFloppySel);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam)>= IDC_FLOPPYA&& LOWORD(wParam) <= IDC_FLOPPYB )
		{
			dwFloppySel=LOWORD(wParam)-IDC_FLOPPYA;
			SetDlgItemTextW(hDlg,IDC_EDIT1,wcsFloppyImage[dwFloppySel]);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}