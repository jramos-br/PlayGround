/*
 * A list of window classes.
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

#include "WndClassList.h"
#include "StrUtil.h"
#include "Debug.h"

typedef struct _LISTITEM LISTITEM, *LPLISTITEM;

struct _LISTITEM
{
    LPLISTITEM pNext;
    HINSTANCE hInstance;
    LPCTSTR pszClassName;
};

static LPLISTITEM m_pFirstItem;

static BOOL WndClassList_FreeItem(LPLISTITEM pItem)
{
    TRACE(TEXT("WndClassList_FreeItem: hInstance=%08X pszClassName=%s\n"), pItem->hInstance, pItem->pszClassName);

    pItem->pszClassName = NULL;
    pItem->hInstance = NULL;
    pItem->pNext = NULL;

    if (!HeapFree(GetProcessHeap(), 0, (LPVOID)pItem))
    {
        TraceWin32(GetLastError(), TEXT("WndClassList_FreeItem"), TEXT("HeapFree has failed"));
    }

    return TRUE;
}

BOOL WndClassList_RegisterWndClass(HINSTANCE hInstance, LPCTSTR pszClassName)
{
    size_t cchClassName;

    TRACE(TEXT("WndClassList_RegisterWndClass: hInstance=%08X pszClassName=%s\n"), hInstance, pszClassName);

    if (StrUtil_GetLength(pszClassName, &cchClassName) && cchClassName > 0)
    {
        size_t cchNameSize = cchClassName + 1;
        size_t cbNameSize = cchNameSize * sizeof(TCHAR);
        size_t cbBufferSize = sizeof(LISTITEM) + cbNameSize;
        LPLISTITEM pItem = (LPLISTITEM)HeapAlloc(GetProcessHeap(), 0, cbBufferSize);

        if (pItem != NULL)
        {
            LPTSTR pszTemp = (LPTSTR)(pItem + 1);

            if (StrUtil_Copy(pszTemp, cchNameSize, pszClassName))
            {
                pItem->pszClassName = pszTemp;
                pItem->hInstance = hInstance;
                pItem->pNext = m_pFirstItem;
                m_pFirstItem = pItem;
                return TRUE;
            }

            WndClassList_FreeItem(pItem);
        }

        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        TraceWin32(GetLastError(), TEXT("WndClassList_RegisterWndClass"), TEXT("HeapAlloc has failed"));
        return FALSE;
    }

    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    TraceWin32(GetLastError(), TEXT("WndClassList_RegisterWndClass"), TEXT("StrUtil_GetLength has failed"));
    return FALSE;
}

BOOL WndClassList_UnregisterWndClass(HINSTANCE hInstance, LPCTSTR pszClassName)
{
    TRACE(TEXT("WndClassList_UnregisterWndClass: hInstance=%08X pszClassName=%s\n"), hInstance, pszClassName);

    for (LPLISTITEM *pp = &m_pFirstItem, p = *pp; p != NULL; pp = &p->pNext, p = *pp)
    {
        if (p->hInstance == hInstance && _tcscmp(p->pszClassName, pszClassName) == 0)
        {
            *pp = p->pNext;
            return WndClassList_FreeItem(p);
        }
    }

    SetLastError(ERROR_NOT_FOUND);
    TraceWin32(GetLastError(), TEXT("WndClassList_UnregisterWndClass"), NULL);
    return FALSE;
}

void WndClassList_UnRegisterAll()
{
    TRACE(TEXT("WndClassList_UnRegisterAll\n"));

    while (m_pFirstItem != NULL)
    {
        LPLISTITEM p = m_pFirstItem;
        m_pFirstItem = p->pNext;
        WndClassList_FreeItem(p);
    }
}