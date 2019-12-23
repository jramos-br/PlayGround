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

#ifndef _WIN32FORMATMESSAGE_H_AA0177F9_E3BA_4A6E_A491_E2769EAE4734_
#define _WIN32FORMATMESSAGE_H_AA0177F9_E3BA_4A6E_A491_E2769EAE4734_

#ifdef _MSC_VER
#pragma once
#endif

#include <Windows.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////////////////////////
//
// Value for default formatting option and default language selection.
#define FW32M_DEFAULT                                                   0

//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//
// The formatting options used with the FormatWin32Message* functions. The values are a
// bitmask and can be combined.

// Appends the numeric value of the message ID to the formatted message.
#define FW32M_FLAG_FORMAT_ID                                            0x0100

// Returns NULL if a message cannot be retrieved for the specified message ID. In this
// case a standard error message will be formatted in the specified output buffer.
#define FW32M_FLAG_NULL_ON_ERROR                                        0x0200

// Uses the FormatMessage function default language search order to find a fallback
// language, if the message cannot be retrieved with the specified language.
#define FW32M_FLAG_FALLBACK_TO_DEFAULT                                  0x0400

// Uses the preferred UI languages list as a fallback, if the message cannot be
// retrieved with the specified language.
#define FW32M_FLAG_FALLBACK_TO_PREFERRED                                0x0800

// Searches the system message table if the message is not found in the module
// specified to the FormatWin32ModuleMessage* functions. Not used by the
// FormatWin32Message* functions.
#define FW32M_FLAG_MESSAGE_FROM_SYSTEM                                  0x1000

// Mask of the language selection field (internal use).
#define FW32M_LANGUAGE_MASK                                             0x00FF

//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//
// The language of the messages retrieved by FormatWin32Message* functions. Some options
// define a single language and others define a language list to be searched. Pick one of
// them and combine with the formatting flags.

// Uses the FormatMessage function default language search order: language neutral;
// thread LANGID, based on the thread's locale value; user default LANGID, based on
// the user's default locale value; system default LANGID, based on the system default
// locale value; system default UI language (also known as "install language").
#define FW32M_DEFAULT_LANGUAGES                                         0

// Uses a composite list consisting of the thread preferred UI languages, process
// preferred UI languages, user preferred UI languages, system preferred UI languages,
// and the system default UI language, along with associated fallback and neutral
// languages.
#define FW32M_PREFERRED_UI_LANGUAGES                                    1

// Uses the language identifier of the Region Format setting for the current user as
// set under Control Panel > Clock, Language, and Region > Change date, time, or
// number formats > Formats tab > Format dropdown.
#define FW32M_USER_DEFAULT_LANGID                                       2

// Uses the language identifier for the user UI language for the current user. If the
// current user has not set a language, uses the preferred language set for the
// system. If there is no preferred language set for the system, then uses the system
// default UI language (also known as "install language").
#define FW32M_USER_DEFAULT_UI_LANGUAGE                                  3

// Uses the user preferred UI languages. The user preferred UI languages list includes
// languages that the user prefers.
#define FW32M_USER_PREFERRED_UI_LANGUAGES                               4

// Uses the language identifier for the system locale. This is the language used when
// displaying text in programs that do not support Unicode. It is set by the
// Administrator under Control Panel > Clock, Language, and Region > Change date,
// time, or number formats > Administrative tab.
#define FW32M_SYSTEM_DEFAULT_LANGID                                     5

// Uses the language identifier for the system default UI language of the operating
// system, also known as the "install language" on Windows Vista and later.
#define FW32M_SYSTEM_DEFAULT_UI_LANGUAGE                                6

// Uses the system preferred UI languages. The system preferred UI languages list is
// made up of the system default UI language and the system UI language and their
// fallbacks.
#define FW32M_SYSTEM_PREFERRED_UI_LANGUAGES                             7

// Uses the language identifier of the first user interface language for the current
// thread.
#define FW32M_THREAD_UI_LANGUAGE                                        8

// Uses a thread preferred UI languages list that consists of up to five valid
// languages set by a thread in a running process for a MUI application. These
// languages are used to customize the application user interface languages and make
// them different from the operating system language.
#define FW32M_THREAD_PREFERRED_UI_LANGUAGES                             9

// Uses a composite list consisting of the thread preferred UI languages, followed by
// process preferred UI languages, followed by any user preferred UI languages that
// are distinct from these, followed by the system default UI language, if it is not
// already in the list. If the user preferred UI languages list is empty, uses the
// system preferred UI languages.
#define FW32M_THREAD_PREFERRED_UI_LANGUAGES_MERGE_USER_FALLBACK         10

// Uses a composite list consisting of the thread preferred UI languages, followed by
// process preferred UI languages, followed by any user preferred UI languages that
// are distinct from these, followed by the system default UI language, if it is not
// already in the list, along with associated fallback and neutral languages. If the
// user preferred UI languages list is empty, uses the system preferred UI languages.
#define FW32M_THREAD_PREFERRED_UI_LANGUAGES_MERGE_SYSTEM_FALLBACK       11

// Uses a composite list consisting of the thread preferred UI languages, followed by
// process preferred UI languages, followed by any user preferred UI languages that
// are distinct from these, followed by the system default UI language, if it is not
// already in the list, along with associated fallback and neutral languages. If the
// user preferred UI languages list is empty, uses the system preferred UI languages.
#define FW32M_THREAD_PREFERRED_UI_LANGUAGES_UI_FALLBACK                 12

// Uses a process preferred UI languages list consisting of up to five valid languages
// set by a running process for a MUI application. These languages are used to
// customize the application user interface languages and make them different from the
// operating system language.
#define FW32M_PROCESS_PREFERRED_UI_LANGUAGES                            13

//
//////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

    // Formats the message corresponding to 'dwMessageId' using the FormatMessage function
    // default language search order.
    LPTSTR FormatWin32Message(DWORD dwMessageId, LPTSTR pszBuffer, UINT cchBuffer);

    // Formats the message corresponding to 'dwMessageId' using the language and the
    // formatting options in 'dwFlags'.
    LPTSTR FormatWin32MessageEx(DWORD dwMessageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer);

    // Formats the message corresponding to 'dwMessageId' using the language identified by
    // 'languageId' and the formatting options in 'dwFlags'.
    LPTSTR FormatWin32MessageExL(DWORD dwMessageId, LANGID languageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer);

    // Formats the message corresponding to 'dwMessageId' using the FormatMessage function
    // default language search order. The function searches the message table in the
    // module identified by 'hModule'.
    LPTSTR FormatWin32ModuleMessage(HMODULE hModule, DWORD dwMessageId, LPTSTR pszBuffer, UINT cchBuffer);

    // Formats the message corresponding to 'dwMessageId' using the language and the
    // formatting options in 'dwFlags'. The function searches the message table in the
    // module identified by 'hModule'.
    LPTSTR FormatWin32ModuleMessageEx(HMODULE hModule, DWORD dwMessageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer);

    // Formats the message corresponding to 'dwMessageId' using the language identified by
    // 'languageId' and the formatting options in 'dwFlags'. The function searches the
    // message table in the module identified by 'hModule'.
    LPTSTR FormatWin32ModuleMessageExL(HMODULE hModule, DWORD dwMessageId, LANGID languageId, DWORD dwFlags, LPTSTR pszBuffer, UINT cchBuffer);

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////

#endif
