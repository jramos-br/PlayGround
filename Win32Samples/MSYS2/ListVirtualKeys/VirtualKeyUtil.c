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

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0604
#endif

#include "VirtualKeyUtil.h"
#include <tchar.h>
#include <wchar.h>
#include <ctype.h>

typedef struct _VKEYMAP VKEYMAP, *LPVKEYMAP;

struct _VKEYMAP
{
    UINT uVirtualKeyCode;
    LPCTSTR pszVirtualKeyName;
};

static VKEYMAP m_vkeymap[] = {
    { VK_LBUTTON, TEXT("VK_LBUTTON") },
    { VK_RBUTTON, TEXT("VK_RBUTTON") },
    { VK_CANCEL, TEXT("VK_CANCEL") },
    { VK_MBUTTON, TEXT("VK_MBUTTON") },
    { VK_XBUTTON1, TEXT("VK_XBUTTON1") },
    { VK_XBUTTON2, TEXT("VK_XBUTTON2") },
    { VK_BACK, TEXT("VK_BACK") },
    { VK_TAB, TEXT("VK_TAB") },
    { VK_CLEAR, TEXT("VK_CLEAR") },
    { VK_RETURN, TEXT("VK_RETURN") },
    { VK_SHIFT, TEXT("VK_SHIFT") },
    { VK_CONTROL, TEXT("VK_CONTROL") },
    { VK_MENU, TEXT("VK_MENU") },
    { VK_PAUSE, TEXT("VK_PAUSE") },
    { VK_CAPITAL, TEXT("VK_CAPITAL") },
    { VK_KANA, TEXT("VK_KANA") },
    { VK_HANGEUL, TEXT("VK_HANGEUL") },
    { VK_HANGUL, TEXT("VK_HANGUL") },
    { VK_IME_ON, TEXT("VK_IME_ON") },
    { VK_JUNJA, TEXT("VK_JUNJA") },
    { VK_FINAL, TEXT("VK_FINAL") },
    { VK_HANJA, TEXT("VK_HANJA") },
    { VK_KANJI, TEXT("VK_KANJI") },
    { VK_IME_OFF, TEXT("VK_IME_OFF") },
    { VK_ESCAPE, TEXT("VK_ESCAPE") },
    { VK_CONVERT, TEXT("VK_CONVERT") },
    { VK_NONCONVERT, TEXT("VK_NONCONVERT") },
    { VK_ACCEPT, TEXT("VK_ACCEPT") },
    { VK_MODECHANGE, TEXT("VK_MODECHANGE") },
    { VK_SPACE, TEXT("VK_SPACE") },
    { VK_PRIOR, TEXT("VK_PRIOR") },
    { VK_NEXT, TEXT("VK_NEXT") },
    { VK_END, TEXT("VK_END") },
    { VK_HOME, TEXT("VK_HOME") },
    { VK_LEFT, TEXT("VK_LEFT") },
    { VK_UP, TEXT("VK_UP") },
    { VK_RIGHT, TEXT("VK_RIGHT") },
    { VK_DOWN, TEXT("VK_DOWN") },
    { VK_SELECT, TEXT("VK_SELECT") },
    { VK_PRINT, TEXT("VK_PRINT") },
    { VK_EXECUTE, TEXT("VK_EXECUTE") },
    { VK_SNAPSHOT, TEXT("VK_SNAPSHOT") },
    { VK_INSERT, TEXT("VK_INSERT") },
    { VK_DELETE, TEXT("VK_DELETE") },
    { VK_HELP, TEXT("VK_HELP") },
    { VK_LWIN, TEXT("VK_LWIN") },
    { VK_RWIN, TEXT("VK_RWIN") },
    { VK_APPS, TEXT("VK_APPS") },
    { VK_SLEEP, TEXT("VK_SLEEP") },
    { VK_NUMPAD0, TEXT("VK_NUMPAD0") },
    { VK_NUMPAD1, TEXT("VK_NUMPAD1") },
    { VK_NUMPAD2, TEXT("VK_NUMPAD2") },
    { VK_NUMPAD3, TEXT("VK_NUMPAD3") },
    { VK_NUMPAD4, TEXT("VK_NUMPAD4") },
    { VK_NUMPAD5, TEXT("VK_NUMPAD5") },
    { VK_NUMPAD6, TEXT("VK_NUMPAD6") },
    { VK_NUMPAD7, TEXT("VK_NUMPAD7") },
    { VK_NUMPAD8, TEXT("VK_NUMPAD8") },
    { VK_NUMPAD9, TEXT("VK_NUMPAD9") },
    { VK_MULTIPLY, TEXT("VK_MULTIPLY") },
    { VK_ADD, TEXT("VK_ADD") },
    { VK_SEPARATOR, TEXT("VK_SEPARATOR") },
    { VK_SUBTRACT, TEXT("VK_SUBTRACT") },
    { VK_DECIMAL, TEXT("VK_DECIMAL") },
    { VK_DIVIDE, TEXT("VK_DIVIDE") },
    { VK_F1, TEXT("VK_F1") },
    { VK_F2, TEXT("VK_F2") },
    { VK_F3, TEXT("VK_F3") },
    { VK_F4, TEXT("VK_F4") },
    { VK_F5, TEXT("VK_F5") },
    { VK_F6, TEXT("VK_F6") },
    { VK_F7, TEXT("VK_F7") },
    { VK_F8, TEXT("VK_F8") },
    { VK_F9, TEXT("VK_F9") },
    { VK_F10, TEXT("VK_F10") },
    { VK_F11, TEXT("VK_F11") },
    { VK_F12, TEXT("VK_F12") },
    { VK_F13, TEXT("VK_F13") },
    { VK_F14, TEXT("VK_F14") },
    { VK_F15, TEXT("VK_F15") },
    { VK_F16, TEXT("VK_F16") },
    { VK_F17, TEXT("VK_F17") },
    { VK_F18, TEXT("VK_F18") },
    { VK_F19, TEXT("VK_F19") },
    { VK_F20, TEXT("VK_F20") },
    { VK_F21, TEXT("VK_F21") },
    { VK_F22, TEXT("VK_F22") },
    { VK_F23, TEXT("VK_F23") },
    { VK_F24, TEXT("VK_F24") },
#if _WIN32_WINNT >= 0x0604
    { VK_NAVIGATION_VIEW, TEXT("VK_NAVIGATION_VIEW") },
    { VK_NAVIGATION_MENU, TEXT("VK_NAVIGATION_MENU") },
    { VK_NAVIGATION_UP, TEXT("VK_NAVIGATION_UP") },
    { VK_NAVIGATION_DOWN, TEXT("VK_NAVIGATION_DOWN") },
    { VK_NAVIGATION_LEFT, TEXT("VK_NAVIGATION_LEFT") },
    { VK_NAVIGATION_RIGHT, TEXT("VK_NAVIGATION_RIGHT") },
    { VK_NAVIGATION_ACCEPT, TEXT("VK_NAVIGATION_ACCEPT") },
    { VK_NAVIGATION_CANCEL, TEXT("VK_NAVIGATION_CANCEL") },
#endif /* _WIN32_WINNT >= 0x0604 */
    { VK_NUMLOCK, TEXT("VK_NUMLOCK") },
    { VK_SCROLL, TEXT("VK_SCROLL") },
    { VK_OEM_NEC_EQUAL, TEXT("VK_OEM_NEC_EQUAL") },
    { VK_OEM_FJ_JISHO, TEXT("VK_OEM_FJ_JISHO") },
    { VK_OEM_FJ_MASSHOU, TEXT("VK_OEM_FJ_MASSHOU") },
    { VK_OEM_FJ_TOUROKU, TEXT("VK_OEM_FJ_TOUROKU") },
    { VK_OEM_FJ_LOYA, TEXT("VK_OEM_FJ_LOYA") },
    { VK_OEM_FJ_ROYA, TEXT("VK_OEM_FJ_ROYA") },
    { VK_LSHIFT, TEXT("VK_LSHIFT") },
    { VK_RSHIFT, TEXT("VK_RSHIFT") },
    { VK_LCONTROL, TEXT("VK_LCONTROL") },
    { VK_RCONTROL, TEXT("VK_RCONTROL") },
    { VK_LMENU, TEXT("VK_LMENU") },
    { VK_RMENU, TEXT("VK_RMENU") },
    { VK_BROWSER_BACK, TEXT("VK_BROWSER_BACK") },
    { VK_BROWSER_FORWARD, TEXT("VK_BROWSER_FORWARD") },
    { VK_BROWSER_REFRESH, TEXT("VK_BROWSER_REFRESH") },
    { VK_BROWSER_STOP, TEXT("VK_BROWSER_STOP") },
    { VK_BROWSER_SEARCH, TEXT("VK_BROWSER_SEARCH") },
    { VK_BROWSER_FAVORITES, TEXT("VK_BROWSER_FAVORITES") },
    { VK_BROWSER_HOME, TEXT("VK_BROWSER_HOME") },
    { VK_VOLUME_MUTE, TEXT("VK_VOLUME_MUTE") },
    { VK_VOLUME_DOWN, TEXT("VK_VOLUME_DOWN") },
    { VK_VOLUME_UP, TEXT("VK_VOLUME_UP") },
    { VK_MEDIA_NEXT_TRACK, TEXT("VK_MEDIA_NEXT_TRACK") },
    { VK_MEDIA_PREV_TRACK, TEXT("VK_MEDIA_PREV_TRACK") },
    { VK_MEDIA_STOP, TEXT("VK_MEDIA_STOP") },
    { VK_MEDIA_PLAY_PAUSE, TEXT("VK_MEDIA_PLAY_PAUSE") },
    { VK_LAUNCH_MAIL, TEXT("VK_LAUNCH_MAIL") },
    { VK_LAUNCH_MEDIA_SELECT, TEXT("VK_LAUNCH_MEDIA_SELECT") },
    { VK_LAUNCH_APP1, TEXT("VK_LAUNCH_APP1") },
    { VK_LAUNCH_APP2, TEXT("VK_LAUNCH_APP2") },
    { VK_OEM_1, TEXT("VK_OEM_1") },
    { VK_OEM_PLUS, TEXT("VK_OEM_PLUS") },
    { VK_OEM_COMMA, TEXT("VK_OEM_COMMA") },
    { VK_OEM_MINUS, TEXT("VK_OEM_MINUS") },
    { VK_OEM_PERIOD, TEXT("VK_OEM_PERIOD") },
    { VK_OEM_2, TEXT("VK_OEM_2") },
    { VK_OEM_3, TEXT("VK_OEM_3") },
#if _WIN32_WINNT >= 0x0604
    { VK_GAMEPAD_A, TEXT("VK_GAMEPAD_A") },
    { VK_GAMEPAD_B, TEXT("VK_GAMEPAD_B") },
    { VK_GAMEPAD_X, TEXT("VK_GAMEPAD_X") },
    { VK_GAMEPAD_Y, TEXT("VK_GAMEPAD_Y") },
    { VK_GAMEPAD_RIGHT_SHOULDER, TEXT("VK_GAMEPAD_RIGHT_SHOULDER") },
    { VK_GAMEPAD_LEFT_SHOULDER, TEXT("VK_GAMEPAD_LEFT_SHOULDER") },
    { VK_GAMEPAD_LEFT_TRIGGER, TEXT("VK_GAMEPAD_LEFT_TRIGGER") },
    { VK_GAMEPAD_RIGHT_TRIGGER, TEXT("VK_GAMEPAD_RIGHT_TRIGGER") },
    { VK_GAMEPAD_DPAD_UP, TEXT("VK_GAMEPAD_DPAD_UP") },
    { VK_GAMEPAD_DPAD_DOWN, TEXT("VK_GAMEPAD_DPAD_DOWN") },
    { VK_GAMEPAD_DPAD_LEFT, TEXT("VK_GAMEPAD_DPAD_LEFT") },
    { VK_GAMEPAD_DPAD_RIGHT, TEXT("VK_GAMEPAD_DPAD_RIGHT") },
    { VK_GAMEPAD_MENU, TEXT("VK_GAMEPAD_MENU") },
    { VK_GAMEPAD_VIEW, TEXT("VK_GAMEPAD_VIEW") },
    { VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON, TEXT("VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON") },
    { VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON, TEXT("VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON") },
    { VK_GAMEPAD_LEFT_THUMBSTICK_UP, TEXT("VK_GAMEPAD_LEFT_THUMBSTICK_UP") },
    { VK_GAMEPAD_LEFT_THUMBSTICK_DOWN, TEXT("VK_GAMEPAD_LEFT_THUMBSTICK_DOWN") },
    { VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT, TEXT("VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT") },
    { VK_GAMEPAD_LEFT_THUMBSTICK_LEFT, TEXT("VK_GAMEPAD_LEFT_THUMBSTICK_LEFT") },
    { VK_GAMEPAD_RIGHT_THUMBSTICK_UP, TEXT("VK_GAMEPAD_RIGHT_THUMBSTICK_UP") },
    { VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN, TEXT("VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN") },
    { VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT, TEXT("VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT") },
    { VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT, TEXT("VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT") },
#endif /* _WIN32_WINNT >= 0x0604 */
    { VK_OEM_4, TEXT("VK_OEM_4") },
    { VK_OEM_5, TEXT("VK_OEM_5") },
    { VK_OEM_6, TEXT("VK_OEM_6") },
    { VK_OEM_7, TEXT("VK_OEM_7") },
    { VK_OEM_8, TEXT("VK_OEM_8") },
    { VK_OEM_AX, TEXT("VK_OEM_AX") },
    { VK_OEM_102, TEXT("VK_OEM_102") },
    { VK_ICO_HELP, TEXT("VK_ICO_HELP") },
    { VK_ICO_00, TEXT("VK_ICO_00") },
    { VK_PROCESSKEY, TEXT("VK_PROCESSKEY") },
    { VK_ICO_CLEAR, TEXT("VK_ICO_CLEAR") },
    { VK_PACKET, TEXT("VK_PACKET") },
    { VK_OEM_RESET, TEXT("VK_OEM_RESET") },
    { VK_OEM_JUMP, TEXT("VK_OEM_JUMP") },
    { VK_OEM_PA1, TEXT("VK_OEM_PA1") },
    { VK_OEM_PA2, TEXT("VK_OEM_PA2") },
    { VK_OEM_PA3, TEXT("VK_OEM_PA3") },
    { VK_OEM_WSCTRL, TEXT("VK_OEM_WSCTRL") },
    { VK_OEM_CUSEL, TEXT("VK_OEM_CUSEL") },
    { VK_OEM_ATTN, TEXT("VK_OEM_ATTN") },
    { VK_OEM_FINISH, TEXT("VK_OEM_FINISH") },
    { VK_OEM_COPY, TEXT("VK_OEM_COPY") },
    { VK_OEM_AUTO, TEXT("VK_OEM_AUTO") },
    { VK_OEM_ENLW, TEXT("VK_OEM_ENLW") },
    { VK_OEM_BACKTAB, TEXT("VK_OEM_BACKTAB") },
    { VK_ATTN, TEXT("VK_ATTN") },
    { VK_CRSEL, TEXT("VK_CRSEL") },
    { VK_EXSEL, TEXT("VK_EXSEL") },
    { VK_EREOF, TEXT("VK_EREOF") },
    { VK_PLAY, TEXT("VK_PLAY") },
    { VK_ZOOM, TEXT("VK_ZOOM") },
    { VK_NONAME, TEXT("VK_NONAME") },
    { VK_PA1, TEXT("VK_PA1") },
    { VK_OEM_CLEAR, TEXT("VK_OEM_CLEAR") }
};

