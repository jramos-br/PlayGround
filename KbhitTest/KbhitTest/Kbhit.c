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

#include <conio.h>
#include <tchar.h>
#include "Kbhit.h"

int Kbhit(void)
{
    int ch;

    if (_kbhit())
    {
        // The _gettch function reads a single character without echoing it. When reading
        // a function key or an arrow key, it must be called twice; the first call returns
        // 0x00 or 0xE0 and the second call returns the actual key code. Source: MSDN.

        ch = _gettch();

        switch (ch)
        {
        case 0x00:
            ch = (_gettch() & 0x0FFFFFF) | 0xF0000000;
            break;
        case 0xE0:
            ch = (_gettch() & 0x0FFFFFF) | 0xE0000000;
            break;
        default:
            ch &= 0x0FFFFFFF;
            break;
        }
    }
    else
        ch = 0;

    return ch;
}

void Kbflush(void)
{
    while (_kbhit())
    {
        // The _gettch function reads a single character without echoing it. When reading
        // a function key or an arrow key, it must be called twice; the first call returns
        // 0x00 or 0xE0 and the second call returns the actual key code. Source: MSDN.

        int ch = _gettch();

        if (ch == 0x00 || ch == 0xE0)
            (void)_gettch();
    }
}
