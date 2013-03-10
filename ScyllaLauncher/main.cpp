#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

#pragma comment (lib, "Comctl32.lib")

#define SMALL 1

#ifdef SMALL
#pragma comment(linker, "/ENTRY:WinMain")
#endif

#define SCYLLA_NAME_X86 "Scylla_x86.exe"
#define SCYLLA_NAME_X64 "Scylla_x64.exe"

typedef void (WINAPI *def_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);

WCHAR scyllaX86[] = TEXT(SCYLLA_NAME_X86);
WCHAR scyllaX64[] = TEXT(SCYLLA_NAME_X64);

HINSTANCE hInst;
STARTUPINFO si = {0};
PROCESS_INFORMATION pi = {0};
SYSTEM_INFO sysi = {0};
def_GetNativeSystemInfo _GetNativeSystemInfo = 0;

BOOL isWindows64()
{
	_GetNativeSystemInfo = (def_GetNativeSystemInfo)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
	if (_GetNativeSystemInfo)
	{
		_GetNativeSystemInfo(&sysi);
	}
	else
	{
		GetSystemInfo(&sysi);
	}

	if (sysi.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)));

		if (!isWindows64())
		{
			EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_X64), FALSE);
		}
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDC_BTN_X86:
        {
            SetFocus(GetDlgItem(hwndDlg, IDC_STC_FOCUS));
            si.cb = sizeof(si);
            if(!CreateProcess(0, scyllaX86, 0, 0, FALSE, 0, 0, 0, &si, &pi))
            {
                MessageBox(hwndDlg, TEXT("Failed to start ")TEXT(SCYLLA_NAME_X86), TEXT("Error!"), MB_ICONERROR);
                return TRUE;
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            SendMessage(hwndDlg, WM_CLOSE, 0, 0);
        }
        return TRUE;

        case IDC_BTN_X64:
        {
            SetFocus(GetDlgItem(hwndDlg, IDC_STC_FOCUS));
            si.cb = sizeof(si);
            if(!CreateProcess(0, scyllaX64, 0, 0, FALSE, 0, 0, 0, &si, &pi))
            {
                MessageBox(hwndDlg, TEXT("Failed to start ")TEXT(SCYLLA_NAME_X64), TEXT("Error!"), MB_ICONERROR);
                return TRUE;
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            SendMessage(hwndDlg, WM_CLOSE, 0, 0);
        }
        return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef SMALL
	hInst = GetModuleHandle(0);
#else
    hInst = hInstance;
#endif

    InitCommonControls();
	return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), 0, (DLGPROC)DlgMain);
}
