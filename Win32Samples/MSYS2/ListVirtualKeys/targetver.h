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

#ifndef _TARGETVER_H_5783F34B_0001_4F54_937D_8D6988020CB2_
#define _TARGETVER_H_5783F34B_0001_4F54_937D_8D6988020CB2_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#pragma once
#endif

#include <winsdkver.h>

#define _WIN32_WINNT _WIN32_WINNT_MAXVER /* The minimum required platform */

#include <sdkddkver.h>

#endif