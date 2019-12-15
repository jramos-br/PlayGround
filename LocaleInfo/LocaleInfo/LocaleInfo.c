////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1600
#define WINVER          0x0600
#define _WIN32_WINNT    0x0600
#define _WIN32_WINDOWS  0x0600
#define _WIN32_IE       0x0700
#else
#define WINVER          0x0400
#define _WIN32_WINNT    0x0400
#define _WIN32_WINDOWS  0x0400
#define _WIN32_IE       0x0600
#endif

#if _MSC_VER >= 1600
#include <SDKDDKVer.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif

#if 0
#pragma warning (disable: 4127)     /* conditional expression is constant */
#pragma warning (disable: 4201)     /* nonstandard extension used : nameless struct/union */
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4115)     /* named type definition in parentheses */
#pragma warning (disable: 4505)     /* unreferenced local function has been removed */
#pragma warning (disable: 4514)     /* unreferenced inline function has been removed */
#pragma warning (disable: 4702)     /* unreachable code */
#endif

#ifndef STRICT
#define STRICT                      /* Enable STRICT type checking. */
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning (push)
#include <windows.h>
#include <windowsx.h>
#pragma warning (pop)
#else
#include <windows.h>
#include <windowsx.h>
#endif

#ifndef _INC_SDKDDKVER

#define _WIN32_WINDOWS_WIN95                0x0400
#define _WIN32_WINDOWS_WIN98                0x0410
#define _WIN32_WINDOWS_WINME                0x0490

#define _WIN32_WINNT_NT31                   0x030A
#define _WIN32_WINNT_NT4                    0x0400
#define _WIN32_WINNT_WIN2K                  0x0500
#define _WIN32_WINNT_WINXP                  0x0501
#define _WIN32_WINNT_WS03                   0x0502
#define _WIN32_WINNT_WIN6                   0x0600
#define _WIN32_WINNT_VISTA                  0x0600
#define _WIN32_WINNT_WS08                   0x0600
#define _WIN32_WINNT_LONGHORN               0x0600
#define _WIN32_WINNT_WIN7                   0x0601
#define _WIN32_WINNT_WIN8                   0x0602
#define _WIN32_WINNT_WINBLUE                0x0603  /* 8.1 */

#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <limits.h>
#include <locale.h>
#include <ctype.h>
#include <mbctype.h>
#include <tchar.h>
#include <tchar.h>
#include <crtdbg.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER < 1600
#ifdef _WIN64
typedef __int64 LONG_PTR;
#else
typedef long LONG_PTR;
#endif
#endif

#ifndef ERROR_MUI_FILE_NOT_FOUND
#define ERROR_MUI_FILE_NOT_FOUND    15100L
#endif

#ifndef ERROR_MUI_FILE_NOT_LOADED
#define ERROR_MUI_FILE_NOT_LOADED   15105L
#endif

#ifndef STATUS_INVALID_PARAMETER
#define STATUS_INVALID_PARAMETER    ((DWORD) 0xC000000DL)
#endif

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES     ((DWORD) -1)
#endif

#ifdef DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#ifndef _countof
#ifndef __cplusplus
#define _countof(a) (sizeof (a) / sizeof (a [0]))
#else
template <typename T, size_t N> char(&_CountOfHelper(const T(&a)[N]))[N];
#define _countof(a) sizeof (_CountOfHelper (a))
#endif
#endif

#ifndef IS_SURROGATE_PAIR

//
//  Special Unicode code point values, for use with UTF-16 surrogate pairs.
//
#define HIGH_SURROGATE_START  0xd800
#define HIGH_SURROGATE_END    0xdbff
#define LOW_SURROGATE_START   0xdc00
#define LOW_SURROGATE_END     0xdfff

//
//  Macros to determine whether a character is a high or low surrogate,
//  and whether two code points make up a surrogate pair (a high surrogate
//  and a low surrogate).
//
#define IS_HIGH_SURROGATE(wch) (((wch) >= HIGH_SURROGATE_START) && ((wch) <= HIGH_SURROGATE_END))
#define IS_LOW_SURROGATE(wch)  (((wch) >= LOW_SURROGATE_START) && ((wch) <= LOW_SURROGATE_END))
#define IS_SURROGATE_PAIR(hs, ls) (IS_HIGH_SURROGATE(hs) && IS_LOW_SURROGATE(ls))

#endif

#ifndef LOCALE_NAME_MAX_LENGTH
// 8 characters for language
// 8 characters for region
// 64 characters for suffix (script)
// 2 characters for '-' separators
// 2 characters for prefix like "i-" or "x-"
// 1 null termination
#define LOCALE_NAME_MAX_LENGTH   85
#endif

#ifndef _TRUNCATE
#define _TRUNCATE   ((size_t) -1)

