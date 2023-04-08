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

#include <math.h>

void string_stats1(const char* s, double* pmean, double* pvariance, double* pstddev, double* pcv)
{
    const char* p;
    int x;
    int n = 0;

    double mean = 0.0;

    for (p = s; (x = (unsigned char)*p) != '\0'; ++p)
    {
        mean += x;
        ++n;
    }

    if (n > 1)
        mean /= n;

    if (pmean != NULL)
        *pmean = mean;

    double variance = 0.0;

    for (p = s; (x = (unsigned char)*p) != '\0'; ++p)
        variance += (x - mean) * (x - mean);

    if (n > 1)
        variance /= n;

    if (pvariance != NULL)
        *pvariance = variance;

    double stddev = sqrt(variance);

    if (pstddev != NULL)
        *pstddev = stddev;

    double cv = n > 0 ? stddev / mean : 0.0;

    if (pcv != NULL)
        *pcv = cv;
}

void string_stats2(const char* s, double* pmean, double* pvariance, double* pstddev, double* pcv)
{
    int n = 0;

    double old_M = -1, new_M = -1, old_S = -1, new_S = -1;

    for (int x; (x = (unsigned char)*s) != '\0'; ++s)
    {
        if (++n == 1)
        {
            old_M = new_M = x;
            old_S = 0.0;
        }
        else
        {
            new_M = old_M + (x - old_M) / n;
            new_S = old_S + (x - old_M) * (x - new_M);

            old_M = new_M;
            old_S = new_S;
        }
    }

    double mean = (n > 0) ? new_M : 0.0;
    double variance = (n > 1) ? new_S / n : 0.0;

    if (pmean != NULL)
        *pmean = mean;

    if (pvariance != NULL)
        *pvariance = variance;

    double stddev = sqrt(variance);

    if (pstddev != NULL)
        *pstddev = stddev;

    double cv = n > 0 ? stddev / mean : 0.0;

    if (pcv != NULL)
        *pcv = cv;
}

void string_stats3(const char* s, double* pmean, double* pvariance, double* pstddev, double* pcv)
{
    int n = 0;

    double mean = 0.0;
    double variance = 0.0;

    for (int x; (x = (unsigned char)*s) != '\0'; ++s)
    {
        double d = x - mean;
        mean += d / ++n;
        variance += (x - mean) * d;
    }

    if (n > 1)
        variance /= n;

    if (pmean != NULL)
        *pmean = mean;

    if (pvariance != NULL)
        *pvariance = variance;

    double stddev = sqrt(variance);

    if (pstddev != NULL)
        *pstddev = stddev;

    double cv = n > 0 ? stddev / mean : 0.0;

    if (pcv != NULL)
        *pcv = cv;
}

void string_stats4(const char* s, double* pmean, double* pvariance, double* pstddev, double* pcv)
{
    int n = 0;

    double mean = 0.0;
    double variance = 0.0;

    for (int x; (x = (unsigned char)*s) != '\0'; ++s)
    {
        double d = x - mean;
        mean += d / ++n;
        variance += ((x - mean) * d - variance) / n;
    }

    if (pmean != NULL)
        *pmean = mean;

    if (pvariance != NULL)
        *pvariance = variance;

    double stddev = sqrt(variance);

    if (pstddev != NULL)
        *pstddev = stddev;

    double cv = n > 0 ? stddev / mean : 0.0;

    if (pcv != NULL)
        *pcv = cv;
}

double string_mean(const char* s)
{
    double mean = 0.0;

    for (int n = 0, x; (x = (unsigned char)*s) != '\0'; ++s)
    {
        mean += (x - mean) / ++n;
    }

    return mean;
}

double string_variance(const char* s)
{
    double mean = 0.0;
    double variance = 0.0;

    for (int n = 0, x; (x = (unsigned char)*s) != '\0'; ++s)
    {
        double d = x - mean;
        mean += d / ++n;
        variance += ((x - mean) * d - variance) / n;
    }

    return variance;
}

double string_stddev(const char* s)
{
    double mean = 0.0;
    double variance = 0.0;

    for (int n = 0, x; (x = (unsigned char)*s) != '\0'; ++s)
    {
        double d = x - mean;
        mean += d / ++n;
        variance += ((x - mean) * d - variance) / n;
    }

    return sqrt(variance);
}

double string_cv(const char* s)
{
    int n = 0;

    double mean = 0.0;
    double variance = 0.0;

    for (int x; (x = (unsigned char)*s) != '\0'; ++s)
    {
        double d = x - mean;
        mean += d / ++n;
        variance += ((x - mean) * d - variance) / n;
    }

    return n > 0 ? sqrt(variance) / mean : 0.0;
}