static void CopyKeyName(LPTSTR pszDest, size_t cchDest, LPCTSTR pszKeyName)
{
    TCHAR *pchDest = pszDest;
    const TCHAR *pchKeyName = pszKeyName;

    while (*pchKeyName != TEXT('\0') && cchDest > 1)
    {
        *pchDest++ = *pchKeyName++;
        --cchDest;
    }

    if (cchDest > 0)
    {
        *pchDest = TEXT('\0');
    }
}

static void FormatKeyName(LPTSTR pszDest, size_t cchDest, LPCTSTR pszKeyName)
{
    TCHAR *pchDest = pszDest;
    const TCHAR *pchKeyName = pszKeyName;
    int cchKeyName = lstrlen(pchKeyName);

    if (cchKeyName >= 3 && pchKeyName[0] == 'V' && pchKeyName[1] == 'K' && pchKeyName[2] == '_')
    {
        pchKeyName += 3, cchKeyName -= 3;
    }

    while (cchKeyName > 0 && cchDest > 1)
    {
        if (*pchKeyName == TEXT('_'))
        {
            *pchDest = TEXT(' ');
        }
        else if (pchDest == pszDest || *(pchDest - 1) == TEXT(' '))
        {
            *pchDest = *pchKeyName;
            CharUpperBuff(pchDest, 1);
        }
        else
        {
            *pchDest = *pchKeyName;
            CharLowerBuff(pchDest, 1);
        }

        ++pchKeyName, --cchKeyName, ++pchDest, --cchDest;
    }

    if (cchDest > 0)
    {
        *pchDest = TEXT('\0');
    }
}

