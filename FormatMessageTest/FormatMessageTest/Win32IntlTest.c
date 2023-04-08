//////////////////////////////////////////////////////////////////////////////////////////
//
// Test functions for WIN32 NLS.
//
// Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
//
// This is free software. Redistribution and use in source and binary forms,
// with or without modification, for any purpose and with or without fee are
// hereby permitted. Altered source versions must be plainly marked as such.
//
// If you find this software useful, an acknowledgment would be appreciated
// but is not required.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
// FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WIN32_WINNT
//#define _WIN32_WINNT    0x0400
#endif

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef FKG_FORCED_USAGE
#define FKG_FORCED_USAGE
#endif

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#ifndef _INC_SDKDDKVER

#define _WIN32_WINNT_NT4                0x0400 // Windows NT 4.0
#define _WIN32_WINNT_WIN2K              0x0500 // Windows 2000
#define _WIN32_WINNT_WINXP              0x0501 // Windows XP
#define _WIN32_WINNT_WS03               0x0502 // Windows Server 2003
#define _WIN32_WINNT_WIN6               0x0600 // Windows Vista
#define _WIN32_WINNT_VISTA              0x0600 // Windows Vista
#define _WIN32_WINNT_WS08               0x0600 // Windows Server 2008
#define _WIN32_WINNT_LONGHORN           0x0600 // Windows Vista
#define _WIN32_WINNT_WIN7               0x0601 // Windows 7
#define _WIN32_WINNT_WIN8               0x0602 // Windows 8
#define _WIN32_WINNT_WINBLUE            0x0603 // Windows 8.1
#define _WIN32_WINNT_WINTHRESHOLD       0x0A00 // Windows 10
#define _WIN32_WINNT_WIN10              0x0A00 // Windows 10

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#include "Win32IntlTest.h"

//////////////////////////////////////////////////////////////////////////////////////////

// https://docs.microsoft.com/en-us/windows/win32/intl/national-language-support
// https://docs.microsoft.com/en-us/windows/win32/intl/user-interface-language-management

// GetThreadLocale
// Returns the locale identifier of the current locale for the calling thread.
// Windows Vista: This function can return the identifier of a custom locale. If the
// current thread locale is a custom locale, the function returns LOCALE_CUSTOM_DEFAULT.
// If the current thread locale is a supplemental custom locale, the function can return
// LOCALE_CUSTOM_UNSPECIFIED. All supplemental locales share this locale identifier.
//
// GetThreadUILanguage
// Returns the identifier for a language explicitly associated with the thread by
// SetThreadUILanguage or SetThreadPreferredUILanguages. Alternatively, if no language has
// been explicitly associated with the current thread, the identifier can indicate a user
// or system user interface language.
//
// GetThreadPreferredUILanguages
// Retrieves the thread preferred UI languages for the current thread.

// GetUserDefaultLCID
// Returns the locale identifier for the user default locale, represented as
// LOCALE_USER_DEFAULT. If the user default locale is a custom locale, this function
// always returns LOCALE_CUSTOM_DEFAULT, regardless of the custom locale that is selected.
//
// GetUserDefaultLocaleName
// Retrieves the user default locale name.
//
// GetUserDefaultLangID
// Returns the language identifier of the Region Format setting for the current user.
//
// GetUserDefaultUILanguage
// Returns the language identifier for the user UI language for the current user. If the
// current user has not set a language, GetUserDefaultUILanguage returns the preferred
// language set for the system. If there is no preferred language set for the system, then
// the system default UI language (also known as "install language") is returned.
//
// GetUserPreferredUILanguages
// Retrieves information about the user preferred UI languages.

// GetSystemDefaultLCID
// Returns the locale identifier for the system default locale, identified by
// LOCALE_SYSTEM_DEFAULT.
//
// GetSystemDefaultLocaleName
// Retrieves the system default locale name.
//
// GetSystemDefaultLangID
// Returns the language identifier for the system locale. This is the language used when
// displaying text in programs that do not support Unicode.
//
// GetSystemDefaultUILanguage
// Retrieves the language identifier for the system default UI language of the operating
// system, also known as the "install language" on Windows Vista and later. For more
// information, see User Interface Language Management.
// This function never returns a language identifier for a Language Interface Pack (LIP).
// It also never returns a language identifier corresponding to the locale identifier
// LOCALE_CUSTOM_UNSPECIFIED or LOCALE_CUSTOM_UI_DEFAULT.
//
// GetSystemPreferredUILanguages
// Retrieves the system preferred UI languages.

