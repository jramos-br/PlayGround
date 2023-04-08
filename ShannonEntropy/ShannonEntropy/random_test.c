/*
 * Test of random functions.
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

#include <stdlib.h>

#include "random_test.h"

void random_seed(int seed)
{
    srand((unsigned int)seed);
}

int random_next(int maxval)
{
    /* The rand function returns a pseudorandom integer in the range 0 to RAND_MAX. */
    double result = (double)rand() / (RAND_MAX + 1.0) * maxval;
    return (int)result;
}

int random_next2(int minval, int maxval)
{
    /* The rand function returns a pseudorandom integer in the range 0 to RAND_MAX. */
    double result = (double)rand() / (RAND_MAX + 1.0) * (maxval - minval) + minval;
    return (int)result;
}
