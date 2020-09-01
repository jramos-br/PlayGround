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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <tchar.h>
#include "DumpConst.h"

struct Pair
{
    unsigned char First;
    unsigned char Second;
};

bool operator==(const Pair& lhs, const Pair& rhs)
{
    return memcmp(&lhs, &rhs, sizeof(Pair)) == 0;
}

bool operator!=(const Pair& lhs, const Pair& rhs)
{
    return !operator==(lhs, rhs);
}

#define IREGULAR 0
#define ISHIFT   1
#define ICTRL    2
#define IALT     3

static const _TCHAR* TypeNames[] =
{
    _T(""),
    _T("SHIFT"),
    _T("CTRL"),
    _T("ALT")
};

struct KeyVals
{
    unsigned short ScanCode;
    Pair Regular;
    Pair Shift;
    Pair Ctrl;
    Pair Alt;
    const _TCHAR* KeyName;
    const _TCHAR* RegularName;
    const _TCHAR* ShiftName;
    const _TCHAR* CtrlName;
    const _TCHAR* AltName;
};

struct KeyValsArray
{
    unsigned short ScanCode;
    Pair Pairs[4];
    const _TCHAR* KeyName;
    const _TCHAR* Names[4];
};

union KeyValsUnion
{
    KeyVals kv;
    KeyValsArray kva;
};

#define DFLT _T("")

static const KeyValsUnion Keys1[] =
{
    { 28, {  13,   0 }, {  13,   0 }, {  10,   0 }, {   0, 166 } }, // ENTER
    { 53, {  47,   0 }, {  63,   0 }, {   0, 149 }, {   0, 164 } }, // /?
    { 71, { 224,  71 }, { 224,  71 }, { 224, 119 }, {   0, 151 }, _T("HOME")  , DFLT, DFLT, DFLT, DFLT },
    { 72, { 224,  72 }, { 224,  72 }, { 224, 141 }, {   0, 152 }, _T("UP")    , DFLT, DFLT, DFLT, DFLT },
    { 73, { 224,  73 }, { 224,  73 }, { 224, 134 }, {   0, 153 }, _T("PGUP")  , DFLT, DFLT, DFLT, DFLT },
    { 75, { 224,  75 }, { 224,  75 }, { 224, 115 }, {   0, 155 }, _T("LEFT")  , DFLT, DFLT, DFLT, DFLT },
    { 77, { 224,  77 }, { 224,  77 }, { 224, 116 }, {   0, 157 }, _T("RIGHT") , DFLT, DFLT, DFLT, DFLT },
    { 79, { 224,  79 }, { 224,  79 }, { 224, 117 }, {   0, 159 }, _T("END")   , DFLT, DFLT, DFLT, DFLT },
    { 80, { 224,  80 }, { 224,  80 }, { 224, 145 }, {   0, 160 }, _T("DOWN")  , DFLT, DFLT, DFLT, DFLT },
    { 81, { 224,  81 }, { 224,  81 }, { 224, 118 }, {   0, 161 }, _T("PGDN")  , DFLT, DFLT, DFLT, DFLT },
    { 82, { 224,  82 }, { 224,  82 }, { 224, 146 }, {   0, 162 }, _T("INSERT"), DFLT, DFLT, DFLT, DFLT },
    { 83, { 224,  83 }, { 224,  83 }, { 224, 147 }, {   0, 163 }, _T("DELETE"), DFLT, DFLT, DFLT, DFLT }
};