// LCIDToLocaleName
// Converts a locale identifier to a locale name.

// Default System and User IDs for language and locale.
// Locale names such as LOCALE_NAME_SYSTEM_DEFAULT, LOCALE_NAME_USER_DEFAULT,
// and LOCALE_NAME_INVARIANT are preferred.
//
//    Primary Language ID   Sublanguage ID      Result
//    -------------------   ---------------     ------------------------
//    LANG_NEUTRAL          SUBLANG_NEUTRAL     Language neutral
//    LANG_NEUTRAL          SUBLANG_DEFAULT     User default language
//    LANG_NEUTRAL          SUBLANG_SYS_DEFAULT System default language
//    LANG_INVARIANT        SUBLANG_NEUTRAL     Invariant locale
//
// #define LANG_SYSTEM_DEFAULT    (MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT))
// #define LANG_USER_DEFAULT      (MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
// #define LOCALE_SYSTEM_DEFAULT  (MAKELCID(LANG_SYSTEM_DEFAULT, SORT_DEFAULT))
// #define LOCALE_USER_DEFAULT    (MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT))

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

#define MUI_LANGUAGE_ID             0x04 // Use traditional language ID convention
#define MUI_LANGUAGE_NAME           0x08 // Use ISO language (culture) name convention
#define MUI_MERGE_SYSTEM_FALLBACK   0x10 // GetThreadPreferredUILanguages merges in parent and base languages
#define MUI_MERGE_USER_FALLBACK     0x20 // GetThreadPreferredUILanguages merges in user preferred languages

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_WIN7

#define MUI_UI_FALLBACK             MUI_MERGE_SYSTEM_FALLBACK | MUI_MERGE_USER_FALLBACK
#define LOCALE_ALLOW_NEUTRAL_NAMES  0x08000000 //Flag to allow returning neutral names/lcids for name conversion

#endif

//////////////////////////////////////////////////////////////////////////////////////////

static HMODULE GetKernel32ModuleHandle(void)
{
    static HMODULE hKernel32;

    if (hKernel32 == NULL)
    {
        hKernel32 = GetModuleHandle(_T("KERNEL32.DLL"));
    }

    return hKernel32;
}

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_WIN2K

typedef ULONGLONG(NTAPI* PFNVERSETCONDITIONMASK)(ULONGLONG ConditionMask, DWORD TypeMask, BYTE Condition);

static ULONGLONG NTAPI VerSetConditionMaskAux(ULONGLONG ConditionMask, DWORD TypeMask, BYTE Condition)
{
    return ConditionMask | (Condition << (1 << TypeMask));
}

static ULONGLONG VerSetConditionMask(ULONGLONG ConditionMask, DWORD TypeMask, BYTE Condition)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNVERSETCONDITIONMASK pfnVerSetConditionMask;

    if (hKernel32 == NULL)
        return 0;

    if (pfnVerSetConditionMask == NULL)
    {
        pfnVerSetConditionMask = (PFNVERSETCONDITIONMASK)GetProcAddress(hKernel32, "VerSetConditionMask");

        if (pfnVerSetConditionMask == NULL)
            pfnVerSetConditionMask = VerSetConditionMaskAux;
    }

    return pfnVerSetConditionMask(ConditionMask, TypeMask, Condition);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

typedef LANGID(WINAPI* PFNGETTHREADUILANGUAGE)(void);

static LANGID GetThreadUILanguage(void)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNGETTHREADUILANGUAGE pfnGetThreadUILanguage;

    if (hKernel32 == NULL)
        return 0;

    if (pfnGetThreadUILanguage == NULL)
    {
        pfnGetThreadUILanguage = (PFNGETTHREADUILANGUAGE)GetProcAddress(hKernel32, "GetThreadUILanguage");

        if (pfnGetThreadUILanguage == NULL)
            return 0;
    }

    return pfnGetThreadUILanguage();
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

typedef BOOL(WINAPI* PFNGETTHREADPREFERREDUILANGUAGES)(DWORD dwFlags, PULONG pulNumLanguages, PZZWSTR pwszLanguagesBuffer, PULONG pcchLanguagesBuffer);

