/*
 * The Win32 Application.
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

#include "WinApp.h"
#include "AppData.h"
#include "MainWindow.h"
#include "AccUtil.h"
#include "DlgUtil.h"
#include "WndClassList.h"

static const TCHAR m_szApplicationName[] = TEXT("HelloWorld");

static HWND WinApp_CreateMainWindow(int nCmdShow)
{
    HWND hwnd = MainWindow_CreateWindow();

    if (hwnd == NULL)
    {
        WinUtil_DisplayError(GetLastError(), TEXT("MainWindow_CreateWindow has failed"));
        return hwnd;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return hwnd;
}

BOOL WinApp_InitInstance(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    AppData_SetInstance(hInstance);
    AppData_SetApplicationName(m_szApplicationName);

    return WinApp_CreateMainWindow(nCmdShow) != NULL;
}

void WinApp_ExitInstance()
{
    AccUtil_UnregisterAllWindows();
    DlgUtil_UnregisterAllDialogs();
    WndClassList_UnRegisterAll();
    AppData_Free();
}

int WinApp_Run()
{
    MSG msg;
    BOOL bRet;

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            WinUtil_DisplayError(GetLastError(), TEXT("GetMessage has failed"));
            msg.wParam = EXIT_FAILURE;
            break;
        }

        if (!AccUtil_TranslateAccelerator(&msg))
        {
            if (!DlgUtil_IsDialogMessage(&msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return (int)msg.wParam;
}