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

/* GetLength - ERROR_INVALID_PARAMETER */

static void StrUtil_GetLength_Test01()
{
    size_t cchLength;
    BOOL bResult = StrUtil_GetLength(NULL, &cchLength);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_GetLength_Test02()
{
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_GetLength(pszSrc, NULL);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_GetLength_Test_Invalid_Parameter()
{
    StrUtil_GetLength_Test01();
    StrUtil_GetLength_Test02();
}

/* GetLength - SUCCESS */

static void StrUtil_GetLength_Test21()
{
    LPCTSTR pszSrc = TEXT("");
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLength(pszSrc, &cchLength);
    assert(bResult);
    assert(cchLength == _tcslen(pszSrc));
}

static void StrUtil_GetLength_Test22()
{
    LPCTSTR pszSrc = TEXT("1");
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLength(pszSrc, &cchLength);
    assert(bResult);
    assert(cchLength == _tcslen(pszSrc));
}

static void StrUtil_GetLength_Test23()
{
    LPCTSTR pszSrc = TEXT("1234");
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLength(pszSrc, &cchLength);
    assert(bResult);
    assert(cchLength == _tcslen(pszSrc));
}

static void StrUtil_GetLength_Test_Success()
{
    StrUtil_GetLength_Test21();
    StrUtil_GetLength_Test22();
    StrUtil_GetLength_Test23();
}

/* GetLengthEx - ERROR_INVALID_PARAMETER */

static void StrUtil_GetLengthEx_Test01()
{
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLengthEx(NULL, STRSAFE_MAX_CCH, &cchLength);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_GetLengthEx_Test02()
{
    const TCHAR szSrc[] = TEXT("");
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLengthEx(szSrc, 0, &cchLength);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_GetLengthEx_Test03()
{
    const TCHAR szSrc[] = TEXT("");
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLengthEx(szSrc, (size_t)STRSAFE_MAX_CCH + 1, &cchLength);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_GetLengthEx_Test04()
{
    const TCHAR szSrc[] = TEXT("");
    BOOL bResult = StrUtil_GetLengthEx(szSrc, _countof(szSrc), NULL);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_GetLengthEx_Test05()
{
    const TCHAR szSrc[] = TEXT("1234");
    BOOL bResult = StrUtil_GetLengthEx(szSrc, _countof(szSrc) - 1, NULL);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_GetLengthEx_Test_Invalid_Parameter()
{
    StrUtil_GetLengthEx_Test01();
    StrUtil_GetLengthEx_Test02();
    StrUtil_GetLengthEx_Test03();
    StrUtil_GetLengthEx_Test04();
    StrUtil_GetLengthEx_Test05();
}

/* GetLengthEx - SUCCESS */

static void StrUtil_GetLengthEx_Test21()
{
    const TCHAR szSrc[] = TEXT("");
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLengthEx(szSrc, _countof(szSrc), &cchLength);
    assert(bResult);
    assert(cchLength == _tcslen(szSrc));
}

static void StrUtil_GetLengthEx_Test22()
{
    const TCHAR szSrc[] = TEXT("1");
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLengthEx(szSrc, _countof(szSrc), &cchLength);
    assert(bResult);
    assert(cchLength == _tcslen(szSrc));
}

static void StrUtil_GetLengthEx_Test23()
{
    const TCHAR szSrc[] = TEXT("1234");
    size_t cchLength = 42;
    BOOL bResult = StrUtil_GetLengthEx(szSrc, _countof(szSrc), &cchLength);
    assert(bResult);
    assert(cchLength == _tcslen(szSrc));
}

static void StrUtil_GetLengthEx_Test_Success()
{
    StrUtil_GetLengthEx_Test21();
    StrUtil_GetLengthEx_Test22();
    StrUtil_GetLengthEx_Test23();
}

/* StrUtil_Copy - ERROR_INVALID_PARAMETER */

static void StrUtil_Copy_Test01()
{
    TCHAR szDest[8];
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Copy(NULL, _countof(szDest), pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_Copy_Test02()
{
    TCHAR szDest[8];
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Copy(szDest, 0, pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_Copy_Test03()
{
    TCHAR szDest[8];
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Copy(szDest, (size_t)STRSAFE_MAX_CCH + 1, pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_Copy_Test_Invalid_Parameter()
{
    StrUtil_Copy_Test01();
    StrUtil_Copy_Test02();
    StrUtil_Copy_Test03();
}

/* StrUtil_Copy - ERROR_INSUFFICIENT_BUFFER */

static void StrUtil_Copy_Test11()
{
    TCHAR szDest[1];
    LPCTSTR pszSrc = TEXT("1");
    BOOL bResult = StrUtil_Copy(szDest, _countof(szDest), pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INSUFFICIENT_BUFFER);
    assert(_tcscmp(szDest, TEXT("")) == 0);
}

static void StrUtil_Copy_Test12()
{
    TCHAR szDest[4];
    LPCTSTR pszSrc = TEXT("1234");
    BOOL bResult = StrUtil_Copy(szDest, _countof(szDest), pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INSUFFICIENT_BUFFER);
    assert(_tcscmp(szDest, TEXT("123")) == 0);
}

static void StrUtil_Copy_Test_Insufficient_Buffer()
{
    StrUtil_Copy_Test11();
    StrUtil_Copy_Test12();
}

/* StrUtil_Copy - SUCCESS */

static void StrUtil_Copy_Test21()
{
    TCHAR szDest[1];
    LPCTSTR pszSrc = NULL;
    BOOL bResult = StrUtil_Copy(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcslen(szDest) == 0);
}

static void StrUtil_Copy_Test22()
{
    TCHAR szDest[1];
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Copy(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, pszSrc) == 0);
}

static void StrUtil_Copy_Test23()
{
    TCHAR szDest[4];
    LPCTSTR pszSrc = TEXT("123");
    BOOL bResult = StrUtil_Copy(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, pszSrc) == 0);
}

static void StrUtil_Copy_Test24()
{
    TCHAR szDest[8];
    LPCTSTR pszSrc = TEXT("1234");
    BOOL bResult = StrUtil_Copy(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, pszSrc) == 0);
}

static void StrUtil_Copy_Test_Success()
{
    StrUtil_Copy_Test21();
    StrUtil_Copy_Test22();
    StrUtil_Copy_Test23();
    StrUtil_Copy_Test24();
}

/* StrUtil_Append - ERROR_INVALID_PARAMETER */

static void StrUtil_Append_Test01()
{
    TCHAR szDest[8] = TEXT("");
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Append(NULL, _countof(szDest), pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_Append_Test02()
{
    TCHAR szDest[8] = TEXT("");
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Append(szDest, 0, pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_Append_Test03()
{
    TCHAR szDest[8] = TEXT("");
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Append(szDest, (size_t)STRSAFE_MAX_CCH + 1, pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INVALID_PARAMETER);
}

static void StrUtil_Append_Test_Invalid_Parameter()
{
    StrUtil_Append_Test01();
    StrUtil_Append_Test02();
    StrUtil_Append_Test03();
}

/* StrUtil_Append - ERROR_INSUFFICIENT_BUFFER */

static void StrUtil_Append_Test11()
{
    TCHAR szDest[1] = TEXT("");
    LPCTSTR pszSrc = TEXT("1");
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INSUFFICIENT_BUFFER);
    assert(_tcscmp(szDest, TEXT("")) == 0);
}

static void StrUtil_Append_Test12()
{
    TCHAR szDest[4] = TEXT("");
    LPCTSTR pszSrc = TEXT("1234");
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(!bResult);
    assert(GetLastError() == ERROR_INSUFFICIENT_BUFFER);
    assert(_tcscmp(szDest, TEXT("123")) == 0);
}

static void StrUtil_Append_Test_Insufficient_Buffer()
{
    StrUtil_Append_Test11();
    StrUtil_Append_Test12();
}

/* StrUtil_Append - SUCCESS */

static void StrUtil_Append_Test21()
{
    TCHAR szDest[1] = TEXT("");
    LPCTSTR pszSrc = NULL;
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcslen(szDest) == 0);
}

static void StrUtil_Append_Test22()
{
    TCHAR szDest[1] = TEXT("");
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, pszSrc) == 0);
}

static void StrUtil_Append_Test23()
{
    TCHAR szDest[4] = TEXT("");
    LPCTSTR pszSrc = TEXT("123");
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, pszSrc) == 0);
}

static void StrUtil_Append_Test24()
{
    TCHAR szDest[8] = TEXT("");
    LPCTSTR pszSrc = TEXT("1234");
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, pszSrc) == 0);
}

static void StrUtil_Append_Test25()
{
    TCHAR szDest[4] = TEXT("123");
    LPCTSTR pszSrc = NULL;
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, TEXT("123")) == 0);
}

static void StrUtil_Append_Test26()
{
    TCHAR szDest[4] = TEXT("123");
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, TEXT("123")) == 0);
}

static void StrUtil_Append_Test27()
{
    TCHAR szDest[4] = TEXT("12");
    LPCTSTR pszSrc = TEXT("3");
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, TEXT("123")) == 0);
}

