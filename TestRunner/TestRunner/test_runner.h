#pragma once

#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(a[0]))
#endif

struct test_param_
{
    int minlen;
    int maxlen;
    int minval;
    int maxval;
    unsigned int seed;
    int maxrun;
};

typedef struct test_param_ test_param_t;

typedef int(*string_test_start_func)(test_param_t* param);
typedef int(*string_test_run_func)(const char* line, int len);
typedef int(*string_test_end_func)(void);

typedef int(*double_test_start_func)(test_param_t* param);
typedef int(*double_test_run_func)(const double* data, int len);
typedef int(*double_test_end_func)(void);

#ifdef __cplusplus
extern "C" {
#endif

    int string_test(string_test_start_func start, string_test_run_func func, string_test_end_func end);
    int double_test(double_test_start_func start, double_test_run_func func, double_test_end_func end);

#ifdef __cplusplus
}
#endif
