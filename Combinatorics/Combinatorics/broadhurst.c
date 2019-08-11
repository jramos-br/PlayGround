#include <stdlib.h>
#include <stdio.h>
#include "broadhurst.h"

/* http://www.martinbroadhurst.com/combinations.html */

static unsigned int next_combination(unsigned int *ar, size_t n, unsigned int k)
{
    unsigned int finished = 0;
    unsigned int changed = 0;
    unsigned int i;

    if (k > 0) {
        for (i = k - 1; !finished && !changed; i--) {
            if (ar[i] < (n - 1) - (k - 1) + i) {
                /* Increment this element */
                ar[i]++;
                if (i < k - 1) {
                    /* Turn the elements after it into a linear sequence */
                    unsigned int j;
                    for (j = i + 1; j < k; j++) {
                        ar[j] = ar[j - 1] + 1;
                    }
                }
                changed = 1;
            }
            finished = i == 0;
        }
        if (!changed) {
            /* Reset to first combination */
            for (i = 0; i < k; i++) {
                ar[i] = i;
            }
        }
    }
    return changed;
}

/* http://www.martinbroadhurst.com/multicombinations.html */

static unsigned int next_multicombination(unsigned int *ar, size_t n, unsigned int k)
{
    unsigned int changed = 0;
    int i;

    for (i = k - 1; i >= 0 && !changed; i--) {
        if (ar[i] < n - 1) {
            /* Increment this element */
            ar[i]++;
            if (i < k - 1) {
                /* Make the elements after it the same */
                unsigned int j;
                for (j = i + 1; j < k; j++) {
                    ar[j] = ar[j - 1];
                }
            }
            changed = 1;
        }
    }
    if (!changed) {
        /* Reset to first combination */
        for (i = 0; i < k; i++) {
            ar[i] = 0;
        }
    }
    return changed;
}

unsigned int next_multiset_combination(const unsigned int *multiset, unsigned int *ar, size_t n, unsigned int k)
{
    unsigned int finished = 0;
    unsigned int changed = 0;
    unsigned int i;

    for (i = k - 1; !finished && !changed; i--) {
        if (ar[i] < multiset[i + (n - k)]) {
            /* Find the successor */
            unsigned int j;
            for (j = 0; multiset[j] <= ar[i]; j++);
            /* Replace this element with it */
            ar[i] = multiset[j];
            if (i < k - 1) {
                /* Make the elements after it the same as this part of the multiset */
                unsigned int l;
                for (l = i + 1, j = j + 1; l < k; l++, j++) {
                    ar[l] = multiset[j];
                }
            }
            changed = 1;
        }
        finished = i == 0;
    }
    if (!changed) {
        /* Reset to first combination */
        for (i = 0; i < k; i++) {
            ar[i] = multiset[i];
        }
    }
    return changed;
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
    unsigned int numbers[3] = { 0, 1, 2 };
    const size_t k = sizeof(numbers) / sizeof(numbers[0]);
    unsigned int n = 4;

    do {
        print_array(numbers, k, stdout);
        putchar('\n');
    } while (next_combination(numbers, n, k));

    return 0;
}

static int multicombination(void)
{
    unsigned int numbers[3] = { 0, 0, 0 };
    const size_t k = sizeof(numbers) / sizeof(numbers[0]);
    unsigned int n = 4;

    do {
        print_array(numbers, k, stdout);
        putchar('\n');
    } while (next_multicombination(numbers, n, k));

    return 0;
}

int multiset_combination(void)
{
    unsigned int multiset[] = { 0, 1, 1, 2, 2, 2, 3 };
    unsigned int n = sizeof(multiset) / sizeof(unsigned int);
    unsigned int numbers[] = { 0, 1, 1, 2 };
    const unsigned int k = sizeof(numbers) / sizeof(unsigned int);

    do {
        print_array(numbers, k, stdout);
        putchar('\n');
    } while (next_multiset_combination(multiset, numbers, n, k));

    return 0;
}

int broadhurst(void)
{
    printf("broadhurst combination\n");
    combination();
    printf("broadhurst multicombination\n");
    multicombination();
    printf("broadhurst multiset_combination\n");
    multiset_combination();
    return 0;
}
