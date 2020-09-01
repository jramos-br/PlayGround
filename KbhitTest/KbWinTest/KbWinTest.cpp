// Windows application input from keyboard.
//
// Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
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

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <stdlib.h>

const TCHAR _szMainWindowClassName[] = _T("KbWinTestMainWindow");
const TCHAR _szMainWindowClassTitle[] = _T("KbWinTest");

HINSTANCE _hInstance;

int FormatStringVa(LPTSTR lpszBuffer, size_t cchBuffer, LPCTSTR lpszFormat, va_list args)
{
    int cchFormatted;
#ifdef _TRUNCATE
    cchFormatted = _vsntprintf_s(lpszBuffer, cchBuffer, _TRUNCATE, lpszFormat, args);
#else
    cchFormatted = _vsntprintf(lpszBuffer, cchBuffer, lpszFormat, args);

    if (cchFormatted < 0)
        lpszBuffer[cchBuffer - 1] = _T('\0');
    else if ((size_t)cchFormatted >= cchBuffer)
        lpszBuffer[cchBuffer - 1] = _T('\0'), cchFormatted = -1;
#endif
    return cchFormatted;
}

int FormatString(LPTSTR lpszBuffer, size_t cchBuffer, LPCTSTR lpszFormat, ...)
{
    int cchFormatted;
    va_list args;
    va_start(args, lpszFormat);

    cchFormatted = FormatStringVa(lpszBuffer, cchBuffer, lpszFormat, args);

    va_end(args);
    return cchFormatted;
}

void Trace(LPCTSTR lpszText)
{
    size_t cchFormatted = _tcslen(lpszText);

    if (cchFormatted > 0)
    {
        if (lpszText[cchFormatted - 1] != _T('\n'))
        {
            TCHAR szBuffer[256];
            int cchBuffer = _countof(szBuffer) - 2;
            FormatString(szBuffer, _countof(szBuffer), _T("%.*s\n"), cchBuffer, lpszText);
            OutputDebugString(szBuffer);
        }
        else
            OutputDebugString(lpszText);
    }
}

void TraceMessageVa(LPCTSTR lpszFormat, va_list args)
{
    TCHAR szBuffer[256];
    int cchFormatted;

    cchFormatted = FormatStringVa(szBuffer, _countof(szBuffer) - 1, lpszFormat, args);

    if (cchFormatted < 0)
        cchFormatted = (int)_tcslen(szBuffer);
    if (cchFormatted > 0 && szBuffer[cchFormatted - 1] != _T('\n'))
        szBuffer[cchFormatted] = _T('\n'), szBuffer[cchFormatted + 1] = _T('\0');

    OutputDebugString(szBuffer);
}

void TraceMessage(LPCTSTR lpszFormat, ...)
{
    va_list args;
    va_start(args, lpszFormat);

    TraceMessageVa(lpszFormat, args);

    va_end(args);
}

LPTSTR FormatWin32Message(DWORD dwMessageId, LPTSTR lpszBuffer, size_t cchBuffer)
{
    static const TCHAR szDefaultWin32Message[] = _T("Win32 Message ID 0x%08lX (%ld)");
    static const DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS;

    LPTSTR lpszMessage = NULL;
    DWORD dwCount = FormatMessage(dwFlags, NULL, dwMessageId, 0, (LPTSTR)&lpszMessage, 0, NULL);

    if (lpszMessage != NULL)
    {
        while (dwCount > 0 && (lpszMessage[dwCount - 1] == _T('\n') || lpszMessage[dwCount - 1] == _T('\r')))
            lpszMessage[--dwCount] = _T('\0');

        if (dwCount > 0)
            FormatString(lpszBuffer, cchBuffer, _T("%s"), lpszMessage);
        else
            FormatString(lpszBuffer, cchBuffer, szDefaultWin32Message, (long)dwMessageId, (long)dwMessageId);

        LocalFree(lpszMessage);
    }
    else
        FormatString(lpszBuffer, cchBuffer, szDefaultWin32Message, (long)dwMessageId, (long)dwMessageId);

    return lpszBuffer;
}

void TraceWin32Message(DWORD dwMessageId, LPCTSTR lpszSource)
{
    TCHAR szMessage[256];
    FormatWin32Message(dwMessageId, szMessage, _countof(szMessage));

    if (lpszSource != NULL)
        TraceMessage(_T("%s: %s"), lpszSource, szMessage);
    else
        Trace(szMessage);
}

void OnDestroy(HWND hwnd)
{
    UNREFERENCED_PARAMETER(hwnd);
    PostQuitMessage(EXIT_SUCCESS);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    if (hdc != NULL)
    {
        EndPaint(hwnd, &ps);
    }
    else
    {
        TraceWin32Message(ERROR_DC_NOT_FOUND, _T("BeginPaint"));
    }
}

void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    UNREFERENCED_PARAMETER(hwnd);
    UNREFERENCED_PARAMETER(fDown);
    UNREFERENCED_PARAMETER(cRepeat);

    TraceMessage(_T("vk=%08X flags=%08X"), vk, flags);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hwnd, WM_KEYDOWN, OnKey);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL InitApplication(HINSTANCE hInstance)
{
    WNDCLASS wc;

    _hInstance = hInstance;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = _szMainWindowClassName;

    if (RegisterClass(&wc) == 0)
    {
        TraceWin32Message(GetLastError(), _T("RegisterClass"));
        return FALSE;
    }

    return TRUE;
}

BOOL InitInstance(int nCmdShow)
{
    LPCTSTR lpClassName = _szMainWindowClassName;
    LPCTSTR lpWindowName = _szMainWindowClassTitle;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    int x = CW_USEDEFAULT;
    int y = 0;
    int nWidth = CW_USEDEFAULT;
    int nHeight = 0;
    HWND hwndParent = NULL;
    HMENU hMenu = NULL;
    HINSTANCE hInstance = _hInstance;
    LPVOID lpParam = NULL;

    HWND hwnd = CreateWindow(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight,
        hwndParent, hMenu, hInstance, lpParam);

    if (!hwnd)
    {
        TraceWin32Message(GetLastError(), _T("CreateWindow"));
        return FALSE;
    }

    ShowWindow(hwnd, nCmdShow);

    if (!UpdateWindow(hwnd))
    {
        TraceWin32Message(GetLastError(), _T("UpdateWindow"));
        return FALSE;
    }

    return TRUE;
}

int PumpMessages(void)
{
    MSG msg;
    int retval;

    while ((retval = (int)GetMessage(&msg, NULL, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (retval < 0)
    {
        TraceWin32Message(GetLastError(), _T("GetMessage"));
        return EXIT_FAILURE;
    }

    return (int)msg.wParam;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpzCmdLine, int nCmdShow)
{
    int retval = EXIT_FAILURE;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpzCmdLine);

    if (InitApplication(hInstance))
    {
        if (InitInstance(nCmdShow))
        {
            retval = PumpMessages();
        }
    }

    return retval;
}
