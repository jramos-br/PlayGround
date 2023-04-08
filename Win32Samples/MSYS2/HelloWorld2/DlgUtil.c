/*
 * Utility functions for Win32 dialogs.
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

#include "DlgUtil.h"
#include "WinUtil.h"
#include "StrUtil.h"
#include "Debug.h"

typedef struct _DIALOG DIALOG, *LPDIALOG;

struct _DIALOG
{
    LPDIALOG pNext;
    HWND hwnd;
    LPCTSTR pszName;
};

static LPDIALOG m_pFirstDialog;

static BOOL DlgUtil_FreeDialog(LPDIALOG pDialog)
{
    TRACE(TEXT("DlgUtil_FreeDialog: hwnd=%08X pszName=[%s]\n"), pDialog->hwnd, pDialog->pszName);

    pDialog->pszName = NULL;
    pDialog->hwnd = NULL;
    pDialog->pNext = NULL;

    if (!HeapFree(GetProcessHeap(), 0, (LPVOID)pDialog))
    {
        TRACEWIN32(GetLastError(), TEXT("DlgUtil_FreeDialog"), TEXT("HeapFree has failed"));
    }

    return TRUE;
}

BOOL DlgUtil_RegisterDialog(HWND hwnd, LPCTSTR pszName)
{
    size_t cchName;

    TRACE(TEXT("DlgUtil_RegisterDialog: hwnd=%08X pszName=[%s]\n"), hwnd, pszName);

    if (StrUtil_GetLength(pszName, &cchName) && cchName > 0)
    {
        size_t cchNameSize = cchName + 1;
        size_t cbNameSize = cchNameSize * sizeof(TCHAR);
        size_t cbBufferSize = sizeof(DIALOG) + cbNameSize;
        LPDIALOG pDialog = (LPDIALOG)HeapAlloc(GetProcessHeap(), 0, cbBufferSize);

        if (pDialog != NULL)
        {
            LPTSTR pszTemp = (LPTSTR)(pDialog + 1);

            if (StrUtil_Copy(pszTemp, cchNameSize, pszName))
            {
                pDialog->pszName = pszTemp;
                pDialog->hwnd = hwnd;
                pDialog->pNext = m_pFirstDialog;
                m_pFirstDialog = pDialog;
                return TRUE;
            }

            DlgUtil_FreeDialog(pDialog);
        }
    }

    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    TRACEWIN32(GetLastError(), TEXT("DlgUtil_RegisterDialog"), TEXT("HeapAlloc has failed"));
    return FALSE;
}

BOOL DlgUtil_UnregisterDialog(HWND hwnd)
{
    TRACE(TEXT("DlgUtil_UnregisterDialog: %08X\n"), hwnd);

    for (LPDIALOG *pp = &m_pFirstDialog, p = *pp; p != NULL; pp = &p->pNext, p = *pp)
    {
        if (p->hwnd == hwnd)
        {
            *pp = p->pNext;
            return DlgUtil_FreeDialog(p);
        }
    }

    SetLastError(ERROR_NOT_FOUND);
    return FALSE;
}

void DlgUtil_UnregisterAllDialogs()
{
    while (m_pFirstDialog != NULL)
    {
        LPDIALOG p = m_pFirstDialog;
        m_pFirstDialog = p->pNext;
        DlgUtil_FreeDialog(p);
    }
}

HWND DlgUtil_FindDialogWindow(LPCTSTR pszName)
{
    for (LPDIALOG p = m_pFirstDialog; p != NULL; p = p->pNext)
    {
        if (lstrcmp(p->pszName, pszName) == 0)
        {
            return p->hwnd;
        }
    }

    return NULL;
}

BOOL DlgUtil_IsDialogMessage(LPMSG pMsg)
{
    BOOL bResult = FALSE;

    for (LPDIALOG p = m_pFirstDialog; p != NULL; p = p->pNext)
    {
        if (p->hwnd == pMsg->hwnd || IsChild(p->hwnd, pMsg->hwnd))
        {
            if (IsDialogMessage(p->hwnd, pMsg))
            {
                bResult = TRUE;
            }

            break;
        }
    }

    return bResult;
}