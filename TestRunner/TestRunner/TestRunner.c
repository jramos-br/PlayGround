// The TestRunner program.
//
// This program runs some tests.
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
#include <locale.h>
#include <time.h>
#include <crtdbg.h>

#include "test_runner.h"
#include "test1.h"
#include "test2.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    setlocale(LC_ALL, "C");
    _tzset();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF |
        _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);

    int result = EXIT_SUCCESS;

    //if (string_test(test1_start, test1_run, test1_end) != 0)
    //    result = EXIT_FAILURE;

    if (double_test(test2_start, test2_run, test2_end) != 0)
        result = EXIT_FAILURE;

    return result;
}
