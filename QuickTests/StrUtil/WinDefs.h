/*
 * Win32 API macros and function declarations.
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

#ifndef _WINDEFS_H_5B5AC5A3_AF8A_4EBB_BB97_93B49CD4E517_
#define _WINDEFS_H_5B5AC5A3_AF8A_4EBB_BB97_93B49CD4E517_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#pragma once
#endif

/*
 * Define the same character set for both Windows headers and C-runtime/MFC headers.
 *
 * For more information, please visit:
 *  Microsoft Docs by Microsoft
 *  Unicode in the Windows API
 *  https://docs.microsoft.com/en-us/windows/win32/intl/unicode-in-the-windows-api
 *
 *  The Old New Thing by Raymond Chen
 *  TEXT vs. _TEXT vs. _T, and UNICODE vs. _UNICODE
 *  https://devblogs.microsoft.com/oldnewthing/?p=40643
 */

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE /* _UNICODE is used by C-runtime/MFC headers */
#endif
#endif

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE /* UNICODE is used by Windows headers */
#endif
#endif

/*
 * Define the minimum required platform.
 *
 * The minimum required platform is the earliest version of Windows that has
 * the necessary features to run the application. All features available on
 * platform versions up to and including the version specified will be enabled.
 *
 * For more information, please visit:
 *  Microsoft Docs by Microsoft
 *  Using the Windows Headers
 *  https://docs.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers
 *
 *  The Old New Thing by Raymond Chen
 *  What is the default version of a header file?
 *  https://devblogs.microsoft.com/oldnewthing/20070410-00/?p=27313
 *  What's the difference between WINVER, _WIN32_WINNT, _WIN32_WINDOWS, and _WIN32_IE?
 *  https://devblogs.microsoft.com/oldnewthing/20070411-00/?p=27283
 */

#include <winsdkver.h>

#define _WIN32_WINNT _WIN32_WINNT_MAXVER /* The minimum required platform */

#include <sdkddkver.h>

/*
 * Include common headers.
 *
 * For more information, please visit:
 *  Microsoft Docs by Microsoft
 *  Programming reference for the Win32 API
 *  https://docs.microsoft.com/en-us/windows/win32/api
 */

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <wchar.h>
#include <ctype.h>
#include <strsafe.h>
#include <errno.h>
#include <assert.h>

/*
 * Fixes a bug in the original SetDlgMsgResult macro.
 *
 * The bug affects only 64-bit systems. The expression (BOOL)(result) should be changed to
 * (LRESULT)(result) so that the upper half (32 bits) of the return value is not truncated.
 *
 * For more information, please visit:
 *  The Old New Thing by Raymond Chen
 *  A different type of dialog procedure
 * https://devblogs.microsoft.com/oldnewthing/20031112-00/?p=41863
 */

#ifdef SetDlgMsgResult
#undef SetDlgMsgResult
#endif

#define SetDlgMsgResult(hwnd, msg, result) \
    (((msg) == WM_CTLCOLORMSGBOX ||        \
      (msg) == WM_CTLCOLOREDIT ||          \
      (msg) == WM_CTLCOLORLISTBOX ||       \
      (msg) == WM_CTLCOLORBTN ||           \
      (msg) == WM_CTLCOLORDLG ||           \
      (msg) == WM_CTLCOLORSCROLLBAR ||     \
      (msg) == WM_CTLCOLORSTATIC ||        \
      (msg) == WM_COMPAREITEM ||           \
      (msg) == WM_VKEYTOITEM ||            \
      (msg) == WM_CHARTOITEM ||            \
      (msg) == WM_QUERYDRAGICON ||         \
      (msg) == WM_INITDIALOG)              \
         ? (LRESULT)(result)               \
         : (SetWindowLongPtr((hwnd), DWLP_MSGRESULT, (LPARAM)(LRESULT)(result)), TRUE))

/*
 * The normal HANDLE_MSG macro in WINDOWSX.H does not work properly for dialog boxes
 * because DlgProcs return a BOOL instead of an LRESULT (like WndProcs).
 */

#define HANDLE_DLGMSG(hwnd, message, fn) \
    case (message):                      \
        return SetDlgMsgResult(hwnd, message, HANDLE_##message((hwnd), (wParam), (lParam), (fn)))

/*
 * Additional message crackers.
 */

/* void Cls_OnTaskbarMsg(HWND hwnd, UINT uID, UINT uMouseMsg) */
#define HANDLE_WM_TASKBARMSG(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (UINT)(wParam), (UINT)(lParam)), 0L)
#define FORWARD_WM_TASKBARMSG(hwnd, uID, uMouseMsg, fn) \
    (void)(fn)((hwnd), WM_TASKBARMSG, (WPARAM)(UINT)(uID), (LPARAM)(UINT)(uMouseMsg))

/* void Cls_OnTaskbarRestart(HWND hwnd) */
#define HANDLE_WM_TASKBARRESTART(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd)), 0L)
#define FORWARD_WM_TASKBARRESTART(hwnd, fn) \
    (void)(fn)((hwnd), m_uTaskbarRestart, (WPARAM)0, (LPARAM)0)

/*
 * Constants for FILETIME arithmetic.
 */

#define FT_SECOND ((ULONGLONG)10000000)
#define FT_MINUTE (60 * FT_SECOND)
#define FT_HOUR (60 * FT_MINUTE)
#define FT_DAY (24 * FT_HOUR)

#endif