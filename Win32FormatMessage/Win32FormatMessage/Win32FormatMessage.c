//////////////////////////////////////////////////////////////////////////////////////////
//
// Win32 FormatMessage function wrappers.
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

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <crtdbg.h>

#include "Win32FormatMessage.h"

#define DEBUG_MODE  0

#define LANGID_MAX  ((LANGID)~0)

static const TCHAR szDefaultWin32Message[] = _T("Win32 Message ID 0x%08I32X (%I32d)");

#define IsCharLineSeparator(ch) (ch == _T('\n') || ch == _T('\r'))

#ifdef _TRUNCATE

static int sntprintf(LPTSTR buffer, UINT count, LPCTSTR format, ...)
{
    int result;
    va_list args;

    va_start(args, format);
    result = _vsntprintf_s(buffer, count, _TRUNCATE, format, args);
    va_end(args);

    return result;
}

#else

static int sntprintf(LPTSTR buffer, UINT count, LPCTSTR format, ...)
{
    int result;
    va_list args;

    va_start(args, format);
    result = _vsntprintf(buffer, count, format, args);
    va_end(args);

    if (result < 0)
        buffer[count - 1] = _T('\0');
    else if ((UINT)result >= count)
        buffer[count - 1] = _T('\0'), result = -1;

    return result;
}

#endif

static BOOL FindLanguage(LANGID language, LANGID languages[], UINT used)
{
    for (UINT i = 0; i < used; ++i)
    {
        if (languages[i] == language)
        {
            return TRUE;
        }
    }

    return FALSE;
}

static UINT AppendLanguages(LANGID languages[], UINT count, UINT used, LPWSTR pwszzLanguagesBuffer)
{
    for (LPWSTR pwszz = pwszzLanguagesBuffer; *pwszz != L'\0' && used < count; pwszz += wcslen(pwszz) + 1)
    {
        LPWSTR pwszEnd = NULL;
        ULONG value = wcstoul(pwszz, &pwszEnd, 16);

        if (pwszEnd != NULL && *pwszEnd == _T('\0') && value <= LANGID_MAX)
        {
            LANGID language = (LANGID)value;

            if (language != LOCALE_CUSTOM_UI_DEFAULT && language != LOCALE_CUSTOM_UNSPECIFIED)
            {
                if (!FindLanguage(language, languages, used))
                {
                    languages[used++] = language;
                }
            }
        }
    }

    return used;
}

typedef BOOL(WINAPI* PFN_GETUILANGUAGES)(DWORD dwFlags, PULONG pulNumLanguages, PZZWSTR pwszLanguagesBuffer, PULONG pcchLanguagesBuffer);

static DWORD GetUILanguagesAux(PFN_GETUILANGUAGES pfnGetUILanguages, DWORD dwFlags, LANGID languages[], UINT count, PUINT pUsed, ULONG cchLanguagesBuffer)
{
    DWORD dwLastError = ERROR_SUCCESS;
    LPWSTR pwszzLanguagesBuffer = (LPWSTR)_malloca(cchLanguagesBuffer * sizeof(WCHAR));

    if (pwszzLanguagesBuffer != NULL)
    {
        ULONG ulNumLanguages = 0;

        if ((*pfnGetUILanguages)(dwFlags, &ulNumLanguages, pwszzLanguagesBuffer, &cchLanguagesBuffer))
        {
            *pUsed = AppendLanguages(languages, count, *pUsed, pwszzLanguagesBuffer);
        }
        else
        {
            dwLastError = GetLastError();
        }

        _freea(pwszzLanguagesBuffer);
    }
    else
    {
        dwLastError = ERROR_NOT_ENOUGH_MEMORY;
    }

    return dwLastError;
}

static UINT GetUILanguages(PFN_GETUILANGUAGES pfnGetUILanguages, DWORD dwFlags, LANGID languages[], UINT count, UINT used)
{
    DWORD dwLastError;
    ULONG ulNumLanguages = 0;
    WCHAR wszzLanguagesBuffer[128];
    ULONG cchLanguagesBuffer = _countof(wszzLanguagesBuffer);

    dwFlags |= MUI_LANGUAGE_ID;

    if ((*pfnGetUILanguages)(dwFlags, &ulNumLanguages, wszzLanguagesBuffer, &cchLanguagesBuffer))
    {
        return AppendLanguages(languages, count, used, wszzLanguagesBuffer);
    }

    dwLastError = GetLastError();

    if (dwLastError == ERROR_INSUFFICIENT_BUFFER)
    {
        // Returning correct cchLanguagesBuffer when input is not zero is undocumented.

        if (cchLanguagesBuffer > _countof(wszzLanguagesBuffer) && cchLanguagesBuffer < 1024)
        {
            dwLastError = GetUILanguagesAux(pfnGetUILanguages, dwFlags, languages, count, &used, cchLanguagesBuffer);
        }
    }

    if (dwLastError == ERROR_INSUFFICIENT_BUFFER)
    {
        ulNumLanguages = 0;
        cchLanguagesBuffer = 0;

        if ((*pfnGetUILanguages)(dwFlags, &ulNumLanguages, NULL, &cchLanguagesBuffer))
        {
            dwLastError = GetUILanguagesAux(pfnGetUILanguages, dwFlags, languages, count, &used, cchLanguagesBuffer);
        }
    }

    return used;
}

