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

#ifndef _UNICODE_H_6532DF46_AFA5_4568_B4FA_D9AE58C1A3D3_
#define _UNICODE_H_6532DF46_AFA5_4568_B4FA_D9AE58C1A3D3_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#pragma once
#endif

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE /* UNICODE is used by Windows headers */
#endif
#endif

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE /* _UNICODE is used by C-runtime/MFC headers */
#endif
#endif

#endif