/*
 * Application global data.
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

#include "AppData.h"
#include "StrUtil.h"

static HINSTANCE m_hInstance;
static LPTSTR m_pszApplicationName;

HINSTANCE AppData_GetInstance()
{
    return m_hInstance;
}

void AppData_SetInstance(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
}

LPCTSTR AppData_GetApplicationName()
{
    return m_pszApplicationName != NULL ? m_pszApplicationName : EMPTY_STRING;
}

void AppData_SetApplicationName(LPCTSTR pszApplicationName)
{
    m_pszApplicationName = StrUtil_Duplicate(pszApplicationName);
}

LPCTSTR AppData_GetCompanyName()
{
    return NULL;
}

LPCTSTR AppData_GetProductName()
{
    return NULL;
}

LPCTSTR AppData_GetProductVersion()
{
    return NULL;
}

LPCTSTR AppData_GetStartupPath()
{
    return NULL;
}

LPCTSTR AppData_GetExecutablePath()
{
    return _tpgmptr;
}

void AppData_Free()
{
    if (m_pszApplicationName == NULL)
    {
        StrUtil_Free(m_pszApplicationName);
        m_pszApplicationName = NULL;
    }
}