static BOOL GetThreadPreferredUILanguages(DWORD dwFlags, PULONG pulNumLanguages, PZZWSTR pwszLanguagesBuffer, PULONG pcchLanguagesBuffer)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNGETTHREADPREFERREDUILANGUAGES pfnGetThreadPreferredUILanguages;

    if (hKernel32 == NULL)
        return FALSE;

    if (pfnGetThreadPreferredUILanguages == NULL)
    {
        pfnGetThreadPreferredUILanguages = (PFNGETTHREADPREFERREDUILANGUAGES)GetProcAddress(hKernel32, "GetThreadPreferredUILanguages");

        if (pfnGetThreadPreferredUILanguages == NULL)
            return FALSE;
    }

    return pfnGetThreadPreferredUILanguages(dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

typedef LANGID(WINAPI* PFNGETUSERDEFAULTLOCALENAME)(LPWSTR lpLocaleName, int cchLocaleName);

static LANGID GetUserDefaultLocaleName(LPWSTR lpLocaleName, int cchLocaleName)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNGETUSERDEFAULTLOCALENAME pfnGetUserDefaultLocaleName;

    if (hKernel32 == NULL)
        return 0;

    if (pfnGetUserDefaultLocaleName == NULL)
    {
        pfnGetUserDefaultLocaleName = (PFNGETUSERDEFAULTLOCALENAME)GetProcAddress(hKernel32, "GetUserDefaultLocaleName");

        if (pfnGetUserDefaultLocaleName == NULL)
            return 0;
    }

    return pfnGetUserDefaultLocaleName(lpLocaleName, cchLocaleName);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

typedef BOOL(WINAPI* PFNGETUSERPREFERREDUILANGUAGES)(DWORD dwFlags, PULONG pulNumLanguages, PZZWSTR pwszLanguagesBuffer, PULONG pcchLanguagesBuffer);

static BOOL GetUserPreferredUILanguages(DWORD dwFlags, PULONG pulNumLanguages, PZZWSTR pwszLanguagesBuffer, PULONG pcchLanguagesBuffer)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNGETUSERPREFERREDUILANGUAGES pfnGetUserPreferredUILanguages;

    if (hKernel32 == NULL)
        return FALSE;

    if (pfnGetUserPreferredUILanguages == NULL)
    {
        pfnGetUserPreferredUILanguages = (PFNGETUSERPREFERREDUILANGUAGES)GetProcAddress(hKernel32, "GetUserPreferredUILanguages");

        if (pfnGetUserPreferredUILanguages == NULL)
            return FALSE;
    }

    return pfnGetUserPreferredUILanguages(dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

typedef LANGID(WINAPI* PFNGETSYSTEMDEFAULTLOCALENAME)(LPWSTR lpLocaleName, int cchLocaleName);

static LANGID GetSystemDefaultLocaleName(LPWSTR lpLocaleName, int cchLocaleName)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNGETSYSTEMDEFAULTLOCALENAME pfnGetSystemDefaultLocaleName;

    if (hKernel32 == NULL)
        return 0;

    if (pfnGetSystemDefaultLocaleName == NULL)
    {
        pfnGetSystemDefaultLocaleName = (PFNGETSYSTEMDEFAULTLOCALENAME)GetProcAddress(hKernel32, "GetSystemDefaultLocaleName");

        if (pfnGetSystemDefaultLocaleName == NULL)
            return 0;
    }

    return pfnGetSystemDefaultLocaleName(lpLocaleName, cchLocaleName);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_WIN2K

typedef LANGID(WINAPI* PFNGETSYSTEMDEFAULTUILANGUAGE)(void);

static LANGID GetSystemDefaultUILanguage(void)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNGETSYSTEMDEFAULTUILANGUAGE pfnGetSystemDefaultUILanguage;

    if (hKernel32 == NULL)
        return 0;

    if (pfnGetSystemDefaultUILanguage == NULL)
    {
        pfnGetSystemDefaultUILanguage = (PFNGETSYSTEMDEFAULTUILANGUAGE)GetProcAddress(hKernel32, "GetSystemDefaultUILanguage");

        if (pfnGetSystemDefaultUILanguage == NULL)
            return 0;
    }

    return pfnGetSystemDefaultUILanguage();
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

typedef BOOL(WINAPI* PFNGETSYSTEMPREFERREDUILANGUAGES)(DWORD dwFlags, PULONG pulNumLanguages, PZZWSTR pwszLanguagesBuffer, PULONG pcchLanguagesBuffer);

static BOOL GetSystemPreferredUILanguages(DWORD dwFlags, PULONG pulNumLanguages, PZZWSTR pwszLanguagesBuffer, PULONG pcchLanguagesBuffer)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNGETSYSTEMPREFERREDUILANGUAGES pfnGetSystemPreferredUILanguages;

    if (hKernel32 == NULL)
        return FALSE;

    if (pfnGetSystemPreferredUILanguages == NULL)
    {
        pfnGetSystemPreferredUILanguages = (PFNGETSYSTEMPREFERREDUILANGUAGES)GetProcAddress(hKernel32, "GetSystemPreferredUILanguages");

        if (pfnGetSystemPreferredUILanguages == NULL)
            return FALSE;
    }

    return pfnGetSystemPreferredUILanguages(dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

typedef int (WINAPI* PFNLCIDTOLOCALENAME)(LCID Locale, LPWSTR lpName, int cchName, DWORD dwFlags);

static int LCIDToLocaleName(LCID Locale, LPWSTR lpName, int cchName, DWORD dwFlags)
{
    HMODULE hKernel32 = GetKernel32ModuleHandle();
    static PFNLCIDTOLOCALENAME pfnLCIDToLocaleName;

    if (hKernel32 == NULL)
        return FALSE;

    if (pfnLCIDToLocaleName == NULL)
    {
        pfnLCIDToLocaleName = (PFNLCIDTOLOCALENAME)GetProcAddress(hKernel32, "LCIDToLocaleName");

        if (pfnLCIDToLocaleName == NULL)
            return FALSE;
    }

    return pfnLCIDToLocaleName(Locale, lpName, cchName, dwFlags);
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////

static BOOL IsWindowsNT(void)
{
    OSVERSIONINFO osvi = { sizeof(osvi) };
    return GetVersionEx(&osvi) && osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

//////////////////////////////////////////////////////////////////////////////////////////

static BOOL IsWin32WinNtVersionOrGreater(WORD wWin32WinNtVersion)
{
    BOOL fResult = FALSE;

    if (IsWindowsNT())
    {
        OSVERSIONINFOEX osvi = { sizeof(osvi) };
        DWORDLONG dwlConditionMask = 0;
        DWORD dwTypeMask = VER_MAJORVERSION | VER_MINORVERSION;

        dwlConditionMask = VerSetConditionMask(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
        dwlConditionMask = VerSetConditionMask(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

        osvi.dwMajorVersion = (wWin32WinNtVersion & 0xFF00) >> 8;
        osvi.dwMinorVersion = (wWin32WinNtVersion & 0x00FF);

        if (!VerifyVersionInfo(&osvi, dwTypeMask, dwlConditionMask))
        {
            DWORD dwLastError = GetLastError();
            if (dwLastError != ERROR_OLD_WIN_VERSION)
                _tprintf(_T("VerifyVersionInfo = [ERROR %d 0x%08X]\n"), dwLastError, dwLastError);
        }
        else
            fResult = TRUE;
    }

    return fResult;
}

//////////////////////////////////////////////////////////////////////////////////////////

static BOOL GetLocaleName(LCID lcid, LPTSTR pszLocaleName, size_t cchLocaleName)
{
    DWORD dwFlags = IsWin32WinNtVersionOrGreater(_WIN32_WINNT_WIN7) ? LOCALE_ALLOW_NEUTRAL_NAMES : 0;

#ifdef UNICODE

    if (LCIDToLocaleName(lcid, pszLocaleName, cchLocaleName, dwFlags) == 0)
        return FALSE;

#else

    WCHAR wszLocaleName[LOCALE_NAME_MAX_LENGTH];
    size_t cchLength;

    if (LCIDToLocaleName(lcid, wszLocaleName, _countof(wszLocaleName), dwFlags) == 0)
        return FALSE;

    if ((cchLength = wcslen(wszLocaleName)) >= cchLocaleName)
    {
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return FALSE;
    }

    if (WideCharToMultiByte(CP_THREAD_ACP, 0, wszLocaleName, cchLength + 1, pszLocaleName, cchLocaleName, NULL, NULL) == 0)
        return FALSE;

#endif

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////

static BOOL GetLanguageName(LANGID langid, LPTSTR pszLanguageName, size_t cchLanguageName)
{
    LCID lcid = MAKELCID(langid, SORT_DEFAULT);

    return GetLocaleInfo(lcid, LOCALE_SLANGUAGE, pszLanguageName, cchLanguageName) > 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

static void PrintLocale(LCID lcid, LPCTSTR name, LPCTSTR extra)
{
    TCHAR szLocaleName[LOCALE_NAME_MAX_LENGTH];

    if (!GetLocaleName(lcid, szLocaleName, _countof(szLocaleName)))
    {
        DWORD dwLastError = GetLastError();

        if (extra == NULL)
            _tprintf(_T("%s = %d 0x%04X [ERROR %d 0x%08X]\n"), name, lcid, lcid, dwLastError, dwLastError);
        else
            _tprintf(_T("%s = %d 0x%04X [ERROR %d 0x%08X] <= %s\n"), name, lcid, lcid, dwLastError, dwLastError, extra);
    }
    else
    {
        if (extra == NULL)
            _tprintf(_T("%s = %d 0x%04X [%s]\n"), name, lcid, lcid, szLocaleName);
        else
            _tprintf(_T("%s = %d 0x%04X [%s] <= %s\n"), name, lcid, lcid, szLocaleName, extra);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

static void PrintLocaleName(BOOL status, LPCWSTR szLocaleName, LPCTSTR name, LPCTSTR extra)
{
    if (status == 0)
    {
        DWORD dwLastError = GetLastError();

        if (extra == NULL)
            _tprintf(_T("%s = [ERROR %d 0x%08X]\n"), name, dwLastError, dwLastError);
        else
            _tprintf(_T("%s = [ERROR %d 0x%08X] <= %s\n"), name, dwLastError, dwLastError, extra);
    }
    else
    {
        if (extra == NULL)
            _tprintf(_T("%s = %ls\n"), name, szLocaleName);
        else
            _tprintf(_T("%s = %ls <= %s\n"), name, szLocaleName, extra);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

static void PrintLanguage(LANGID langid, LPCTSTR name, LPCTSTR extra)
{
    TCHAR szLanguageName[LOCALE_NAME_MAX_LENGTH];

    if (!GetLanguageName(langid, szLanguageName, _countof(szLanguageName)))
    {
        DWORD dwLastError = GetLastError();

        if (extra == NULL)
            _tprintf(_T("%s = %d 0x%04X [ERROR %d 0x%08X]\n"), name, langid, langid, dwLastError, dwLastError);
        else
            _tprintf(_T("%s = %d 0x%04X [ERROR %d 0x%08X] <= %s\n"), name, langid, langid, dwLastError, dwLastError, extra);
    }
    else
    {
        if (extra == NULL)
            _tprintf(_T("%s = %d 0x%04X [%s]\n"), name, langid, langid, szLanguageName);
        else
            _tprintf(_T("%s = %d 0x%04X [%s] <= %s\n"), name, langid, langid, szLanguageName, extra);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

static void PrintThreadPreferredUILanguages(DWORD dwFlags)
{
    ULONG ulNumLanguages = 0;
    ULONG cchLanguagesBuffer = 0;
    LPWSTR pwszzLanguagesBuffer;

    if (!GetThreadPreferredUILanguages(dwFlags, &ulNumLanguages, NULL, &cchLanguagesBuffer))
    {
        DWORD dwLastError = GetLastError();
        _tprintf(_T("GetThreadPreferredUILanguages = [ERROR %d 0x%08X]\n"),
            dwLastError, dwLastError);
        return;
    }

    pwszzLanguagesBuffer = (LPWSTR)calloc(cchLanguagesBuffer, sizeof(WCHAR));

    if (pwszzLanguagesBuffer == NULL)
    {
        errno = ENOMEM;
        _tperror(_T("GetThreadPreferredUILanguages"));
        return;
    }

    if (!GetThreadPreferredUILanguages(dwFlags, &ulNumLanguages, pwszzLanguagesBuffer, &cchLanguagesBuffer))
    {
        DWORD dwLastError = GetLastError();
        _tprintf(_T("GetThreadPreferredUILanguages = [ERROR %d 0x%08X]\n"),
            dwLastError, dwLastError);
    }
    else
    {
        LPWSTR pszz = pwszzLanguagesBuffer;
        int count = 0;

        while (*pszz != L'\0')
        {
            size_t len = lstrlenW(pszz);
            _tprintf(_T("GetThreadPreferredUILanguages [%d/%d] = %ls\n"), ++count, ulNumLanguages, pszz);
            pszz += len + 1;
        }
    }

    free(pwszzLanguagesBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

static void PrintUserPreferredUILanguages(DWORD dwFlags)
{
    ULONG ulNumLanguages = 0;
    ULONG cchLanguagesBuffer = 0;
    LPWSTR pwszzLanguagesBuffer;

    if (!GetUserPreferredUILanguages(dwFlags, &ulNumLanguages, NULL, &cchLanguagesBuffer))
    {
        DWORD dwLastError = GetLastError();
        _tprintf(_T("GetUserPreferredUILanguages = [ERROR %d 0x%08X]\n"),
            dwLastError, dwLastError);
        return;
    }

    pwszzLanguagesBuffer = (LPWSTR)calloc(cchLanguagesBuffer, sizeof(WCHAR));

    if (pwszzLanguagesBuffer == NULL)
    {
        errno = ENOMEM;
        _tperror(_T("GetUserPreferredUILanguages"));
        return;
    }

    if (!GetUserPreferredUILanguages(dwFlags, &ulNumLanguages, pwszzLanguagesBuffer, &cchLanguagesBuffer))
    {
        DWORD dwLastError = GetLastError();
        _tprintf(_T("GetUserPreferredUILanguages = [ERROR %d 0x%08X]\n"),
            dwLastError, dwLastError);
    }
    else
    {
        LPWSTR pszz = pwszzLanguagesBuffer;
        int count = 0;

        while (*pszz != L'\0')
        {
            size_t len = lstrlenW(pszz);
            _tprintf(_T("GetUserPreferredUILanguages [%d/%d] = %ls\n"), ++count, ulNumLanguages, pszz);
            pszz += len + 1;
        }
    }

    free(pwszzLanguagesBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

static void PrintSystemPreferredUILanguages(DWORD dwFlags)
{
    ULONG ulNumLanguages = 0;
    ULONG cchLanguagesBuffer = 0;
    LPWSTR pwszzLanguagesBuffer;

    if (!GetSystemPreferredUILanguages(dwFlags, &ulNumLanguages, NULL, &cchLanguagesBuffer))
    {
        DWORD dwLastError = GetLastError();
        _tprintf(_T("GetSystemPreferredUILanguages = [ERROR %d 0x%08X]\n"),
            dwLastError, dwLastError);
        return;
    }

    pwszzLanguagesBuffer = (LPWSTR)calloc(cchLanguagesBuffer, sizeof(WCHAR));

    if (pwszzLanguagesBuffer == NULL)
    {
        errno = ENOMEM;
        _tperror(_T("GetSystemPreferredUILanguages"));
        return;
    }

    if (!GetSystemPreferredUILanguages(dwFlags, &ulNumLanguages, pwszzLanguagesBuffer, &cchLanguagesBuffer))
    {
        DWORD dwLastError = GetLastError();
        _tprintf(_T("GetSystemPreferredUILanguages = [ERROR %d 0x%08X]\n"),
            dwLastError, dwLastError);
    }
    else
    {
        LPWSTR pszz = pwszzLanguagesBuffer;
        int count = 0;

        while (*pszz != L'\0')
        {
            size_t len = lstrlenW(pszz);
            _tprintf(_T("GetSystemPreferredUILanguages [%d/%d] = %ls\n"), ++count, ulNumLanguages, pszz);
            pszz += len + 1;
        }
    }

    free(pwszzLanguagesBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

static void PrintData(void)
{
    WCHAR szLocaleName[LOCALE_NAME_MAX_LENGTH];
    DWORD dwFlags = IsWin32WinNtVersionOrGreater(_WIN32_WINNT_WIN7) ? MUI_UI_FALLBACK : MUI_MERGE_USER_FALLBACK;

    // Thread

    PrintLocale(GetThreadLocale(), _T("GetThreadLocale"), NULL);
    PrintLanguage(GetThreadUILanguage(), _T("GetThreadUILanguage"), NULL);

    PrintThreadPreferredUILanguages(MUI_LANGUAGE_ID | dwFlags);
    PrintThreadPreferredUILanguages(MUI_LANGUAGE_NAME | dwFlags);

    _tprintf(_T("\n"));

    // User

    PrintLocale(GetUserDefaultLCID(), _T("GetUserDefaultLCID"), NULL);

    szLocaleName[0] = _T('\0');
    PrintLocaleName(GetUserDefaultLocaleName(szLocaleName, _countof(szLocaleName)), szLocaleName, _T("GetUserDefaultLocaleName"), NULL);

    PrintLanguage(GetUserDefaultLangID(), _T("GetUserDefaultLangID"), NULL);
    PrintLanguage(GetUserDefaultUILanguage(), _T("GetUserDefaultUILanguage"), NULL);

    PrintUserPreferredUILanguages(MUI_LANGUAGE_ID);
    PrintUserPreferredUILanguages(MUI_LANGUAGE_NAME);

    _tprintf(_T("\n"));

    // System

    PrintLocale(GetSystemDefaultLCID(), _T("GetSystemDefaultLCID"), NULL);

    szLocaleName[0] = _T('\0');
    PrintLocaleName(GetSystemDefaultLocaleName(szLocaleName, _countof(szLocaleName)), szLocaleName, _T("GetSystemDefaultLocaleName"), NULL);

    PrintLanguage(GetSystemDefaultLangID(), _T("GetSystemDefaultLangID"), NULL);
    PrintLanguage(GetSystemDefaultUILanguage(), _T("GetSystemDefaultUILanguage"), _T("Install Language"));

    PrintSystemPreferredUILanguages(MUI_LANGUAGE_ID);
    PrintSystemPreferredUILanguages(MUI_LANGUAGE_NAME);
}

//////////////////////////////////////////////////////////////////////////////////////////

static DWORD PrintMessage(DWORD dwLanguageId, LPCTSTR name)
{
    static const DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

    DWORD dwLastError = 0;
    LPTSTR lpszMessage = NULL;
    DWORD dwMessageId = ERROR_ACCESS_DENIED;
    DWORD dwCount = FormatMessage(dwFlags, NULL, dwMessageId, dwLanguageId, (LPTSTR)&lpszMessage, 0, NULL);

    if (lpszMessage != NULL)
    {
        if (dwCount > 0)
        {
            _tprintf(_T("FormatMessage (%s) = %d 0x%04X - %s"),
                name, dwLanguageId, dwLanguageId, lpszMessage);
        }
        else
        {
            dwLastError = GetLastError();
            _tprintf(_T("FormatMessage (%s) = %d 0x%04X [ERROR (dwCount=0) %d 0x%08X]\n"),
                name, dwLanguageId, dwLanguageId, dwLastError, dwLastError);
        }

        LocalFree(lpszMessage);
    }
    else
    {
        dwLastError = GetLastError();
        _tprintf(_T("FormatMessage (%s) = %d 0x%04X [ERROR (lpszMessage == NULL) %d 0x%08X]\n"),
            name, dwLanguageId, dwLanguageId, dwLastError, dwLastError);
    }

    return dwLastError;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PrintWin32IntlData(void)
{
    PrintData();

    _tprintf(_T("\n"));

    PrintMessage(GetThreadUILanguage(), _T("GetThreadUILanguage"));
    PrintMessage(GetUserDefaultUILanguage(), _T("GetUserDefaultUILanguage"));
    PrintMessage(GetUserDefaultLangID(), _T("GetUserDefaultLangID"));
    PrintMessage(LANG_USER_DEFAULT, _T("LANG_USER_DEFAULT"));
    PrintMessage(GetSystemDefaultUILanguage(), _T("GetSystemDefaultUILanguage"));
    PrintMessage(GetSystemDefaultLangID(), _T("GetSystemDefaultLangID"));
    PrintMessage(LANG_SYSTEM_DEFAULT, _T("LANG_SYSTEM_DEFAULT"));
}
