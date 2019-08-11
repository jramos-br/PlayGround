#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "choose.h"

static int choose1(int n, int k)
{
    if (k == 0)
        return 1;
    return n * choose1(n - 1, k - 1) / k;
}

static int choose2(int n, int k)
{
    if (k == 0 || k == n)
        return 1;
    return choose2(n - 1, k - 1) + choose2(n - 1, k);
}

static int choose3(int n, int k)
{
    size_t S = (k + 1) * sizeof(int);
    int* C = _alloca(S);
    memset(C, 0, S);

    C[0] = 1;  // nC0 is 1

    for (int i = 1; i <= n; i++)
    {
        // Compute next row of pascal triangle using
        // the previous row
        for (int j = min(i, k); j > 0; j--)
            C[j] = C[j] + C[j - 1];
    }

    return C[k];
}

static int choose4(int n, int k)
{
    if (k > n - k)
    {
        k = n - k;
    }

    int c = 1;

    for (int i = 0; i < k; i++)
    {
        c = c * (n - i);
        c = c / (i + 1);
    }

    return c;
}

static int choose5(int n, int k)
{
    if (k > n - k)
        k = n - k;

    int b = 1;

    for (int i = 1, m = n; i <= k; i++, m--)
        b = b * m / i;

    return b;
}

int choose(int n, int k)
{
    return choose3(n, k);
}

int choose_test()
{
    for (int n = 0; n < 16; ++n)
    {
        for (int k = 0; k <= n; ++k)
        {
            int c1 = choose1(n, k);
            int c2 = choose2(n, k);
            int c3 = choose3(n, k);
            int c4 = choose4(n, k);
            int c5 = choose5(n, k);

            if (c1 != c2 || c2 != c3 || c3 != c4 || c4 != c5)
                printf("n=%d k=%d c1=%d c2=%d c3=%d c4=%d c5=%d\n", n, k, c1, c2, c3, c4, c5);
        }
    }

    return 0;
}
