/*
 * The MsiEnumProductsTest function.
 *
 * This file is part of the ListApps program.
 *
 * Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
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

#include "Win32.h"
#include <Msi.h>

#pragma comment(lib, "Msi.lib")

static int compare(const void *arg1, const void *arg2)
{
    return _tcsicmp(*(LPCTSTR*)arg1, *(LPCTSTR*)arg2);
}

void MsiEnumProductsTest()
{
    constexpr int cchGUID = 38;

    DWORD nProductsLength = 0;
    DWORD nProductsCapacity = 16;
    LPTSTR* lpProducts = new LPTSTR[nProductsCapacity];

    DWORD iProductIndex = 0;
    TCHAR szProductCode[cchGUID + 1] = { 0 };
    DWORD cchProductName = 128;
    TCHAR* lpszProductName = new TCHAR[cchProductName];
    DWORD cchVersionString = 32;
    TCHAR* lpszVersionString = new TCHAR[cchVersionString];
    UINT rc;

    while ((rc = MsiEnumProducts(iProductIndex++, szProductCode)) == ERROR_SUCCESS)
    {
        rc = MsiGetProductInfo(szProductCode, INSTALLPROPERTY_PRODUCTNAME, lpszProductName, &cchProductName);

        if (rc == ERROR_MORE_DATA)
        {
            delete[] lpszProductName;
            lpszProductName = new TCHAR[++cchProductName];
            rc = MsiGetProductInfo(szProductCode, INSTALLPROPERTY_PRODUCTNAME, lpszProductName, &cchProductName);
        }

        if (rc != ERROR_SUCCESS)
        {
            _ftprintf(stdout, _T("%u %s ERROR 0x%08X\n"), iProductIndex, szProductCode, rc);
            continue;
        }

        rc = MsiGetProductInfo(szProductCode, INSTALLPROPERTY_VERSIONSTRING, lpszVersionString, &cchVersionString);

        if (rc == ERROR_MORE_DATA)
        {
            delete[] lpszVersionString;
            lpszVersionString = new TCHAR[++cchVersionString];
            rc = MsiGetProductInfo(szProductCode, INSTALLPROPERTY_VERSIONSTRING, lpszVersionString, &cchVersionString);
        }

        if (rc != ERROR_SUCCESS)
        {
            _ftprintf(stdout, _T("%u %s ERROR 0x%08X\n"), iProductIndex, szProductCode, rc);
            continue;
        }

        DWORD cchProductInfo = cchProductName + 1 + cchVersionString + 1 + 1;
        TCHAR* lpszProductInfo = new TCHAR[cchProductInfo];

        _stprintf_s(lpszProductInfo, cchProductInfo, _T("%s %s"), lpszProductName, lpszVersionString);

        _ftprintf_s(stdout, _T("%s\n"), lpszProductInfo);

        if (nProductsLength == nProductsCapacity)
        {
            DWORD nProductsCapacityNew = nProductsCapacity * 2;
            LPTSTR* lpProductsNew = new LPTSTR[nProductsCapacityNew];
            memcpy(lpProductsNew, lpProducts, nProductsCapacity * sizeof(LPTSTR));
            delete[] lpProducts;
            lpProducts = lpProductsNew;
            nProductsCapacity = nProductsCapacityNew;
        }

        lpProducts[nProductsLength++] = lpszProductInfo;
    }

    if (rc != ERROR_NO_MORE_ITEMS)
    {
        _ftprintf(stderr, _T("MsiEnumProducts: ERROR 0x%08X\n"), rc);
    }

    qsort(lpProducts, nProductsLength, sizeof(LPTSTR), compare);

    for (DWORD i = 0; i < nProductsLength; i++)
    {
        // _ftprintf_s(stdout, _T("%s\n"), lpProducts[i]);
    }
}
