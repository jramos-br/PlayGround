//////////////////////////////////////////////////////////////////////////////////////////
//
// Internationalization for Windows Applications.
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
#define _WIN32_WINNT    0x0400
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

#include "Win32IntlUtil.h"

//////////////////////////////////////////////////////////////////////////////////////////





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

#if _WIN32_WINNT < _WIN32_WINNT_VISTA

typedef LANGID(WINAPI* PFNGETUSERDEFAULTLOCALENAME)(LPWSTR lpLocaleName, int cchLocaleName);

static LANGID WINAPI GetUserDefaultLocaleNameAux(LPWSTR lpLocaleName, int cchLocaleName)
{
    int WINAPI GetLocaleInfoW(LOCALE_USER_DEFAULT, LCTYPE LCType, LPTSTR lpLCData, int cchData);
}

static LANGID WINAPI GetUserDefaultLocaleName(LPWSTR lpLocaleName, int cchLocaleName)
{
    static PFNGETUSERDEFAULTLOCALENAME pfnGetUserDefaultLocaleName;

    if (pfnGetUserDefaultLocaleName == NULL)
    {
        HMODULE hKernel32 = GetKernel32ModuleHandle();

        if (hKernel32 == NULL)
            return 0;

        pfnGetUserDefaultLocaleName = (PFNGETUSERDEFAULTLOCALENAME)GetProcAddress(hKernel32, "GetUserDefaultLocaleName");

        if (pfnGetUserDefaultLocaleName == NULL)
            return 0;
    }

    return pfnGetUserDefaultLocaleName(lpLocaleName, cchLocaleName);
}

#endif































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

BOOL GetLocaleName(LCID lcid, LPTSTR pszLocaleName, size_t cchLocaleName)
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

BOOL GetLanguageName(LANGID langid, LPTSTR pszLanguageName, size_t cchLanguageName)
{
    LCID lcid = MAKELCID(langid, SORT_DEFAULT);

    return GetLocaleInfo(lcid, LOCALE_SLANGUAGE, pszLanguageName, cchLanguageName) > 0;
}
