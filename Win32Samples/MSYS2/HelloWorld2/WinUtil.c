/*
 * Utility functions for Win32 programs.
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

#include "WinUtil.h"
#include "StrUtil.h"
#include "AppData.h"
#include "WndClassList.h"

#define CCHMAXMSGLEN 512

static BOOL WinUtil_FormatWin32MessageSuccess(LPTSTR pszDest, size_t cchDest, LPCTSTR pszMessage, BOOL bAppend)
{
    return bAppend ? StrUtil_Append(pszDest, cchDest, pszMessage)
                   : StrUtil_Copy(pszDest, cchDest, pszMessage);
}

static BOOL WinUtil_FormatWin32MessageError(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, BOOL bAppend)
{
    static const TCHAR szDefaultWin32Message[] = TEXT("Win32 Message ID 0x%08lX (%ld)");
    LONG nMessageId = (long)dwMessageId;
    return bAppend ? StrUtil_AppendFormat(pszDest, cchDest, szDefaultWin32Message, nMessageId, nMessageId)
                   : StrUtil_Format(pszDest, cchDest, szDefaultWin32Message, nMessageId, nMessageId);
}

static BOOL WinUtil_FormatWin32MessageBase(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, BOOL bAppend)
{
    BOOL bResult;
    DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS;
    LPTSTR pszMessage = NULL;
    DWORD dwCount = FormatMessage(dwFlags, NULL, dwMessageId, 0, (LPTSTR)&pszMessage, 0, NULL);

    if (pszMessage != NULL)
    {
        while (dwCount > 0 && (pszMessage[dwCount - 1] == TEXT('\n') || pszMessage[dwCount - 1] == TEXT('\r')))
        {
            pszMessage[--dwCount] = TEXT('\0');
        }

        if (dwCount > 0)
        {
            bResult = WinUtil_FormatWin32MessageSuccess(pszDest, cchDest, pszMessage, bAppend);
        }
        else
        {
            bResult = WinUtil_FormatWin32MessageError(pszDest, cchDest, dwMessageId, bAppend);
        }

        LocalFree(pszMessage);
    }
    else
    {
        bResult = WinUtil_FormatWin32MessageError(pszDest, cchDest, dwMessageId, bAppend);
    }

    if (!bResult)
    {
        StrUtil_WriteEllipsisAtEnd(pszDest, cchDest);
    }

    return bResult;
}

static BOOL WinUtil_FormatWin32MessageBase3(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail, BOOL bAppend)
{
    static const TCHAR szSeparator[] = TEXT(": ");
    BOOL bResult;
    size_t cchSource;
    size_t cchDetail;

    if (StrUtil_GetLength(pszSource, &cchSource) && cchSource > 0)
    {
        if (StrUtil_GetLength(pszDetail, &cchDetail) && cchDetail > 0)
        {
            bResult = bAppend ? StrUtil_Append(pszDest, cchDest, pszSource)
                              : StrUtil_Copy(pszDest, cchDest, pszSource);
            bResult = bResult && StrUtil_Append(pszDest, cchDest, szSeparator);
            bResult = bResult && StrUtil_Append(pszDest, cchDest, pszDetail);
            bResult = bResult && StrUtil_Append(pszDest, cchDest, szSeparator);
            bResult = bResult && WinUtil_FormatWin32MessageBase(pszDest, cchDest, dwMessageId, TRUE);
        }
        else
        {
            bResult = bAppend ? StrUtil_Append(pszDest, cchDest, pszSource)
                              : StrUtil_Copy(pszDest, cchDest, pszSource);
            bResult = bResult && StrUtil_Append(pszDest, cchDest, szSeparator);
            bResult = bResult && WinUtil_FormatWin32MessageBase(pszDest, cchDest, dwMessageId, TRUE);
        }
    }
    else
    {
        if (StrUtil_GetLength(pszDetail, &cchDetail) && cchDetail > 0)
        {
            bResult = bAppend ? StrUtil_Append(pszDest, cchDest, pszDetail)
                              : StrUtil_Copy(pszDest, cchDest, pszDetail);
            bResult = bResult && StrUtil_Append(pszDest, cchDest, szSeparator);
            bResult = bResult && WinUtil_FormatWin32MessageBase(pszDest, cchDest, dwMessageId, TRUE);
        }
        else
        {
            bResult = WinUtil_FormatWin32MessageBase(pszDest, cchDest, dwMessageId, bAppend);
        }
    }

    return bResult;
}

static BOOL WinUtil_FormatWin32MessageBaseExVa(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, BOOL bAppend, LPCTSTR pszFormat, va_list pArgList)
{
    static const TCHAR szSeparator[] = TEXT(" ");
    BOOL bResult = bAppend ? StrUtil_AppendFormatVa(pszDest, cchDest, pszFormat, pArgList)
                           : StrUtil_FormatVa(pszDest, cchDest, pszFormat, pArgList);
    bResult = bResult && StrUtil_Append(pszDest, cchDest, szSeparator);
    bResult = bResult && WinUtil_FormatWin32MessageBase(pszDest, cchDest, dwMessageId, TRUE);
    return bResult;
}

BOOL WinUtil_FormatWin32Message(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId)
{
    return WinUtil_FormatWin32MessageBase3(pszDest, cchDest, dwMessageId, NULL, NULL, FALSE);
}

BOOL WinUtil_AppendWin32Message(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId)
{
    return WinUtil_FormatWin32MessageBase3(pszDest, cchDest, dwMessageId, NULL, NULL, TRUE);
}

BOOL WinUtil_FormatWin32Message2(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource)
{
    return WinUtil_FormatWin32MessageBase3(pszDest, cchDest, dwMessageId, pszSource, NULL, FALSE);
}

BOOL WinUtil_AppendWin32Message2(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource)
{
    return WinUtil_FormatWin32MessageBase3(pszDest, cchDest, dwMessageId, pszSource, NULL, TRUE);
}

BOOL WinUtil_FormatWin32Message3(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail)
{
    return WinUtil_FormatWin32MessageBase3(pszDest, cchDest, dwMessageId, pszSource, pszDetail, FALSE);
}

BOOL WinUtil_AppendWin32Message3(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail)
{
    return WinUtil_FormatWin32MessageBase3(pszDest, cchDest, dwMessageId, pszSource, pszDetail, TRUE);
}

BOOL WinUtil_FormatWin32MessageEx(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszFormat, ...)
{
    BOOL bResult;
    va_list pArgList;
    va_start(pArgList, pszFormat);
    bResult = WinUtil_FormatWin32MessageExVa(pszDest, cchDest, dwMessageId, pszFormat, pArgList);
    va_end(pArgList);
    return bResult;
}

BOOL WinUtil_FormatWin32MessageExVa(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszFormat, va_list pArgList)
{
    return WinUtil_FormatWin32MessageBaseExVa(pszDest, cchDest, dwMessageId, FALSE, pszFormat, pArgList);
}

BOOL WinUtil_AppendWin32MessageEx(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszFormat, ...)
{
    BOOL bResult;
    va_list pArgList;
    va_start(pArgList, pszFormat);
    bResult = WinUtil_AppendWin32MessageExVa(pszDest, cchDest, dwMessageId, pszFormat, pArgList);
    va_end(pArgList);
    return bResult;
}

BOOL WinUtil_AppendWin32MessageExVa(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszFormat, va_list pArgList)
{
    return WinUtil_FormatWin32MessageBaseExVa(pszDest, cchDest, dwMessageId, TRUE, pszFormat, pArgList);
}

BOOL WinUtil_RemoveQuitMessage(WPARAM *pwParam)
{
    MSG msg;
    BOOL bResult = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);

    if (bResult)
    {
        do
        {
            *pwParam = msg.wParam;
        } while (PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE));
    }

    return bResult;
}

int WinUtil_MessageBox(HWND hwndOwner, LPCTSTR pszText, LPCTSTR pszCaption, UINT uType)
{
    WPARAM wParam;
    BOOL bQuitMessage = WinUtil_RemoveQuitMessage(&wParam);

    int iResult = MessageBox(hwndOwner, pszText, pszCaption, uType);

    if (bQuitMessage)
    {
        PostQuitMessage(wParam);
    }

    return iResult;
}

void WinUtil_ErrorBox(HWND hwndOwner, LPCTSTR pszText, LPCTSTR pszCaption)
{
    UINT uType = MB_OK | MB_ICONSTOP | (hwndOwner == NULL ? MB_TASKMODAL : MB_APPLMODAL);
    WinUtil_MessageBox(hwndOwner, pszText, pszCaption, uType);
}

void WinUtil_DisplayWin32ErrorBox(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId)
{
    WinUtil_DisplayWin32ErrorBox3(hwndOwner, pszCaption, dwMessageId, NULL, NULL);
}

void WinUtil_DisplayWin32ErrorBox2(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId, LPCTSTR pszSource)
{
    WinUtil_DisplayWin32ErrorBox3(hwndOwner, pszCaption, dwMessageId, pszSource, NULL);
}

void WinUtil_DisplayWin32ErrorBox3(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail)
{
    TCHAR szBuffer[CCHMAXMSGLEN];
    WinUtil_FormatWin32Message3(szBuffer, _countof(szBuffer), dwMessageId, pszSource, pszDetail);
    WinUtil_ErrorBox(hwndOwner, szBuffer, pszCaption);
}

void WinUtil_DisplayWin32ErrorBoxEx(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId, LPCTSTR pszFormat, ...)
{
    va_list pArgList;
    va_start(pArgList, pszFormat);
    WinUtil_DisplayWin32ErrorBoxExVa(hwndOwner, pszCaption, dwMessageId, pszFormat, pArgList);
    va_end(pArgList);
}

void WinUtil_DisplayWin32ErrorBoxExVa(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId, LPCTSTR pszFormat, va_list pArgList)
{
    TCHAR szBuffer[CCHMAXMSGLEN];
    WinUtil_FormatWin32MessageExVa(szBuffer, _countof(szBuffer), dwMessageId, pszFormat, pArgList);
    WinUtil_ErrorBox(hwndOwner, szBuffer, pszCaption);
}

void WinUtil_DisplayError(DWORD dwLastError, LPCTSTR pszMessage)
{
    WinUtil_DisplayWin32ErrorBox3(NULL, AppData_GetApplicationName(), dwLastError, NULL, pszMessage);
}

int WinUtil_ComputeFontHeight(HDC hdc, int nPointSize)
{
    int nHeight;

    if (hdc != NULL)
    {
        int ppi = GetDeviceCaps(hdc, LOGPIXELSY);
        nHeight = -MulDiv(nPointSize, ppi, 72);
    }
    else
    {
        if ((hdc = GetDC(NULL)) != NULL)
        {
            int ppi = GetDeviceCaps(hdc, LOGPIXELSY);
            nHeight = -MulDiv(nPointSize, ppi, 72);
            ReleaseDC(NULL, hdc);
        }
        else
        {
            int ppi = 96; /* Historical Windows screen PPI (pixels per logical inch) */
            nHeight = -MulDiv(nPointSize, ppi, 72);
        }
    }

    return nHeight;
}

