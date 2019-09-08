// The Bugtest1 program.
//
// Tests the bugcheck macro.
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
#include "bugcheck.h"

void identify()
{
#ifdef __cplusplus
    printf("I'm a C++ program\n");
#else
    printf("I'm a C program\n");
#endif
}

int main(int argc, char* argv[])
{
    (void)argv;
    identify();
    bugcheck(argc == 1);
    return EXIT_SUCCESS;
}
