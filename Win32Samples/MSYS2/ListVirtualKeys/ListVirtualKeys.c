#include "WinDefs.h"
#include "VirtualKeyuTIL.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <wchar.h>

void RemoveNonPrintable(LPTSTR pszSrc)
{
    PTCHAR pchDest;

    for (pchDest = pszSrc; *pszSrc != TEXT('\0'); ++pszSrc)
    {
        if (_istascii(*pszSrc) && (_istgraph(*pszSrc) || *pszSrc == TEXT(' ')))
        {
            if (pchDest < pszSrc)
            {
                *pchDest = *pszSrc;
            }

            ++pchDest;
        }
    }

    if (pchDest < pszSrc)
    {
        *pchDest = TEXT('\0');
    }
}

int _tmain(int argc, PTCHAR argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

#ifdef _UNICODE
    _tprintf(TEXT("_UNICODE is defined\n"));
#endif

#ifdef UNICODE
    _tprintf(TEXT("UNICODE is defined\n"));
#endif

    HKL hkl = GetKeyboardLayout(0);

    for (UINT uVirtualKeyCode = 0; uVirtualKeyCode < 256; ++uVirtualKeyCode)
    {
        TCHAR szVirtualKeyName[64];
        BOOL bExtended = VirtualKeyUtil_IsExtendedKey(uVirtualKeyCode);
        UINT uScanCode1;
        UINT uScanCode2;
        TCHAR szBuffer1[64];
        TCHAR szBuffer2[64];
        TCHAR szBuffer3[64];
        TCHAR szBuffer4[64];
        TCHAR szBuffer5[64];

        BOOL bStatus = FALSE;

        VirtualKeyUtil_GetVirtualKeyFormattedName(szVirtualKeyName, _countof(szVirtualKeyName), uVirtualKeyCode);

        switch (uVirtualKeyCode)
        {
        case VK_CANCEL:
        case VK_SNAPSHOT:
        case VK_DIVIDE:

        case VK_SELECT:
        case VK_EXECUTE:
        case VK_HELP:
        case VK_PRINT:
        case VK_PAUSE:
        case VK_RETURN:

            bStatus = TRUE;
        }

        if (!bStatus)
        {
            //continue;
        }

        ZeroMemory(szBuffer1, sizeof(szBuffer1));
        ZeroMemory(szBuffer2, sizeof(szBuffer2));
        ZeroMemory(szBuffer3, sizeof(szBuffer3));
        ZeroMemory(szBuffer4, sizeof(szBuffer4));
        ZeroMemory(szBuffer5, sizeof(szBuffer5));

        uScanCode1 = MapVirtualKey(uVirtualKeyCode, MAPVK_VK_TO_VSC);

        if (uScanCode1 > 0)
        {
            if (GetKeyNameText(uScanCode1 << 16, szBuffer1, _countof(szBuffer1)) == 0)
            {
                szBuffer1[0] = TEXT('\0');
            }

            uScanCode1 |= KF_EXTENDED;

            if (GetKeyNameText(uScanCode1 << 16, szBuffer2, _countof(szBuffer2)) == 0)
            {
                szBuffer2[0] = TEXT('\0');
            }
        }

        uScanCode2 = MapVirtualKeyEx(uVirtualKeyCode, MAPVK_VK_TO_VSC, hkl);

        if (uScanCode2 > 0)
        {
            if (GetKeyNameText(uScanCode2 << 16, szBuffer3, _countof(szBuffer3)) == 0)
            {
                szBuffer3[0] = TEXT('\0');
            }

            if (bExtended)
            {
                uScanCode2 |= KF_EXTENDED;
            }

            if (GetKeyNameText(uScanCode2 << 16, szBuffer4, _countof(szBuffer4)) == 0)
            {
                szBuffer4[0] = TEXT('\0');
            }
        }

        RemoveNonPrintable(szBuffer1);
        RemoveNonPrintable(szBuffer2);
        RemoveNonPrintable(szBuffer3);
        RemoveNonPrintable(szBuffer4);

        VirtualKeyUtil_GetVirtualKeyNameText(szBuffer5, _countof(szBuffer5), uVirtualKeyCode);

        if (_tcscmp(szBuffer1, szBuffer3) != 0 || _tcscmp(szBuffer2, szBuffer4) != 0)
        {
            _tprintf(TEXT("VKC = %3u %ls %-34ls SC1 = %3u %-16ls %-16ls SC2 = %3u %-16ls %-16ls | %ls\n"),
                     uVirtualKeyCode, bExtended ? TEXT("ext") : TEXT("   "), szVirtualKeyName,
                     uScanCode1, szBuffer1, szBuffer2,
                     uScanCode2, szBuffer3, szBuffer4,
                     szBuffer5);
        }
    }

    return 0;
}