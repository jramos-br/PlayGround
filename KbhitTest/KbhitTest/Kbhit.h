// Console input from keyboard.
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

#ifndef _KBHIT_H_18B22700_BED7_410B_B07E_2B0F34E2BCCB_
#define _KBHIT_H_18B22700_BED7_410B_B07E_2B0F34E2BCCB_

#ifdef _MSC_VER
#pragma once
#endif

#ifndef NO_KBHIT_KB_DEFS

#define KB_HOME                 (0xE0 - 0x200000E0 + 71)
#define KB_UP                   (0xE0 - 0x200000E0 + 72)
#define KB_PGUP                 (0xE0 - 0x200000E0 + 73)
#define KB_LEFT                 (0xE0 - 0x200000E0 + 75)
#define KB_RIGHT                (0xE0 - 0x200000E0 + 77)
#define KB_END                  (0xE0 - 0x200000E0 + 79)
#define KB_DOWN                 (0xE0 - 0x200000E0 + 80)
#define KB_PGDN                 (0xE0 - 0x200000E0 + 81)
#define KB_INSERT               (0xE0 - 0x200000E0 + 82)
#define KB_DELETE               (0xE0 - 0x200000E0 + 83)

#define KB_CTRL_HOME            (0xE0 - 0x200000E0 + 119)
#define KB_CTRL_UP              (0xE0 - 0x200000E0 + 141)
#define KB_CTRL_PGUP            (0xE0 - 0x200000E0 + 134)
#define KB_CTRL_LEFT            (0xE0 - 0x200000E0 + 115)
#define KB_CTRL_RIGHT           (0xE0 - 0x200000E0 + 116)
#define KB_CTRL_END             (0xE0 - 0x200000E0 + 117)
#define KB_CTRL_DOWN            (0xE0 - 0x200000E0 + 145)
#define KB_CTRL_PGDN            (0xE0 - 0x200000E0 + 118)
#define KB_CTRL_INSERT          (0xE0 - 0x200000E0 + 146)
#define KB_CTRL_DELETE          (0xE0 - 0x200000E0 + 147)

#define KB_F1                   (0xF0 - 0x100000F0 + 59)
#define KB_F2                   (0xF0 - 0x100000F0 + 60)
#define KB_F3                   (0xF0 - 0x100000F0 + 61)
#define KB_F4                   (0xF0 - 0x100000F0 + 62)
#define KB_F5                   (0xF0 - 0x100000F0 + 63)
#define KB_F6                   (0xF0 - 0x100000F0 + 64)
#define KB_F7                   (0xF0 - 0x100000F0 + 65)
#define KB_F8                   (0xF0 - 0x100000F0 + 66)
#define KB_F9                   (0xF0 - 0x100000F0 + 67)
#define KB_F10                  (0xF0 - 0x100000F0 + 68)
#define KB_F11                  (0xE0 - 0x200000E0 + 133)
#define KB_F12                  (0xE0 - 0x200000E0 + 134)

#define KB_SHIFT_F1             (0xF0 - 0x100000F0 + 84)
#define KB_SHIFT_F2             (0xF0 - 0x100000F0 + 85)
#define KB_SHIFT_F3             (0xF0 - 0x100000F0 + 86)
#define KB_SHIFT_F4             (0xF0 - 0x100000F0 + 87)
#define KB_SHIFT_F5             (0xF0 - 0x100000F0 + 88)
#define KB_SHIFT_F6             (0xF0 - 0x100000F0 + 89)
#define KB_SHIFT_F7             (0xF0 - 0x100000F0 + 90)
#define KB_SHIFT_F8             (0xF0 - 0x100000F0 + 91)
#define KB_SHIFT_F9             (0xF0 - 0x100000F0 + 92)
#define KB_SHIFT_F10            (0xF0 - 0x100000F0 + 93)
#define KB_SHIFT_F11            (0xE0 - 0x200000E0 + 135)
#define KB_SHIFT_F12            (0xE0 - 0x200000E0 + 136)

#define KB_CTRL_F1              (0xF0 - 0x100000F0 + 94)
#define KB_CTRL_F2              (0xF0 - 0x100000F0 + 95)
#define KB_CTRL_F3              (0xF0 - 0x100000F0 + 96)
#define KB_CTRL_F4              (0xF0 - 0x100000F0 + 97)
#define KB_CTRL_F5              (0xF0 - 0x100000F0 + 98)
#define KB_CTRL_F6              (0xF0 - 0x100000F0 + 99)
#define KB_CTRL_F7              (0xF0 - 0x100000F0 + 100)
#define KB_CTRL_F8              (0xF0 - 0x100000F0 + 101)
#define KB_CTRL_F9              (0xF0 - 0x100000F0 + 102)
#define KB_CTRL_F10             (0xF0 - 0x100000F0 + 103)
#define KB_CTRL_F11             (0xE0 - 0x200000E0 + 137)
#define KB_CTRL_F12             (0xE0 - 0x200000E0 + 138)

#define KB_BEL                  7
#define KB_BACK                 8
#define KB_TAB                  9
#define KB_LF                   10
#define KB_VT                   11
#define KB_FF                   12
#define KB_CR                   13
#define KB_ENTER                13
#define KB_EOF                  26
#define KB_ESCAPE               27
#define KB_SPACE                32

#define KB_CTRL_BACK            127
#define KB_CTRL_TAB             (0xF0 - 0x100000F0 + 148)
#define KB_CTRL_ENTER           10

#define KB_CTRL_A               1
#define KB_CTRL_B               2
#define KB_CTRL_C               3
#define KB_CTRL_D               4
#define KB_CTRL_E               5
#define KB_CTRL_F               6
#define KB_CTRL_G               7
#define KB_CTRL_H               8
#define KB_CTRL_I               9
#define KB_CTRL_J               10
#define KB_CTRL_K               11
#define KB_CTRL_L               12
#define KB_CTRL_M               13
#define KB_CTRL_N               14
#define KB_CTRL_O               15
#define KB_CTRL_P               16
#define KB_CTRL_Q               17
#define KB_CTRL_R               18
#define KB_CTRL_S               19
#define KB_CTRL_T               20
#define KB_CTRL_U               21
#define KB_CTRL_V               22
#define KB_CTRL_W               23
#define KB_CTRL_X               24
#define KB_CTRL_Y               25
#define KB_CTRL_Z               26
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // Checks the console for keyboard input and reads the next character available
    // without echo. Returns the character read. If no character is ready, returns zero.
    int Kbhit(void);

    // Flushes the console keyboard input buffer.
    void Kbflush(void);

#ifdef __cplusplus
}
#endif

#endif
