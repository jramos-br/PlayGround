#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "bugcheck.h"
#include "xmalloc.h"
#include "test1.h"

static int maxlen = 100;
static int* hist = NULL;

int test1_start(test_param_t* param)
{
    param->maxlen = maxlen;
    param->seed = 1;

    hist = (int*)xcalloc(maxlen + 1, sizeof(int));

    for (int i = 0; i <= maxlen; ++i)
        hist[i] = 0;

    return 0;
}

int test1_run(const char* line, int len)
{
    bugcheck(line != NULL);
    bugcheck(len >= 0);
    bugcheck(len <= maxlen);

    ++hist[len];

    return 0;
}

int test1_end(void)
{
    for (int i = 0; i <= maxlen; ++i)
        printf("hist[%d] = %d\n", i, hist[i]);

    free(hist);
    hist = NULL;

    return 0;
}
