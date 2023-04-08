/*
 * The MsiEnumProductsExTest function.
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

void MsiEnumProductsExTest()
{
    constexpr int cchGUID = 38;

    DWORD nProductsLength = 0;
    DWORD nProductsCapacity = 16;
    LPTSTR* lpProducts = new LPTSTR[nProductsCapacity];

    MSIINSTALLCONTEXT dwInstalledContext;

    DWORD dwProductIndex = 0;
    TCHAR szUserSid[] = _T("s-1-1-0");
    TCHAR szInstalledProductCode[cchGUID + 1];
    DWORD cchSid = 128;
    TCHAR* lpszSid = new TCHAR[cchSid];

    DWORD cchValue;

    DWORD cchProductState = 8;
    TCHAR* lpszProductState = new TCHAR[cchProductState];

    DWORD cchProductName = 128;
    TCHAR* lpszProductName = new TCHAR[cchProductName];

    DWORD cchVersionString = 32;
    TCHAR* lpszVersionString = new TCHAR[cchVersionString];

    for (;;)
    {
        constexpr DWORD dwContext = MSIINSTALLCONTEXT_ALL;

        UINT rc = MsiEnumProductsEx(NULL, szUserSid, dwContext, dwProductIndex, szInstalledProductCode, &dwInstalledContext, lpszSid, &cchSid);

        if (rc == ERROR_MORE_DATA)
        {
            delete[] lpszSid;
            lpszSid = new TCHAR[++cchSid];
            rc = MsiEnumProductsEx(NULL, szUserSid, dwContext, dwProductIndex, szInstalledProductCode, &dwInstalledContext, lpszSid, &cchSid);
        }

        if (rc != ERROR_SUCCESS)
        {
            if (rc != ERROR_NO_MORE_ITEMS)
            {
                _ftprintf(stderr, _T("MsiEnumProductsEx: ERROR 0x%08X\n"), rc);
            }

            break;
        }

        ++dwProductIndex;

        LPCTSTR lpszUserSid = (dwInstalledContext == MSIINSTALLCONTEXT_MACHINE) ? NULL : lpszSid;

        cchValue = cchProductState;
        rc = MsiGetProductInfoEx(szInstalledProductCode, lpszUserSid, dwInstalledContext, INSTALLPROPERTY_PRODUCTSTATE, lpszProductState, &cchValue);

        if (rc == ERROR_MORE_DATA)
        {
            delete[] lpszProductState;
            cchProductState = ++cchValue;
            lpszProductState = new TCHAR[cchProductState];
            rc = MsiGetProductInfoEx(szInstalledProductCode, lpszUserSid, dwInstalledContext, INSTALLPROPERTY_PRODUCTSTATE, lpszProductState, &cchValue);
        }

        if (rc != ERROR_SUCCESS)
        {
            _ftprintf(stderr, _T("MsiGetProductInfoEx: ERROR 0x%08X\n"), rc);
            continue;
        }

        cchValue = cchProductName;
        rc = MsiGetProductInfoEx(szInstalledProductCode, lpszUserSid, dwInstalledContext, INSTALLPROPERTY_PRODUCTNAME, lpszProductName, &cchValue);

        if (rc == ERROR_MORE_DATA)
        {
            delete[] lpszProductName;
            cchProductName = ++cchValue;
            lpszProductName = new TCHAR[cchProductName];
            rc = MsiGetProductInfoEx(szInstalledProductCode, lpszUserSid, dwInstalledContext, INSTALLPROPERTY_PRODUCTNAME, lpszProductName, &cchValue);
        }

        if (rc != ERROR_SUCCESS)
        {
            _ftprintf(stderr, _T("MsiGetProductInfoEx: ERROR 0x%08X\n"), rc);
            continue;
        }

        cchValue = cchVersionString;
        rc = MsiGetProductInfoEx(szInstalledProductCode, lpszUserSid, dwInstalledContext, INSTALLPROPERTY_VERSIONSTRING, lpszVersionString, &cchValue);

        if (rc == ERROR_MORE_DATA)
        {
            delete[] lpszVersionString;
            cchVersionString = ++cchValue;
            lpszVersionString = new TCHAR[cchVersionString];
            rc = MsiGetProductInfoEx(szInstalledProductCode, lpszUserSid, dwInstalledContext, INSTALLPROPERTY_VERSIONSTRING, lpszVersionString, &cchValue);
        }

        if (rc != ERROR_SUCCESS)
        {
            _ftprintf(stderr, _T("MsiGetProductInfoEx: ERROR 0x%08X\n"), rc);
            continue;
        }

        _ftprintf_s(stdout, _T("%s %s\n"), lpszProductName, lpszVersionString);
    }
}
