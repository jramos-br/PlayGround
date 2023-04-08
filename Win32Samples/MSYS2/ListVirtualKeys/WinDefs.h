/*
 * Common definitions for Win32 applications.
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
 * For more information please visit:
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
 * For more information please visit:
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
 */

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <wchar.h>
#include <ctype.h>

#endif