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

#include "Debug.h"
#include "StrUtil.h"
#include "WinUtil.h"

#define CCHMAXMSGLEN 512

void TraceInternal(LPCTSTR pszText)
{
    OutputDebugString(pszText);
}

void Trace(LPCTSTR pszFormat, ...)
{
    va_list pArgList;
    va_start(pArgList, pszFormat);
    TraceVa(pszFormat, pArgList);
    va_end(pArgList);
}

void TraceVa(LPCTSTR pszFormat, va_list pArgList)
{
    DWORD dwLastError = GetLastError();
    TCHAR szMessage[CCHMAXMSGLEN];
    StrUtil_FormatVa(szMessage, _countof(szMessage), pszFormat, pArgList);
    TraceInternal(szMessage);
    SetLastError(dwLastError);
}

void TraceText(LPCTSTR pszText)
{
    DWORD dwLastError = GetLastError();
    TraceInternal(pszText);
    SetLastError(dwLastError);
}

void TraceWin32Message(DWORD dwMessageId)
{
    TraceWin32Message3(dwMessageId, NULL, NULL);
}

void TraceWin32Message2(DWORD dwMessageId, LPCTSTR pszSource)
{
    TraceWin32Message3(dwMessageId, pszSource, NULL);
}

void TraceWin32Message3(DWORD dwMessageId, LPCTSTR pszSource, LPCTSTR pszDetail)
{
    DWORD dwLastError = GetLastError();
    TCHAR szBuffer[CCHMAXMSGLEN];
    WinUtil_FormatWin32Message3(szBuffer, _countof(szBuffer), dwMessageId, pszSource, pszDetail);
    TraceInternal(szBuffer);
    SetLastError(dwLastError);
}

void TraceWin32MessageEx(DWORD dwMessageId, LPCTSTR pszFormat, ...)
{
    va_list pArgList;
    va_start(pArgList, pszFormat);
    TraceWin32MessageExVa(dwMessageId, pszFormat, pArgList);
    va_end(pArgList);
}

void TraceWin32MessageExVa(DWORD dwMessageId, LPCTSTR pszFormat, va_list pArgList)
{
    DWORD dwLastError = GetLastError();
    TCHAR szBuffer[CCHMAXMSGLEN];
    WinUtil_FormatWin32MessageExVa(szBuffer, _countof(szBuffer), dwMessageId, pszFormat, pArgList);
    TraceInternal(szBuffer);
    SetLastError(dwLastError);
}
