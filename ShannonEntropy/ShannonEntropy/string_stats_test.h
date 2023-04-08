/*
 * Test of statistical functions.
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

#ifndef _STRING_STATS_TEST_H_C16713A1_D9E1_4A6F_BFC4_17F8D765BC23_
#define _STRING_STATS_TEST_H_C16713A1_D9E1_4A6F_BFC4_17F8D765BC23_

#ifdef _MSC_VER
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void string_stats1(const char* s, double* pmean, double* pvariance, double* pstddev, double* pcv);
    void string_stats2(const char* s, double* pmean, double* pvariance, double* pstddev, double* pcv);
    void string_stats3(const char* s, double* pmean, double* pvariance, double* pstddev, double* pcv);
    void string_stats4(const char* s, double* pmean, double* pvariance, double* pstddev, double* pcv);

    double string_mean(const char* s);
    double string_variance(const char* s);
    double string_stddev(const char* s);
    double string_cv(const char* s);

#ifdef __cplusplus
}
#endif

#endif
