#include <iostream>
#include <cstdlib>
#include <cmath>
#include <array>
#include <tuple>

#include "bugcheck.h"
#include "xrand.h"

typedef int* ptr2int_t;

int add(int a, int b)
{
    bugcheck(b <= INT_MAX - a);
    return a + b;
}

long long int add(long long int a, int b)
{
    bugcheck(b <= LLONG_MAX - a);
    return a + b;
}

bool is_full(ptr2int_t hist, int histlen)
{
    for (ptr2int_t p = hist, q = p + histlen; p < q; ++p)
    {
        if (*p == 0)
        {
            return false;
        }
    }

    return true;
}

double mean(ptr2int_t hist, int histlen)
{
    long long sum = 0;

    for (ptr2int_t p = hist, q = p + histlen; p < q; ++p)
    {
        sum = add(sum, *p);
    }

    return (double)sum / histlen;
}

double variance(ptr2int_t hist, int histlen)
{
    long long sum = 0;

    for (ptr2int_t p = hist, q = p + histlen; p < q; ++p)
    {
        sum = add(sum, *p);
    }

    double mean = (double)sum / histlen;

    double variance = 0.0;

    for (ptr2int_t p = hist, q = p + histlen; p < q; ++p)
    {
        variance += (*p - mean) * (*p - mean);
    }

    return variance / histlen;
}

double stddev(ptr2int_t hist, int histlen)
{
    long long sum = 0;

    for (ptr2int_t p = hist, q = p + histlen; p < q; ++p)
    {
        sum = add(sum, *p);
    }

    double mean = (double)sum / histlen;

    double variance = 0.0;

    for (ptr2int_t p = hist, q = p + histlen; p < q; ++p)
    {
        variance += (*p - mean) * (*p - mean);
    }

    variance /= histlen;
    return sqrt(variance);
}

typedef int(*fn_xrand_get_int_0N_t)(int N);
typedef int(*fn_xrand_get_int_MN_t)(int M, int N);
typedef double(*fn_stats_t)(int* hist, int histlen);

int main()
{
    const int M = 13;
    const int N = M + 17;
    const int HISTLEN = N - M;

    std::array<ptr2int_t, 4> hist[4] =
    {
        new int[HISTLEN](),
        new int[HISTLEN](),
        new int[HISTLEN](),
        new int[HISTLEN]()
    };

    std::array<fn_xrand_get_int_0N_t, 4> fn_xrand_get_int_0N[4] =
    {
        xrand_get_int_0N_v1,
        xrand_get_int_0N_v2,
        xrand_get_int_0N_v3,
        xrand_get_int_0N_v4
    };

    std::array<fn_xrand_get_int_MN_t, 4> fn_xrand_get_int_MN[4] =
    {
        xrand_get_int_MN_v1,
        xrand_get_int_MN_v2,
        xrand_get_int_MN_v3,
        xrand_get_int_MN_v4
    };

    std::array<std::tuple<const char*, fn_stats_t>, 3> fn_stats[3] =
    {
        std::make_tuple("mean", mean),
        std::make_tuple("variance", variance),
        std::make_tuple("stddev", stddev)
    };

    for (int i = 0; i < 1000; ++i)
    {
        for (int hi = 0; hi < _countof(hist); ++hi)
        {
            int r = (*fn_xrand_get_int_0N)(HISTLEN);
            bugcheck(r >= 0 && r < HISTLEN);
            hist[hi][r]++;
        }
    }

    for (int hj = 0; hj < HISTLEN; ++hj)
    {
        std::cout << "hist[" << hj << "] =";

        for (int hi = 0; hi < _countof(hist); ++hi)
        {
            std::cout << " " << hist[hi][hj];
        }

        std::cout << std::endl;
    }

    for (int j = 0; j < _countof(fn_stats); ++j)
    {
        for (int hj = 0; hj < HISTLEN; ++hj)
        {
            std::cout << "hist[" << hj << "] =";

            for (int hi = 0; hi < _countof(hist); ++hi)
            {
                std::cout << " " << mean(hist[hi]);
            }

            std::cout << std::endl;
        }
    }

    for (int hi = 0; hi < _countof(hist); ++hi)
    {
        delete[] hist[hi];
        hist[hi] = NULL;
    }

    return EXIT_SUCCESS;
}