static void RemoveNonPrintable(LPTSTR pszDest)
{
    TCHAR *pchDest;

    for (pchDest = pszDest; *pszDest != TEXT('\0'); ++pszDest)
    {
        if (_istascii(*pszDest) && (_istgraph(*pszDest) || *pszDest == TEXT(' ')))
        {
            if (pchDest < pszDest)
            {
                *pchDest = *pszDest;
            }

            ++pchDest;
        }
    }

    if (pchDest < pszDest)
    {
        *pchDest = TEXT('\0');
    }
}

/* From MSDN:
   The extended-key flag indicates whether the keystroke message originated from one of
   the additional keys on the enhanced keyboard. The extended keys consist of the ALT and
   CTRL keys on the right-hand side of the keyboard; the INS, DEL, HOME, END, PAGE UP,
   PAGE DOWN, and arrow keys in the clusters to the left of the numeric keypad; the NUM
   LOCK key; the BREAK (CTRL+PAUSE) key; the PRINT SCRN key; and the divide (/) and ENTER
   keys in the numeric keypad. The extended-key flag is set if the key is an extended key. */
BOOL VirtualKeyUtil_IsExtendedKey(UINT uVirtualKeyCode)
{
    switch (uVirtualKeyCode)
    {
    case VK_RMENU:
    case VK_RCONTROL:
    case VK_INSERT:
    case VK_DELETE:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_UP:
    case VK_DOWN:
    case VK_LEFT:
    case VK_RIGHT:
    case VK_NUMLOCK:

    case VK_CANCEL:
    case VK_SNAPSHOT:
    case VK_DIVIDE:

    case VK_LWIN:
    case VK_RWIN:
    case VK_APPS:
        return TRUE;
    }

    return FALSE;
}

