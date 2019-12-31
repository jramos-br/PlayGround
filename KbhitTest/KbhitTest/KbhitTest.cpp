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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <tchar.h>
#include "Kbhit.h"
#include "DumpConst.h"

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc > 1 && _tcsicmp(argv[1], _T("-d")) == 0)
    {
        DumpConst();
    }

    Kbflush();

    for (;;)
    {
        int ch;

        while ((ch = Kbhit()) == 0)
        {
        }

        PrintChar(ch);

        if (ch == 27)
            break;
    }

    return EXIT_SUCCESS;
}