static BOOL InternalFormatWin32Message(HMODULE hModule, DWORD dwMessageId, LANGID languages[], UINT count, UINT used, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    BOOL bResult;
    DWORD dwFormatFlags;

    if ((dwFlags & FW32M_FLAG_FALLBACK_TO_PREFERRED) != 0)
    {
        used = GetUILanguages(GetThreadPreferredUILanguages, MUI_UI_FALLBACK, languages, count, used);
    }

    if ((dwFlags & FW32M_FLAG_FALLBACK_TO_DEFAULT) != 0)
    {
        if (!FindLanguage(0, languages, used))
        {
            languages[used++] = 0;
        }
    }

#if DEBUG_MODE

    for (UINT i = 0; i < used; ++i)
    {
        TCHAR szLanguageName[LOCALE_NAME_MAX_LENGTH];
        LCID localeId = MAKELCID(languages[i], SORT_DEFAULT);

        if (GetLocaleInfo(localeId, LOCALE_SLANGUAGE, szLanguageName, _countof(szLanguageName)) > 0)
            _tprintf(_T("  - 0x%04X %s\n"), languages[i], szLanguageName);
        else
            _tprintf(_T("  - 0x%04X\n"), languages[i]);
    }

#endif

    dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS;

    if (hModule == NULL)
    {
        dwFormatFlags |= FORMAT_MESSAGE_FROM_SYSTEM;
    }
    else
    {
        if ((dwFlags & FW32M_FLAG_MESSAGE_FROM_SYSTEM) != 0)
        {
            dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM;
        }
        else
        {
            dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
        }
    }

    bResult = FALSE;

    for (UINT i = 0; i < used && !bResult; ++i)
    {
        LPTSTR pszMessage = NULL;
        DWORD dwCount = FormatMessage(dwFormatFlags, hModule, dwMessageId, languages[i], (LPTSTR)&pszMessage, 0, NULL);

        if (pszMessage != NULL)
        {
            while (dwCount > 0 && IsCharLineSeparator(pszMessage[dwCount - 1]))
            {
                pszMessage[--dwCount] = _T('\0');
            }

            if (dwCount > 0)
            {
                if ((dwFlags & FW32M_FLAG_FORMAT_ID) != 0)
                {
                    sntprintf(pszBuffer, cchBuffer, _T("%s (0x%08I32X, %I32d)"), pszMessage, dwMessageId, dwMessageId);
                }
                else
                {
                    sntprintf(pszBuffer, cchBuffer, _T("%s"), pszMessage);
                }

                bResult = TRUE;
            }

            LocalFree(pszMessage);
        }

#if DEBUG_MODE

        if (bResult)
            _tprintf(_T("    - OK 0x%04X\n"), languages[i]);
        else
            _tprintf(_T("    - FAIL 0x%04X\n"), languages[i]);

#endif

    }

    return bResult;
}

static BOOL InternalFormatWin32Message2(HMODULE hModule, DWORD dwMessageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    LANGID languages[16];
    UINT used = 0;

    switch (dwFlags & FW32M_LANGUAGE_MASK)
    {
    case FW32M_DEFAULT_LANGUAGES:
        languages[used++] = 0;
        break;
    case FW32M_PREFERRED_UI_LANGUAGES:
        used = GetUILanguages(GetThreadPreferredUILanguages, MUI_UI_FALLBACK, languages, _countof(languages), used);
        break;
    case FW32M_USER_DEFAULT_LANGID:
        languages[used++] = GetUserDefaultLangID();
        break;
    case FW32M_USER_DEFAULT_UI_LANGUAGE:
        languages[used++] = GetUserDefaultUILanguage();
        break;
    case FW32M_USER_PREFERRED_UI_LANGUAGES:
        used = GetUILanguages(GetUserPreferredUILanguages, 0, languages, _countof(languages), used);
        break;
    case FW32M_SYSTEM_DEFAULT_LANGID:
        languages[used++] = GetSystemDefaultLangID();
        break;
    case FW32M_SYSTEM_DEFAULT_UI_LANGUAGE:
        languages[used++] = GetSystemDefaultUILanguage();
        break;
    case FW32M_SYSTEM_PREFERRED_UI_LANGUAGES:
        used = GetUILanguages(GetSystemPreferredUILanguages, 0, languages, _countof(languages), used);
        break;
    case FW32M_THREAD_UI_LANGUAGE:
        languages[used++] = GetThreadUILanguage();
        break;
    case FW32M_THREAD_PREFERRED_UI_LANGUAGES:
        used = GetUILanguages(GetThreadPreferredUILanguages, MUI_THREAD_LANGUAGES, languages, _countof(languages), used);
        break;
    case FW32M_THREAD_PREFERRED_UI_LANGUAGES_MERGE_USER_FALLBACK:
        used = GetUILanguages(GetThreadPreferredUILanguages, MUI_MERGE_USER_FALLBACK, languages, _countof(languages), used);
        break;
    case FW32M_THREAD_PREFERRED_UI_LANGUAGES_MERGE_SYSTEM_FALLBACK:
        used = GetUILanguages(GetThreadPreferredUILanguages, MUI_MERGE_SYSTEM_FALLBACK, languages, _countof(languages), used);
        break;
    case FW32M_THREAD_PREFERRED_UI_LANGUAGES_UI_FALLBACK:
        used = GetUILanguages(GetThreadPreferredUILanguages, MUI_UI_FALLBACK, languages, _countof(languages), used);
        break;
    case FW32M_PROCESS_PREFERRED_UI_LANGUAGES:
        used = GetUILanguages(GetProcessPreferredUILanguages, 0, languages, _countof(languages), used);
        break;
    default:
        languages[used++] = 0;
        break;
    }

    return InternalFormatWin32Message(hModule, dwMessageId, languages, _countof(languages), used, dwFlags, pszBuffer, cchBuffer);
}

