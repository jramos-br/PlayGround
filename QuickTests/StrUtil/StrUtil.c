/*
 * String functions for Win32 programs.
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

#include "StrUtil.h"

static const TCHAR m_szEmpty[] = TEXT("");

#define SAFESRC(p) (p != NULL ? p : m_szEmpty)

BOOL StrUtil_GetLength(LPCTSTR pszSrc, size_t *pcchLength)
{
    HRESULT hr = pcchLength != NULL
                     ? StringCchLength(pszSrc, STRSAFE_MAX_CCH, pcchLength)
                     : STRSAFE_E_INVALID_PARAMETER;

    if (SUCCEEDED(hr))
    {
        return TRUE;
    }

    SetLastError(StrUtil_TranslateStrsafeError(hr));
    return FALSE;
}

BOOL StrUtil_GetLengthEx(LPCTSTR pszSrc, size_t cchSrc, size_t *pcchLength)
{
    HRESULT hr = pcchLength != NULL
                     ? StringCchLength(pszSrc, cchSrc, pcchLength)
                     : STRSAFE_E_INVALID_PARAMETER;

    if (SUCCEEDED(hr))
    {
        return TRUE;
    }

    SetLastError(StrUtil_TranslateStrsafeError(hr));
    return FALSE;
}

BOOL StrUtil_Copy(LPTSTR pszDest, size_t cchDest, LPCTSTR pszSrc)
{
    HRESULT hr = pszDest != NULL
                     ? StringCchCopy(pszDest, cchDest, SAFESRC(pszSrc))
                     : STRSAFE_E_INVALID_PARAMETER;

    if (SUCCEEDED(hr))
    {
        return TRUE;
    }

    SetLastError(StrUtil_TranslateStrsafeError(hr));
    return FALSE;
}

BOOL StrUtil_Append(LPTSTR pszDest, size_t cchDest, LPCTSTR pszSrc)
{
    HRESULT hr = pszDest != NULL
                     ? StringCchCat(pszDest, cchDest, SAFESRC(pszSrc))
                     : STRSAFE_E_INVALID_PARAMETER;

    if (SUCCEEDED(hr))
    {
        return TRUE;
    }

    SetLastError(StrUtil_TranslateStrsafeError(hr));
    return FALSE;
}

BOOL StrUtil_Format(LPTSTR pszDest, size_t cchDest, LPCTSTR pszFormat, ...)
{
    BOOL bResult;
    va_list pArgList;
    va_start(pArgList, pszFormat);
    bResult = StrUtil_FormatVa(pszDest, cchDest, pszFormat, pArgList);
    va_end(pArgList);
    return bResult;
}

BOOL StrUtil_FormatVa(LPTSTR pszDest, size_t cchDest, LPCTSTR pszFormat, va_list pArgList)
{
    HRESULT hr = pszDest != NULL && pszFormat != NULL
                     ? StringCchVPrintf(pszDest, cchDest, pszFormat, pArgList)
                     : STRSAFE_E_INVALID_PARAMETER;

    if (SUCCEEDED(hr))
    {
        return TRUE;
    }

    SetLastError(StrUtil_TranslateStrsafeError(hr));
    return FALSE;
}

BOOL StrUtil_AppendFormat(LPTSTR pszDest, size_t cchDest, LPCTSTR pszFormat, ...)
{
    BOOL bResult;
    va_list pArgList;
    va_start(pArgList, pszFormat);
    bResult = StrUtil_AppendFormatVa(pszDest, cchDest, pszFormat, pArgList);
    va_end(pArgList);
    return bResult;
}

BOOL StrUtil_AppendFormatVa(LPTSTR pszDest, size_t cchDest, LPCTSTR pszFormat, va_list pArgList)
{
    size_t cchUsed;
    HRESULT hr = StringCchLength(pszDest, cchDest, &cchUsed);

    if (SUCCEEDED(hr))
    {
        hr = pszFormat != NULL
                 ? StringCchVPrintf(pszDest + cchUsed, cchDest - cchUsed, pszFormat, pArgList)
                 : STRSAFE_E_INVALID_PARAMETER;

        if (SUCCEEDED(hr))
        {
            return TRUE;
        }
    }

    SetLastError(StrUtil_TranslateStrsafeError(hr));
    return FALSE;
}

BOOL StrUtil_IsNullOrEmpty(LPCTSTR pszSrc)
{
    return pszSrc == NULL || *pszSrc == TEXT('\0');
}

void StrUtil_WriteEllipsisAtEnd(LPTSTR pszDest, size_t cchDest)
{
    const size_t cchEllipsis = 3;
    size_t cchUsed;
    HRESULT hr = StringCchLength(pszDest, cchDest, &cchUsed);

    if (SUCCEEDED(hr))
    {
        size_t right = cchDest > cchEllipsis && cchUsed < cchDest - cchEllipsis
                           ? cchUsed + cchEllipsis
                           : cchDest - 1;

        if (right < cchDest)
        {
            pszDest[right] = TEXT('\0');
        }

        for (size_t i = 0; i < cchEllipsis && right > 0; ++i)
        {
            pszDest[--right] = TEXT('.');
        }
    }
}

LPTSTR StrUtil_Duplicate(LPCTSTR pszSrc)
{
    size_t cchSrcLen;
    HRESULT hr = StringCchLength(pszSrc, STRSAFE_MAX_CCH, &cchSrcLen);

    if (SUCCEEDED(hr))
    {
        size_t cchDestLen = cchSrcLen + 1;
        size_t cbDestLen = cchDestLen * sizeof(TCHAR);
        LPTSTR pszDest = (LPTSTR)HeapAlloc(GetProcessHeap(), 0, cbDestLen);

        if (pszDest == NULL)
        {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            return NULL;
        }

        hr = StringCchCopy(pszDest, cchDestLen, pszSrc);

        if (SUCCEEDED(hr))
        {
            return pszDest;
        }
    }

    SetLastError(StrUtil_TranslateStrsafeError(hr));
    return NULL;
}

BOOL StrUtil_Free(LPTSTR pszSrc)
{
    return HeapFree(GetProcessHeap(), 0, (LPVOID)pszSrc);
}

DWORD StrUtil_TranslateStrsafeError(HRESULT hr)
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