/*
 * Debug functions for Win32 programs.
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

#ifndef _DEBUG_H_27FF2904_21B4_4AE8_A275_89467278E950_
#define _DEBUG_H_27FF2904_21B4_4AE8_A275_89467278E950_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#pragma once
#endif

#include "WinUtil.h"

#define YYTRACE Trace
#define YYTRACEVA TraceVa
#define YYTRACETEXT TraceText
#define YYTRACEWIN32 TraceWin32Message3
#define YYTRACEWIN32MESSAGE TraceWin32Message
#define YYTRACEWIN32MESSAGE2 TraceWin32Message2
#define YYTRACEWIN32MESSAGE3 TraceWin32Message3
#define YYTRACEWIN32MESSAGEEX TraceWin32MessageEx
#define YYTRACEWIN32MESSAGEEXVA TraceWin32MessageExVa

#define NOTRACE 1 ? (void)0 : Trace
#define NOTRACEVA 1 ? (void)0 : TraceVa
#define NOTRACETEXT 1 ? (void)0 : TraceText
#define NOTRACEWIN32 1 ? (void)0 : TraceWin32Message3
#define NOTRACEWIN32MESSAGE 1 ? (void)0 : TraceWin32Message
#define NOTRACEWIN32MESSAGE2 1 ? (void)0 : TraceWin32Message2
#define NOTRACEWIN32MESSAGE3 1 ? (void)0 : TraceWin32Message3
#define NOTRACEWIN32MESSAGEEX 1 ? (void)0 : TraceWin32MessageEx
#define NOTRACEWIN32MESSAGEEXVA 1 ? (void)0 : TraceWin32MessageExVa

#ifdef _DEBUG
#define TRACE YYTRACE
#define TRACEVA YYTRACEVA
#define TRACETEXT YYTRACETEXT
#define TRACEWIN32 YYTRACEWIN32
#define TRACEWIN32MESSAGE YYTRACEWIN32MESSAGE
#define TRACEWIN32MESSAGE2 YYTRACEWIN32MESSAGE2
#define TRACEWIN32MESSAGE3 YYTRACEWIN32MESSAGE3
#define TRACEWIN32MESSAGEEX YYTRACEWIN32MESSAGEEX
#define TRACEWIN32MESSAGEEXVA YYTRACEWIN32MESSAGEEXVA
#else
#define TRACE NOTRACE
#define TRACEVA NOTRACEVA
#define TRACETEXT NOTRACETEXT
#define TRACEWIN32 NOTRACEWIN32
#define TRACEWIN32MESSAGE NOTRACEWIN32MESSAGE
#define TRACEWIN32MESSAGE2 NOTRACEWIN32MESSAGE2
#define TRACEWIN32MESSAGE3 NOTRACEWIN32MESSAGE3
#define TRACEWIN32MESSAGEEX NOTRACEWIN32MESSAGEEX
#define TRACEWIN32MESSAGEEXVA NOTRACEWIN32MESSAGEEXVA
#endif

#define TraceWin32 TraceWin32Message3

#ifdef __cplusplus
extern "C"
{
#endif

    void Trace(LPCTSTR pszFormat, ...);
    void TraceVa(LPCTSTR pszFormat, va_list pArgList);
    void TraceText(LPCTSTR pszText);
    void TraceWin32Message(DWORD dwMessageId);
    void TraceWin32Message2(DWORD dwMessageId, LPCTSTR pszSource);
    void TraceWin32Message3(DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail);
    void TraceWin32MessageEx(DWORD dwMessageId, LPCTSTR pszFormat, ...);
    void TraceWin32MessageExVa(DWORD dwMessageId, LPCTSTR pszFormat, va_list pArgList);

#ifdef __cplusplus
}
#endif

#endif