static const KeyValsUnion Keys2[] =
{
    { 29, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 42, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 54, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 56, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 58, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 69, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 70, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 84, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 85, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
    { 86, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

    { 71, {   0,  71 }, {  55,   0 }, {   0, 119 }, {   0,   0 } },
    { 72, {   0,  72 }, {  56,   0 }, {   0, 141 }, {   0,   0 } },
    { 73, {   0,  73 }, {  57,   0 }, {   0, 132 }, {   0,   0 } },
    { 74, {   0,   0 }, {  45,   0 }, {   0,   0 }, {   0,   0 } },
    { 75, {   0,  75 }, {  52,   0 }, {   0, 115 }, {   0,   0 } },
    { 76, {   0,   0 }, {  53,   0 }, {   0,   0 }, {   0,   0 } },
    { 77, {   0,  77 }, {  54,   0 }, {   0, 116 }, {   0,   0 } },
    { 78, {   0,   0 }, {  43,   0 }, {   0,   0 }, {   0,   0 } },
    { 79, {   0,  79 }, {  49,   0 }, {   0, 117 }, {   0,   0 } },
    { 80, {   0,  80 }, {  50,   0 }, {   0, 145 }, {   0,   0 } },
    { 81, {   0,  81 }, {  51,   0 }, {   0, 118 }, {   0,   0 } },
    { 82, {   0,  82 }, {  48,   0 }, {   0, 146 }, {   0,   0 } },
    { 83, {   0,  83 }, {  46,   0 }, {   0, 147 }, {   0,   0 } },

    { 12, {  45,   0 }, {  95,   0 }, {  31,   0 }, {   0, 130 } }, // -_
    { 13, {  61,   0 }, {  43,   0 }, {   0,   0 }, {   0, 131 } }, // =+
    { 26, {  91,   0 }, { 123,   0 }, {  27,   0 }, {   0,  26 } }, // [{
    { 27, {  93,   0 }, { 125,   0 }, {  29,   0 }, {   0,  27 } }, // ]}
    { 39, {  59,   0 }, {  58,   0 }, {   0,   0 }, {   0,  39 } }, // ;:
    { 40, {  39,   0 }, {  34,   0 }, {   0,   0 }, {   0,  40 } }, // '"
    { 41, {  96,   0 }, { 126,   0 }, {   0,   0 }, {   0,  41 } }, // `~
    { 43, {  92,   0 }, { 124,   0 }, {  28,   0 }, {   0,   0 } }, // \|
    { 51, {  44,   0 }, {  60,   0 }, {   0,   0 }, {   0,  51 } }, // ,<
    { 52, {  46,   0 }, {  62,   0 }, {   0,   0 }, {   0,  52 } }, // .>
    { 53, {  47,   0 }, {  63,   0 }, {   0,   0 }, {   0,  53 } }, // /?
    { 55, {  42,   0 }, {   0,   0 }, { 114,   0 }, {   0,   0 } }  // *
};

static const KeyValsUnion Keys3[] =
{
    { 59, {   0,  59 }, {   0,  84 }, {   0,  94 }, {   0, 104 }, _T("F1") , DFLT, DFLT, DFLT, DFLT },
    { 60, {   0,  60 }, {   0,  85 }, {   0,  95 }, {   0, 105 }, _T("F2") , DFLT, DFLT, DFLT, DFLT },
    { 61, {   0,  61 }, {   0,  86 }, {   0,  96 }, {   0, 106 }, _T("F3") , DFLT, DFLT, DFLT, DFLT },
    { 62, {   0,  62 }, {   0,  87 }, {   0,  97 }, {   0, 107 }, _T("F4") , DFLT, DFLT, DFLT, DFLT },
    { 63, {   0,  63 }, {   0,  88 }, {   0,  98 }, {   0, 108 }, _T("F5") , DFLT, DFLT, DFLT, DFLT },
    { 64, {   0,  64 }, {   0,  89 }, {   0,  99 }, {   0, 109 }, _T("F6") , DFLT, DFLT, DFLT, DFLT },
    { 65, {   0,  65 }, {   0,  90 }, {   0, 100 }, {   0, 110 }, _T("F7") , DFLT, DFLT, DFLT, DFLT },
    { 66, {   0,  66 }, {   0,  91 }, {   0, 101 }, {   0, 111 }, _T("F8") , DFLT, DFLT, DFLT, DFLT },
    { 67, {   0,  67 }, {   0,  92 }, {   0, 102 }, {   0, 112 }, _T("F9") , DFLT, DFLT, DFLT, DFLT },
    { 68, {   0,  68 }, {   0,  93 }, {   0, 103 }, {   0, 113 }, _T("F10"), DFLT, DFLT, DFLT, DFLT },
    { 87, { 224, 133 }, { 224, 135 }, { 224, 137 }, { 224, 139 }, _T("F11"), DFLT, DFLT, DFLT, DFLT },
    { 88, { 224, 134 }, { 224, 136 }, { 224, 138 }, { 224, 140 }, _T("F12"), DFLT, DFLT, DFLT, DFLT }
};

static const KeyValsUnion Keys4[] =
{
    {  0, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("NUL")   , DFLT, NULL, NULL, NULL },
    {  0, {   1,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("SOH")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {   2,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("STX")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {   3,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("ETX")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {   4,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("EOT")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {   5,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("ENQ")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {   6,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("ACK")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {   7,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("BEL")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {   8,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("BS")    , DFLT, NULL, NULL, NULL }, // FAKE
    { 14, {   8,   0 }, {   8,   0 }, { 127,   0 }, {   0,  14 }, _T("BACK")  , DFLT, NULL, DFLT, NULL },
    { 15, {   9,   0 }, {   0,  15 }, {   0, 148 }, {   0,  15 }, _T("TAB")   , DFLT, NULL, DFLT, NULL },
    {  0, {  10,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("LF")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  11,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("VT")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  12,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("FF")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  13,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("CR")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  13,   0 }, {  13,   0 }, {  10,   0 }, {   0,  28 }, _T("RETURN"), DFLT, NULL, DFLT, NULL }, // FAKE
    { 28, {  13,   0 }, {  13,   0 }, {  10,   0 }, {   0,  28 }, _T("ENTER") , DFLT, NULL, DFLT, NULL },
    {  0, {  14,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("SO")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  15,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("SI")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  16,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("DLE")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  17,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("DC1")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  18,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("DC2")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  19,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("DC3")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  20,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("DC4")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  21,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("NAK")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  22,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("SYN")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  23,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("ETB")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  24,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("CAN")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  25,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("EM")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  26,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("SUB")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  26,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("EOF")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  27,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("ESC")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  1, {  27,   0 }, {  27,   0 }, {  27,   0 }, {   0,   1 }, _T("ESCAPE"), DFLT, NULL, NULL, NULL },
    {  0, {  28,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("FS")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  29,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("GS")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  30,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("RS")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  31,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("US")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  32,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("SP")    , DFLT, NULL, NULL, NULL }, // FAKE
    { 57, {  32,   0 }, {  32,   0 }, {  32,   0 }, {  32,   0 }, _T("SPACE") , DFLT, NULL, NULL, NULL },
    {  0, { 127,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("DEL")   , DFLT, NULL, NULL, NULL }  // FAKE
};

static const KeyValsUnion Keys5[] =
{
    {  0, {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("NUL")   , DFLT, NULL, NULL, NULL },
    {  0, {   7,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("BEL")   , DFLT, NULL, NULL, NULL }, // FAKE
    { 14, {   8,   0 }, {   8,   0 }, { 127,   0 }, {   0,  14 }, _T("BACK")  , DFLT, NULL, DFLT, NULL },
    { 15, {   9,   0 }, {   0,  15 }, {   0, 148 }, {   0,  15 }, _T("TAB")   , DFLT, NULL, DFLT, NULL },
    {  0, {  10,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("LF")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  11,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("VT")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  12,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("FF")    , DFLT, NULL, NULL, NULL }, // FAKE
    {  0, {  13,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("CR")    , DFLT, NULL, NULL, NULL }, // FAKE
    { 28, {  13,   0 }, {  13,   0 }, {  10,   0 }, {   0,  28 }, _T("ENTER") , DFLT, NULL, DFLT, NULL },
    {  0, {  26,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 }, _T("EOF")   , DFLT, NULL, NULL, NULL }, // FAKE
    {  1, {  27,   0 }, {  27,   0 }, {  27,   0 }, {   0,   1 }, _T("ESCAPE"), DFLT, NULL, NULL, NULL },
    { 57, {  32,   0 }, {  32,   0 }, {  32,   0 }, {  32,   0 }, _T("SPACE") , DFLT, NULL, NULL, NULL }
};

static const KeyValsUnion Keys6[] =
{
    { 11, {  48,   0 }, {  41,   0 }, {   0,   0 }, {   0, 129 }, _T("0"), NULL, NULL, NULL, DFLT },
    {  2, {  49,   0 }, {  33,   0 }, {   0,   0 }, {   0, 120 }, _T("1"), NULL, NULL, NULL, DFLT },
    {  3, {  50,   0 }, {  64,   0 }, {   0,   3 }, {   0, 121 }, _T("2"), NULL, NULL, NULL, DFLT },
    {  4, {  51,   0 }, {  35,   0 }, {   0,   0 }, {   0, 122 }, _T("3"), NULL, NULL, NULL, DFLT },
    {  5, {  52,   0 }, {  36,   0 }, {   0,   0 }, {   0, 123 }, _T("4"), NULL, NULL, NULL, DFLT },
    {  6, {  53,   0 }, {  37,   0 }, {   0,   0 }, {   0, 124 }, _T("5"), NULL, NULL, NULL, DFLT },
    {  7, {  54,   0 }, {  94,   0 }, {  30,   0 }, {   0, 125 }, _T("6"), NULL, NULL, NULL, DFLT },
    {  8, {  55,   0 }, {  38,   0 }, {   0,   0 }, {   0, 126 }, _T("7"), NULL, NULL, NULL, DFLT },
    {  9, {  56,   0 }, {  42,   0 }, {   0,   0 }, {   0, 127 }, _T("8"), NULL, NULL, NULL, DFLT },
    { 10, {  57,   0 }, {  40,   0 }, {   0,   0 }, {   0, 128 }, _T("9"), NULL, NULL, NULL, DFLT }
};

static const KeyValsUnion Keys7[] =
{
    { 30, {  97,   0 }, {  65,   0 }, {   1,   0 }, {   0,  30 }, _T("A"), NULL, NULL, DFLT, DFLT },
    { 48, {  98,   0 }, {  66,   0 }, {   2,   0 }, {   0,  48 }, _T("B"), NULL, NULL, DFLT, DFLT },
    { 46, {  99,   0 }, {  67,   0 }, {   3,   0 }, {   0,  46 }, _T("C"), NULL, NULL, DFLT, DFLT },
    { 32, { 100,   0 }, {  68,   0 }, {   4,   0 }, {   0,  32 }, _T("D"), NULL, NULL, DFLT, DFLT },
    { 18, { 101,   0 }, {  69,   0 }, {   5,   0 }, {   0,  18 }, _T("E"), NULL, NULL, DFLT, DFLT },
    { 33, { 102,   0 }, {  70,   0 }, {   6,   0 }, {   0,  33 }, _T("F"), NULL, NULL, DFLT, DFLT },
    { 34, { 103,   0 }, {  71,   0 }, {   7,   0 }, {   0,  34 }, _T("G"), NULL, NULL, DFLT, DFLT },
    { 35, { 104,   0 }, {  72,   0 }, {   8,   0 }, {   0,  35 }, _T("H"), NULL, NULL, DFLT, DFLT },
    { 23, { 105,   0 }, {  73,   0 }, {   9,   0 }, {   0,  23 }, _T("I"), NULL, NULL, DFLT, DFLT },
    { 36, { 106,   0 }, {  74,   0 }, {  10,   0 }, {   0,  36 }, _T("J"), NULL, NULL, DFLT, DFLT },
    { 37, { 107,   0 }, {  75,   0 }, {  11,   0 }, {   0,  37 }, _T("K"), NULL, NULL, DFLT, DFLT },
    { 38, { 108,   0 }, {  76,   0 }, {  12,   0 }, {   0,  38 }, _T("L"), NULL, NULL, DFLT, DFLT },
    { 50, { 109,   0 }, {  77,   0 }, {  13,   0 }, {   0,  50 }, _T("M"), NULL, NULL, DFLT, DFLT },
    { 49, { 110,   0 }, {  78,   0 }, {  14,   0 }, {   0,  49 }, _T("N"), NULL, NULL, DFLT, DFLT },
    { 24, { 111,   0 }, {  79,   0 }, {  15,   0 }, {   0,  24 }, _T("O"), NULL, NULL, DFLT, DFLT },
    { 25, { 112,   0 }, {  80,   0 }, {  16,   0 }, {   0,  25 }, _T("P"), NULL, NULL, DFLT, DFLT },
    { 16, { 113,   0 }, {  81,   0 }, {  17,   0 }, {   0,  16 }, _T("Q"), NULL, NULL, DFLT, DFLT },
    { 19, { 114,   0 }, {  82,   0 }, {  18,   0 }, {   0,  19 }, _T("R"), NULL, NULL, DFLT, DFLT },
    { 31, { 115,   0 }, {  83,   0 }, {  19,   0 }, {   0,  31 }, _T("S"), NULL, NULL, DFLT, DFLT },
    { 20, { 116,   0 }, {  84,   0 }, {  20,   0 }, {   0,  20 }, _T("T"), NULL, NULL, DFLT, DFLT },
    { 22, { 117,   0 }, {  85,   0 }, {  21,   0 }, {   0,  22 }, _T("U"), NULL, NULL, DFLT, DFLT },
    { 47, { 118,   0 }, {  86,   0 }, {  22,   0 }, {   0,  47 }, _T("V"), NULL, NULL, DFLT, DFLT },
    { 17, { 119,   0 }, {  87,   0 }, {  23,   0 }, {   0,  17 }, _T("W"), NULL, NULL, DFLT, DFLT },
    { 45, { 120,   0 }, {  88,   0 }, {  24,   0 }, {   0,  45 }, _T("X"), NULL, NULL, DFLT, DFLT },
    { 21, { 121,   0 }, {  89,   0 }, {  25,   0 }, {   0,  21 }, _T("Y"), NULL, NULL, DFLT, DFLT },
    { 44, { 122,   0 }, {  90,   0 }, {  26,   0 }, {   0,  44 }, _T("Z"), NULL, NULL, DFLT, DFLT }
};

static _TCHAR* outbuf[256];
static size_t outlen;

static void Format(_TCHAR* buf, size_t count, size_t& len, const _TCHAR* format, ...)
{
    if (len >= count)
    {
        errno = ENOSPC;
        _tperror(_T("Format"));
        exit(EXIT_FAILURE);
    }

    va_list args;
    va_start(args, format);

    int n = _vsntprintf_s(buf + len, count - len, _TRUNCATE, format, args);

    va_end(args);

    if (n < 0 || (size_t)n >= count - len)
    {
        errno = ENOSPC;
        _tperror(_T("Format"));
        exit(EXIT_FAILURE);
    }

    len += (size_t)n;
}

static bool PrintKeyVal(const KeyValsUnion* kvu, int type)
{
    static const _TCHAR* kf0 = _T("0xF0 - 0x100000F0");
    static const _TCHAR* ke0 = _T("0xE0 - 0x200000E0");

    if (kvu->kva.KeyName == NULL || kvu->kva.Names[type] == NULL)
        return false;

    if (_tcslen(kvu->kva.KeyName) == 0 && _tcslen(kvu->kva.Names[type]) == 0)
        return false;

    if (kvu->kva.Pairs[type].First == 0 && kvu->kva.Pairs[type].Second == 0)
        return false;

    if (type > IREGULAR)
        for (int i = IREGULAR; i < type; ++i)
            if (kvu->kva.Pairs[type] == kvu->kva.Pairs[i])
                return false;

    if (outlen >= _countof(outbuf))
        return false;

    _TCHAR buf[256];
    size_t len = 0;

    Format(buf, _countof(buf), len, _T("#define KB_"));

    if (_tcslen(kvu->kva.Names[type]) > 0)
        Format(buf, _countof(buf), len, _T("%s"), kvu->kva.Names[type]);
    else
        if (_tcslen(TypeNames[type]) > 0)
            Format(buf, _countof(buf), len, _T("%s_%s"), TypeNames[type], kvu->kva.KeyName);
        else
            Format(buf, _countof(buf), len, _T("%s"), kvu->kva.KeyName);

    Format(buf, _countof(buf), len, _T("%*s"), 32 > len ? 32 - len : 1, _T(""));

    switch (kvu->kva.Pairs[type].First)
    {
    case 0:
        Format(buf, _countof(buf), len, _T("(%s + %d)"), kf0, kvu->kva.Pairs[type].Second);
        break;
    case 224:
        Format(buf, _countof(buf), len, _T("(%s + %d)"), ke0, kvu->kva.Pairs[type].Second);
        break;
    default:
        Format(buf, _countof(buf), len, _T("%d"), kvu->kva.Pairs[type].First);
        break;
    }

    outbuf[outlen++] = _tcsdup(buf);
    return true;
}

static bool bRegular = true;
static bool bShift = true;
static bool bCtrl = true;
static bool bAlt = false;

static void DumpConst(const KeyValsUnion keyVals[], size_t keyCount)
{
    int print = 0;

    if (bRegular)
    {
        for (size_t i = 0; i < keyCount; ++i)
            print += (int)PrintKeyVal(&keyVals[i], IREGULAR);

        if (print > 0)
        {
            print = 0;

            if (outlen < _countof(outbuf))
                outbuf[outlen++] = _tcsdup(_T(""));
        }
    }

    if (bShift)
    {
        for (size_t i = 0; i < keyCount; ++i)
            print += (int)PrintKeyVal(&keyVals[i], ISHIFT);

        if (print > 0)
        {
            print = 0;

            if (outlen < _countof(outbuf))
                outbuf[outlen++] = _tcsdup(_T(""));
        }
    }

    if (bCtrl)
    {
        for (size_t i = 0; i < keyCount; ++i)
            print += (int)PrintKeyVal(&keyVals[i], ICTRL);

        if (print > 0)
        {
            print = 0;

            if (outlen < _countof(outbuf))
                outbuf[outlen++] = _tcsdup(_T(""));
        }
    }

    if (bAlt)
    {
        for (size_t i = 0; i < keyCount; ++i)
            print += (int)PrintKeyVal(&keyVals[i], IALT);

        if (print > 0)
        {
            print = 0;

            if (outlen < _countof(outbuf))
                outbuf[outlen++] = _tcsdup(_T(""));
        }
    }
}

void DumpConst(void)
{
    DumpConst(Keys1, _countof(Keys1));
    DumpConst(Keys3, _countof(Keys3));
    DumpConst(Keys5, _countof(Keys5));
    DumpConst(Keys6, _countof(Keys6));
    DumpConst(Keys7, _countof(Keys7));

    for (size_t i = 0; i < outlen; ++i)
        _ftprintf(stdout, _T("%s\n"), outbuf[i]);

    for (size_t i = 0; i < outlen; ++i)
        free(outbuf[i]);
}

void PrintChar(int ch)
{
    static const KeyValsUnion* Keys[] =
    {
        Keys1,
        Keys2,
        Keys3,
        Keys4,
        Keys6,
        Keys7
    };

    static size_t KeysCount[] =
    {
      _countof(Keys1),
      _countof(Keys2),
      _countof(Keys3),
      _countof(Keys4),
      _countof(Keys6),
      _countof(Keys7)
    };

    _TCHAR buf[256];
    size_t len = 0;

    Format(buf, _countof(buf), len, _T("%d 0x%08X %d"), ch, ch, ch & 0x00FFFFFF);

    if (_istprint((_TCHAR)ch))
        Format(buf, _countof(buf), len, _T(" %c"), (unsigned int)(_TCHAR)ch);

    Pair p;
    bool unicode = false;

    switch (ch & 0xF0000000)
    {
    case 0xF0000000:
        p.First = 0;
        p.Second = (unsigned char)ch;
        break;
    case 0xE0000000:
        p.First = 224;
        p.Second = (unsigned char)ch;
        break;
    default:
        unicode = ch < 0 || ch > 255;
        p.First = (unsigned char)ch;
        p.Second = 0;
    }

    for (size_t i = 0; i < _countof(Keys) && !unicode; ++i)
    {
        const KeyValsUnion* kvua = Keys[i];
        size_t kvuc = KeysCount[i];

        for (const KeyValsUnion* kvu = kvua; kvu < kvua + kvuc; ++kvu)
        {
            for (size_t type = 0; type < _countof(kvu->kva.Pairs); ++type)
            {
                if (kvu->kva.Pairs[type] == p)
                {
                    Format(buf, _countof(buf), len, _T(" "));

                    if (kvu->kva.Names[type] != NULL && _tcslen(kvu->kva.Names[type]) > 0)
                    {
                        Format(buf, _countof(buf), len, _T("%s"), kvu->kva.Names[type]);
                    }
                    else
                    {
                        if (kvu->kva.KeyName != NULL && _tcslen(kvu->kva.KeyName) > 0)
                        {
                            if (_tcslen(TypeNames[type]) > 0)
                                Format(buf, _countof(buf), len, _T("%s_%s"), TypeNames[type], kvu->kva.KeyName);
                            else
                                Format(buf, _countof(buf), len, _T("%s"), kvu->kva.KeyName);
                        }
                        else
                        {
                            if (_tcslen(TypeNames[type]) > 0)
                                Format(buf, _countof(buf), len, _T("%s_%s(%d,%04X)"), TypeNames[type], _T("NONAME"), kvu->kva.ScanCode, kvu->kva.ScanCode);
                            else
                                Format(buf, _countof(buf), len, _T("%s(%d,%04X)"), _T("NONAME"), kvu->kva.ScanCode, kvu->kva.ScanCode);
                        }
                    }
                }
            }
        }
    }

    _ftprintf(stdout, _T("%s\n"), buf);
}
