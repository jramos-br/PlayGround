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

#include "AccUtil.h"
#include "WinUtil.h"
#include "Debug.h"

typedef struct _WINACC WINACC, *LPWINACC;

struct _WINACC
{
    LPWINACC pNext;
    HWND hwnd;
    HACCEL haccel;
};

static LPWINACC m_pFirstWindow;

static BOOL AccUtil_FreeWinacc(LPWINACC pWinacc)
{
    TRACE(TEXT("AccUtil_FreeWinacc: hwnd=%08X haccel=%08X\n"), pWinacc->hwnd, pWinacc->haccel);

    if (!DestroyAcceleratorTable(pWinacc->haccel))
    {
        TraceWin32(GetLastError(), TEXT("AccUtil_FreeWinacc"), TEXT("DestroyAcceleratorTable has failed"));
    }

    pWinacc->haccel = NULL;
    pWinacc->hwnd = NULL;
    pWinacc->pNext = NULL;

    if (!HeapFree(GetProcessHeap(), 0, (LPVOID)pWinacc))
    {
        TraceWin32(GetLastError(), TEXT("AccUtil_FreeWinacc"), TEXT("HeapFree has failed"));
    }

    return TRUE;
}

BOOL AccUtil_RegisterWindow(HWND hwnd, HACCEL haccel)
{
    LPWINACC pWinacc = (LPWINACC)HeapAlloc(GetProcessHeap(), 0, sizeof(WINACC));

    TRACE(TEXT("AccUtil_RegisterWindow: hwnd=%08X haccel=%08X\n"), hwnd, haccel);

    if (pWinacc != NULL)
    {
        pWinacc->haccel = haccel;
        pWinacc->hwnd = hwnd;
        pWinacc->pNext = m_pFirstWindow;
        m_pFirstWindow = pWinacc;
        return TRUE;
    }

    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    TraceWin32(GetLastError(), TEXT("AccUtil_RegisterWindow"), TEXT("HeapAlloc has failed"));
    return FALSE;
}

BOOL AccUtil_UnregisterWindow(HWND hwnd)
{
    TRACE(TEXT("AccUtil_UnregisterWindow: %08X\n"), hwnd);

    for (LPWINACC *pp = &m_pFirstWindow, p = *pp; p != NULL; pp = &p->pNext, p = *pp)
    {
        if (p->hwnd == hwnd)
        {
            *pp = p->pNext;
            return AccUtil_FreeWinacc(p);
        }
    }

    SetLastError(ERROR_NOT_FOUND);
    TraceWin32(GetLastError(), TEXT("AccUtil_UnregisterWindow"), NULL);
    return FALSE;
}

void AccUtil_UnregisterAllWindows()
{
    while (m_pFirstWindow != NULL)
    {
        LPWINACC p = m_pFirstWindow;
        m_pFirstWindow = p->pNext;
        AccUtil_FreeWinacc(p);
    }
}

BOOL AccUtil_TranslateAccelerator(LPMSG pMsg)
{
    BOOL bResult = FALSE;

    for (LPWINACC p = m_pFirstWindow; p != NULL; p = p->pNext)
    {
        if (p->hwnd == pMsg->hwnd || IsChild(p->hwnd, pMsg->hwnd))
        {
            if (TranslateAccelerator(p->hwnd, p->haccel, pMsg))
            {
                bResult = TRUE;
            }

            break;
        }
    }

    return bResult;
}