static void StrUtil_Append_Test28()
{
    TCHAR szDest[8] = TEXT("12");
    LPCTSTR pszSrc = TEXT("34");
    BOOL bResult = StrUtil_Append(szDest, _countof(szDest), pszSrc);
    assert(bResult);
    assert(_tcscmp(szDest, TEXT("1234")) == 0);
}

static void StrUtil_Append_Test_Success()
{
    StrUtil_Append_Test21();
    StrUtil_Append_Test22();
    StrUtil_Append_Test23();
    StrUtil_Append_Test24();
    StrUtil_Append_Test25();
    StrUtil_Append_Test26();
    StrUtil_Append_Test27();
    StrUtil_Append_Test28();
}

/* StrUtil_IsNullOrEmpty */

static void StrUtil_IsNullOrEmpty_Test01()
{
    LPCTSTR pszSrc = NULL;
    BOOL bResult = StrUtil_IsNullOrEmpty(pszSrc);
    assert(bResult);
}

static void StrUtil_IsNullOrEmpty_Test02()
{
    LPCTSTR pszSrc = TEXT("");
    BOOL bResult = StrUtil_IsNullOrEmpty(pszSrc);
    assert(bResult);
}

static void StrUtil_IsNullOrEmpty_Test03()
{
    LPCTSTR pszSrc = TEXT(" ");
    BOOL bResult = StrUtil_IsNullOrEmpty(pszSrc);
    assert(!bResult);
}

