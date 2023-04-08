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

#ifndef _BSF_DBL_TEST_H_351BF5F3_3057_47A4_9FBC_99B81718C8DB_
#define _BSF_DBL_TEST_H_351BF5F3_3057_47A4_9FBC_99B81718C8DB_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    struct bsfs_dbl_data_t
    {
        double value;
    };

    typedef struct bsfs_dbl_data_t bsfs_dbl_data;
    /*
    void bsf_dbl_test1(const double* pdata, int length, double* pmean, double* pvariance, double* pstddev, double* pcv);
    void bsf_dbl_test2(const double* pdata, int length, double* pmean, double* pvariance, double* pstddev, double* pcv);
    void bsf_dbl_test3(const double* pdata, int length, double* pmean, double* pvariance, double* pstddev, double* pcv);
    void bsf_dbl_test4(const double* pdata, int length, double* pmean, double* pvariance, double* pstddev, double* pcv);
    */
    double bsf_dbl_mean(const double* pdata, int length);
    double bsf_dbl_variance(const double* pdata, int length);
    double bsf_dbl_stddev(const double* pdata, int length);
    double bsf_dbl_cv(const double* pdata, int length);

#ifdef __cplusplus
}
#endif

#endif
