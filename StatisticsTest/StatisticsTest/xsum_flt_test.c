// Floating-point numbers summation.
//
// Test module.
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
#include <stdio.h>
#include <errno.h>
//#include <string.h>
//#include <math.h>
//#include <fenv.h>

#include "bugcheck.h"
#include "xsum_flt.h"
#include "xsum_flt_test.h"

static void bsf_flt_test1a(int length)
{
    printf("bsf_flt_test1a length = %d\n", length);

    float* pu = calloc(length, sizeof(float));

    if (pu == NULL)
    {
        errno = ENOMEM;
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < length; ++i)
    {
        switch (i % 6)
        {
        case 0:
            pu[i] = (float)(length - i) / length;
            break;
        case 1:
            pu[i] = (float)i / -length;
            break;
        case 2:
            pu[i] = (float)(length - i) / -length;
            break;
        case 3:
            pu[i] = (float)i / length;
            break;
        case 4:
            pu[i] = i + powf(10, (float)(i % 6));
            break;
        case 5:
            pu[i] = i - powf(10, (float)(i % 6));
            break;
        }
    }

    size_t k = length * sizeof(float);
    float* pa = NULL;// (float*)memcpy(malloc(k), pu, k);
    float* pd = NULL;// (float*)memcpy(malloc(k), pu, k);

    if (0)
    {
        qsort(pa, length, sizeof(float), bsf_flt_cmp_absa);

        for (int i = 0, j = length - 1; i < length; ++i, --j)
        {
            pd[i] = pa[j];
        }

        for (int i = 1; i < length; ++i)
        {
            bugcheck(fabsf(pa[i]) >= fabsf(pa[i - i]));
            bugcheck(fabsf(pd[i]) <= fabsf(pd[i - i]));
        }
    }

    for (int i = 0; i < length; ++i)
    {
        //printf("data[%d] = %f\n", i, pu[i]);
    }

    //double dsumu = bsf_dbl_sum(pu, 0, length);
    //double dsuma = bsf_dbl_sum(pa, 0, length);
    //double dsumd = bsf_dbl_sum(pd, 0, length);

    //float sumu = bsf_flt_sum(pu, 0, length);
    //float suma = bsf_flt_sum(pa, 0, length);
    //float sumd = bsf_flt_sum(pd, 0, length);

    float sum_pairwise = bsf_flt_sum_pairwise(pu, 0, length);
    float sum_pairwise1 = bsf_flt_sum_pairwise1(pu, 0, length);
    float sum_pairwise2 = bsf_flt_sum_pairwise2(pu, 0, length);
    //float sum_kahan = bsf_flt_sum_kahan(pu, 0, length);
    //float sum_kahan2 = bsf_flt_sum_kahan2(pu, 0, length);
    //float sum_priest_comp = bsf_flt_sum_priest_comp(pd, 0, length);
    //float sum_priest_dcomp = bsf_flt_sum_priest_dcomp(pd, 0, length);
    //float sum_priest_dcomp2 = bsf_flt_sum_priest_dcomp2(pd, 0, length);
    //float sum_neumaier = bsf_flt_sum_neumaier(pu, 0, length);
    //float sum_neumaier2 = bsf_flt_sum_neumaier2(pu, 0, length);

    //float sum_klein = bsf_flt_sum_klein(pu, length);
    //float sum_klein2 = bsf_flt_sum_klein2(pu, length);
    //float sum_klein3 = bsf_flt_sum_klein3(pu, length);
    //float sum_kleinx = bsf_flt_sum_kleinx(pu, length);

    //printf("dsumu............. = %f\n", dsumu);
    //printf("dsuma............. = %f\n", dsuma);
    //printf("dsumd............. = %f\n", dsumd);
    //printf("sumu.............. = %f\n", sumu);
    //printf("suma.............. = %f\n", suma);
    //printf("sumd.............. = %f\n", sumd);
    if (sum_pairwise1 != sum_pairwise2)
    {
        printf("sum_pairwise...... = %f\n", sum_pairwise);
        printf("sum_pairwise1..... = %f\n", sum_pairwise1);
        printf("sum_pairwise2..... = %f\n", sum_pairwise2);
    }
    //printf("sum_kahan......... = %f\n", sum_kahan);
    //printf("sum_kahan2........ = %f\n", sum_kahan2);
    //printf("sum_priest_comp... = %f\n", sum_priest_comp);
    //printf("sum_priest_dcomp.. = %f\n", sum_priest_dcomp);
    //printf("sum_priest_dcomp2. = %f\n", sum_priest_dcomp2);
    //printf("sum_neumaier...... = %f\n", sum_neumaier);
    //printf("sum_neumaier2..... = %f\n", sum_neumaier2);

    //printf("sum_klein......... = %f\n", sum_klein);
    //printf("sum_klein2........ = %f\n", sum_klein2);
    //printf("sum_klein3........ = %f\n", sum_klein3);
    //printf("sum_kleinx........ = %f\n", sum_kleinx);

    free(pd);
    free(pa);
    free(pu);
}

static void bsf_flt_test11(void)
{
    bsf_flt_test1a(100);
}

static void bsf_flt_test12(void)
{
    for (int i = 0; i < 31; ++i)
        bsf_flt_test1a((1 << i) + 31);
}

static void bsf_flt_test13(void)
{
    int i;
    int j;

    for (i = 1; i < 256; j = i++)
    {
        bsf_flt_test1a(i);
    }

    while (i < INT_MAX / 2)
    {
        i *= 2;
        bsf_flt_test1a(xrand(j + 1, i));
        bsf_flt_test1a(i);
        j = i;
    }
}

static void bsf_flt_test14(void)
{
    for (int length = 10; length <= INT_MAX / 100000000; length *= 10)
    {
        bsf_flt_test1a(length);
    }
}

static void bsf_flt_test15(void)
{
    int hist[11] = { 0 };

    for (int i = 0; i < 10000; ++i)
    {
        int value = xrand(0, 10);
        hist[value]++;
    }
    INT_MAX
    for (int i = 0; i < 11; ++i)
    {
        printf("[%d] = %d\n", i, hist[i]);
    }
}

static void bsf_flt_test16(void)
{
    for (int i = 0; i < 100; ++i)
    {
        int x = ilog2(i);
        printf("i=%5d x=%d p=%.0f\n", i, x, pow(2, x));
    }
}

int xsum_flt_test(void)
{
    bsf_flt_test13();
    return 1;
}
