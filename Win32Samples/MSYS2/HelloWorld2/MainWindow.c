/*
 * The Win32 HelloWorld program main window.
 *
 * Copyright (c) 2021 Jorge Ramos (mailto jramos at pobox dot com)
 *
 * This is free software. Redistribution and use in source and binary forms,
 * with or without modification, for any purpose and with or without fee are
 * hereby permitted. Altered source versions must be plainly marked as such.
 *
 * If you find this software useful, an acknowledgment would be appreciated
 * but is not required.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
 * FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.
 */

#include "MainWindow.h"
#include "AboutDialog.h"
#include "AppData.h"
#include "Resource.h"
#include "StrUtil.h"
#include "WinMain.h"
#include "WinUtil.h"
#include "AccUtil.h"
#include "Debug.h"
#include "VirtualKeyUtil.h"

static LPCTSTR m_pszClassName = TEXT("HelloWorld Main Window Class");
static LPCTSTR m_pszWindowName = TEXT("HelloWorld");
static LPCTSTR m_pszWelcomeText = TEXT("Welcome to the HelloWorld program");
static LPCTSTR m_pszHelpAboutMenuItem = TEXT("About");

static DWORD m_iPitchAndFamily = WINUTIL_FF_SWISS;
static int m_nPointSize = 32;

static ACCEL m_aceleratorTable[] = {
    { FCONTROL, 'Z', IDM_EDIT_UNDO },
    { FCONTROL, 'Y', IDM_EDIT_REDO },
    { FCONTROL, 'X', IDM_EDIT_CUT },
    { FCONTROL, 'C', IDM_EDIT_COPY },
    { FCONTROL, 'V', IDM_EDIT_PASTE }
};

/* If the function succeeds, the return value is the handle to the created accelerator table;
   otherwise, it is NULL. To get extended error information, call GetLastError. */
static HACCEL MainWindow_CreateAcceleratorTable()
{
    HACCEL haccel = CreateAcceleratorTable(m_aceleratorTable, _countof(m_aceleratorTable));

    if (haccel == NULL)
    {
    }

    return haccel;
}

static void ShowAboutDialog(HWND hwnd)
{
    if (!AboutDialog_ShowModal(hwnd))
    {
        DWORD dwLastError = GetLastError();
        LPCTSTR pszApplicationName = AppData_GetApplicationName();
        LPCTSTR pszMessage = TEXT("DialogBox has failed");
        WinUtil_DisplayWin32ErrorBox3(hwnd, pszApplicationName, dwLastError, NULL, pszMessage);
    }
}

static BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    UNREFERENCED_PARAMETER(lpCreateStruct);
    HMENU hSysMenu = GetSystemMenu(hwnd, FALSE);
    AppendMenu(hSysMenu, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    AppendMenu(hSysMenu, MF_BYPOSITION, SCX_HELP_ABOUT, m_pszHelpAboutMenuItem);
    return TRUE;
}

static void OnDestroy(HWND hwnd)
{
    AccUtil_UnregisterWindow(hwnd);
    PostQuitMessage(0);
}

static void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    InvalidateRect(hwnd, NULL, TRUE);
    FORWARD_WM_SIZE(hwnd, state, cx, cy, DefWindowProc);
}

static void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    HFONT hFont = WinUtil_CreateFontByPitchAndFamily(hdc, m_iPitchAndFamily, m_nPointSize);

    if (hFont != NULL)
    {
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        RECT rect;
        GetClientRect(hwnd, &rect);
        DrawText(hdc, m_pszWelcomeText, -1, &rect, DT_NOPREFIX | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
    }

    EndPaint(hwnd, &ps);
}

static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    UNREFERENCED_PARAMETER(hwndCtl);
    UNREFERENCED_PARAMETER(codeNotify);
    switch (id)
    {
    case IDM_FILE_EXIT:
        FORWARD_WM_CLOSE(hwnd, PostMessage);
        break;
    case IDM_HELP_ABOUT:
        ShowAboutDialog(hwnd);
        break;
    }
}

static void OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
    switch (cmd & 0xFFF0)
    {
    case SCX_HELP_ABOUT:
        ShowAboutDialog(hwnd);
        return;
    }

    FORWARD_WM_SYSCOMMAND(hwnd, cmd, x, y, DefWindowProc);
}