HFONT WinUtil_CreateFontByFaceName(HDC hdc, LPCTSTR pszFaceName, int nPointSize)
{
    int cHeight = WinUtil_ComputeFontHeight(hdc, nPointSize);
    int cWidth = 0;
    int cEscapement = 0;
    int cOrientation = 0;
    int cWeight = FW_NORMAL;
    DWORD bItalic = FALSE;
    DWORD bUnderline = FALSE;
    DWORD bStrikeOut = FALSE;
    DWORD iCharSet = DEFAULT_CHARSET;
    DWORD iOutPrecision = OUT_DEFAULT_PRECIS;
    DWORD iClipPrecision = CLIP_DEFAULT_PRECIS;
    DWORD iQuality = DEFAULT_QUALITY;
    DWORD iPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    HFONT hFont = CreateFont(cHeight, cWidth, cEscapement, cOrientation, cWeight,
                             bItalic, bUnderline, bStrikeOut, iCharSet,
                             iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily,
                             pszFaceName);
    return hFont;
}

HFONT WinUtil_CreateFontByPitchAndFamily(HDC hdc, DWORD iPitchAndFamily, int nPointSize)
{
    int cHeight = WinUtil_ComputeFontHeight(hdc, nPointSize);
    int cWidth = 0;
    int cEscapement = 0;
    int cOrientation = 0;
    int cWeight = FW_NORMAL;
    DWORD bItalic = FALSE;
    DWORD bUnderline = FALSE;
    DWORD bStrikeOut = FALSE;
    DWORD iCharSet = DEFAULT_CHARSET;
    DWORD iOutPrecision = OUT_DEFAULT_PRECIS;
    DWORD iClipPrecision = CLIP_DEFAULT_PRECIS;
    DWORD iQuality = DEFAULT_QUALITY;
    LPCTSTR pszFaceName = EMPTY_STRING;
    HFONT hFont = CreateFont(cHeight, cWidth, cEscapement, cOrientation, cWeight,
                             bItalic, bUnderline, bStrikeOut, iCharSet,
                             iOutPrecision, iClipPrecision, iQuality, iPitchAndFamily,
                             pszFaceName);
    return hFont;
}

