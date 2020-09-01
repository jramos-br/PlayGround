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
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#include "Win32FormatMessage.h"

//////////////////////////////////////////////////////////////////////////////////////////

BOOL GetLanguageName(LANGID languageId, LPTSTR pszLanguageName, UINT cchLanguageName)
{
    LCID localeId = MAKELCID(languageId, SORT_DEFAULT);

    return GetLocaleInfo(localeId, LOCALE_SLANGUAGE, pszLanguageName, cchLanguageName) > 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PrintMessage1_(DWORD dwMessageId, LPCTSTR pszFlags, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    _tprintf(_T("  %s 0x%04X [%s]\n"),
        pszFlags, dwFlags,
        FormatWin32MessageEx(dwMessageId, dwFlags, pszBuffer, cchBuffer));
}

//////////////////////////////////////////////////////////////////////////////////////////

void PrintMessage2_(DWORD dwMessageId, LPCTSTR pszLanguageId, LANGID languageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    TCHAR szLanguageName[LOCALE_NAME_MAX_LENGTH];

    if (!GetLanguageName(languageId, szLanguageName, _countof(szLanguageName)))
        _tcscpy_s(szLanguageName, _countof(szLanguageName), _T("NONAME"));

    _tprintf(_T("  %s 0x%04X %s [%s]\n"),
        pszLanguageId, languageId, szLanguageName,
        FormatWin32MessageExL(dwMessageId, languageId, dwFlags, pszBuffer, cchBuffer));
}

//////////////////////////////////////////////////////////////////////////////////////////

void PrintMessage3_(HMODULE hModule, DWORD dwMessageId, LPCTSTR pszLanguageId, LANGID languageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    TCHAR szLanguageName[LOCALE_NAME_MAX_LENGTH];

    if (!GetLanguageName(languageId, szLanguageName, _countof(szLanguageName)))
        _tcscpy_s(szLanguageName, _countof(szLanguageName), _T("NONAME"));

    _tprintf(_T("  %s 0x%04X %s [%s]\n"),
        pszLanguageId, languageId, szLanguageName,
        FormatWin32ModuleMessageExL(hModule, dwMessageId, languageId, dwFlags, pszBuffer, cchBuffer));
}

//////////////////////////////////////////////////////////////////////////////////////////

#define PrintMessage1(language, pszBuffer, cchBuffer) \
    PrintMessage1_(dwMessageId, _T(#language), language | dwFlags, pszBuffer, cchBuffer)

#define PrintMessage2(languageId, pszBuffer, cchBuffer) \
    PrintMessage2_(dwMessageId, _T(#languageId), languageId, dwFlags, pszBuffer, cchBuffer)

#define PrintMessage3(languageId, pszBuffer, cchBuffer) \
    PrintMessage3_(hModule, dwMessageId, _T(#languageId), languageId, dwFlags, pszBuffer, cchBuffer)

//////////////////////////////////////////////////////////////////////////////////////////

void Test10(LPCTSTR pszTitle, const DWORD dwMessageId, const DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    _tprintf(_T("> %s\n"), pszTitle);

    // Uses the FormatMessage function default language search order: language neutral;
    // thread LANGID, based on the thread's locale value; user default LANGID, based on
    // the user's default locale value; system default LANGID, based on the system default
    // locale value; system default UI language (also known as "install language").
    PrintMessage1(FW32M_DEFAULT_LANGUAGES, pszBuffer, cchBuffer);

    // Uses a composite list consisting of the thread preferred UI languages, process
    // preferred UI languages, user preferred UI languages, system preferred UI languages,
    // and the system default UI language, along with associated fallback and neutral
    // languages.
    PrintMessage1(FW32M_PREFERRED_UI_LANGUAGES, pszBuffer, cchBuffer);

    // Uses the language identifier of the Region Format setting for the current user as
    // set under Control Panel > Clock, Language, and Region > Change date, time, or
    // number formats > Formats tab > Format dropdown.
    PrintMessage1(FW32M_USER_DEFAULT_LANGID, pszBuffer, cchBuffer);

    // Uses the language identifier for the user UI language for the current user. If the
    // current user has not set a language, uses the preferred language set for the
    // system. If there is no preferred language set for the system, then uses the system
    // default UI language (also known as "install language").
    PrintMessage1(FW32M_USER_DEFAULT_UI_LANGUAGE, pszBuffer, cchBuffer);

    // Uses the user preferred UI languages. The user preferred UI languages list includes
    // languages that the user prefers.
    PrintMessage1(FW32M_USER_PREFERRED_UI_LANGUAGES, pszBuffer, cchBuffer);

    // Uses the language identifier for the system locale. This is the language used when
    // displaying text in programs that do not support Unicode. It is set by the
    // Administrator under Control Panel > Clock, Language, and Region > Change date,
    // time, or number formats > Administrative tab.
    PrintMessage1(FW32M_SYSTEM_DEFAULT_LANGID, pszBuffer, cchBuffer);

    // Uses the language identifier for the system default UI language of the operating
    // system, also known as the "install language" on Windows Vista and later.
    PrintMessage1(FW32M_SYSTEM_DEFAULT_UI_LANGUAGE, pszBuffer, cchBuffer);

    // Uses the system preferred UI languages. The system preferred UI languages list is
    // made up of the system default UI language and the system UI language and their
    // fallbacks.
    PrintMessage1(FW32M_SYSTEM_PREFERRED_UI_LANGUAGES, pszBuffer, cchBuffer);

    // Uses the language identifier of the first user interface language for the current
    // thread.
    PrintMessage1(FW32M_THREAD_UI_LANGUAGE, pszBuffer, cchBuffer);

    // Uses a thread preferred UI languages list that consists of up to five valid
    // languages set by a thread in a running process for a MUI application. These
    // languages are used to customize the application user interface languages and make
    // them different from the operating system language.
    PrintMessage1(FW32M_THREAD_PREFERRED_UI_LANGUAGES, pszBuffer, cchBuffer);

    // Uses a composite list consisting of the thread preferred UI languages, followed by
    // process preferred UI languages, followed by any user preferred UI languages that
    // are distinct from these, followed by the system default UI language, if it is not
    // already in the list. If the user preferred UI languages list is empty, uses the
    // system preferred UI languages.
    PrintMessage1(FW32M_THREAD_PREFERRED_UI_LANGUAGES_MERGE_USER_FALLBACK, pszBuffer, cchBuffer);

    // Uses a composite list consisting of the thread preferred UI languages, followed by
    // process preferred UI languages, followed by any user preferred UI languages that
    // are distinct from these, followed by the system default UI language, if it is not
    // already in the list, along with associated fallback and neutral languages. If the
    // user preferred UI languages list is empty, uses the system preferred UI languages.
    PrintMessage1(FW32M_THREAD_PREFERRED_UI_LANGUAGES_MERGE_SYSTEM_FALLBACK, pszBuffer, cchBuffer);

    // Uses a composite list consisting of the thread preferred UI languages, followed by
    // process preferred UI languages, followed by any user preferred UI languages that
    // are distinct from these, followed by the system default UI language, if it is not
    // already in the list, along with associated fallback and neutral languages. If the
    // user preferred UI languages list is empty, uses the system preferred UI languages.
    PrintMessage1(FW32M_THREAD_PREFERRED_UI_LANGUAGES_UI_FALLBACK, pszBuffer, cchBuffer);

    // Uses a process preferred UI languages list consisting of up to five valid languages
    // set by a running process for a MUI application. These languages are used to
    // customize the application user interface languages and make them different from the
    // operating system language.
    PrintMessage1(FW32M_PROCESS_PREFERRED_UI_LANGUAGES, pszBuffer, cchBuffer);

    _tprintf(_T("< %s\n"), pszTitle);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test11(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test10(_T("Test11"), ERROR_INVALID_FUNCTION, 0, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test12(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test10(_T("Test12"), ERROR_INVALID_FUNCTION, FW32M_FLAG_FALLBACK_TO_DEFAULT, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test13(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test10(_T("Test13"), ERROR_INVALID_FUNCTION, FW32M_FLAG_FALLBACK_TO_PREFERRED, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test20(LPCTSTR pszTitle, const DWORD dwMessageId, const DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    _tprintf(_T("> %s\n"), pszTitle);

    // Language neutral.
    PrintMessage2(MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), pszBuffer, cchBuffer);

    // User default language.
    PrintMessage2(MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), pszBuffer, cchBuffer);

    // System default language.
    PrintMessage2(MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), pszBuffer, cchBuffer);

    // The language of the Region Format setting for the current user as set under Control
    // Panel > Clock, Language, and Region > Change date, time, or number formats >
    // Formats tab > Format dropdown.
    PrintMessage2(GetUserDefaultLangID(), pszBuffer, cchBuffer);

    // The language for the user UI language for the current user. If the current user has
    // not set a language, GetUserDefaultUILanguage returns the preferred language set for
    // the system. If there is no preferred language set for the system, then the system
    // default UI language (also known as "install language") is returned.
    PrintMessage2(GetUserDefaultUILanguage(), pszBuffer, cchBuffer);

    // The language for the system locale. This is the language used when displaying text
    // in programs that do not support Unicode. It is set by the Administrator under
    // Control Panel > Clock, Language, and Region > Change date, time, or number formats
    // > Administrative tab.
    PrintMessage2(GetSystemDefaultLangID(), pszBuffer, cchBuffer);

    // The language for the system default UI language of the operating system, also known
    // as the "install language" on Windows Vista and later.
    PrintMessage2(GetSystemDefaultUILanguage(), pszBuffer, cchBuffer);

    // The language of the first user interface language for the current thread.
    // Calling GetThreadUILanguage is identical to calling GetThreadPreferredUILanguages
    // with dwFlags set to MUI_MERGE_SYSTEM_FALLBACK | MUI_MERGE_USER_FALLBACK |
    // MUI_LANGUAGE_ID and using the first language in the retrieved list.
    PrintMessage2(GetThreadUILanguage(), pszBuffer, cchBuffer);

    _tprintf(_T("< %s\n"), pszTitle);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test21(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test20(_T("Test21"), ERROR_FILE_NOT_FOUND, 0, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test22(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test20(_T("Test22"), ERROR_FILE_NOT_FOUND, FW32M_FLAG_FALLBACK_TO_DEFAULT, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test23(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test20(_T("Test23"), ERROR_FILE_NOT_FOUND, FW32M_FLAG_FALLBACK_TO_PREFERRED, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test30(LPCTSTR pszTitle, const DWORD dwMessageId, const DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer)
{
    HMODULE hModule = LoadLibrary(_T("Mswsock"));

    if (hModule != NULL)
    {
        _tprintf(_T("> %s\n"), pszTitle);

        // The language of the Region Format setting for the current user as set under Control
        // Panel > Clock, Language, and Region > Change date, time, or number formats >
        // Formats tab > Format dropdown.
        PrintMessage3(GetUserDefaultLangID(), pszBuffer, cchBuffer);

        // The language for the user UI language for the current user. If the current user has
        // not set a language, GetUserDefaultUILanguage returns the preferred language set for
        // the system. If there is no preferred language set for the system, then the system
        // default UI language (also known as "install language") is returned.
        PrintMessage3(GetUserDefaultUILanguage(), pszBuffer, cchBuffer);

        // The language of the first user interface language for the current thread.
        // Calling GetThreadUILanguage is identical to calling GetThreadPreferredUILanguages
        // with dwFlags set to MUI_MERGE_SYSTEM_FALLBACK | MUI_MERGE_USER_FALLBACK |
        // MUI_LANGUAGE_ID and using the first language in the retrieved list.
        PrintMessage3(GetThreadUILanguage(), pszBuffer, cchBuffer);

        _tprintf(_T("< %s\n"), pszTitle);

        FreeLibrary(hModule);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test31(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test30(_T("Test31"), WSAEINTR, 0, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test32(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test30(_T("Test32"), ERROR_DISK_REPAIR_UNSUCCESSFUL, 0, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Test33(LPTSTR pszBuffer, UINT cchBuffer)
{
    Test30(_T("Test33"), ERROR_DISK_REPAIR_UNSUCCESSFUL, FW32M_FLAG_MESSAGE_FROM_SYSTEM, pszBuffer, cchBuffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, TCHAR* argv[])
{
    int value = argc > 1 ? _ttoi(argv[1]) : -1;
    UINT cchBuffer = value >= 0 ? value : 80;
    LPTSTR pszBuffer = new TCHAR[cchBuffer];

    Test11(pszBuffer, cchBuffer);
    Test12(pszBuffer, cchBuffer);
    Test13(pszBuffer, cchBuffer);

    Test21(pszBuffer, cchBuffer);
    Test22(pszBuffer, cchBuffer);
    Test23(pszBuffer, cchBuffer);

    Test31(pszBuffer, cchBuffer);
    Test32(pszBuffer, cchBuffer);
    Test33(pszBuffer, cchBuffer);

    delete[] pszBuffer;
    return EXIT_SUCCESS;
}