int _sntprintf_s(LPTSTR pszBuffer, size_t pchBuffer, size_t pchMaxCount, LPCTSTR pszFormat, ...)
{
    int nChars;
    va_list pArgList;
    va_start(pArgList, pszFormat);
    UNREFERENCED_PARAMETER(pchMaxCount);
    nChars = _vsntprintf(pszBuffer, pchBuffer, pszFormat, pArgList);
    if (nChars == -1 || (size_t)nChars >= pchBuffer)
    {
        if (pchBuffer > 0)
        {
            pszBuffer[pchBuffer - 1] = _T('\0');
        }
    }
    va_end(pArgList);
    return nChars;
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL IsWindowsNT(void)
{
#if WINVER < _WIN32_WINNT_WINXP
    OSVERSIONINFO osvi = { sizeof(OSVERSIONINFO) };
    return GetVersionEx(&osvi) && osvi.dwPlatformId == VER_PLATFORM_WIN32_NT;
#else
    return TRUE;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned __int32 xchar_t;
typedef unsigned __int32 xint_t;

static const int UTF16_LEAD = 0;
static const int UTF16_TRAIL = 1;

static const unsigned int LEAD_OFFSET = 0xD800 - (0x10000 >> 10);
static const unsigned int SURROGATE_OFFSET = 0xDC00 + (0xD800 << 10) - 0x10000;

static __inline wchar_t* UTF16_CharNext(wchar_t* pwszText)
{
    wchar_t wchLead = pwszText[UTF16_LEAD];
    wchar_t wchTrail = pwszText[UTF16_TRAIL];

    return IS_SURROGATE_PAIR(wchLead, wchTrail) ? pwszText + 2 : pwszText + 1;
}

static __inline wchar_t* UTF16_CharPrev(wchar_t* pwszText)
{
    wchar_t wchLead = pwszText[-2];
    wchar_t wchTrail = pwszText[-1];

    return IS_SURROGATE_PAIR(wchLead, wchTrail) ? pwszText - 2 : pwszText - 1;
}

static __inline xchar_t UTF16_GetCurrentChar(wchar_t* pwszText)
{
    wchar_t wchLead = pwszText[UTF16_LEAD];
    wchar_t wchTrail = pwszText[UTF16_TRAIL];

    return IS_SURROGATE_PAIR(wchLead, wchTrail) ? ((xchar_t)wchLead << 10) - SURROGATE_OFFSET + wchTrail : wchLead;
}

static __inline xchar_t UTF16_GetNextChar(wchar_t** ppwszText)
{
    wchar_t wchLead = **ppwszText;

    if (IS_HIGH_SURROGATE(wchLead))
    {
        wchar_t wchTrail = *++(*ppwszText);

        if (IS_LOW_SURROGATE(wchTrail))
        {
            (*ppwszText)++; return ((xchar_t)wchLead << 10) - SURROGATE_OFFSET + wchTrail;
        }
    }
    else
    {
        if (wchLead != 0)
        {
            (*ppwszText)++;
        }
    }

    return wchLead;
}

static __inline xchar_t UTF16_GetPrevChar(wchar_t* pwszStart, wchar_t** ppwszText)
{
    wchar_t wchTrail = *--(*ppwszText);

    if (IS_LOW_SURROGATE(wchTrail) && *ppwszText > pwszStart)
    {
        wchar_t wchLead = *((*ppwszText) - 1);

        if (IS_HIGH_SURROGATE(wchLead))
        {
            (*ppwszText)--; return ((xchar_t)wchLead << 10) - SURROGATE_OFFSET + wchTrail;
        }
    }

    return wchTrail;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CharNext uses the system default code-page,

LPTSTR RemoveControlChars(LPTSTR pszText)
{
    LPTCH pch;
    LPTCH pchSpace;
    _TINT ch;

    for (pch = pszText, pchSpace = NULL; (ch = (_TINT)_tcsnextc(pch)) != _T('\0'); pch = _tcsinc(pch))
    {
        if (ch == _T(' '))
        {
            if (pchSpace == NULL)
            {
                pchSpace = pch;
            }
        }
        else if (ch == _T('\r') || ch == _T('\n'))
        {
            *pch = _T(' ');

            if (pchSpace == NULL)
            {
                pchSpace = pch;
            }
        }
        else
        {
            pchSpace = NULL;
        }
    }

    if (pchSpace != NULL)
    {
        *pchSpace = _T('\0');
    }

    return pszText;
}

LPTSTR RemoveFinalDot(LPTSTR pszText)
{
    LPTCH pch;
    LPTCH pchDot;
    _TINT ch;

    for (pch = pszText, pchDot = NULL; (ch = (_TINT)_tcsnextc(pch)) != _T('\0'); pch = _tcsinc(pch))
    {
        if (ch == _T('.'))
        {
            if (pchDot == NULL)
            {
                pchDot = pch;
            }
        }
        else
        {
            pchDot = NULL;
        }
    }

    if (pchDot != NULL)
    {
        *pchDot = _T('\0');
    }

    return pszText;
}

// Language neutral
// Thread LANGID, based on the thread's locale value
// User default LANGID, based on the user's default locale value
// System default LANGID, based on the system default locale value
// US English

BOOL MyFormatMessageEx(HMODULE hModule, DWORD dwMessageId, LANGID wLanguageId, BOOL bLanguageFallback, LPTSTR pszBuffer, size_t cchBuffer, LPCTSTR pArgs[])
{
    BOOL bStatus = FALSE;
    DWORD dwLastError;
    DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER;
    LPTSTR pszMessage;
    DWORD dwCount;

    dwFlags |= (hModule == NULL) ? FORMAT_MESSAGE_FROM_SYSTEM : FORMAT_MESSAGE_FROM_HMODULE;
    dwFlags |= (pArgs == NULL) ? FORMAT_MESSAGE_IGNORE_INSERTS : FORMAT_MESSAGE_ARGUMENT_ARRAY;

    pszMessage = NULL;
    dwCount = FormatMessage(dwFlags, hModule, dwMessageId, wLanguageId, (LPTSTR)&pszMessage, 0, (va_list*)pArgs);
    dwLastError = GetLastError();

    if (dwCount == 0 && wLanguageId != 0 && bLanguageFallback)
    {
        LocalFree(pszMessage);
        pszMessage = NULL;
        dwCount = FormatMessage(dwFlags, hModule, dwMessageId, 0, (LPTSTR)&pszMessage, 0, (va_list*)pArgs);
    }

    if (dwCount > 0 && pszMessage != NULL)
    {
        _sntprintf_s(pszBuffer, cchBuffer, _TRUNCATE, _T("%s"), RemoveControlChars(pszMessage));
        dwLastError = ERROR_SUCCESS;
        bStatus = TRUE;
    }

    LocalFree(pszMessage);
    SetLastError(dwLastError);
    return bStatus;
}

BOOL MyFormatMessage(HMODULE hModule, DWORD dwMessageId, LANGID wLanguageId, LPTSTR pszBuffer, size_t cchBuffer, LPCTSTR pArgs[])
{
    return MyFormatMessageEx(hModule, dwMessageId, wLanguageId, TRUE, pszBuffer, cchBuffer, pArgs);
}

LPTSTR FormatErrorMessageIdNotFound(HMODULE hModule, DWORD dwMessageId, LANGID wLanguageId, LPTSTR pszBuffer, size_t cchBuffer)
{
    TCHAR szMessageId[64];
    TCHAR szModulePathName[MAX_PATH];
    LPCTSTR pszModuleFileName;
    LPCTSTR pArgs[2];

    _sntprintf_s(szMessageId, _countof(szMessageId), _TRUNCATE, _T("%08X (%d)"), dwMessageId, dwMessageId);

    if (GetModuleFileName(hModule, szModulePathName, _countof(szModulePathName)) > 0)
    {
        szModulePathName[_countof(szModulePathName) - 1] = _T('\0');

        if ((pszModuleFileName = _tcsrchr(szModulePathName, _T('\\'))) == NULL)
            if ((pszModuleFileName = _tcsrchr(szModulePathName, _T('/'))) == NULL)
                pszModuleFileName = szModulePathName;
            else
                pszModuleFileName = _tcsinc(pszModuleFileName);
        else
            pszModuleFileName = _tcsinc(pszModuleFileName);
    }
    else
    {
        DWORD dwLastError2 = GetLastError();
        _sntprintf_s(szModulePathName, _countof(szModulePathName), _TRUNCATE, _T("[GetModuleFileName failed: 0x%08X (%d)]"), dwLastError2, dwLastError2);
        pszModuleFileName = szModulePathName;
    }

    // The system cannot find message text for message number 0x%1 in the message file for %2.

    pArgs[0] = szMessageId;
    pArgs[1] = pszModuleFileName;

    if (!MyFormatMessageEx(NULL, ERROR_MR_MID_NOT_FOUND, wLanguageId, TRUE, pszBuffer, cchBuffer, pArgs))
    {
        _sntprintf_s(pszBuffer, cchBuffer, _TRUNCATE, _T("The system cannot find message text for message number 0x%s in the message file for %s."), szMessageId, pszModuleFileName);
    }

    return pszBuffer;
}

LPTSTR FormatWin32MessageEx(HMODULE hModule, DWORD dwMessageId, LANGID wLanguageId, LPTSTR pszBuffer, size_t cchBuffer, LPCTSTR pArgs[])
{
    TCHAR szNotFoundMessage[256];
    TCHAR szLastErrorMessage[256];

    if (!MyFormatMessageEx(hModule, dwMessageId, wLanguageId, TRUE, pszBuffer, cchBuffer, pArgs))
    {
        DWORD dwLastError = GetLastError();

        if (dwLastError == ERROR_MR_MID_NOT_FOUND)
        {
            return FormatErrorMessageIdNotFound(hModule, dwMessageId, wLanguageId, pszBuffer, cchBuffer);
        }

        FormatErrorMessageIdNotFound(hModule, dwMessageId, wLanguageId, szNotFoundMessage, _countof(szNotFoundMessage));

        if (MyFormatMessageEx(NULL, dwLastError, wLanguageId, TRUE, szLastErrorMessage, _countof(szLastErrorMessage), NULL))
        {
            _sntprintf_s(pszBuffer, cchBuffer, _TRUNCATE, _T("%s: %s"), RemoveFinalDot(szNotFoundMessage), szLastErrorMessage);
        }
        else
        {
            _sntprintf_s(pszBuffer, cchBuffer, _TRUNCATE, _T("%s: Error code 0x%08X (%d)."), RemoveFinalDot(szNotFoundMessage), dwLastError, dwLastError);
        }
    }

    return pszBuffer;
}

LPTSTR FormatWin32Message(DWORD dwMessageId, LPTSTR pszBuffer, size_t cchBuffer)
{
    return FormatWin32MessageEx(NULL, dwMessageId, 0, pszBuffer, cchBuffer, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LPTSTR FormatWin32MessageTst(HMODULE hModule, DWORD dwMessageId, LANGID wLanguageId, LPTSTR pszBuffer, size_t cchBuffer, LPCTSTR pArgs[])
{
    TCHAR szNotFoundMessage[256];
    TCHAR szLastErrorMessage[256];

    if (!MyFormatMessageEx(hModule, dwMessageId, wLanguageId, FALSE, pszBuffer, cchBuffer, pArgs))
    {
        DWORD dwLastError = GetLastError();

        if (dwLastError == ERROR_MR_MID_NOT_FOUND)
        {
            FormatErrorMessageIdNotFound(hModule, dwMessageId, wLanguageId, szNotFoundMessage, _countof(szNotFoundMessage));
            _sntprintf_s(pszBuffer, cchBuffer, _TRUNCATE, _T("%s (0x%08X, %d)."), RemoveFinalDot(szNotFoundMessage), dwLastError, dwLastError);
            return pszBuffer;
        }

        FormatErrorMessageIdNotFound(hModule, dwMessageId, wLanguageId, szNotFoundMessage, _countof(szNotFoundMessage));

        if (MyFormatMessageEx(NULL, dwLastError, wLanguageId, TRUE, szLastErrorMessage, _countof(szLastErrorMessage), NULL))
        {
            _sntprintf_s(pszBuffer, cchBuffer, _TRUNCATE, _T("%s: %s (0x%08X, %d)."), RemoveFinalDot(szNotFoundMessage), RemoveFinalDot(szLastErrorMessage), dwLastError, dwLastError);
        }
        else
        {
            _sntprintf_s(pszBuffer, cchBuffer, _TRUNCATE, _T("%s (0x%08X, %d)."), RemoveFinalDot(szNotFoundMessage), dwLastError, dwLastError);
        }
    }

    return pszBuffer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintErrorMessage(LPCTSTR pszFormat, ...)
{
    va_list pArgList;
    va_start(pArgList, pszFormat);
    _vftprintf(stdout, pszFormat, pArgList);
    va_end(pArgList);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintWin32Message(DWORD dwMessageId, LPCTSTR pszSource)
{
    TCHAR szMessage[2048];
    FormatWin32Message(dwMessageId, szMessage, _countof(szMessage));

    if (pszSource == NULL || *pszSource == _T('\n'))
        PrintErrorMessage(_T("%s\n"), szMessage);
    else
        PrintErrorMessage(_T("%s: %s\n"), pszSource, szMessage);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINT GetAnsiCodePageForLocale(LCID lcid)
{
    UINT acp;

    int sizeInChars = sizeof(acp) / sizeof(TCHAR);

    if (GetLocaleInfo(lcid, LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER, (LPTSTR)(&acp), sizeInChars) != sizeInChars)
    {
        PrintWin32Message(GetLastError(), _T("GetLocaleInfo"));
        acp = 0;
    }

    return acp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Sets a new multibyte code page.
//
// The codepage argument can be set to any of the following values:
//
// _MB_CP_ANSI      Use ANSI code page obtained from operating system at program startup.
// _MB_CP_LOCALE    Use the current locale's code page obtained from a previous call to setlocale.
// _MB_CP_OEM       Use OEM code page obtained from operating system at program startup.
// _MB_CP_SBCS      Use single-byte code page. When the code page is set to _MB_CP_SBCS, a routine such as _ismbblead always returns
//                  false.
//
// Any other valid code page value, regardless of whether the value is an ANSI, OEM, or other operating-system-supported code page
// (except UTF-7 and UTF-8, which are not supported).

int setmbcp(int codepage)
{
    int retval = _setmbcp(codepage);
    if (retval < 0)
        _tperror(_T("_setmbcp failed"));
    return retval;
}

// Returns the current multibyte code page. A return value of 0 indicates that a single byte code page is in use.

int getmbcp(void)
{
    return _getmbcp();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0

// What is the correct way of temporarily changing a thread's preferred UI language?
// http://blogs.msdn.com/b/oldnewthing/archive/2010/12/23/10108275.aspx

void x(void)
{
    // error checking elided for simplicity

    // Get the current thread preferred UI languages
    ULONG cLanguages;
    PZZWSTR pszzPrevLanguages;
    ULONG cchPrevLanguages = 0;
    GetThreadPreferredUILanguages(MUI_LANGUAGE_NAME | MUI_THREAD_LANGUAGES, &cLanguages, NULL, &cchPrevLanguages);
    pszzPrevLanguages = (PZZWSTR)calloc(cchPrevLanguages, sizeof(WCHAR));
    GetThreadPreferredUILanguages(MUI_LANGUAGE_NAME | MUI_THREAD_LANGUAGES, &cLanguages, pszzPrevLanguages, &cchPrevLanguages);

    //... change the thread preferred UI languages ...
    //... load resources ...

    // Restore the original thread preferred UI languages
    SetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, pszzPrevLanguages, &cLanguages);
    free(pszzPrevLanguages);
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const TCHAR* pszLocaleInfoArray[] =
{
#if WINVER >= _WIN32_WINNT_NT4
    (const TCHAR*) LOCALE_FONTSIGNATURE, _T("LOCALE_FONTSIGNATURE"),
#endif
    (const TCHAR*)LOCALE_ICENTURY, _T("LOCALE_ICENTURY"),
    (const TCHAR*)LOCALE_ICOUNTRY, _T("LOCALE_ICOUNTRY"),
    (const TCHAR*)LOCALE_IDATE, _T("LOCALE_IDATE"),
    (const TCHAR*)LOCALE_IDAYLZERO, _T("LOCALE_IDAYLZERO"),
    (const TCHAR*)LOCALE_IDEFAULTANSICODEPAGE, _T("LOCALE_IDEFAULTANSICODEPAGE"),
    (const TCHAR*)LOCALE_IDEFAULTCODEPAGE, _T("LOCALE_IDEFAULTCODEPAGE"),
    (const TCHAR*)LOCALE_IDEFAULTCOUNTRY, _T("LOCALE_IDEFAULTCOUNTRY"),
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_IDEFAULTEBCDICCODEPAGE, _T("LOCALE_IDEFAULTEBCDICCODEPAGE"),
#endif
    (const TCHAR*)LOCALE_IDEFAULTLANGUAGE, _T("LOCALE_IDEFAULTLANGUAGE"),
    (const TCHAR*)LOCALE_IDEFAULTMACCODEPAGE, _T("LOCALE_IDEFAULTMACCODEPAGE"),
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_IGEOID, _T("LOCALE_IGEOID"),
#endif
    (const TCHAR*)LOCALE_IINTLCURRDIGITS, _T("LOCALE_IINTLCURRDIGITS"),
    (const TCHAR*)LOCALE_ILANGUAGE, _T("LOCALE_ILANGUAGE"),
    (const TCHAR*)LOCALE_ILDATE, _T("LOCALE_ILDATE"),
    (const TCHAR*)LOCALE_IMONLZERO, _T("LOCALE_IMONLZERO"),
#if WINVER >= _WIN32_WINNT_WIN7
    (const TCHAR*) LOCALE_INEGATIVEPERCENT, _T("LOCALE_INEGATIVEPERCENT"),
#endif
    (const TCHAR*)LOCALE_INEGSEPBYSPACE, _T("LOCALE_INEGSEPBYSPACE"),
    (const TCHAR*)LOCALE_INEGSIGNPOSN, _T("LOCALE_INEGSIGNPOSN"),
    (const TCHAR*)LOCALE_INEGSYMPRECEDES, _T("LOCALE_INEGSYMPRECEDES"),
    (const TCHAR*)LOCALE_IOPTIONALCALENDAR, _T("LOCALE_IOPTIONALCALENDAR"),
#if WINVER >= _WIN32_WINNT_WIN7
    (const TCHAR*) LOCALE_IPOSITIVEPERCENT, _T("LOCALE_IPOSITIVEPERCENT"),
#endif
    (const TCHAR*)LOCALE_IPOSSEPBYSPACE, _T("LOCALE_IPOSSEPBYSPACE"),
    (const TCHAR*)LOCALE_IPOSSIGNPOSN, _T("LOCALE_IPOSSIGNPOSN"),
    (const TCHAR*)LOCALE_IPOSSYMPRECEDES, _T("LOCALE_IPOSSYMPRECEDES"),
    (const TCHAR*)LOCALE_ITIMEMARKPOSN, _T("LOCALE_ITIMEMARKPOSN"),
    (const TCHAR*)LOCALE_ITLZERO, _T("LOCALE_ITLZERO"),
    (const TCHAR*)LOCALE_SABBREVCTRYNAME, _T("LOCALE_SABBREVCTRYNAME"),
    (const TCHAR*)LOCALE_SABBREVDAYNAME1, _T("LOCALE_SABBREVDAYNAME1"),
    (const TCHAR*)LOCALE_SABBREVDAYNAME2, _T("LOCALE_SABBREVDAYNAME2"),
    (const TCHAR*)LOCALE_SABBREVDAYNAME3, _T("LOCALE_SABBREVDAYNAME3"),
    (const TCHAR*)LOCALE_SABBREVDAYNAME4, _T("LOCALE_SABBREVDAYNAME4"),
    (const TCHAR*)LOCALE_SABBREVDAYNAME5, _T("LOCALE_SABBREVDAYNAME5"),
    (const TCHAR*)LOCALE_SABBREVDAYNAME6, _T("LOCALE_SABBREVDAYNAME6"),
    (const TCHAR*)LOCALE_SABBREVDAYNAME7, _T("LOCALE_SABBREVDAYNAME7"),
    (const TCHAR*)LOCALE_SABBREVLANGNAME, _T("LOCALE_SABBREVLANGNAME"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME1, _T("LOCALE_SABBREVMONTHNAME1"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME2, _T("LOCALE_SABBREVMONTHNAME2"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME3, _T("LOCALE_SABBREVMONTHNAME3"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME4, _T("LOCALE_SABBREVMONTHNAME4"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME5, _T("LOCALE_SABBREVMONTHNAME5"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME6, _T("LOCALE_SABBREVMONTHNAME6"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME7, _T("LOCALE_SABBREVMONTHNAME7"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME8, _T("LOCALE_SABBREVMONTHNAME8"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME9, _T("LOCALE_SABBREVMONTHNAME9"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME10, _T("LOCALE_SABBREVMONTHNAME10"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME11, _T("LOCALE_SABBREVMONTHNAME11"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME12, _T("LOCALE_SABBREVMONTHNAME12"),
    (const TCHAR*)LOCALE_SABBREVMONTHNAME13, _T("LOCALE_SABBREVMONTHNAME13"),
#if WINVER >= _WIN32_WINNT_VISTA
    (const TCHAR*) LOCALE_SCONSOLEFALLBACKNAME, _T("LOCALE_SCONSOLEFALLBACKNAME"),
#endif
    (const TCHAR*)LOCALE_SDAYNAME1, _T("LOCALE_SDAYNAME1"),
    (const TCHAR*)LOCALE_SDAYNAME2, _T("LOCALE_SDAYNAME2"),
    (const TCHAR*)LOCALE_SDAYNAME3, _T("LOCALE_SDAYNAME3"),
    (const TCHAR*)LOCALE_SDAYNAME4, _T("LOCALE_SDAYNAME4"),
    (const TCHAR*)LOCALE_SDAYNAME5, _T("LOCALE_SDAYNAME5"),
    (const TCHAR*)LOCALE_SDAYNAME6, _T("LOCALE_SDAYNAME6"),
    (const TCHAR*)LOCALE_SDAYNAME7, _T("LOCALE_SDAYNAME7"),
#if WINVER >= _WIN32_WINNT_VISTA
    (const TCHAR*) LOCALE_SDURATION, _T("LOCALE_SDURATION"),
#endif
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_SENGCURRNAME, _T("LOCALE_SENGCURRNAME"),
    (const TCHAR*)LOCALE_SENGLISHCOUNTRYNAME, _T("LOCALE_SENGLISHCOUNTRYNAME"),
#endif
#if WINVER >= _WIN32_WINNT_WIN7
    (const TCHAR*) LOCALE_SENGLISHDISPLAYNAME, _T("LOCALE_SENGLISHDISPLAYNAME"),
#endif
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_SENGLISHLANGUAGENAME, _T("LOCALE_SENGLISHLANGUAGENAME"),
#endif
    (const TCHAR*)LOCALE_SINTLSYMBOL, _T("LOCALE_SINTLSYMBOL"),
#if 0
    (const TCHAR*)LOCALE_SIETFLANGUAGE, _T("LOCALE_SIETFLANGUAGE"),
#endif
#if WINVER >= _WIN32_WINNT_NT4
    (const TCHAR*) LOCALE_SISO3166CTRYNAME, _T("LOCALE_SISO3166CTRYNAME"),
#endif
#if WINVER >= _WIN32_WINNT_VISTA
    (const TCHAR*) LOCALE_SISO3166CTRYNAME2, _T("LOCALE_SISO3166CTRYNAME2"),
#endif
#if WINVER >= _WIN32_WINNT_NT4
    (const TCHAR*) LOCALE_SISO639LANGNAME, _T("LOCALE_SISO639LANGNAME"),
#endif
#if WINVER >= _WIN32_WINNT_VISTA
    (const TCHAR*) LOCALE_SISO639LANGNAME2, _T("LOCALE_SISO639LANGNAME2"),
    (const TCHAR*)LOCALE_SKEYBOARDSTOINSTALL, _T("LOCALE_SKEYBOARDSTOINSTALL"),
#endif
    (const TCHAR*)LOCALE_SMONTHNAME1, _T("LOCALE_SMONTHNAME1"),
    (const TCHAR*)LOCALE_SMONTHNAME2, _T("LOCALE_SMONTHNAME2"),
    (const TCHAR*)LOCALE_SMONTHNAME3, _T("LOCALE_SMONTHNAME3"),
    (const TCHAR*)LOCALE_SMONTHNAME4, _T("LOCALE_SMONTHNAME4"),
    (const TCHAR*)LOCALE_SMONTHNAME5, _T("LOCALE_SMONTHNAME5"),
    (const TCHAR*)LOCALE_SMONTHNAME6, _T("LOCALE_SMONTHNAME6"),
    (const TCHAR*)LOCALE_SMONTHNAME7, _T("LOCALE_SMONTHNAME7"),
    (const TCHAR*)LOCALE_SMONTHNAME8, _T("LOCALE_SMONTHNAME8"),
    (const TCHAR*)LOCALE_SMONTHNAME9, _T("LOCALE_SMONTHNAME9"),
    (const TCHAR*)LOCALE_SMONTHNAME10, _T("LOCALE_SMONTHNAME10"),
    (const TCHAR*)LOCALE_SMONTHNAME11, _T("LOCALE_SMONTHNAME11"),
    (const TCHAR*)LOCALE_SMONTHNAME12, _T("LOCALE_SMONTHNAME12"),
    (const TCHAR*)LOCALE_SMONTHNAME13, _T("LOCALE_SMONTHNAME13"),
#if WINVER >= _WIN32_WINNT_VISTA
    (const TCHAR*) LOCALE_SNAME, _T("LOCALE_SNAME"),
    (const TCHAR*)LOCALE_SNAN, _T("LOCALE_SNAN"),
#endif
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_SNATIVECOUNTRYNAME, _T("LOCALE_SNATIVECOUNTRYNAME"),
    (const TCHAR*)LOCALE_SNATIVECURRNAME, _T("LOCALE_SNATIVECURRNAME"),
#endif
#if WINVER >= _WIN32_WINNT_WIN7
    (const TCHAR*) LOCALE_SNATIVEDISPLAYNAME, _T("LOCALE_SNATIVEDISPLAYNAME"),
#endif
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_SNATIVELANGUAGENAME, _T("LOCALE_SNATIVELANGUAGENAME"),
#endif
#if WINVER >= _WIN32_WINNT_VISTA
    (const TCHAR*) LOCALE_SNEGINFINITY, _T("LOCALE_SNEGINFINITY"),
    (const TCHAR*)LOCALE_SPARENT, _T("LOCALE_SPARENT"),
#endif
#if WINVER >= _WIN32_WINNT_WIN7
    (const TCHAR*) LOCALE_SPERCENT, _T("LOCALE_SPERCENT"),
#endif
#if WINVER >= _WIN32_WINNT_VISTA
    (const TCHAR*) LOCALE_SPOSINFINITY, _T("LOCALE_SPOSINFINITY"),
    (const TCHAR*)LOCALE_SSCRIPTS, _T("LOCALE_SSCRIPTS"),
    (const TCHAR*)LOCALE_SSHORTESTDAYNAME1, _T("LOCALE_SSHORTESTDAYNAME1"),
    (const TCHAR*)LOCALE_SSHORTESTDAYNAME2, _T("LOCALE_SSHORTESTDAYNAME2"),
    (const TCHAR*)LOCALE_SSHORTESTDAYNAME3, _T("LOCALE_SSHORTESTDAYNAME3"),
    (const TCHAR*)LOCALE_SSHORTESTDAYNAME4, _T("LOCALE_SSHORTESTDAYNAME4"),
    (const TCHAR*)LOCALE_SSHORTESTDAYNAME5, _T("LOCALE_SSHORTESTDAYNAME5"),
    (const TCHAR*)LOCALE_SSHORTESTDAYNAME6, _T("LOCALE_SSHORTESTDAYNAME6"),
    (const TCHAR*)LOCALE_SSHORTESTDAYNAME7, _T("LOCALE_SSHORTESTDAYNAME7"),
#endif
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_SSORTNAME, _T("LOCALE_SSORTNAME"),
#endif
    (const TCHAR*)LOCALE_ICALENDARTYPE, _T("LOCALE_ICALENDARTYPE"),
    (const TCHAR*)LOCALE_ICURRDIGITS, _T("LOCALE_ICURRDIGITS"),
    (const TCHAR*)LOCALE_ICURRENCY, _T("LOCALE_ICURRENCY"),
    (const TCHAR*)LOCALE_IDIGITS, _T("LOCALE_IDIGITS"),
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_IDIGITSUBSTITUTION, _T("LOCALE_IDIGITSUBSTITUTION"),
#endif
    (const TCHAR*)LOCALE_IFIRSTDAYOFWEEK, _T("LOCALE_IFIRSTDAYOFWEEK"),
    (const TCHAR*)LOCALE_IFIRSTWEEKOFYEAR, _T("LOCALE_IFIRSTWEEKOFYEAR"),
    (const TCHAR*)LOCALE_ILZERO, _T("LOCALE_ILZERO"),
    (const TCHAR*)LOCALE_IMEASURE, _T("LOCALE_IMEASURE"),
    (const TCHAR*)LOCALE_INEGCURR, _T("LOCALE_INEGCURR"),
    (const TCHAR*)LOCALE_INEGNUMBER, _T("LOCALE_INEGNUMBER"),
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_IPAPERSIZE, _T("LOCALE_IPAPERSIZE"),
#endif
#if WINVER >= _WIN32_WINNT_WIN7
    (const TCHAR*) LOCALE_IREADINGLAYOUT, _T("LOCALE_IREADINGLAYOUT"),
#endif
    (const TCHAR*)LOCALE_ITIME, _T("LOCALE_ITIME"),
    (const TCHAR*)LOCALE_S1159, _T("LOCALE_S1159"),
    (const TCHAR*)LOCALE_S2359, _T("LOCALE_S2359"),
    (const TCHAR*)LOCALE_SCURRENCY, _T("LOCALE_SCURRENCY"),
    (const TCHAR*)LOCALE_SDATE, _T("LOCALE_SDATE"),
    (const TCHAR*)LOCALE_SDECIMAL, _T("LOCALE_SDECIMAL"),
    (const TCHAR*)LOCALE_SGROUPING, _T("LOCALE_SGROUPING"),
    (const TCHAR*)LOCALE_SLIST, _T("LOCALE_SLIST"),
    (const TCHAR*)LOCALE_SLONGDATE, _T("LOCALE_SLONGDATE"),
    (const TCHAR*)LOCALE_SMONDECIMALSEP, _T("LOCALE_SMONDECIMALSEP"),
    (const TCHAR*)LOCALE_SMONGROUPING, _T("LOCALE_SMONGROUPING"),
    (const TCHAR*)LOCALE_SMONTHOUSANDSEP, _T("LOCALE_SMONTHOUSANDSEP"),
    (const TCHAR*)LOCALE_SNATIVEDIGITS, _T("LOCALE_SNATIVEDIGITS"),
    (const TCHAR*)LOCALE_SNEGATIVESIGN, _T("LOCALE_SNEGATIVESIGN"),
    (const TCHAR*)LOCALE_SPOSITIVESIGN, _T("LOCALE_SPOSITIVESIGN"),
    (const TCHAR*)LOCALE_SSHORTDATE, _T("LOCALE_SSHORTDATE"),
#if WINVER >= _WIN32_WINNT_WIN7
    (const TCHAR*) LOCALE_SSHORTTIME, _T("LOCALE_SSHORTTIME"),
#endif
    (const TCHAR*)LOCALE_STHOUSAND, _T("LOCALE_STHOUSAND"),
    (const TCHAR*)LOCALE_STIME, _T("LOCALE_STIME"),
    (const TCHAR*)LOCALE_STIMEFORMAT, _T("LOCALE_STIMEFORMAT"),
#if WINVER >= _WIN32_WINNT_WIN2K
    (const TCHAR*) LOCALE_SYEARMONTH, _T("LOCALE_SYEARMONTH"),
#endif
};

void PrintLocaleInfo(LCID lcid)
{
    size_t cchLocaleInfoData;
    size_t cchLocaleInfoAllocatedData = 0;
    LPTSTR pszLocaleInfoData = NULL;
    int i;

    for (i = 0; i < _countof(pszLocaleInfoArray); i += 2)
    {
        LCTYPE nLocaleInfoIndex = (LCTYPE)(size_t)pszLocaleInfoArray[i];
        LPCTSTR pszLocaleInfoName = pszLocaleInfoArray[i + 1];

        if ((cchLocaleInfoData = (size_t)GetLocaleInfo(lcid, nLocaleInfoIndex, NULL, 0)) > 0)
        {
            if (cchLocaleInfoData > cchLocaleInfoAllocatedData)
            {
                free(pszLocaleInfoData); pszLocaleInfoData = (LPTSTR)calloc(cchLocaleInfoAllocatedData = cchLocaleInfoData, sizeof(TCHAR));
            }

            if (GetLocaleInfo(lcid, nLocaleInfoIndex, pszLocaleInfoData, cchLocaleInfoData) > 0)
            {
                _tprintf(_T("  %s = [%.*s]\n"), pszLocaleInfoName, cchLocaleInfoData, pszLocaleInfoData);
            }
            else
                PrintWin32Message(GetLastError(), _T("GetLocaleInfo"));
        }
        else
            PrintWin32Message(GetLastError(), _T("GetLocaleInfo"));
    }

    free(pszLocaleInfoData);
}

BOOL GetLocaleName(LCID lcid, LPTSTR pszLocaleName, size_t cchLocaleName)
{
#if WINVER >= _WIN32_WINNT_VISTA
    if (GetLocaleInfo(lcid, LOCALE_SNAME, pszLocaleName, cchLocaleName) > 0)
    {
        return TRUE;
    }
    else
        PrintWin32Message(GetLastError(), _T("GetLocaleInfo"));
#else
    UNREFERENCED_PARAMETER(lcid);
    _sntprintf_s(pszLocaleName, cchLocaleName, cchLocaleName, _T(""));
#endif
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// User Interface Language Management
// https://msdn.microsoft.com/en-us/library/dd374098(v=vs.85).aspx

void PrintUILanguage(void)
{
    // Retrieves the current Windows ANSI code page identifier for the operating system.

    {
        UINT uACP = GetACP();
        CPINFOEX aCPInfoEx = { 0 };
        int i;

        _tprintf(_T("ANSI code page = %u 0x%04X\n"), uACP, uACP);

        if (GetCPInfoEx(CP_ACP, 0, &aCPInfoEx) || aCPInfoEx.MaxCharSize > 0)
        {
            _tprintf(_T("  MaxCharSize = %u\n"), aCPInfoEx.MaxCharSize);

            for (i = 0; i < _countof(aCPInfoEx.DefaultChar); i++)
                _tprintf(_T("  %s = %hc 0x%02X\n"), i == 0 ? _T("DefaultChar") : _T("           "),
                    isprint(aCPInfoEx.DefaultChar[i]) ? aCPInfoEx.DefaultChar[i] : ' ', aCPInfoEx.DefaultChar[i]);

            for (i = 0; i < _countof(aCPInfoEx.DefaultChar); i += 2)
                _tprintf(_T("  %s = 0x%02X 0x%02X\n"), i == 0 ? _T("LeadByteRange") : _T("             "),
                    aCPInfoEx.LeadByte[i], aCPInfoEx.LeadByte[i + 1]);

            _tprintf(_T("  UnicodeDefaultChar = %wc 0x%04X\n"), aCPInfoEx.UnicodeDefaultChar, aCPInfoEx.UnicodeDefaultChar);
            _tprintf(_T("  CodePage = %u 0x%04X\n"), aCPInfoEx.CodePage, aCPInfoEx.CodePage);
            _tprintf(_T("  CodePageName = %s\n"), aCPInfoEx.CodePageName);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetCPInfoEx"));
    }

    // Returns the current original equipment manufacturer (OEM) code page identifier for the operating system.

    {
        UINT uOEMCP = GetOEMCP();
        CPINFOEX aCPInfoEx = { 0 };
        int i;

        _tprintf(_T("OEM code page = %u 0x%04X\n"), uOEMCP, uOEMCP);

        if (GetCPInfoEx(CP_OEMCP, 0, &aCPInfoEx) || aCPInfoEx.MaxCharSize > 0)
        {
            _tprintf(_T("  MaxCharSize = %u\n"), aCPInfoEx.MaxCharSize);

            for (i = 0; i < _countof(aCPInfoEx.DefaultChar); i++)
                _tprintf(_T("  %s = %hc 0x%02X\n"), i == 0 ? _T("DefaultChar") : _T("           "),
                    isprint(aCPInfoEx.DefaultChar[i]) ? aCPInfoEx.DefaultChar[i] : ' ', aCPInfoEx.DefaultChar[i]);

            for (i = 0; i < _countof(aCPInfoEx.DefaultChar); i += 2)
                _tprintf(_T("  %s = 0x%02X 0x%02X\n"), i == 0 ? _T("LeadByteRange") : _T("             "),
                    aCPInfoEx.LeadByte[i], aCPInfoEx.LeadByte[i + 1]);

            _tprintf(_T("  UnicodeDefaultChar = %wc 0x%04X\n"), aCPInfoEx.UnicodeDefaultChar, aCPInfoEx.UnicodeDefaultChar);
            _tprintf(_T("  CodePage = %u 0x%04X\n"), aCPInfoEx.CodePage, aCPInfoEx.CodePage);
            _tprintf(_T("  CodePageName = %s\n"), aCPInfoEx.CodePageName);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetCPInfoEx"));
    }

    // Retrieves the current thread's ANSI code page.

    {
        CPINFOEX aCPInfoEx = { 0 };
        int i;

        _tprintf(_T("Current thread's ANSI code page\n"));

        if (GetCPInfoEx(CP_THREAD_ACP, 0, &aCPInfoEx))
        {
            _tprintf(_T("  MaxCharSize = %u\n"), aCPInfoEx.MaxCharSize);

            for (i = 0; i < _countof(aCPInfoEx.DefaultChar); i++)
                _tprintf(_T("  %s = %hc 0x%02X\n"), i == 0 ? _T("DefaultChar") : _T("           "),
                    isprint(aCPInfoEx.DefaultChar[i]) ? aCPInfoEx.DefaultChar[i] : ' ', aCPInfoEx.DefaultChar[i]);

            for (i = 0; i < _countof(aCPInfoEx.DefaultChar); i += 2)
                _tprintf(_T("  %s = 0x%02X 0x%02X\n"), i == 0 ? _T("LeadByteRange") : _T("             "),
                    aCPInfoEx.LeadByte[i], aCPInfoEx.LeadByte[i + 1]);

            _tprintf(_T("  UnicodeDefaultChar = %wc 0x%04X\n"), aCPInfoEx.UnicodeDefaultChar, aCPInfoEx.UnicodeDefaultChar);
            _tprintf(_T("  CodePage = %u 0x%04X\n"), aCPInfoEx.CodePage, aCPInfoEx.CodePage);
            _tprintf(_T("  CodePageName = %s\n"), aCPInfoEx.CodePageName);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetCPInfoEx"));
    }

    // Returns the locale identifier for the system default locale, identified by LOCALE_SYSTEM_DEFAULT.

    {
        LCID lcid = GetSystemDefaultLCID();

#if WINVER >= _WIN32_WINNT_VISTA
        WCHAR szLocaleName[LOCALE_NAME_MAX_LENGTH];

        if (GetSystemDefaultLocaleName(szLocaleName, _countof(szLocaleName)) > 0)
        {
            _tprintf(_T("SystemDefaultLocale = %u 0x%04X [%ls]\n"), lcid, lcid, szLocaleName);
            PrintLocaleInfo(LOCALE_SYSTEM_DEFAULT);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetSystemPreferredUILanguages"));
#else
        _tprintf(_T("SystemDefaultLocale = %u 0x%04X\n"), lcid, lcid);
#endif
    }

    // Returns the locale identifier for the user default locale, represented as LOCALE_USER_DEFAULT. If the user default locale is
    // a custom locale, this function always returns LOCALE_CUSTOM_DEFAULT, regardless of the custom locale that is selected.

    {
        LCID lcid = GetUserDefaultLCID();

#if WINVER >= _WIN32_WINNT_VISTA
        WCHAR szLocaleName[LOCALE_NAME_MAX_LENGTH];

        if (GetUserDefaultLocaleName(szLocaleName, _countof(szLocaleName)) > 0)
        {
            _tprintf(_T("UserDefaultLocale = %u 0x%04X [%ls]\n"), lcid, lcid, szLocaleName);
            PrintLocaleInfo(LOCALE_USER_DEFAULT);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetUserPreferredUILanguages"));
#else
        _tprintf(_T("UserDefaultLocale = %u 0x%04X\n"), lcid, lcid);
#endif
    }

    // Returns the locale identifier of the current locale for the calling thread.

    {
        LCID lcid = GetThreadLocale();

#if WINVER >= _WIN32_WINNT_VISTA
        TCHAR szLocaleName[LOCALE_NAME_MAX_LENGTH];

        if (GetLocaleName(lcid, szLocaleName, _countof(szLocaleName)))
        {
            _tprintf(_T("ThreadLocale = %u 0x%04X [%s]\n"), lcid, lcid, szLocaleName);
            // PrintLocaleInfo (lcid);
        }
#else
        _tprintf(_T("ThreadLocale = %u 0x%04X\n"), lcid, lcid);
#endif
    }

    // Returns the language identifier for the system locale. This is the language used when displaying text in programs that do
    // not support Unicode. It is set by the Administrator under Control Panel > Clock, Language, and Region > Change date, time,
    // or number formats > Administrative tab.

    {
        LANGID nSystemDefaultLanguage = GetSystemDefaultLangID();
        _tprintf(_T("SystemDefaultLanguage = %u 0x%04X\n"), nSystemDefaultLanguage, nSystemDefaultLanguage);
    }

    // System Default UI Language/Install Language
    //
    // The system default UI language is the language of the localized version used to set up Windows. All menus, dialog boxes,
    // error messages, and help files are represented in this language, except when the user selects a different language.
    //
    // On Windows Vista and later, the system default UI language is known as the "install language" and plays a more limited role.
    // For most purposes, it is superseded by the system preferred UI languages. However, in certain contexts it is useful to have a
    // single install language that is always known to be fully supported.

#if WINVER >= _WIN32_WINNT_WIN2K
    {
        LANGID nSystemDefaultUILanguage = GetSystemDefaultUILanguage();
        _tprintf(_T("SystemDefaultUILanguage = %u 0x%04X\n"), nSystemDefaultUILanguage, nSystemDefaultUILanguage);
    }
#endif

    // System UI Language
    //
    // The operating system defines the system UI language as a user interface language that can be set by an administrator in the
    // Advanced tab of the regional and language options portion of Control Panel. The operating system uses this language if the
    // current user has not made specific language settings or if no active account is logged in. The language can be changed only
    // if more than one user interface language is installed on the computer.

#if WINVER >= _WIN32_WINNT_VISTA
    {
        ULONG ulNumLanguages = 0;
        PZZWSTR pwszLanguagesBuffer = NULL;
        ULONG cchLanguagesBuffer = 0;
        PWSTR pwszLanguage;

        if (GetSystemPreferredUILanguages(MUI_LANGUAGE_ID, &ulNumLanguages, pwszLanguagesBuffer, &cchLanguagesBuffer))
        {
            ulNumLanguages = 0;
            pwszLanguagesBuffer = (PZZWSTR)calloc(cchLanguagesBuffer, sizeof(WCHAR));

            if (GetSystemPreferredUILanguages(MUI_LANGUAGE_ID, &ulNumLanguages, pwszLanguagesBuffer, &cchLanguagesBuffer))
            {
                _tprintf(_T("SystemPreferredUILanguages: %lu\n"), ulNumLanguages);

                for (pwszLanguage = pwszLanguagesBuffer; *pwszLanguage != '\0'; pwszLanguage += wcslen(pwszLanguage) + 1)
                {
                    _tprintf(_T("  %ls%s\n"), pwszLanguage, pwszLanguage == pwszLanguagesBuffer ? _T(" <- System UI Language") : _T(""));
                }
            }
            else
                PrintWin32Message(GetLastError(), _T("GetSystemPreferredUILanguages"));

            free(pwszLanguagesBuffer);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetSystemPreferredUILanguages"));
    }
#else
    _tprintf(_T("SystemPreferredUILanguages: N/A\n"));
#endif

    // User UI Language
    //
    // The user UI language determines the user interface language used for menus, dialog boxes, help files, and so forth. It can be
    // set by the current user in the Language tab of the regional and language options portion of Control Panel. This language can
    // be changed only if more than one user interface language is installed on the computer. Note that the user will have to log
    // off and then log back on to see the effect.
    //
    // If the current user has not set a language, GetUserDefaultUILanguage returns the preferred language set for the system. If
    // there is no preferred language set for the system, then the system default UI language (also known as "install language") is
    // returned.

#if WINVER >= _WIN32_WINNT_VISTA
    {
        LANGID nUserDefaultUILanguage = GetUserDefaultUILanguage();
        _tprintf(_T("UserDefaultUILanguage = %u 0x%04X\n"), nUserDefaultUILanguage, nUserDefaultUILanguage);
    }
#endif

#if WINVER >= _WIN32_WINNT_VISTA
    {
        ULONG ulNumLanguages = 0;
        PZZWSTR pwszLanguagesBuffer = NULL;
        ULONG cchLanguagesBuffer = 0;
        PWSTR pwszLanguage;

        if (GetUserPreferredUILanguages(MUI_LANGUAGE_ID, &ulNumLanguages, pwszLanguagesBuffer, &cchLanguagesBuffer))
        {
            ulNumLanguages = 0;
            pwszLanguagesBuffer = (PZZWSTR)calloc(cchLanguagesBuffer, sizeof(WCHAR));

            if (GetUserPreferredUILanguages(MUI_LANGUAGE_ID, &ulNumLanguages, pwszLanguagesBuffer, &cchLanguagesBuffer))
            {
                _tprintf(_T("UserPreferredUILanguages: %lu\n"), ulNumLanguages);

                for (pwszLanguage = pwszLanguagesBuffer; *pwszLanguage != '\0'; pwszLanguage += wcslen(pwszLanguage) + 1)
                {
                    _tprintf(_T("  %ls%s\n"), pwszLanguage, pwszLanguage == pwszLanguagesBuffer ? _T(" <- User UI Language") : _T(""));
                }
            }
            else
                PrintWin32Message(GetLastError(), _T("GetUserPreferredUILanguages"));

            free(pwszLanguagesBuffer);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetUserPreferredUILanguages"));
    }
#else
    _tprintf(_T("UserPreferredUILanguages: N/A\n"));
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// User Interface Language Management
// https://msdn.microsoft.com/en-us/library/dd374098(v=vs.85).aspx

void PrintLocaleId(void)
{
    // System default locale and language.

    {
        LCID nSystemDefaultLocale;
        LANGID nSystemDefaultLanguage;
        WCHAR wszSystemDefaultLocaleName[LOCALE_NAME_MAX_LENGTH];

        nSystemDefaultLocale = GetSystemDefaultLCID();
        nSystemDefaultLanguage = GetSystemDefaultLangID();

#if WINVER >= _WIN32_WINNT_VISTA
        if (LCIDToLocaleName(nSystemDefaultLocale, wszSystemDefaultLocaleName, _countof(wszSystemDefaultLocaleName), 0) == 0)
        {
            PrintWin32Message(GetLastError(), _T("LCIDToLocaleName"));
            wszSystemDefaultLocaleName[0] = '\0';
        }
#else
        wszSystemDefaultLocaleName[0] = '\0';
#endif

        _tprintf(_T("SystemDefaultLocale = %u 0x%04X [%ls]\n"), nSystemDefaultLocale, nSystemDefaultLocale, wszSystemDefaultLocaleName);
        _tprintf(_T("SystemDefaultLanguage = %u 0x%04X\n"), nSystemDefaultLanguage, nSystemDefaultLanguage);

#if WINVER >= _WIN32_WINNT_VISTA
        if (GetSystemDefaultLocaleName(wszSystemDefaultLocaleName, _countof(wszSystemDefaultLocaleName)) > 0)
        {
            _tprintf(_T("SystemDefaultLocaleName = [%ls]\n"), wszSystemDefaultLocaleName);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetSystemDefaultLocaleName"));
#endif
    }

    // User default locale and language.

    {
        LCID nUserDefaultLocale;
        LANGID nUserDefaultLanguage;
        WCHAR wszUserDefaultLocaleName[LOCALE_NAME_MAX_LENGTH];

        nUserDefaultLocale = GetUserDefaultLCID();
        nUserDefaultLanguage = GetUserDefaultLangID();

#if WINVER >= _WIN32_WINNT_VISTA
        if (LCIDToLocaleName(nUserDefaultLocale, wszUserDefaultLocaleName, _countof(wszUserDefaultLocaleName), 0) == 0)
        {
            PrintWin32Message(GetLastError(), _T("LCIDToLocaleName"));
            wszUserDefaultLocaleName[0] = '\0';
        }
#else
        wszUserDefaultLocaleName[0] = '\0';
#endif

        _tprintf(_T("UserDefaultLocale = %u 0x%04X [%ls]\n"), nUserDefaultLocale, nUserDefaultLocale, wszUserDefaultLocaleName);
        _tprintf(_T("UserDefaultLanguage = %u 0x%04X\n"), nUserDefaultLanguage, nUserDefaultLanguage);

#if WINVER >= _WIN32_WINNT_VISTA
        if (GetUserDefaultLocaleName(wszUserDefaultLocaleName, _countof(wszUserDefaultLocaleName)) > 0)
        {
            _tprintf(_T("UserDefaultLocaleName = [%ls]\n"), wszUserDefaultLocaleName);
        }
        else
            PrintWin32Message(GetLastError(), _T("GetUserDefaultLocaleName"));
#endif
    }

    {
        UINT uConsoleOutputCP = GetConsoleOutputCP();
        _tprintf(_T("ConsoleOutputCP = %d 0x%04X\n"), uConsoleOutputCP, uConsoleOutputCP);
    }

#if WINVER >= _WIN32_WINNT_WIN2K
    {
        LANGID nSystemDefaultUILanguage = GetSystemDefaultUILanguage();
        _tprintf(_T("SystemDefaultUILanguage = %u 0x%04X\n"), nSystemDefaultUILanguage, nSystemDefaultUILanguage);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestStringScan(void)
{
    static const WCHAR wszText[] = L"\x0061\x030a\x0061\x0301\x0302\x0303\x0304\x0075\x0308\xD841\xDC0A\x006f\x0323\x0302\xDC0A\xD841\x0068";
    LPCWCH pch;

    _tprintf(_T("Test1\n"));

    for (pch = wszText; *pch != L'\0'; pch++)
    {
        wint_t wch = *pch; _tprintf(_T("%4Id %5u %04X %lc%s%s\n"), pch - wszText, wch, wch, iswprint(wch) ? wch : L' ',
            IS_LOW_SURROGATE(wch) ? _T(" IS_LOW_SURROGATE") : _T(""),
            IS_HIGH_SURROGATE(wch) ? _T(" IS_HIGH_SURROGATE") : _T(""));
    }

    // CharNext
    //
    // This function works with default "user" expectations of characters when dealing with diacritics. For example: A string that
    // contains U+0061 U+030a "LATIN SMALL LETTER A" + COMBINING RING ABOVE" — which looks like "å", will advance two code points,
    // not one. A string that contains U+0061 U+0301 U+0302 U+0303 U+0304 — which looks like "a´^~¯", will advance five code points,
    // not one, and so on.
    //
    // Because the surrogate mechanism to represent supplementary characters is well-defined, your application can include code to
    // handle UTF-16 surrogate text processing. When the application encounters a separated UTF-16 value from either the lower
    // reserved surrogate range (a low surrogate) or the upper reserved surrogate range (a high surrogate), the value must be one
    // half of a surrogate pair. Thus, the application can detect a surrogate pair by doing simple range checking. If it encounters
    // a UTF-16 value in the lower or upper range, it must track backward or forward one 16-bit width to get the rest of the
    // character. When writing your application, keep in mind that CharNext and CharPrev move by 16-bit code points, not by
    // surrogate pairs.

    _tprintf(_T("Test2\n"));

    for (pch = wszText; *pch != L'\0'; pch = CharNextW(pch))
    {
        wint_t wch = *pch; _tprintf(_T("%4Id %5u %04X %lc%s%s\n"), pch - wszText, wch, wch, iswprint(wch) ? wch : L' ',
            IS_LOW_SURROGATE(wch) ? _T(" IS_LOW_SURROGATE") : _T(""),
            IS_HIGH_SURROGATE(wch) ? _T(" IS_HIGH_SURROGATE") : _T(""));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const unsigned int UN_LEAD_OFFSET = 0xD800 - (0x10000 >> 10);
const unsigned int UN_SURROGATE_OFFSET = (unsigned int)0x10000 - (0xD800 << 10) - 0xDC00;

void un_write_utf16(unsigned int codepoint, unsigned short* buffer)
{
    if (codepoint < 0x10000)
    {
        *buffer++ = (unsigned short)codepoint;
    }
    else if (codepoint <= 0x10FFFF)
    {
        *buffer++ = (unsigned short)(UN_LEAD_OFFSET + (codepoint >> 10));
        *buffer++ = (unsigned short)(0xDC00 + (codepoint & 0x3FF));
    }
    else
    {
        // invalid codepoint"
    }
}

unsigned int un_read_utf16(unsigned short* buffer)
{
    unsigned int lead = *buffer++;

    if (lead >= 0xD800 && lead <= 0xDBFF)
    {
        unsigned int trail = *buffer;

        if (trail >= 0xDC00 && trail <= 0xDFFF)
        {
            buffer++;
            return (lead << 10) + trail + UN_SURROGATE_OFFSET;
        }
    }

    return lead;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void wp_write_utf16(unsigned int codepoint, unsigned short* buffer)
{
    if (codepoint < 0x10000)
    {
        *buffer++ = (unsigned short)codepoint;
    }
    else if (codepoint <= 0x10FFFF)
    {
        *buffer++ = (unsigned short)(0xD7C0 + (codepoint >> 10));
        *buffer++ = (unsigned short)(0xDC00 + (codepoint & 0x3FF));
    }
    else
    {
        // invalid codepoint"
    }
}

unsigned int wp_read_utf16(unsigned short* buffer)
{
    unsigned int lead = *buffer++;

    if (lead >= 0xD800 && lead <= 0xDBFF)
    {
        unsigned int trail = *buffer;

        if (trail >= 0xDC00 && trail <= 0xDFFF)
        {
            buffer++;
            return (lead << 10) + trail - 0x35FDC00;
        }
    }

    return lead;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// UTF-16
// http://en.wikipedia.org/wiki/UTF-16
// http://www.unicode.org/faq/utf_bom.html#utf16-4

//#define LEAD_OFFSET         (0xD800 - (0x10000 >> 10))
//#define SURROGATE_OFFSET    (0xDC00 + (0xD800  << 10) - 0x10000)

void write_utf16(unsigned int codepoint, unsigned short* buffer)
{
    if (codepoint < 0x10000)
    {
        *buffer++ = (unsigned short)codepoint; /* no surrogate */
    }
    else if (codepoint <= 0x10FFFF)
    {
        *buffer++ = (unsigned short)(LEAD_OFFSET + (codepoint >> 10)); /* high surrogate */
        *buffer++ = (unsigned short)(0xDC00 + (codepoint & 0x3FF)); /* low surrogate */
    }
    else
    {
        /* invalid codepoint */
    }
}

unsigned int read_utf16(unsigned short* buffer)
{
    unsigned int lead = *buffer++;

    if (lead >= 0xD800 && lead <= 0xDBFF) /* high surrogate */
    {
        unsigned int trail = *buffer;

        if (trail >= 0xDC00 && trail <= 0xDFFF) /* low surrogate */
        {
            buffer++;
            return (lead << 10) - SURROGATE_OFFSET + trail;
        }
    }

    return lead;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void jr_write_utf16(unsigned int codepoint, unsigned short* buffer)
{
    if (codepoint < 0x10000)
    {
        *buffer++ = (unsigned short)codepoint;
    }
    else if (codepoint <= 0x10FFFF)
    {
        *buffer++ = (unsigned short)(0xD800 + ((codepoint - 0x10000) >> 10));
        *buffer++ = (unsigned short)(0xDC00 + (codepoint & 0x3FF));
    }
    else
    {
        // invalid codepoint"
    }
}

unsigned int jr_read_utf16(unsigned short* buffer)
{
    unsigned int lead = *buffer++;

    if (lead >= 0xD800 && lead <= 0xDBFF)
    {
        unsigned int trail = *buffer;

        if (trail >= 0xDC00 && trail <= 0xDFFF)
        {
            buffer++;
            return ((lead - 0xD800) << 10) + 0x10000 + (trail - 0xDC00);
        }
    }

    return lead;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void utf16(void)
{
    unsigned int codepoint;
    unsigned short buffer1[2];
    unsigned short buffer2[2];

    printf("%X %X %X %X\n", UN_LEAD_OFFSET, UN_SURROGATE_OFFSET, (0xD800 << 10) - 0x10000 + 0xDC00, 1 << 5);
    printf("%X %X %X %X\n", -(0xD800 << 10), 0xD800 << 10, 0x10000, 0xDC00);

    for (codepoint = 0; codepoint <= 0x10FFFF; codepoint++)
    {
        unsigned int codepoint1;
        unsigned int codepoint2;
        buffer1[0] = buffer1[1] = 0;
        jr_write_utf16(codepoint, buffer1);
        buffer2[0] = buffer2[1] = 0;
        write_utf16(codepoint, buffer2);
        if (buffer1[0] != buffer2[0] || buffer1[1] != buffer2[1])
        {
            _tprintf(_T("ERRO codepoint = %06X buffer1 = %04X %04X buffer2 = %04X %04X\n"), codepoint, buffer1[0], buffer1[1], buffer2[0], buffer2[1]);
        }
        codepoint1 = jr_read_utf16(buffer1);
        codepoint2 = read_utf16(buffer2);
        if (codepoint != codepoint1 || codepoint1 != codepoint2)
        {
            _tprintf(_T("ERRO codepoint = %06X codepoint1 = %06X codepoint2 = %06X buffer1 = %04X %04X buffer2 = %04X %04X\n"), codepoint, codepoint1, codepoint2, buffer1[0], buffer1[1], buffer2[0], buffer2[1]);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1600 && 0
void PrintLocale1(void)
{
    _locale_t locale = _get_current_locale();
    //LOCALE_USE_CP_ACP

    int i;
    for (i = 0; i < _countof(locale->locinfo->lc_id); i++)
        _tprintf(_T("wLanguage [%d] = %d 0x%04X\n"), i, locale->locinfo->lc_id[i].wLanguage, locale->locinfo->lc_id[i].wLanguage);
}
#else
void PrintLocale1(void)
{
}
#endif

/*

GetLocaleInfo/LOCALE_IDEFAULTANSICODEPAGE

AFX_STATIC BOOL PASCAL _AfxInitDBCS()
{
    CPINFO info;
    GetCPInfo(GetOEMCP(), &info);
    return info.MaxCharSize > 1;
}

MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT)

    // Returns the language identifier for the system locale. This is the language used when displaying text in programs that do
    // not support Unicode. It is set by the Administrator under Control Panel > Clock, Language, and Region > Change date, time,
    // or number formats > Administrative tab.

    {
        LANGID nSystemDefaultLanguage  = GetSystemDefaultLangID ();
        _tprintf (_T ("SystemDefaultLanguage = %u 0x%04X\n"), nSystemDefaultLanguage, nSystemDefaultLanguage);
    }


*/

// https://msdn.microsoft.com/en-us/goglobal/bb688115


// Get the user's UI language.
// Get the code page of the UI language by generating an LCID for the language and calling GetLocaleInfo.
// Get the console output code page with GetConsoleOutputCP.
// If the code pages found in step 2 match the console code page, and the UI language is not Hebrew, Arabic, or another language
// that uses complex scripts, load the resources that match the user's UI language. Otherwise, load English resources.

void PrintData(void)
{
}

/*

For each you have the option to get them in a numeric form
(LCID, used all the way to XP) or string (starting with Vista)
You can also get a language ID (LANGID), but it might not be enough
for all operations (a locale is usualy a better choice).

The language in which the UI was localized at install time?
    GetSystemDefaultUILanguage -> LANGID

The language in which the UI is displayed right now?
(might be different than the system UI language if you have a MUI or LIP)
    GetUserDefaultUILanguage -> LANGID

The system locale (that determines the ANSI and OEM code pages)?
    GetSystemDefaultLocaleName -> string
    GetSystemDefaultLCID -> LCID
    GetSystemDefaultLangID -> LANGID

The locale used for locale-sensitive operations
(number/currency/date/time formatting, sorting, case conversion)?
    GetUserDefaultLocaleName -> string
    GetUserDefaultLCID -> LCID
    GetUserDefaultLangID -> LANGID


For system code page there are two of them:
- the ANSI code page => GetACP
- the OEM code page, used (by default) by console apps => GetOEMCP

FormatMessage

1. Language neutral
2. Thread LANGID, based on the thread's locale value
3. User default LANGID, based on the user's default locale value
4. System default LANGID, based on the system default locale value
5. US English

// The operating system assigns a locale to each thread, initially assigning the "system default locale," defined by
// LOCALE_SYSTEM_DEFAULT. This locale is set when the operating system is installed or when the user selects it using the regional
// and language options portion of the Control Panel. When running a thread in a process belonging to the user, the operating system
// assigns the "user default locale" to the thread. This locale is defined by LOCALE_USER_DEFAULT. An application can override
// either default by using the SetThreadLocale function to explicitly set the locale for a thread.


WORD wDefLang = LANGIDFROMLCID(GetUserDefaultLCID());
LANGID wDefLang = GetSystemDefaultLangID();     // or
GetUserDefaultLangID();

    if (GetLocaleInfo (lcid, LOCALE_IDEFAULTCODEPAGE, pszLocaleName, cchLocaleName) > 0)
    {
        return TRUE;
    }
    else
        PrintWin32Message (GetLastError (), _T ("GetLocaleInfo"));

*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FormatMessageTestAuxF(DWORD dwMessageId, LANGID wLanguageId, LPCTSTR pszLanguageDescription)
{
#ifndef UNICODE
    TCHAR szBuffer[2048];
    DWORD nWritten;
    FormatWin32MessageTst(NULL, dwMessageId, wLanguageId, szBuffer, _countof(szBuffer), NULL);
    fflush(stdout);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), pszLanguageDescription, _tcslen(pszLanguageDescription), &nWritten, NULL);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), _T(": "), 2, &nWritten, NULL);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), szBuffer, _tcslen(szBuffer), &nWritten, NULL);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), _T("\r\n"), 2, &nWritten, NULL);
#endif
}

#define FormatMessageTestAux(dwMessageId,wLanguageId) FormatMessageTestAuxF (dwMessageId, wLanguageId, _T (#wLanguageId))

//
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


void FormatMessageTest(void)
{
    FormatMessageTestAux(0, 0);
    FormatMessageTestAux(0, GetUserDefaultLangID());
    FormatMessageTestAux(0, GetSystemDefaultLangID());
    FormatMessageTestAux(0, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
    FormatMessageTestAux(0, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
    FormatMessageTestAux(0, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT));
    FormatMessageTestAux(0, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
#ifdef SUBLANG_JAPANESE_JAPAN
    FormatMessageTestAux(0, MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN));
#endif
    FormatMessageTestAux(0, MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
    FormatMessageTestAux(0, LANG_SYSTEM_DEFAULT);
    FormatMessageTestAux(0, LANG_USER_DEFAULT);
}

void FormatMessageTest1(void)
{
    TCHAR szBuffer[2048];
    LPTCH pch;
    int i;

    FormatWin32MessageTst(NULL, 0, GetSystemDefaultLangID(), szBuffer, _countof(szBuffer), NULL);

    for (i = 0, pch = szBuffer; *pch != 0; pch++, i++)
    {
        if (IsDBCSLeadByte((BYTE)*pch))
        {
            _tprintf(_T("idx = %3d pos = %3d chr = %02X <= IsDBCSLeadByte\n"), i, pch - szBuffer, (BYTE)*pch);

            if (*++pch == 0)
                break;
        }

        _tprintf(_T("idx = %3d pos = %3d chr = %02X\n"), i, pch - szBuffer, (BYTE)*pch);
    }
}

// IsDBCSLeadByteEx: 932, 936, 949, 950, and 1361.

/***
*CPtoLCID() - Code page to LCID.
*
*Purpose:
*       Some API calls want an LCID, so convert MB CP to appropriate LCID,
*       and then API converts back to ANSI CP for that LCID.
*
*Entry:
*   codepage - code page to convert
*Exit:
*       returns appropriate LCID
*
*Exceptions:
*
*******************************************************************************/

static int CPtoLCID(int codepage)
{
    switch (codepage) {
    case 932:
        return MAKELCID(MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT),
            SORT_DEFAULT);
    case 936:
        return MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED),
            SORT_DEFAULT);
    case 949:
        return MAKELCID(MAKELANGID(LANG_KOREAN, SUBLANG_DEFAULT),
            SORT_DEFAULT);
    case 950:
        return MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL),
            SORT_DEFAULT);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintCrtLocaleInfo(void)
{
    struct lconv* plc;
    char* gp;

    _tprintf(_T("LC_ALL = [%s]\n"), _tsetlocale(LC_ALL, NULL));
    _tprintf(_T("LC_COLLATE = [%s]\n"), _tsetlocale(LC_COLLATE, NULL));
    _tprintf(_T("LC_CTYPE = [%s]\n"), _tsetlocale(LC_CTYPE, NULL));
    _tprintf(_T("LC_MONETARY = [%s]\n"), _tsetlocale(LC_MONETARY, NULL));
    _tprintf(_T("LC_NUMERIC = [%s]\n"), _tsetlocale(LC_NUMERIC, NULL));
    _tprintf(_T("LC_TIME = [%s]\n"), _tsetlocale(LC_TIME, NULL));
    return;
    plc = localeconv();

    // Decimal-point character for nonmonetary quantities.

    _tprintf(_T("decimal_point = [%hs] Decimal-point character for nonmonetary quantities\n"), plc->decimal_point);

    // Character that separates groups of digits to left of decimal point for nonmonetary quantities.

    _tprintf(_T("thousands_sep = [%hs] Character that separates groups of digits to left of decimal point for nonmonetary quantities\n"), plc->thousands_sep);

    // Size of each group of digits in nonmonetary quantities.

    for (_tprintf(_T("grouping = [%d"), *(gp = plc->grouping)); *gp != 0 && *gp != CHAR_MAX; _tprintf(_T(",%d"), *++gp));

    _tprintf(_T("] Size of each group of digits in nonmonetary quantities\n"));

    // International currency symbol for current locale.

    _tprintf(_T("int_curr_symbol = [%hs] International currency symbol for current locale\n"), plc->int_curr_symbol);

    // Local currency symbol for current locale.

    _tprintf(_T("currency_symbol = [%hs] Local currency symbol for current locale\n"), plc->currency_symbol);

    // Decimal-point character for monetary quantities.

    _tprintf(_T("mon_decimal_point = [%hs] Decimal-point character for monetary quantities\n"), plc->mon_decimal_point);

    // Separator for groups of digits to left of decimal place in monetary quantities.

    _tprintf(_T("mon_thousands_sep = [%hs] Separator for groups of digits to left of decimal place in monetary quantities\n"), plc->mon_thousands_sep);

    // Size of each group of digits in monetary quantities.

    for (_tprintf(_T("mon_grouping = [%d"), *(gp = plc->mon_grouping)); *gp != 0 && *gp != CHAR_MAX; _tprintf(_T(",%d"), *++gp));

    _tprintf(_T("] Size of each group of digits in monetary quantities\n"));

    // String denoting sign for nonnegative monetary quantities.

    _tprintf(_T("positive_sign = [%hs] String denoting sign for nonnegative monetary quantities\n"), plc->positive_sign);

    // String denoting sign for negative monetary quantities.

    _tprintf(_T("negative_sign = [%hs] String denoting sign for negative monetary quantities\n"), plc->negative_sign);

    // Number of digits to right of decimal point in internationally formatted monetary quantities.

    _tprintf(_T("int_frac_digits = [%d] Number of digits to right of decimal point in internationally formatted monetary quantities\n"), plc->int_frac_digits);

    // Number of digits to right of decimal point in formatted monetary quantities.

    _tprintf(_T("frac_digits = [%d] Number of digits to right of decimal point in formatted monetary quantities\n"), plc->frac_digits);

    // Currency symbol placement for nonnegative formatted monetary quantity.

    _tprintf(_T("p_cs_precedes = [%d] "), plc->p_cs_precedes);

    switch (plc->p_cs_precedes)
    {
    case 0:
        _tprintf(_T("Currency symbol follows value for nonnegative formatted monetary quantity\n")); break;
    case 1:
        _tprintf(_T("Currency symbol precedes value for nonnegative formatted monetary quantity\n")); break;
    case CHAR_MAX:
        _tprintf(_T("Currency symbol placement for nonnegative formatted monetary quantity is not supported\n")); break;
    default:
        _tprintf(_T("Currency symbol placement for nonnegative formatted monetary quantity is not recognized\n")); break;
    }

    // Currency symbol placement for negative formatted monetary quantity.

    _tprintf(_T("n_cs_precedes = [%d] "), plc->n_cs_precedes);

    switch (plc->n_cs_precedes)
    {
    case 0:
        _tprintf(_T("Currency symbol follows value for negative formatted monetary quantity\n")); break;
    case 1:
        _tprintf(_T("Currency symbol precedes value for negative formatted monetary quantity\n")); break;
    case CHAR_MAX:
        _tprintf(_T("Currency symbol placement for negative formatted monetary quantity is not supported\n")); break;
    default:
        _tprintf(_T("Currency symbol placement for negative formatted monetary quantity is not recognized\n")); break;
    }

    // Currency symbol separation from value for nonnegative formatted monetary quantity.

    _tprintf(_T("p_sep_by_space = [%d] "), plc->p_sep_by_space);

    switch (plc->p_sep_by_space)
    {
    case 0:
        _tprintf(_T("There is no space separation between currency symbol and value for nonnegative formatted monetary quantity\n")); break;
    case 1:
        _tprintf(_T("Currency symbol is separated from value by space for nonnegative formatted monetary quantity\n")); break;
    case CHAR_MAX:
        _tprintf(_T("Currency symbol separation from value for nonnegative formatted monetary quantity is not supported\n")); break;
    default:
        _tprintf(_T("Currency symbol separation from value for nonnegative formatted monetary quantity is not recognized\n")); break;
    }

    // Currency symbol separation from value for negative formatted monetary quantity.

    _tprintf(_T("n_sep_by_space = [%d] "), plc->n_sep_by_space);

    switch (plc->n_sep_by_space)
    {
    case 0:
        _tprintf(_T("There is no space separation between currency symbol and value for negative formatted monetary quantity\n")); break;
    case 1:
        _tprintf(_T("Currency symbol is separated from value by space for negative formatted monetary quantity\n")); break;
    case CHAR_MAX:
        _tprintf(_T("Currency symbol separation from value for negative formatted monetary quantity is not supported\n")); break;
    default:
        _tprintf(_T("Currency symbol separation from value for negative formatted monetary quantity is not recognized\n")); break;
    }

    // Position of sign for nonnegative formatted monetary quantity.

    _tprintf(_T("p_sign_posn = [%d] "), plc->p_sign_posn);

    switch (plc->p_sign_posn)
    {
    case 0:
        _tprintf(_T("Parentheses surround value and currency symbol for nonnegative formatted monetary quantity\n")); break;
    case 1:
        _tprintf(_T("Sign string precedes value and currency symbol for nonnegative formatted monetary quantity\n")); break;
    case 2:
        _tprintf(_T("Sign string follows value and currency symbol for nonnegative formatted monetary quantity\n")); break;
    case 3:
        _tprintf(_T("Sign string immediately precedes currency symbol for nonnegative formatted monetary quantity\n")); break;
    case 4:
        _tprintf(_T("Sign string immediately follows currency symbol for nonnegative formatted monetary quantity\n")); break;
    case CHAR_MAX:
        _tprintf(_T("Position of sign for nonnegative formatted monetary quantity is not supported\n")); break;
    default:
        _tprintf(_T("Position of sign for nonnegative formatted monetary quantity is not recognized\n")); break;
    }

    // Position of sign for negative formatted monetary quantity.

    _tprintf(_T("n_sign_posn = [%d] "), plc->n_sign_posn);

    switch (plc->n_sign_posn)
    {
    case 0:
        _tprintf(_T("Parentheses surround value and currency symbol for negative formatted monetary quantity\n")); break;
    case 1:
        _tprintf(_T("Sign string precedes value and currency symbol for negative formatted monetary quantity\n")); break;
    case 2:
        _tprintf(_T("Sign string follows value and currency symbol for negative formatted monetary quantity\n")); break;
    case 3:
        _tprintf(_T("Sign string immediately precedes currency symbol for negative formatted monetary quantity\n")); break;
    case 4:
        _tprintf(_T("Sign string immediately follows currency symbol for negative formatted monetary quantity\n")); break;
    case CHAR_MAX:
        _tprintf(_T("Position of sign for negative formatted monetary quantity is not supported\n")); break;
    default:
        _tprintf(_T("Position of sign for negative formatted monetary quantity is not recognized\n")); break;
    }

#ifdef _ENABLE_PER_THREAD_LOCALE

    // Decimal-point character for nonmonetary quantities.

    _tprintf(_T("_W_decimal_point = [%ls] Decimal-point character for nonmonetary quantities (UNICODE)\n"), plc->_W_decimal_point);

    // Character that separates groups of digits to left of decimal point for nonmonetary quantities.

    _tprintf(_T("_W_thousands_sep = [%ls] Character that separates groups of digits to left of decimal point for nonmonetary quantities (UNICODE)\n"), plc->_W_thousands_sep);

    // International currency symbol for current locale.

    _tprintf(_T("_W_int_curr_symbol = [%ls] International currency symbol for current locale (UNICODE)\n"), plc->_W_int_curr_symbol);

    // Local currency symbol for current locale.

    _tprintf(_T("_W_currency_symbol = [%ls] Local currency symbol for current locale (UNICODE)\n"), plc->_W_currency_symbol);

    // Decimal-point character for monetary quantities.

    _tprintf(_T("_W_mon_decimal_point = [%ls] Decimal-point character for monetary quantities (UNICODE)\n"), plc->_W_mon_decimal_point);

    // Separator for groups of digits to left of decimal place in monetary quantities.

    _tprintf(_T("_W_mon_thousands_sep = [%ls] Separator for groups of digits to left of decimal place in monetary quantities (UNICODE)\n"), plc->_W_mon_thousands_sep);

    // String denoting sign for nonnegative monetary quantities.

    _tprintf(_T("_W_positive_sign = [%ls] String denoting sign for nonnegative monetary quantities (UNICODE)\n"), plc->_W_positive_sign);

    // String denoting sign for negative monetary quantities.

    _tprintf(_T("_W_negative_sign = [%ls] String denoting sign for negative monetary quantities (UNICODE)\n"), plc->_W_negative_sign);

#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// https://msdn.microsoft.com/en-us/goglobal/bb688115.aspx
//
// In Windows NT 3.5x, and Windows NT 4, because of the absence of a relevant API and consistent registry entries, the
// safest way to check the language of the operating system is to look at the version stamp of Ntdll.dll. The language
// of this DLL is the same as the language of the UI. The only exceptions to this approach are with Arabic, Hebrew, or
// Thai versions of Windows NT 4, where version stamping can help you detect an operating system enabled to support
// these languages.

#define US_LANG_ID  MAKELANGID (LANG_ENGLISH, SUBLANG_ENGLISH_US)

// In our resource enumeration, we only keep non-English-language stampings.

BOOL CALLBACK EnumResLangProc(HMODULE hModule, LPCTSTR pszType, LPCTSTR pszName, WORD wLanguageId, LONG_PTR lParam)
{
    LANGID* wLanguageArray = (LANGID*)lParam;

    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(pszType);
    UNREFERENCED_PARAMETER(pszName);

    _tprintf(_T("EnumResLangProc: %d 0x%04X\n"), wLanguageId, wLanguageId);

    if (wLanguageId == US_LANG_ID)
        wLanguageArray[0] = wLanguageId;
    else
        wLanguageArray[1] = wLanguageId;

    return TRUE;
}

LANGID MySystemDefaultUILanguageWinNT(void)
{
    LANGID wLanguageId = 0;
    LANGID wLanguageArray[2] = { 0, 0 };
    HMODULE hNtdll;

    hNtdll = GetModuleHandle(_T("Ntdll.dll"));

    if (hNtdll != NULL)
    {
        // For East Asian countries, Ntdll.dll contains both English and localized resources. Therefore, only take into
        // consideration the information for the non-English version.

        if (EnumResourceLanguages(hNtdll, RT_VERSION, MAKEINTRESOURCE(1), EnumResLangProc, (LONG_PTR)wLanguageArray))
        {
            if (wLanguageArray[1] != 0)
            {
                wLanguageId = wLanguageArray[1];
            }
            else
            {
                // If you only have English version stamping, you might still be dealing with an enabled language (Arabic,
                // Hebrew, or Thai). Only true for Windows NT 4.

                if (wLanguageArray[0] == US_LANG_ID)
                {
                    UINT uiACP = GetACP();

                    switch (uiACP)
                    {
                    case 874:
                        // Thai code page activated; this is a Thai-enabled system.
                        wLanguageId = MAKELANGID(LANG_THAI, SUBLANG_DEFAULT);
                        break;

                    case 1255:
                        // Hebrew code page; this is a Hebrew-enabled system.
                        wLanguageId = MAKELANGID(LANG_HEBREW, SUBLANG_DEFAULT);
                        break;

                    case 1256:
                        // Arabic code page; this is an Arabic-enabled system.
                        wLanguageId = MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_SAUDI_ARABIA);
                        break;

                    default:
                        wLanguageId = US_LANG_ID;
                    }
                }
            }
        }
        else
            PrintWin32Message(GetLastError(), _T("EnumResourceLanguages"));
    }
    else
        PrintWin32Message(GetLastError(), _T("GetModuleHandle"));

    return wLanguageId;
}

// In Windows 95, Windows 98, and Windows 98 Second Edition, the UI language is stored in the registry at
// HKCU\Control Panel\Desktop\ResourceLocale. This key will return the language ID (LANGID) of the UI in hexadecimal
// (for example, 00000409 for English).

LANGID MySystemDefaultUILanguageWin9x(void)
{
    LANGID wLanguageId = 0;
    LONG lStatus;
    HKEY hKey;
    DWORD dwType;
    LPTSTR pchEndPtr;
    BYTE abData[32];
    DWORD cbData;

    lStatus = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Control Panel\\Desktop\\ResourceLocale"), 0, KEY_QUERY_VALUE, &hKey);

    if (lStatus == ERROR_SUCCESS)
    {
        dwType = REG_NONE;
        cbData = sizeof(abData) - sizeof(TCHAR);
        lStatus = RegQueryValueEx(hKey, _T(""), NULL, &dwType, abData, &cbData);

        if (lStatus == ERROR_SUCCESS)
        {
            if (dwType == REG_SZ)
            {
                *((LPTCH)&abData[cbData]) = _T('\0');
                wLanguageId = (LANGID)_tcstoul((LPTSTR)abData, &pchEndPtr, 16);
            }
            else
                PrintWin32Message((DWORD)REGDB_E_INVALIDVALUE, _T("RegQueryValueEx"));
        }
        else
            PrintWin32Message(lStatus, _T("RegQueryValueEx"));

        if ((lStatus = RegCloseKey(hKey)) != ERROR_SUCCESS)
            PrintWin32Message(lStatus, _T("RegCloseKey"));
    }
    else
        PrintWin32Message(lStatus, _T("RegOpenKeyEx"));

    return wLanguageId;
}

typedef LANGID(WINAPI* PFNGETUSERDEFAULTUILANGUAGE)(void);

LANGID MySystemDefaultUILanguage(void)
{
    HMODULE hKernel32;
    PFNGETUSERDEFAULTUILANGUAGE pfnGetUserDefaultUILanguage;
    LANGID wLanguageId = 0;

    hKernel32 = GetModuleHandle(_T("KERNEL32.DLL"));

    if (hKernel32 != NULL)
    {
        pfnGetUserDefaultUILanguage = (PFNGETUSERDEFAULTUILANGUAGE)GetProcAddress(hKernel32, "GetUserDefaultUILanguage");

        if (pfnGetUserDefaultUILanguage != NULL)
            wLanguageId = pfnGetUserDefaultUILanguage();
        else
            if (IsWindowsNT())
                wLanguageId = MySystemDefaultUILanguageWinNT();
            else
                wLanguageId = MySystemDefaultUILanguageWin9x();
    }
    else
        PrintWin32Message(GetLastError(), _T("GetModuleHandle"));

    return wLanguageId;
}

void PrintSystemDefaultUILanguage(void)
{
    LANGID nSystemDefaultUILanguage = MySystemDefaultUILanguage();
    _tprintf(_T("SystemDefaultUILanguage = %u 0x%04X\n"), nSystemDefaultUILanguage, nSystemDefaultUILanguage);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSet1(int argc, LPTSTR argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    _tprintf(_T("//////////////////////////////////////////////////////////////////////////////////////////\n"));

    //_tsetlocale (LC_ALL, _T (""));
    // ptrdiff_t
    // OEM code page 932 is used for Japanese Kanji.
    //_setmbcp (_MB_CP_ANSI);
    //printf("%d\n", getmbcp ());
    //setmbcp ();
#ifdef ERROR_DRIVERS_LEAKING_LOCKED_PAGES
    //PrintWin32Message (ERROR_DRIVERS_LEAKING_LOCKED_PAGES, NULL);
#else
    //PrintWin32Message (729, NULL);
#endif
    PrintUILanguage();
    PrintLocaleId();
    //TestStringScan ();
    //LANGIDFROMLCID ()
    //PrintLocale1 ();
    //FormatMessageTest ();
    // FormatMessageTest1 ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  ANSI APPLICATION
//
//  https://msdn.microsoft.com/en-us/library/windows/desktop/dd319088%28v=vs.85%29.aspx
//
//  Language for Non-Unicode Programs
//
//      Summary
//
//      Determines which bitmap fonts, and OEM, ANSI, and Macintosh code pages are defaults for the operating system. This language
//      only affects applications that are not fully Unicode. Prior to Windows XP, this language was called "system locale".
//
//      Description
//
//      The language for non-Unicode programs (formerly called "system locale") determines the code page that is used on the
//      operating system by default. The language for non-Unicode programs setting affects only non-Unicode applications, that is,
//      ANSI applications. Setting the language instructs Windows to emulate a non-Unicode-based operating system, localized to this
//      language. Changing the language for non-Unicode programs installs the necessary bitmap font files to support non-Unicode
//      applications in the specified language. To allow the user to select a language for non-Unicode programs, the appropriate
//      language group must be installed. Your application needs the script support to select a language for non-Unicode programs.
//      The language for non-Unicode programs is a per-system setting and requires a restart to be implemented.
//
//      Sometimes there is no noticeable difference between two languages for non-Unicode programs. For example, this is the case
//      with the German (Neutral) and German (Austria) locales. In general, the settings of one language group are very similar and
//      differ only in the OEM or MAC code page.
//
//      An ANSI application should check the language for non-Unicode programs setting during installation. It uses GetACP or
//      GetOEMCP to retrieve the value. No function is supported to set the language for non-Unicode programs. However, users can
//      change it by using the Advanced tab in the regional and language options portion of the Control Panel. The following are
//      some examples of language for non-Unicode programs settings:
//
//      1.  A German user who wants to run a Japanese application that was designed for Japanese Windows 95 must select Japanese as
//          the language for non-Unicode programs. After this selection, non-Unicode German applications have problems. For example,
//          German umlauts (¨) are not displayed correctly.
//
//      2.  A German user who wants to type Japanese text in a non-Unicode German application must select Japanese as the language
//          for non-Unicode programs. As in the first example, this causes problems in entering German text in non-Unicode
//          applications.
//
//      3.  An Arabic user who wants to type Arabic, French, and English in a non-Unicode Arabic application should select Arabic as
//          a language for non-Unicode programs, because the Arabic ANSI code page contains most French characters and all English
//          characters.
//
//      SetThreadLocale (LOCALE_SYSTEM_DEFAULT)
//      _tsetlocale (LC_ALL, _T (""));
//
//      The system locale (that determines the ANSI and OEM code pages)
//          GetSystemDefaultLocaleName -> string
//          GetSystemDefaultLCID -> LCID
//          GetSystemDefaultLangID -> LANGID

//  UNICODE APPLICATION
//
//  https://msdn.microsoft.com/en-us/library/windows/desktop/dd319088%28v=vs.85%29.aspx
//
//  Standards and Formats
//
//      Summary
//
//      Determines which settings are used for formatting dates, times, currency, and numbers as a default for each user. Also
//      determines the sort order for sorting text. Prior to Windows XP, Standards and Formats was called "user locale".
//
//      Description
//
//      Standards and Formats (formerly "user locale") is a per-user variable that determines default sort order and the default
//      settings for formatting dates, times, currency, and numbers. The variable is presented as a language (sometimes in
//      combination with a country/region), but it is not a language itself. For example, setting the Standards and Formats variable
//      to Hebrew indicates that the user wants to use the formatting conventions of Hebrew, not necessarily the Hebrew language. In
//      addition, the Standards and Formats variable determines the string that is used for the names of days and months. For
//      example, if a user displays "November 25, 1998", the "November" string can change based on the Standards and Formats
//      variable. Changing the variable automatically adds an input locale with the default settings for the language.
//
//      To get the Standards and Formats variable setting, the application calls GetUserDefaultLCID or GetUserDefaultLocaleName. No
//      NLS function is available to set the variable. However, users can change it through the Region Options tab in the regional
//      and language options portion of the Control Panel.
//
//      An application should typically use the Standards and Formats variable settings to display data. However, an application
//      that uses a fixed locale to display data should pass a specific locale identifier instead of using LOCALE_USER_DEFAULT.
//
//      SetThreadLocale (LOCALE_USER_DEFAULT);
//      _tsetlocale (LC_ALL, _T (""));
//
//      The locale used for locale-sensitive operations (number/currency/date/time formatting, sorting, case conversion)
//          GetUserDefaultLocaleName -> string
//          GetUserDefaultLCID -> LCID
//          GetUserDefaultLangID -> LANGID

void TestSet2(int argc, LPTSTR argv[])
{
    TCHAR szLocale[128];
    LPCTSTR pszResult;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    _tprintf(_T("//////////////////////////////////////////////////////////////////////////////////////////\n"));

    if (IsWindowsNT())
    {
        if (!SetThreadLocale(LOCALE_SYSTEM_DEFAULT))
        {
            PrintWin32Message(GetLastError(), _T("SetThreadLocale"));
            return;
        }
    }

#if WINVER >= _WIN32_WINNT_VISTA
    {
        TCHAR szLanguage[64];
        TCHAR szCountry[64];
        TCHAR szCodePage[16];

        if (GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SENGLANGUAGE, szLanguage, _countof(szLanguage)) == 0)
        {
            PrintWin32Message(GetLastError(), _T("GetLocaleInfo.LOCALE_SENGLANGUAGE"));
            return;
        }

        if (GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SENGCOUNTRY, szCountry, _countof(szCountry)) == 0)
        {
            PrintWin32Message(GetLastError(), _T("GetLocaleInfo.LOCALE_SENGCOUNTRY"));
            return;
        }

        if (GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szCodePage, _countof(szCodePage)) == 0)
        {
            PrintWin32Message(GetLastError(), _T("GetLocaleInfo.LOCALE_IDEFAULTANSICODEPAGE"));
            return;
        }

        _sntprintf_s(szLocale, _countof(szLocale), _countof(szLocale), _T("%s_%s.%s"), szLanguage, szCountry, szCodePage);
    }
#else
    _sntprintf_s(szLocale, _countof(szLocale), _countof(szLocale), _T(""));
#endif

    _tprintf(_T("Locale = [%s]\n"), szLocale);

    pszResult = _tsetlocale(LC_ALL, szLocale);

    if (pszResult == NULL)
    {
        PrintErrorMessage(_T("%s: Invalid locale name.\n"), szLocale);
        return;
    }

    PrintUILanguage();
    PrintLocaleId();
    PrintCrtLocaleInfo();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSet3(int argc, LPTSTR argv[])
{
    TCHAR szLocale[128];
    LPCTSTR pszResult;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    _tprintf(_T("//////////////////////////////////////////////////////////////////////////////////////////\n"));

    if (IsWindowsNT())
    {
        if (!SetThreadLocale(LOCALE_SYSTEM_DEFAULT))
        {
            PrintWin32Message(GetLastError(), _T("SetThreadLocale"));
            return;
        }
    }

#if WINVER >= _WIN32_WINNT_VISTA
    {
        TCHAR szLanguage[64];
        TCHAR szCountry[64];
        TCHAR szCodePage[16];

        if (GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SENGLANGUAGE, szLanguage, _countof(szLanguage)) == 0)
        {
            PrintWin32Message(GetLastError(), _T("GetLocaleInfo.LOCALE_SENGLANGUAGE"));
            return;
        }

        if (GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SENGCOUNTRY, szCountry, _countof(szCountry)) == 0)
        {
            PrintWin32Message(GetLastError(), _T("GetLocaleInfo.LOCALE_SENGCOUNTRY"));
            return;
        }

        if (GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szCodePage, _countof(szCodePage)) == 0)
        {
            PrintWin32Message(GetLastError(), _T("GetLocaleInfo.LOCALE_IDEFAULTANSICODEPAGE"));
            return;
        }

        _sntprintf_s(szLocale, _countof(szLocale), _countof(szLocale), _T("%s_%s.%s"), szLanguage, szCountry, szCodePage);
    }
#else
    _sntprintf_s(szLocale, _countof(szLocale), _countof(szLocale), _T(""));
#endif

    _tprintf(_T("Locale = [%s]\n"), szLocale);

    pszResult = _tsetlocale(LC_ALL, szLocale);

    if (pszResult == NULL)
    {
        PrintErrorMessage(_T("%s: Invalid locale name.\n"), szLocale);
        return;
    }

    FormatMessageTest();
    FormatMessageTest1();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSet4(int argc, LPTSTR argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    _tprintf(_T("//////////////////////////////////////////////////////////////////////////////////////////\n"));

    PrintSystemDefaultUILanguage();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TestSet5(int argc, LPTSTR argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    _tprintf(_T("//////////////////////////////////////////////////////////////////////////////////////////\n"));

    LOCALE_USER_DEFAULT;
    LCID lcid = GetThreadLocale();
    LANGID langid = LANGIDFROMLCID(lcid);
    _tprintf(_T("GetThreadLocale = %u 0x%04X LANGIDFROMLCID = %u 0x%04X\n"), lcid, lcid, langid, langid);

    WCHAR szLocaleName[LOCALE_NAME_MAX_LENGTH];

    if (GetUserDefaultLocaleName(szLocaleName, _countof(szLocaleName)) == 0)
    {
        PrintWin32Message(GetLastError(), _T("GetUserDefaultLocaleName"));
        return;
    }

    _tprintf(_T("GetUserDefaultLocaleName = %ls\n"), szLocaleName);

    lcid = MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), SORT_DEFAULT);
    int acp = GetAnsiCodePageForLocale(lcid);
    _tprintf(_T("acp = %d\n"), acp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, LPTSTR argv[])
{
    //TestSet1(argc, argv);
    //TestSet2(argc, argv);
    //TestSet3(argc, argv);
    //TestSet4(argc, argv);
    TestSet5(argc, argv);
    return 0;
}

/*
    A call to ConvertDefaultLocale(LOCALE_SYSTEM_DEFAULT) is equivalent to a call to GetSystemDefaultLCID.
    A call to ConvertDefaultLocale(LOCALE_USER_DEFAULT) is equivalent to a call to GetUserDefaultLCID.


            char *tmp = grouping;

            do
                *tmp = *(tmp+1);
            while (*++tmp);

            setlocale (, "C") => CP_ACP
            setlocale (, "") => LOCALE_USER_DEFAULT (GetUserDefaultLCID())


Windows 7
Instalacao - pt-BR
Opcoes Regionais - en-US
Programas nao-UNICODE - ja-JP

setlocale (, "") = en-US Opcoes Regionais

SystemDefaultLocale = 1041 0x0411 [ja-JP] Programas nao-UNICODE
UserDefaultLocale = 1033 0x0409 [en-US] Opcoes Regionais
SystemDefaultLanguage = 1041 0x0411 [ja-JP] Programas nao-UNICODE
SystemDefaultUILanguage = 1046 0x0416 [pt-BR] Instalacao
UserDefaultLanguage = 1033 0x0409 [en-US] Opcoes Regionais
ConsoleOutputCP = 932 0x03A4 [ja-JP] Programas nao-UNICODE
ANSI code page = 932 0x03A4 [ja-JP] Programas nao-UNICODE
OEM code page = 932 0x03A4 [ja-JP] Programas nao-UNICODE

ThreadLocale = 1041 0x0411 [ja-JP] Programas nao-UNICODE
Current thread's ANSI code page = 932 0x03A4 [ja-JP] Programas nao-UNICODE

**** SEM SetThreadLocale ****

Windows 7
Instalacao - pt-BR
Opcoes Regionais - en-US
Programas nao-UNICODE - ja-JP

setlocale (, "") = en-US Opcoes Regionais

SystemDefaultLocale = 1041 0x0411 [ja-JP] Programas nao-UNICODE
UserDefaultLocale = 1033 0x0409 [en-US] Opcoes Regionais
SystemDefaultLanguage = 1041 0x0411 [ja-JP] Programas nao-UNICODE
SystemDefaultUILanguage = 1046 0x0416 [pt-BR] Instalacao
UserDefaultLanguage = 1033 0x0409 [en-US] Opcoes Regionais
ConsoleOutputCP = 932 0x03A4 [ja-JP] Programas nao-UNICODE
ANSI code page = 932 0x03A4 [ja-JP] Programas nao-UNICODE
OEM code page = 932 0x03A4 [ja-JP] Programas nao-UNICODE

ThreadLocale = 1033 0x0409 [en-US] [en-US] Opcoes Regionais
Current thread's ANSI code page = 1252 0x04E4 [en-US] Opcoes Regionais

Windows Vista
Instalação - pt-BR
Opções Regionais - zh-SG
Programas não-UNICODE - en-US

setlocale (, "") = zh-SG Opções Regionais

SystemDefaultLocale = 1033 0x0409 [en-US] Programas não-UNICODE
UserDefaultLocale = 4100 0x1004 [zh-SG] Opções Regionais
SystemDefaultLanguage = 1033 0x0409 [en-US] Programas não-UNICODE
SystemDefaultUILanguage = 1046 0x0416 [pt-BR] Instalação
UserDefaultLanguage = 4100 0x1004 [zh-SG] Opções Regionais
ConsoleOutputCP = 437 0x01B5 [en-US] Programas não-UNICODE
ANSI code page = 1252 0x04E4 [en-US] Programas não-UNICODE
OEM code page = 437 0x01B5 [en-US] Programas não-UNICODE

ThreadLocale = 4100 0x1004 [zh-SG] Opções Regionais
Current thread's ANSI code page = 936 0x03A8 [zh-SG] Opções Regionais

Windows 98
Instalacao - pt-BR
Opcoes Regionais - en-US
Programas nao-UNICODE - N/A

setlocale (, "") = en-US Opcoes Regionais

SystemDefaultLocale = 1033 0x0409 [en-US] Opcoes Regionais (normalmente Programas nao-UNICODE)
UserDefaultLocale = 1033 0x0409 [en-US] Opcoes Regionais
SystemDefaultLanguage = 1033 0x0409 [en-US] Opcoes Regionais (normalmente Programas nao-UNICODE)
SystemDefaultUILanguage = N/A
UserDefaultLanguage = 1033 0x0409 [en-US] Opcoes Regionais
ConsoleOutputCP = 850 0x0352 [pt-BR] Estabelecido manualmente
ANSI code page = 1252 0x04E4 [en-US] Opcoes Regionais (normalmente Programas nao-UNICODE)
OEM code page = 850 0x0352 [pt-BR] Estabelecido manualmente (normalmente Programas nao-UNICODE)

ThreadLocale = 1033 0x0409 [en-US] Opcoes Regionais
Current thread's ANSI code page = N/A


http://www.siao2.com/2005/08/22/454360.aspx
Anyway, we document that developers should use LOCALE_USER_DEFAULT and not GetThreadLocale when
they are trying to respect the user’s preferences.

https://msdn.microsoft.com/en-us/goglobal/bb688115.aspx

In Windows 95, Windows 98, and Windows 98 Second Edition, the UI language is stored in the registry
at HKCU\Control Panel\Desktop\ResourceLocale. This key will return the language ID (LANGID) of the
UI in hexadecimal (for example, 00000409 for English).

https://www.globalyzer.com/gzserver/help/reference/localeSensitiveMethods/winlocalefunc.html

*/