static void OnChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    switch (ch)
    {
    case 27: // ESCAPE
        FORWARD_WM_CLOSE(hwnd, PostMessage);
        break;
    default:
        FORWARD_WM_CHAR(hwnd, ch, cRepeat, DefWindowProc);
    }
}

static LRESULT CALLBACK MainWindow_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_SIZE, OnSize);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_SYSCOMMAND, OnSysCommand);
        HANDLE_MSG(hwnd, WM_CHAR, OnChar);

    case WM_INPUTLANGCHANGE:
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static BOOL AppendPopupMenu(HMENU hMenu, HMENU hPopup, LPCTSTR pszNewItem)
{
    return AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPopup, pszNewItem);
}

static BOOL AppendMenuEx(HMENU hMenu, UINT uFlags, WORD uIDNewItem, LPCTSTR pszNewItem, LPACCEL pAccelTable, int cAccelEntries)
{
    TCHAR szTempName[64];

    for (int i = 0; i < cAccelEntries; ++i)
    {
        if (pAccelTable[i].cmd == uIDNewItem)
        {
            TCHAR szKeyName[64];

            if (VirtualKeyUtil_GetVirtualKeyNameText(szKeyName, _countof(szKeyName), pAccelTable[i].key) ||
                VirtualKeyUtil_GetVirtualKeyFormattedName(szKeyName, _countof(szKeyName), pAccelTable[i].key))
            {
                if (StrUtil_Copy(szTempName, _countof(szTempName), pszNewItem) &&
                    StrUtil_Append(szTempName, _countof(szTempName), TEXT("\t")) &&
                    StrUtil_Append(szTempName, _countof(szTempName), szKeyName))
                {
                    pszNewItem = szTempName;
                }
            }

            break;
        }
    }

    return AppendMenu(hMenu, uFlags, uIDNewItem, pszNewItem);
}

static void MainWindow_DestroyMenu(HMENU hMenu)
{
    DWORD dwLastError = GetLastError();

    if (!DestroyMenu(hMenu))
    {
        TraceWin32(GetLastError(), TEXT("MainWindow_DestroyMenu"), TEXT("DestroyMenu has failed"));
    }

    SetLastError(dwLastError);
}

static BOOL MainWindow_CreateFilePopup(HMENU hMenuBar, LPACCEL pAccelTable, int cAccelEntries)
{
    HMENU hPopup = CreatePopupMenu();

    if (hPopup != NULL)
    {
        if (AppendMenuEx(hPopup, MF_STRING, IDM_FILE_EXIT, TEXT("E&xit"), pAccelTable, cAccelEntries))
        {
            if (AppendPopupMenu(hMenuBar, hPopup, TEXT("&File")))
            {
                return TRUE;
            }
        }

        MainWindow_DestroyMenu(hPopup); /* Preserves GetLastError() current value */
    }

    return FALSE;
}

static BOOL MainWindow_CreateEditPopup(HMENU hMenuBar, LPACCEL pAccelTable, int cAccelEntries)
{
    HMENU hPopup = CreatePopupMenu();

    if (hPopup != NULL)
    {
        if (AppendMenuEx(hPopup, MF_STRING, IDM_EDIT_UNDO, TEXT("&Undo"), pAccelTable, cAccelEntries) &&
            AppendMenuEx(hPopup, MF_STRING, IDM_EDIT_REDO, TEXT("&Redo"), pAccelTable, cAccelEntries) &&
            AppendMenuEx(hPopup, MF_SEPARATOR, 0, NULL, NULL, 0) &&
            AppendMenuEx(hPopup, MF_STRING, IDM_EDIT_COPY, TEXT("&Copy"), pAccelTable, cAccelEntries) &&
            AppendMenuEx(hPopup, MF_STRING, IDM_EDIT_CUT, TEXT("Cu&t"), pAccelTable, cAccelEntries) &&
            AppendMenuEx(hPopup, MF_STRING, IDM_EDIT_PASTE, TEXT("&Paste"), pAccelTable, cAccelEntries) &&
            AppendMenuEx(hPopup, MF_STRING, IDM_EDIT_DELETE, TEXT("&Del"), pAccelTable, cAccelEntries))
        {
            if (AppendPopupMenu(hMenuBar, hPopup, TEXT("&Edit")))
            {
                return TRUE;
            }
        }

        MainWindow_DestroyMenu(hPopup); /* Preserves GetLastError() current value */
    }

    return FALSE;
}

