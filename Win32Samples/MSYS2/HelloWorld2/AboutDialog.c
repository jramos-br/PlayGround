/*
 * The About Box.
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

#include "AboutDialog.h"
#include "DlgUtil.h"
#include "Resource.h"
#include "Debug.h"

static const TCHAR m_szDialogTitle[] = TEXT("About HelloWorld");

static BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(hwndFocus);
    UNREFERENCED_PARAMETER(lParam);
    SetWindowText(hwnd, m_szDialogTitle);
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

static INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_DLGMSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_DLGMSG(hwnd, WM_COMMAND, OnCommand);
    }

    return FALSE;
}

static LPCDLGTEMPLATE CreateDialogTemplate()
{
    size_t cbDlgTemplate = sizeof(DLGTEMPLATE) + sizeof(WORD) * 3;
    LPDLGTEMPLATE pDlgTemplate = (LPDLGTEMPLATE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbDlgTemplate);

    if (pDlgTemplate != NULL)
    {
        pDlgTemplate->style = WS_POPUP | WS_DLGFRAME | WS_CAPTION | WS_SYSMENU; // has caption, has sysmenu, has icon
        pDlgTemplate->style |= WS_VISIBLE;
        pDlgTemplate->dwExtendedStyle = 0;
        pDlgTemplate->cdit = 0;
        pDlgTemplate->x = 0;
        pDlgTemplate->y = 0;
        pDlgTemplate->cx = 100;
        pDlgTemplate->cy = 100;

        LPWORD lpw = (LPWORD)(pDlgTemplate + 1);

        *lpw++ = 0; // Menu
        *lpw++ = 0; // Class
        *lpw++ = 0; // Title
    }

    return pDlgTemplate;
}

static void ReleaseDialogTemplate(LPCDLGTEMPLATE pDlgTemplate)
{
    if (!HeapFree(GetProcessHeap(), 0, (LPVOID)pDlgTemplate))
    {
        TraceWin32(GetLastError(), TEXT("ReleaseDialogTemplate"), TEXT("HeapFree has failed"));
    }
}

BOOL AboutDialog_ShowModal(HWND hwndOwner)
{
    BOOL bResult = FALSE;
    LPCDLGTEMPLATE pDlgTemplate = CreateDialogTemplate();

    if (pDlgTemplate != NULL)
    {
        INT_PTR iResult = DialogBoxIndirectParam(GetInstanceHandle(), pDlgTemplate, hwndOwner, &DialogProc, 0L);
        DWORD dwLastError = GetLastError();
        bResult = iResult != 0 && iResult != -1;
        ReleaseDialogTemplate(pDlgTemplate);
        SetLastError(dwLastError);
    }

    return bResult;
}