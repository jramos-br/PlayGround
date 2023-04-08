/*
 * Utility functions for Win32 accelerator tables.
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

#ifndef _ACCUTIL_H_7E7472EC_B457_479A_8B08_3B77BC0BB0B4_
#define _ACCUTIL_H_7E7472EC_B457_479A_8B08_3B77BC0BB0B4_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#pragma once
#endif

#include "WinUtil.h"

#ifdef __cplusplus
extern "C"
{
#endif

    BOOL AccUtil_RegisterWindow(HWND hwnd, HACCEL haccel);
    BOOL AccUtil_UnregisterWindow(HWND hwnd);
    void AccUtil_UnregisterAllWindows();
    BOOL AccUtil_TranslateAccelerator(LPMSG pMsg);

#ifdef __cplusplus
}
#endif

#endif