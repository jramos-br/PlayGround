/*
 * Utility methods for Win32 programs.
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

#ifndef _RESOURCE_H_27FF2904_21B4_4AE8_A275_89467278E950_
#define _RESOURCE_H_27FF2904_21B4_4AE8_A275_89467278E950_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__GNUC__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#pragma once
#endif

#include "WinDefs.h"

#define IDI_APPICON 101
#define IDR_MAINMENU 102
#define IDR_ACCELERATOR 103
#define IDD_ABOUTDIALOG 104

#define IDM_FILE 1000
#define IDM_FILE_EXIT 1001
#define IDM_EDIT 2000
#define IDM_EDIT_UNDO 2001
#define IDM_EDIT_REDO 2002
#define IDM_EDIT_CUT 2003
#define IDM_EDIT_COPY 2004
#define IDM_EDIT_PASTE 2005
#define IDM_EDIT_DELETE 2006
#define IDM_HELP 9000
#define IDM_HELP_WELCOME 9001
#define IDM_HELP_ABOUT 9002

/* All predefined window menu items have identifier numbers greater than 0xF000.
   If an application adds commands to the window menu, it should use identifier
   numbers less than 0xF000. */

#define SCX_HELP_ABOUT 0xE000

#ifndef IDC_STATIC
#define IDC_STATIC -1
#endif

#endif