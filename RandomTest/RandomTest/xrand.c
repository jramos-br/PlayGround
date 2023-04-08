/*
 *
 */

#include <stdlib.h>
#include "xrand.h"

int xrand_get_int_0N_v1(int N)
{
    int r = rand() % N;
    return r;
}

int xrand_get_int_MN_v1(int M, int N)
{
    int r = M + rand() % (N - M);
    return r;
}

int xrand_get_int_0N_v2(int N)
{
    int r = rand() / (RAND_MAX / N + 1);
    return r;
}

int xrand_get_int_MN_v2(int M, int N)
{
    int r = M + rand() / (RAND_MAX / (N - M) + 1);
    return r;
}

int xrand_get_int_0N_v3(int N)
{
    int m = RAND_MAX / N;
    int r;

    do { r = rand() / m; } while (r >= N);

    return r;
}

int xrand_get_int_MN_v3(int M, int N)
{
    int m = RAND_MAX / (M - N);
    int r;

    do { r = rand() / m; } while (r < M && r >= N);

    return r;
}

static inline double uniform_deviate(int seed)
{
    return seed * (1.0 / (RAND_MAX + 1.0));
}

int xrand_get_int_0N_v4(int N)
{
    int r = (int)(uniform_deviate(rand()) * N);
    return r;
}

int xrand_get_int_MN_v4(int M, int N)
{
    int r = (int)(M + uniform_deviate(rand()) * (N - M));
    return r;
}