static BOOL InternalFormatWin32Message3(HMODULE hModule, DWORD dwMessageId, LANGID languageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    LANGID languages[16];
    UINT used = 0;

    languages[used++] = languageId;

    return InternalFormatWin32Message(hModule, dwMessageId, languages, _countof(languages), used, dwFlags, pszBuffer, cchBuffer);
}

LPTSTR FormatWin32Message(DWORD dwMessageId, LPTSTR pszBuffer, UINT cchBuffer)
{
    if (pszBuffer != NULL && cchBuffer > 0)
    {
        if (!InternalFormatWin32Message2(NULL, dwMessageId, FW32M_DEFAULT, pszBuffer, cchBuffer))
        {
            sntprintf(pszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);
        }
    }
    else
    {
        _ASSERTE(pszBuffer != NULL);
        _ASSERTE(cchBuffer > 0);
    }

    return pszBuffer;
}

LPTSTR FormatWin32MessageEx(DWORD dwMessageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    if (pszBuffer != NULL && cchBuffer > 0)
    {
        if (!InternalFormatWin32Message2(NULL, dwMessageId, dwFlags, pszBuffer, cchBuffer))
        {
            sntprintf(pszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);

            if ((dwFlags & FW32M_FLAG_NULL_ON_ERROR) != 0)
            {
                pszBuffer = NULL;
            }
        }
    }
    else
    {
        _ASSERTE(pszBuffer != NULL);
        _ASSERTE(cchBuffer > 0);
    }

    return pszBuffer;
}

LPTSTR FormatWin32MessageExL(DWORD dwMessageId, LANGID languageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    if (pszBuffer != NULL && cchBuffer > 0)
    {
        if (!InternalFormatWin32Message3(NULL, dwMessageId, languageId, dwFlags, pszBuffer, cchBuffer))
        {
            sntprintf(pszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);

            if ((dwFlags & FW32M_FLAG_NULL_ON_ERROR) != 0)
            {
                pszBuffer = NULL;
            }
        }
    }
    else
    {
        _ASSERTE(pszBuffer != NULL);
        _ASSERTE(cchBuffer > 0);
    }

    return pszBuffer;
}

LPTSTR FormatWin32ModuleMessage(HMODULE hModule, DWORD dwMessageId, LPTSTR pszBuffer, UINT cchBuffer)
{
    if (pszBuffer != NULL && cchBuffer > 0)
    {
        if (!InternalFormatWin32Message2(hModule, dwMessageId, FW32M_DEFAULT, pszBuffer, cchBuffer))
        {
            sntprintf(pszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);
        }
    }
    else
    {
        _ASSERTE(pszBuffer != NULL);
        _ASSERTE(cchBuffer > 0);
    }

    return pszBuffer;
}

LPTSTR FormatWin32ModuleMessageEx(HMODULE hModule, DWORD dwMessageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    if (pszBuffer != NULL && cchBuffer > 0)
    {
        if (!InternalFormatWin32Message2(hModule, dwMessageId, dwFlags, pszBuffer, cchBuffer))
        {
            sntprintf(pszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);

            if ((dwFlags & FW32M_FLAG_NULL_ON_ERROR) != 0)
            {
                pszBuffer = NULL;
            }
        }
    }
    else
    {
        _ASSERTE(pszBuffer != NULL);
        _ASSERTE(cchBuffer > 0);
    }

    return pszBuffer;
}

LPTSTR FormatWin32ModuleMessageExL(HMODULE hModule, DWORD dwMessageId, LANGID languageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    if (pszBuffer != NULL && cchBuffer > 0)
    {
        if (!InternalFormatWin32Message3(hModule, dwMessageId, languageId, dwFlags, pszBuffer, cchBuffer))
        {
            sntprintf(pszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);

            if ((dwFlags & FW32M_FLAG_NULL_ON_ERROR) != 0)
            {
                pszBuffer = NULL;
            }
        }
    }
    else
    {
        _ASSERTE(pszBuffer != NULL);
        _ASSERTE(cchBuffer > 0);
    }

    return pszBuffer;
}
