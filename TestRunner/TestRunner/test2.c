#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "bugcheck.h"
#include "xmalloc.h"
#include "test2.h"

static int maxlen = 100;
static int* hist = NULL;

int test2_start(test_param_t* param)
{
    param->maxlen = maxlen;
    param->seed = 1;

    hist = (int*)xcalloc(maxlen + 1, sizeof(int));

    for (int i = 0; i <= maxlen; ++i)
        hist[i] = 0;

    return 0;
}

int test2_run(const double* data, int len)
{
    bugcheck(data != NULL);
    bugcheck(len >= 0);
    bugcheck(len <= maxlen);

    ++hist[len];

    return 0;
}

int test2_end(void)
{
    for (int i = 0; i <= maxlen; ++i)
        printf("hist[%d] = %d\n", i, hist[i]);

    free(hist);
    hist = NULL;

    return 0;
}