BOOL VirtualKeyUtil_GetVirtualKeySymbolicName(LPTSTR pszDest, size_t cchDest, UINT uVirtualKeyCode)
{
    for (UINT i = 0; i < _countof(m_vkeymap); ++i)
    {
        if (m_vkeymap[i].uVirtualKeyCode == uVirtualKeyCode)
        {
            CopyKeyName(pszDest, cchDest, m_vkeymap[i].pszVirtualKeyName);
            return TRUE;
        }
    }

    if (cchDest > 0)
    {
        *pszDest = TEXT('\0');
    }

    return FALSE;
}

BOOL VirtualKeyUtil_GetVirtualKeyFormattedName(LPTSTR pszDest, size_t cchDest, UINT uVirtualKeyCode)
{
    for (UINT i = 0; i < _countof(m_vkeymap); ++i)
    {
        if (m_vkeymap[i].uVirtualKeyCode == uVirtualKeyCode)
        {
            FormatKeyName(pszDest, cchDest, m_vkeymap[i].pszVirtualKeyName);
            return TRUE;
        }
    }

    if (cchDest > 0)
    {
        *pszDest = TEXT('\0');
    }

    return FALSE;
}

BOOL VirtualKeyUtil_GetVirtualKeyNameText(LPTSTR pszDest, size_t cchDest, UINT uVirtualKeyCode)
{
    HKL hkl = GetKeyboardLayout(0);
    UINT uScanCode = MapVirtualKeyEx(uVirtualKeyCode, MAPVK_VK_TO_VSC, hkl);

    if (uScanCode > 0)
    {
        if (VirtualKeyUtil_IsExtendedKey(uVirtualKeyCode))
        {
            uScanCode |= KF_EXTENDED;
        }

        if (GetKeyNameText(uScanCode << 16, pszDest, cchDest) > 0)
        {
            RemoveNonPrintable(pszDest);

            if (lstrcmp(pszDest, TEXT("<00>")) == 0)
            {
                CopyKeyName(pszDest, cchDest, TEXT("PrtScn"));
            }

            return TRUE;
        }
    }

    return FALSE; /* Preserves GetLastError() */
}