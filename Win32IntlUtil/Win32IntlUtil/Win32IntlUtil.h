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

#ifndef _WIN32INTLUTIL_H_C0917FD4_8765_4EEB_919D_48D26ED351FA_
#define _WIN32INTLUTIL_H_C0917FD4_8765_4EEB_919D_48D26ED351FA_

#ifdef _MSC_VER
#pragma once
#endif

#include <Windows.h>

// GetLocaleInfo
// Retrieves information about a locale specified by identifier.
// For interoperability reasons, the application should prefer the GetLocaleInfoEx
// function to GetLocaleInfo because Microsoft is migrating toward the use of locale
// names instead of locale identifiers for new locales. Any application that runs only
// on Windows Vista and later should use GetLocaleInfoEx.
// Win95
// int WINAPI GetLocaleInfo(LCID Locale, LCTYPE LCType, LPTSTR lpLCData, int cchData);

// GetLocaleInfoEx
// Retrieves information about a locale specified by name.
// The application should call this function in preference to GetLocaleInfo if designed to
// run only on Windows Vista and later.
// WinVista
// int WINAPI GetLocaleInfoEx(LPCWSTR lpLocaleName, LCTYPE LCType, LPWSTR lpLCData, int cchData);

// GetUserGeoID
// Retrieves information about the geographical location of the user. For more
// information, see Table of Geographical Locations.
// GetUserGeoID is available for use in the operating systems specified in the
// Requirements section. It may be altered or unavailable in subsequent versions. Instead,
// use GetUserDefaultGeoName.
// Win2000
// WINBASEAPI GEOID WINAPI GetUserGeoID(GEOCLASS GeoClass);


// GetUserDefaultGeoName
// Retrieves the two-letter International Organization for Standardization (ISO) 3166-1
// code or numeric United Nations (UN) Series M, Number 49 (M.49) code for the default
// geographical location of the user.
// Win10

WINBASEAPI int WINAPI GetUserDefaultGeoName(LPWSTR geoName, int geoNameCount);


// GetUserDefaultLCID
// Returns the locale identifier for the user default locale, represented as
// LOCALE_USER_DEFAULT. If the user default locale is a custom locale, this function
// always returns LOCALE_CUSTOM_DEFAULT, regardless of the custom locale that is selected.
// Applications that are intended to run only on Windows Vista and later should use
// GetUserDefaultLocaleName.
// Win95
// LCID WINAPI GetUserDefaultLCID(void);

// GetUserDefaultLocaleName
// Retrieves the user default locale name.
// The application should call this function in preference to GetUserDefaultLCID if
// designed to run only on Windows Vista and later.
// WinVista
// int WINAPI GetUserDefaultLocaleName(LPWSTR lpLocaleName, int cchLocaleName);









// DownlevelGetLocaleScripts
// Provides a list of scripts for the specified locale.
// This function is used only by applications that run on pre-Windows Vista operating
// systems. Its use requires the download package. Applications that only run on Windows
// Vista and later should call GetLocaleInfo with LCType set to LOCALE_SSCRIPTS.
//DownlevelGetLocaleScripts

// DownlevelLCIDToLocaleName
// Converts a locale identifier to a locale name.
// This function is used only by applications that run on pre-Windows Vista operating
// systems. Its use requires a download package. Applications that run only on Windows
// Vista and later should call LCIDToLocaleName to retrieve a locale name.
// This function does not support custom locales.

// nlsdl
// DownlevelGetParentLocaleLCID Nlsdl.h NlsMap.dll Microsoft NLS Downlevel Data Mapping APIs onWindows XPor Windows Vista
// DownlevelGetParentLocaleName
// DownlevelLCIDToLocaleName
// DownlevelLocaleNameToLCID

LCID WINAPI DownlevelGetParentLocaleLCID(LCID Locale);
int WINAPI DownlevelGetParentLocaleName(LCID Locale, LPWSTR lpName, int cchName);
int WINAPI DownlevelLCIDToLocaleName(LCID Locale, LPWSTR lpName, int cchName, DWORD dwFlags);
LCID WINAPI DownlevelLocaleNameToLCID(LPCWSTR lpName, DWORD dwFlags);



LCID __cdecl _AtlDownlevelLocaleNameToLCID(LPCWSTR localeName);
int __cdecl _AtlDownlevelLCIDToLocaleName(LCID lcid, LPWSTR outLocaleName, int cchLocaleName);

int DownlevelLCIDToLocaleName(
    _In_  LCID   Locale,
    _Out_ LPWSTR lpName,
    _In_  int    cchName,
    _In_  DWORD  dwFlags
);

#ifdef __cplusplus
extern "C" {
#endif

    BOOL GetLocaleName(LCID lcid, LPTSTR pszLocaleName, size_t cchLocaleName);
    BOOL GetLanguageName(LANGID langid, LPTSTR pszLanguageName, size_t cchLanguageName);

#ifdef __cplusplus
}
#endif

#endif
