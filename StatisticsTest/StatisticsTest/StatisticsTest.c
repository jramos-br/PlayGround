// The StatisticsTest program.
//
// This program compares different algorithms to calculate basic statistics.
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
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#include "bugcheck.h"
#include "xsum.h"
#include "string_stats_test.h"

char* format(char* buffer, size_t count, int value)
{
    char temp[64];
    int len = snprintf(temp, _countof(temp), "%d", value);

    if (len < 0 || (size_t)len >= _countof(temp))
    {
        errno = ERANGE;
        perror("format");
        exit(EXIT_FAILURE);
    }

    char* p = temp;
    char* q = buffer;
    char* r = buffer + count;

    if (*p == '-')
    {
        if (q < r)
        {
            *q++ = '-';
        }

        --len;  ++p;
    }

    int x = (len - 1) % 3 + 1;

    while (*p != '\0')
    {
        if (x == 0)
        {
            if (q < r)
            {
                *q++ = '.';
            }

            x = 3;
        }

        if (q < r)
        {
            *q++ = *p;
        }

        --x; ++p;
    }

    if (q < r)
    {
        *q++ = '\0';
    }
    else
    {
        errno = ERANGE;
        perror("format");
        exit(EXIT_FAILURE);
    }

    return buffer;
}

int xrand(int max)
{
    double temp = (double)rand() / RAND_MAX;
    return (int)(temp * max + 0.5);
}

int fequal(double a, double b)
{
    return fabs(a - b) < 0.000000001;
}

time_t sync_time(void)
{
    time_t old = time(NULL);
    time_t now;

    while (time(&now) == old)
        ;

    return now;
}

typedef void(*test_start)(void);
typedef int(*test_func)(const char* line, int len);
typedef void(*test_end)(void);

#define MINLEN 0
#define MAXLEN 1024

int test(test_start start, test_func func, test_end end)
{
    int status;

    if (start != NULL)
        (*start)();

    srand(1);

    char line[MAXLEN + 1];
    long c = 0;

    time_t old = sync_time();

    for (;;)
    {
        int len = xrand(MAXLEN);
        int i = 0;

        while (i < len)
            line[i++] = (char)xrand(255);

        line[i] = '\0';

        status = (*func)(line, len) != 0 ? 1 : 0;

        if (++c == INT_MAX / 1000)
            status = 2;

        time_t now = time(NULL);

        if (now - old >= 5)
        {
            if (status == 0)
                status = -1;

            old = now;
        }

        if (status != 0)
        {
            char buf[32];
            printf("%s\n", format(buf, _countof(buf), c));

            if (status > 0)
                break;
        }
    }

    if (end != NULL)
        (*end)();

    return status == 2 ? EXIT_SUCCESS : EXIT_FAILURE;
}

#pragma region test1

static int hist1[MAXLEN + 1];

static void test1s(void)
{
    for (int i = 0; i < _countof(hist1); ++i)
        hist1[i] = 0;
}

static int test1f(const char* line, int len)
{
    bugcheck(line != NULL);
    bugcheck(len >= 0);
    bugcheck(len <= MAXLEN);

    ++hist1[len];

    return 0;
}

static void test1e(void)
{
    for (int i = 0; i < _countof(hist1); ++i)
        printf("hist1[%d] = %d\n", i, hist1[i]);
}

#pragma endregion

#pragma region test2

int test2(const char* line, int len)
{
    (void)len;

    double m[4], v[4], s[4], cv[4];

    string_stats1(line, &m[0], &v[0], &s[0], &cv[0]);
    string_stats2(line, &m[1], &v[1], &s[1], &cv[1]);
    string_stats3(line, &m[2], &v[2], &s[2], &cv[2]);
    string_stats4(line, &m[3], &v[3], &s[3], &cv[3]);

    double mean = string_mean(line);
    double variance = string_variance(line);
    double stddev = string_stddev(line);
    double coevar = string_cv(line);

    for (int i = 1; i < _countof(m); ++i)
    {
        if (!fequal(m[i], m[0]) || !fequal(v[i], v[0]) || !fequal(s[i], s[0]) || !fequal(cv[i], cv[0]))
        {
            for (i = 0; i < _countof(m); ++i)
            {
                printf("string_stats%d: m=%f v=%f s=%f cv=%f\n", i + 1, m[i], v[i], s[i], cv[i]);
            }

            return 1;
        }
    }

    if (!fequal(m[0], mean) || !fequal(v[0], variance) || !fequal(s[0], stddev) || !fequal(cv[0], coevar))
    {
        printf("string_stats%d: m=%f v=%f s=%f cv=%f\n", 1, m[0], v[0], s[0], cv[0]);

        printf("string_mean: %f\n", mean);
        printf("string_variance: %f\n", variance);
        printf("string_stddev: %f\n", stddev);
        printf("string_coevar: %f\n", coevar);

        return 1;
    }

    return 0;
}

#pragma endregion

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "C");
    _tzset();

    (void)argc;
    (void)argv;

    int result = EXIT_SUCCESS;

    if (1)
    {
        bsf_flt_test1();
    }
    
    if (0)
    {
        result = test(NULL, test2, NULL);
    }

    return result;
}
