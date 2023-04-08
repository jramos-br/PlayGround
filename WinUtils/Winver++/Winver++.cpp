/*
 * The Winver++ program.
 *
 * This program prints some windows predefined constants.
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

#define _WIN32_WINNT 0x0501 /* Windows XP */

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int _tmain(int argc, LPTSTR argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
#ifdef WINVER
    _tprintf(_T("WINVER = 0x%04X\n"), WINVER);
#endif
#ifdef _WIN32_WINNT
    _tprintf(_T("_WIN32_WINNT = 0x%04X\n"), _WIN32_WINNT);
#endif
#ifdef _WIN32_WINDOWS
    _tprintf(_T("_WIN32_WINDOWS = 0x%04X\n"), _WIN32_WINDOWS);
#endif
#ifdef _WIN32_IE
    _tprintf(_T("_WIN32_IE = 0x%04X\n"), _WIN32_IE);
#endif
#ifdef NTDDI_VERSION
    _tprintf(_T("NTDDI_VERSION = 0x%08X\n"), NTDDI_VERSION);
#endif
    return 0;
}
