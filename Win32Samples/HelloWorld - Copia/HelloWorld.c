// The HelloWorld program.
//
// Creates a window and displays some text.
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

int GetScreenFontHeight(int nPointSize)
{
    int nHeight = 0;
    HDC hdc = GetDC(NULL);
    if (hdc != NULL)
    {
        int dpi = GetDeviceCaps(hdc, LOGPIXELSY);
        nHeight = -MulDiv(nPointSize, dpi, 72);
        ReleaseDC(NULL, hdc);
    }
    return nHeight;
}

HFONT CreateTitleFont()
{
    int cHeight = GetScreenFontHeight(32);
    int cWidth = 0;
    int cEscapement = 0;
    int cOrientation = 0;
    int cWeight = FW_NORMAL;
    DWORD bItalic = FALSE;
    DWORD bUnderline = FALSE;
    DWORD bStrikeOut = FALSE;
    DWORD iCharSet = DEFAULT_CHARSET;
    DWORD iOutPrecision = OUT_OUTLINE_PRECIS;
    DWORD iClipPrecision = CLIP_DEFAULT_PRECIS;
    DWORD iQuality = CLEARTYPE_QUALITY;
    DWORD iPitchAndFamily = VARIABLE_PITCH;
    LPCTSTR pszFaceName = TEXT("Arial");
    HFONT hFont = CreateFont(cHeight, cWidth, cEscapement, cOrientation, cWeight,
                             bItalic, bUnderline, bStrikeOut, iCharSet,
                             iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily,
                             pszFaceName);
    return hFont;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;
    HFONT hFont;
    HFONT hOldFont;
    LPCTSTR lpszWelcomeText = TEXT("Welcome to the HelloWorld program");

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        hFont = CreateTitleFont();
        if (hFont != NULL)
        {
            if ((hOldFont = (HFONT)SelectObject(hdc, hFont)) != NULL)
            {
                GetClientRect(hwnd, &rect);
                DrawText(hdc, lpszWelcomeText, -1, &rect, DT_NOPREFIX | DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(hdc, hOldFont);
            }
            DeleteObject(hFont);
        }
        EndPaint(hwnd, &ps);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LPCTSTR lpszClassName = TEXT("HelloWorld Window Class");
    LPCTSTR lpszWindowName = TEXT("HelloWorld");
    LPCTSTR lpszRegisterClassError = TEXT("Could not register the window class.");
    LPCTSTR lpszCreateWindowError = TEXT("Could not create the main window.");

    // Register the window class.

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = lpszClassName;

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, lpszRegisterClassError, NULL, MB_ICONERROR);
        return 0;
    }

    // Set the window position and size.

    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
    int nWidth = CW_USEDEFAULT;
    int nHeight = CW_USEDEFAULT;

    // Create the window.

    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    HWND hwndParent = NULL;
    HMENU hMenu = NULL;
    LPVOID lpParam = NULL;

    HWND hwnd = CreateWindow(lpszClassName, lpszWindowName, dwStyle,
                             x, y, nWidth, nHeight,
                             hwndParent, hMenu, hInstance, lpParam);
    if (hwnd == NULL)
    {
        MessageBox(NULL, lpszCreateWindowError, NULL, MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Run the message loop.

    BOOL bRet;
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            msg.wParam = FALSE;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unregister the window class.

    UnregisterClass(lpszClassName, hInstance);

    // Exit program.

    return (int)msg.wParam;
}

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
