// The Win32 HelloWorld program.
//
// Copyright (c) 2021 Jorge Ramos (mailto jramos at pobox dot com)
//
// This is free software. Redistribution and use in source and binary forms,
// with or without modification, for any purpose and with or without fee are
// hereby permitted. Altered source versions must be plainly marked as such.
//
// If you find this software useful, an acknowledgment would be appreciated
// but is not required.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
// FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.

#include <Windows.h>

const TCHAR szClassName[] = TEXT("HelloWorld Window Class");
const TCHAR szWindowName[] = TEXT("HelloWorld");
const TCHAR szRegisterClassError[] = TEXT("Could not register the window class.");
const TCHAR szCreateWindowError[] = TEXT("Could not create the main window.");
const TCHAR szGetMessageError[] = TEXT("Could not retrieve a message from the thread's message queue.");

void ErrorBox(LPCTSTR lpszText)
{
    MSG msg;

    while (PeekMessage(&msg, (HWND)-1, WM_QUIT, WM_QUIT, PM_REMOVE))
    {
    }

    MessageBox(NULL, lpszText, NULL, MB_OK | MB_ICONERROR);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc;
    HWND hwnd;
    BOOL bRet;
    MSG msg;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Register the window class.

    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = szClassName;

    if (!RegisterClass(&wc))
    {
        ErrorBox(szRegisterClassError);
        return FALSE;
    }

    // Create the window.

    hwnd = CreateWindow(szClassName, szWindowName, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        ErrorBox(szCreateWindowError);
        return FALSE;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Run the message loop.

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            ErrorBox(szGetMessageError);
            msg.wParam = FALSE;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Exit program.

    return (int)msg.wParam;
}

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")