BOOL WinUtil_Is64BitWindows()
{
#if defined(_WIN64)
    return TRUE; // 64-bit programs run only on Win64
#elif defined(_WIN32)
    // 32-bit programs run on both 32-bit and 64-bit Windows
    BOOL fWow64Process = FALSE;
    return IsWow64Process(GetCurrentProcess(), &fWow64Process) && fWow64Process;
#else
    return FALSE;
#endif
}

DWORD WinUtil_TranslateStrsafeError(HRESULT hr)
{
    DWORD dwError;

    switch (hr)
    {
    case S_OK:
        dwError = ERROR_SUCCESS;
        break;
    case STRSAFE_E_INSUFFICIENT_BUFFER:
        dwError = ERROR_INSUFFICIENT_BUFFER;
        break;
    case STRSAFE_E_INVALID_PARAMETER:
        dwError = ERROR_INVALID_PARAMETER;
        break;
    case STRSAFE_E_END_OF_FILE:
        dwError = ERROR_HANDLE_EOF;
        break;
    default:
        dwError = ERROR_INVALID_PARAMETER;
    }

    return dwError;
}

BOOL WinUtil_RegisterWindowClass(const WNDCLASS *pWndclass)
{
    BOOL bResult = FALSE;
    WNDCLASS wndclass;

    if (GetClassInfo(pWndclass->hInstance, pWndclass->lpszClassName, &wndclass))
    {
        bResult = TRUE;
    }
    else
    {
        if (RegisterClass(pWndclass))
        {
            if (WndClassList_RegisterWndClass(pWndclass->hInstance, pWndclass->lpszClassName))
            {
                bResult = TRUE;
            }
        }
    }

    return bResult;
}