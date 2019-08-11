#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "choose.h"
#include "nlucaroni.h"

/* https://stackoverflow.com/questions/561/how-to-use-combinations-of-sets-as-test-data */

/* Algorithm 515: Generation of a Vector from the Lexicographical Index;
   Buckles, B. P., and Lybanon, M.
   ACM Transactions on Mathematical Software, Vol. 3, No. 2, June 1977 */

   /** [combination c n p x]
   * get the [x]th lexicographically ordered set of [p] elements in [n]
   * output is in [c], and should be sizeof(int)*[p] */
static void get_combination(int* c, int n, int p, int x) {
    int i, r, k = 0;
    for (i = 0; i < p - 1; i++) {
        c[i] = (i != 0) ? c[i - 1] : 0;
        do {
            c[i]++;
            r = choose(n - c[i], p - (i + 1));
            k = k + r;
        } while (k < x);
        k = k - r;
    }
    c[p - 1] = c[p - 2] + x - k;
}

static void print_array(const unsigned int *ar, size_t len, FILE *fptr)
{
    unsigned int i;
    fputc('[', fptr);
    for (i = 0; i < len; i++) {
        fprintf(fptr, "%d", ar[i]);
        if (i < len - 1) {
            fputs(", ", fptr);
        }
    }
    fputc(']', fptr);
}

static int combination(void)
{
    unsigned int numbers[3] = { 1, 2, 3 };
    const size_t k = sizeof(numbers) / sizeof(numbers[0]);
    unsigned int n = 4;

    for (int i = 0, j = choose(n, k); i < j; ++i)
    {
        get_combination(numbers, n, k, i);
        print_array(numbers, k, stdout);
        putchar('\n');
    }

    return 0;
}


int nlucaroni(void)
{
    printf("nlucaroni\n");
    combination();
    return 0;
}
