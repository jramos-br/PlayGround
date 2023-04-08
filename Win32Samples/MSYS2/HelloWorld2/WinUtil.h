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

#ifndef _WINUTIL_H_D5EC63B7_6F6B_4986_9928_C4DA30D59721_
#define _WINUTIL_H_D5EC63B7_6F6B_4986_9928_C4DA30D59721_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#pragma once
#endif

#include "WinDefs.h"

/*
 * Typical arguments for iPitchAndFamily.
 */

#define WINUTIL_FF_ROMAN (FF_ROMAN | VARIABLE_PITCH)
#define WINUTIL_FF_SWISS (FF_SWISS | VARIABLE_PITCH)
#define WINUTIL_FF_SCRIPT (FF_SCRIPT | VARIABLE_PITCH)
#define WINUTIL_FF_MONO (FF_MODERN | FIXED_PITCH)

/*
 * Utility macros.
 */

#define GetInstanceHandle() ((HINSTANCE)GetModuleHandle(NULL))
#define EMPTY_STRING TEXT("")

/*
 * Function declarations.
 */

#ifdef __cplusplus
extern "C"
{
#endif

    BOOL WinUtil_FormatWin32Message(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId);
    BOOL WinUtil_AppendWin32Message(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId);
    BOOL WinUtil_FormatWin32Message2(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource);
    BOOL WinUtil_AppendWin32Message2(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource);
    BOOL WinUtil_FormatWin32Message3(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail);
    BOOL WinUtil_AppendWin32Message3(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail);
    BOOL WinUtil_FormatWin32MessageEx(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszFormat, ...);
    BOOL WinUtil_FormatWin32MessageExVa(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszFormat, va_list pArgList);
    BOOL WinUtil_AppendWin32MessageEx(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszFormat, ...);
    BOOL WinUtil_AppendWin32MessageExVa(LPTSTR pszDest, size_t cchDest, DWORD dwMessageId, LPCTSTR pszFormat, va_list pArgList);

    BOOL WinUtil_RemoveQuitMessage(WPARAM *pwParam);
    int WinUtil_MessageBox(HWND hwndOwner, LPCTSTR pszText, LPCTSTR pszCaption, UINT uType);

    void WinUtil_ErrorBox(HWND hwndOwner, LPCTSTR pszText, LPCTSTR pszCaption);
    void WinUtil_DisplayWin32ErrorBox(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId);
    void WinUtil_DisplayWin32ErrorBox2(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId, LPCTSTR pszSource);
    void WinUtil_DisplayWin32ErrorBox3(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail);
    void WinUtil_DisplayWin32ErrorBoxEx(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId, LPCTSTR pszFormat, ...);
    void WinUtil_DisplayWin32ErrorBoxExVa(HWND hwndOwner, LPCTSTR pszCaption, DWORD dwMessageId, LPCTSTR pszFormat, va_list pArgList);

    void WinUtil_DisplayError(DWORD dwLastError, LPCTSTR pszMessage);

    int WinUtil_ComputeFontHeight(HDC hdc, int nPointSize);
    HFONT WinUtil_CreateFontByFaceName(HDC hdc, LPCTSTR pszFaceName, int nPointSize);
    HFONT WinUtil_CreateFontByPitchAndFamily(HDC hdc, DWORD iPitchAndFamily, int nPointSize);

    BOOL WinUtil_Is64BitWindows();
    DWORD WinUtil_TranslateStrsafeError(HRESULT hr);
    BOOL WinUtil_RegisterWindowClass(const WNDCLASS *pWndclass);

#ifdef __cplusplus
}
#endif

#endif