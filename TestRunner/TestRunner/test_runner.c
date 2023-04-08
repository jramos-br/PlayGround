#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <math.h>

#include "bugcheck.h"
#include "xmalloc.h"
#include "test_runner.h"

static char* format(char* buffer, size_t count, int value)
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

/*

// [0..N)
int r = rand() % N;

// [M..N)
int r = M + rand() % ( N - M );

// [0..N)
int r = rand() / ( RAND_MAX / N + 1 );

// [M..N)
int r = M + rand() / ( RAND_MAX / ( N - M ) + 1 );

///

int m = RAND_MAX / n;
int r;

do
  r = rand() / m;
while ( r >= n );

///

double uniform_deviate ( int seed )
{
  return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}

int r = uniform_deviate ( rand() ) * N;
For a more flexible range, the code is simple:

int r = M + uniform_deviate ( rand() ) * ( N - M );

///

unsigned time_seed()
{
  time_t now = time ( 0 );
  unsigned char *p = (unsigned char *)&now;
  unsigned seed = 0;
  size_t i;

  for ( i = 0; i < sizeof now; i++ )
    seed = seed * ( UCHAR_MAX + 2U ) + p[i];

  return seed;
}


*/

int irand(int minval, int maxval)
{
    double value = rand() * (1.0 / (RAND_MAX + 1.0));
    int result = (int)(value * (maxval - minval + 1) + minval);
    bugcheck(result >= minval && result <= maxval);
    return result;
}

double drand(double minval, double maxval)
{
    double value = rand() * (1.0 / RAND_MAX);
    double result = value * (maxval - minval) + minval;
    bugcheck(result >= minval && result <= maxval);
    return result;
}

const static double epsilon = 0.000000001;

static int fequal(double a, double b)
{
    return fabs(a - b) < epsilon;
}

static time_t sync_time(void)
{
    time_t old = time(NULL);
    time_t now;

    while (time(&now) == old)
        ;

    return now;
}

int string_test(string_test_start_func start, string_test_run_func func, string_test_end_func end)
{
    int status;
    test_param_t param;
    param.minlen = 0;
    param.maxlen = 1024;
    param.minval = 1;
    param.maxval = 100;
    param.seed = 1;
    param.maxrun = INT_MAX / 10000;

    if (start != NULL)
    {
        if ((*start)(&param) != 0)
        {
            return 1;
        }
    }

    srand(param.seed);

    char* line = (char*)xcalloc(param.maxlen + 1, sizeof(char));
    long count = 0;

    time_t old = sync_time();

    for (;;)
    {
        int len = irand(param.minlen, param.maxlen);
        int i = 0;

        while (i < len)
            line[i++] = (char)irand(param.minval, param.maxval);

        line[i] = '\0';

        status = (*func)(line, len) != 0 ? 1 : 0;

        if (++count == param.maxrun)
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
            printf("%s\n", format(buf, _countof(buf), count));

            if (status > 0)
                break;
        }
    }

    if (end != NULL)
    {
        if ((*end)() != 0)
        {
            return 1;
        }
    }

    xfree(line);

    return status == 2 ? 0 : 1;
}

int double_test(double_test_start_func start, double_test_run_func func, double_test_end_func end)
{
    int status;
    test_param_t param;
    param.minlen = 0;
    param.maxlen = 1024;
    param.minval = 1;
    param.maxval = 100;
    param.seed = 1;
    param.maxrun = INT_MAX / 10000;

    if (start != NULL)
    {
        if ((*start)(&param) != 0)
        {
            return 1;
        }
    }

    srand(param.seed);

    double* data = (double*)xcalloc(param.maxlen, sizeof(double));
    long count = 0;

    time_t old = sync_time();

    for (;;)
    {
        int len = irand(param.minlen, param.maxlen);
        int i = 0;

        while (i < len)
            data[i++] = drand(param.minval, param.maxval);

        status = (*func)(data, len) != 0 ? 1 : 0;

        if (++count == param.maxrun)
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
            printf("%s\n", format(buf, _countof(buf), count));

            if (status > 0)
                break;
        }
    }

    if (end != NULL)
    {
        if ((*end)() != 0)
        {
            return 1;
        }
    }

    xfree(data);

    return status == 2 ? 0 : 1;
}
