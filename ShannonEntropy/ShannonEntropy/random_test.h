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

#ifndef _RANDOM_TEST_H_B998ED11_B046_4D42_A543_1ABA762CDD57_
#define _RANDOM_TEST_H_B998ED11_B046_4D42_A543_1ABA762CDD57_

#ifdef _MSC_VER
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void random_seed(int seed);
    int random_next(int maxval);
    int random_next2(int minval, int maxval);

#ifdef __cplusplus
}
#endif

#endif
