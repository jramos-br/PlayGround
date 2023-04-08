/*
 * Helper functions for working with Win32 Virtual Keys.
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

#ifndef _VIRTUALKEYMAP_H_A9AE7A99_779C_4322_BA9F_DFCA2EA85031_
#define _VIRTUALKEYMAP_H_A9AE7A99_779C_4322_BA9F_DFCA2EA85031_

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

    BOOL VirtualKeyUtil_IsExtendedKey(UINT uVirtualKeyCode);
    BOOL VirtualKeyUtil_GetVirtualKeySymbolicName(LPTSTR pszDest, size_t cchDest, UINT uVirtualKeyCode);
    BOOL VirtualKeyUtil_GetVirtualKeyFormattedName(LPTSTR pszDest, size_t cchDest, UINT uVirtualKeyCode);
    BOOL VirtualKeyUtil_GetVirtualKeyNameText(LPTSTR pszDest, size_t cchDest, UINT uVirtualKeyCode);

#ifdef __cplusplus
}
#endif

#endif