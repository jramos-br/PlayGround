/*
 * A modeless dialog example.
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

#include "FontDialog.h"
#include "WinMain.h"
#include "WinUtil.h"
#include "DlgUtil.h"
#include "Resource.h"
#include "Debug.h"

static const TCHAR m_szDialogName[] = TEXT("FontDialog");
static const TCHAR m_szDialogTitle[] = TEXT("Choose Font");

static BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(hwndFocus);
    UNREFERENCED_PARAMETER(lParam);
    SetWindowText(hwnd, m_szDialogTitle);
    DlgUtil_RegisterDialog(hwnd, m_szDialogName);
    return TRUE;
}

static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    UNREFERENCED_PARAMETER(hwndCtl);
    UNREFERENCED_PARAMETER(codeNotify);
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        EndDialog(hwnd, TRUE);
        break;
    }
}

/* Typically, the dialog box procedure should return TRUE if it processed the message, and FALSE if
   it did not. If the dialog box procedure returns FALSE, the dialog manager performs the default
   dialog operation in response to the message. */

static INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_DLGMSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_DLGMSG(hwnd, WM_COMMAND, OnCommand);

    case WM_NCDESTROY:
        DlgUtil_UnregisterDialog(hwnd);
        break;
    }

    return FALSE;
}

static LPCDLGTEMPLATE CreateDialogTemplate()
{
    size_t cbTemplate = sizeof(DLGTEMPLATE) + sizeof(WORD) * 3;
    LPDLGTEMPLATE pDlgtemplate = (LPDLGTEMPLATE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbTemplate);

    if (pDlgtemplate != NULL)
    {
        pDlgtemplate->style = WS_POPUP | WS_DLGFRAME | WS_CAPTION | WS_SYSMENU;
        pDlgtemplate->style |= WS_VISIBLE;
        pDlgtemplate->dwExtendedStyle = 0;
        pDlgtemplate->cdit = 0;
        pDlgtemplate->x = 100;
        pDlgtemplate->y = 100;
        pDlgtemplate->cx = 100;
        pDlgtemplate->cy = 100;

        LPWORD lpw = (LPWORD)(pDlgtemplate + 1);

        *lpw++ = 0; // Menu
        *lpw++ = 0; // Class
        *lpw++ = 0; // Title
    }

    return pDlgtemplate;
}

static void FontDialog_ReleaseDialogTemplate(LPCDLGTEMPLATE pDlgtemplate)
{
    if (!HeapFree(GetProcessHeap(), 0, (LPVOID)pDlgtemplate))
    {
        TraceWin32(GetLastError(), TEXT("FontDialog_ReleaseDialogTemplate"), TEXT("HeapFree has failed"));
    }
}

HWND FontDialog_Create(HWND hwndOwner)
{
    HWND hwnd = NULL;
    LPCDLGTEMPLATE pDlgtemplate = CreateDialogTemplate();

    if (pDlgtemplate != NULL)
    {
        hwnd = CreateDialogIndirectParam(GetInstanceHandle(), pDlgtemplate, hwndOwner, &DialogProc, 0L);
        DWORD dwLastError = GetLastError();
        FontDialog_ReleaseDialogTemplate(pDlgtemplate);
        SetLastError(dwLastError);
    }

    return hwnd;
}