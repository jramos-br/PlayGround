/*
 * The bugcheck C/C++ macro.
 *
 * This file defines the function used by bugcheck() to write the diagnostic
 * information to stderr and exits the program.
 *
 * Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
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

#include "bugcheck.h"

#include <stdio.h>
#include <stdlib.h>

/* Writes diagnostic information to stderr and calls exit(EXIT_FAILURE). */
void bugcheck_(const char* expression, const char* file, int line)
{
    fprintf(stderr, "BUGCHECK: (%s) at %s, line %d\n", expression, file, line);
    exit(EXIT_FAILURE);
}
