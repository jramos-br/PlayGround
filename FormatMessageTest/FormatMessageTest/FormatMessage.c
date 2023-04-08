#define WINVER          0x0400
#define _WIN32_WINNT    0x0400
#define _WIN32_WINDOWS  0x0400
#define _WIN32_IE       0x0600

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif

#include <Windows.h>
#include <tchar.h>

#define IsCharLineSeparator(ch) (ch == _T('\n') || ch == _T('\r'))

//UINT GetAnsiCodePageForLocale(LCID lcid)
//{
//    UINT acp;
//
//    int sizeInChars = sizeof(acp) / sizeof(TCHAR);
//
//    if (GetLocaleInfo(lcid, LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER, (LPTSTR)(&acp), sizeInChars) != sizeInChars)
//    {
//        PrintWin32Message(GetLastError(), _T("GetLocaleInfo"));
//        acp = 0;
//    }
//
//    return acp;
//}

// GetThreadLocale
// Returns the locale identifier of the current locale for the calling thread.
// Windows Vista: This function can return the identifier of a custom locale. If the
// current thread locale is a custom locale, the function returns LOCALE_CUSTOM_DEFAULT.
// If the current thread locale is a supplemental custom locale, the function can return
// LOCALE_CUSTOM_UNSPECIFIED. All supplemental locales share this locale identifier.

// GetUserDefaultLCID
// Returns the locale identifier for the user default locale, represented as
// LOCALE_USER_DEFAULT. If the user default locale is a custom locale, this function
// always returns LOCALE_CUSTOM_DEFAULT, regardless of the custom locale that is selected.

// GetUserDefaultLocaleName
// Retrieves the user default locale name.

// GetUserDefaultLangID
// Returns the language identifier of the Region Format setting for the current user.

// GetUserDefaultUILanguage
// Returns the language identifier for the user UI language for the current user. If the
// current user has not set a language, GetUserDefaultUILanguage returns the preferred
// language set for the system. If there is no preferred language set for the system, then
// the system default UI language (also known as "install language") is returned.

// GetSystemDefaultLCID
// Returns the locale identifier for the system default locale, identified by
// LOCALE_SYSTEM_DEFAULT.

// GetSystemDefaultLocaleName
// Retrieves the system default locale name.

// GetSystemDefaultLangID
// Returns the language identifier for the system locale. This is the language used when
// displaying text in programs that do not support Unicode.

//static LPTSTR FormatWin32MessageEx(DWORD dwMessageId, DWORD dwLanguageId, LPTSTR lpszBuffer, size_t cchBuffer)
//{
    // #define LANG_SYSTEM_DEFAULT    (MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT))
    // #define LANG_USER_DEFAULT      (MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
    // #define LOCALE_SYSTEM_DEFAULT  (MAKELCID(LANG_SYSTEM_DEFAULT, SORT_DEFAULT))
    // #define LOCALE_USER_DEFAULT    (MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT))

    //static const TCHAR szDefaultWin32Message[] = _T("Win32 Message ID 0x%08I32X (%I32d)");
    //static const DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

    //HRESULT hr;
    //LPTSTR lpszMessage = NULL;
    //DWORD dwCount = FormatMessage(dwFlags, NULL, dwMessageId, dwLanguageId, (LPTSTR)&lpszMessage, 0, NULL);

    //if (lpszMessage != NULL)
    //{
    //    if (dwCount > 0)
    //        hr = StringCchPrintf(lpszBuffer, cchBuffer, _T("%s (0x%08I32X, %I32d)"), lpszMessage, dwMessageId, dwMessageId);
    //    else
    //        hr = StringCchPrintf(lpszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);

    //    LocalFree(lpszMessage);
    //}
    //else
    //    hr = StringCchPrintf(lpszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);

//    return lpszBuffer;
//}


//LPTSTR FormatWin32Message(DWORD dwMessageId, LPTSTR lpszBuffer, size_t cchBuffer)
//{
//    static const TCHAR szDefaultWin32Message[] = _T("Win32 Message ID 0x%08I32X (%I32d)");
//    static const DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
//
//    HRESULT hr;
//    DWORD dwLanguageId = 0;
//    LPTSTR lpszMessage = NULL;
//    DWORD dwCount = FormatMessage(dwFlags, NULL, dwMessageId, dwLanguageId, (LPTSTR)&lpszMessage, 0, NULL);
//
//    if (lpszMessage != NULL)
//    {
//        //LPTSTR p =
//        //    while (dwCount > 0)
//        //    {
//        //        --dwCount;
//        //    }
//
//
//        //// GetThreadLocale
//
//        //while (((c = *CharPrev(locbuffer, locbuffer + count)) == _T('\r')) ||
//        //    (c == _T('\n'))) {
//        //    count--;
//        //    back++;
//        //}
//        //if (back) {
//        //    locbuffer[count++] = _T('\n');
//        //    locbuffer[count] = _T('\0');
//        //}
//
//        //StringCchLength
//
//
//
//        //    while (dwCount > 0 && IsCharLineSeparator(lpszMessage[dwCount - 1]))
//        //    {
//        //        lpszMessage[--dwCount] = _T('\0');
//        //    }
//
//        //if (dwCount > 0)
//        //    hr = StringCchPrintf(lpszBuffer, cchBuffer, _T("%s (0x%08I32X, %I32d)"), lpszMessage, dwMessageId, dwMessageId);
//        //else
//        //    hr = StringCchPrintf(lpszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);
//
//        //LocalFree(lpszMessage);
//    }
//    else
//        hr = StringCchPrintf(lpszBuffer, cchBuffer, szDefaultWin32Message, dwMessageId, dwMessageId);
//
//    return lpszBuffer;
//}