static void StrUtil_IsNullOrEmpty_Test04()
{
    LPCTSTR pszSrc = TEXT("1234");
    BOOL bResult = StrUtil_IsNullOrEmpty(pszSrc);
    assert(!bResult);
}

static void StrUtil_IsNullOrEmpty_Test()
{
    StrUtil_IsNullOrEmpty_Test01();
    StrUtil_IsNullOrEmpty_Test02();
    StrUtil_IsNullOrEmpty_Test03();
    StrUtil_IsNullOrEmpty_Test04();
}

/* Main */

int _tmain(int argc, LPTSTR argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    StrUtil_GetLength_Test_Invalid_Parameter();
    StrUtil_GetLength_Test_Success();

    StrUtil_GetLengthEx_Test_Invalid_Parameter();
    StrUtil_GetLengthEx_Test_Success();

    StrUtil_Copy_Test_Invalid_Parameter();
    StrUtil_Copy_Test_Insufficient_Buffer();
    StrUtil_Copy_Test_Success();

    StrUtil_Append_Test_Invalid_Parameter();
    StrUtil_Append_Test_Insufficient_Buffer();
    StrUtil_Append_Test_Success();

    StrUtil_IsNullOrEmpty_Test();

    _ftprintf(stdout, TEXT("OK\n"));
    return 0;
}