static BOOL MainWindow_CreateHelpPopup(HMENU hMenuBar, LPACCEL pAccelTable, int cAccelEntries)
{
    HMENU hPopup = CreatePopupMenu();

    if (hPopup != NULL)
    {
        if (AppendMenuEx(hPopup, MF_STRING, IDM_HELP_WELCOME, TEXT("&Welcome"), pAccelTable, cAccelEntries) &&
            AppendMenuEx(hPopup, MF_STRING, IDM_HELP_ABOUT, TEXT("&About"), pAccelTable, cAccelEntries))
        {
            if (AppendPopupMenu(hMenuBar, hPopup, TEXT("&Help")))
            {
                return TRUE;
            }
        }

        MainWindow_DestroyMenu(hPopup); /* Preserves GetLastError() current value */
    }

    return FALSE;
}

static void MainWindow_HeapFree(LPVOID lpMem)
{
    DWORD dwLastError = GetLastError();

    if (!HeapFree(GetProcessHeap(), 0, lpMem))
    {
        TraceWin32(GetLastError(), TEXT("MainWindow_HeapFree"), TEXT("HeapFree has failed"));
    }

    SetLastError(dwLastError);
}

static HMENU MainWindow_CreateMenu(HACCEL hAccel)
{
    HMENU hMenuBar = CreateMenu();

    if (hMenuBar != NULL)
    {
        LPACCEL pAccelTable = NULL;
        int cAccelEntries = hAccel != NULL ? CopyAcceleratorTable(hAccel, NULL, 0) : 0;

        if (cAccelEntries > 0)
        {
            size_t cbAccelTableSize = cAccelEntries * sizeof(ACCEL);
            pAccelTable = (LPACCEL)HeapAlloc(GetProcessHeap(), 0, cbAccelTableSize);
            CopyAcceleratorTable(hAccel, pAccelTable, cAccelEntries);
        }
        else if (cAccelEntries < 0)
        {
            TraceWin32(E_FAIL, TEXT("MainWindow_CreateMenu"), TEXT("CopyAcceleratorTable has returned negative value"));
            cAccelEntries = 0;
        }

        if (!MainWindow_CreateFilePopup(hMenuBar, pAccelTable, cAccelEntries) ||
            !MainWindow_CreateEditPopup(hMenuBar, pAccelTable, cAccelEntries) ||
            !MainWindow_CreateHelpPopup(hMenuBar, pAccelTable, cAccelEntries))
        {
            MainWindow_DestroyMenu(hMenuBar); /* Preserves GetLastError() current value */
            hMenuBar = NULL;
        }

        if (pAccelTable != NULL)
        {
            MainWindow_HeapFree((LPVOID)pAccelTable); /* Preserves GetLastError() current value */
        }
    }

    return hMenuBar;
}

static BOOL MainWindow_RegisterClass()
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc = MainWindow_WindowProc;
    wc.hInstance = GetInstanceHandle();
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = m_pszClassName;

    return WinUtil_RegisterWindowClass(&wc);
}

HWND MainWindow_CreateWindow()
{
    HWND hwnd = NULL;
    HACCEL haccel = MainWindow_CreateAcceleratorTable();
    HMENU hMenu = MainWindow_CreateMenu(haccel);

    if (MainWindow_RegisterClass())
    {
        /* A menu that is assigned to a window is automatically destroyed when the application closes. */
        hwnd = CreateWindow(m_pszClassName, m_pszWindowName, WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                            NULL, hMenu, GetInstanceHandle(), NULL);

        if (hwnd == NULL)
        {
            DWORD dwLastError = GetLastError();

            if (dwLastError == ERROR_SUCCESS)
            {
                dwLastError = E_FAIL;
            }

            if (!DestroyMenu(hMenu))
            {
                DWORD dwLastError2 = GetLastError();

                if (dwLastError2 != ERROR_INVALID_MENU_HANDLE)
                {
                    TraceWin32(dwLastError2, TEXT("MainWindow_CreateWindow"), TEXT("DestroyMenu has failed"));
                }
            }

            if (!DestroyAcceleratorTable(haccel))
            {
                TraceWin32(GetLastError(), TEXT("MainWindow_CreateWindow"), TEXT("DestroyAcceleratorTable has failed"));
            }

            SetLastError(dwLastError);
        }
        else
        {
            AccUtil_RegisterWindow(hwnd, haccel);
        }
    }

    return hwnd;
}