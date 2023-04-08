// Floating-point numbers summation.
//
// Implementation of algorithms for summing an array of floating-point numbers. The main
// goal is to study the differences among these methods.
//
// See the following references for background information:
//
// [1] Borgwardt, M. (n.d.). What every programmer should know about floating-point
//     arithmetic. Retrieved December 1, 2019, from https://floating-point-gui.de/
//
// [2] Cook, J. D. (2019, November 5). Summing an array of floating point numbers.
//     Retrieved December 1, 2019, from https://www.johndcook.com/blog/2019/11/05/kahan/
//
// [3] McNamee, J. M. (2004). A comparison of methods for accurate summation. ACM SIGSAM
//     Bulletin, 38(1), 1-7. https://doi.org/10.1145/980175.980177
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

#ifndef _XSUM_H_B76CCEEB_8EF8_4CF8_A456_783DDBF9CE36_
#define _XSUM_H_B76CCEEB_8EF8_4CF8_A456_783DDBF9CE36_

#ifdef _MSC_VER
#pragma once
#endif

#define FLT_ZERO    0.0F                // (float) 0.0
#define DBL_ZERO    0.0                 // (double) 0.0

// Fast2Sum algorithm for exact addition of two floating-point numbers.
//
// The Fast2Sum algorithm, proposed by Dekker (1971), allows the computation of the error
// of a (rounded to nearest) floating-point addition. The algorithm calculates z and zz so
// that z is the closest floating-point number to x + y, and z + zz is exactly x + y. The
// algorithm requires |x| >= |y|.
//
// This implementation is based on the following formulas:
//
//     z = fl(x + y).       Formula 4.1 (Dekker, 1971, p. 227)
//     w = fl(x - z),       Formula 4.13 (Dekker, 1971, p. 230)
//     zz = fl(w + y).
//
// fl(E) is the expression obtained by replacing the arithmetical operations "+", "-", "*"
// and "/" present in E by the corresponding floating-point operations.
//
// [1] Dekker, T. J. (1971). A floating-point technique for extending the available
//     precision. Numerische Mathematik, 18(3), 224–242. https://doi.org/10.1007/bf01397083

#define Fast2Sum(x,y,z,zz) \
do { \
    z = x + y; \
    zz = (x - z) + y; \
} while (0)

// Fast2Sum algorithm for exact addition of two floating-point numbers.
//
// The Fast2Sum algorithm, proposed by Dekker (1971), allows the computation of the error
// of a (rounded to nearest) floating-point addition. The algorithm calculates z and zz so
// that z is the closest floating-point number to x + y, and z + zz is exactly x + y. The
// algorithm requires |x| >= |y|.
//
// This implementation is based on the following formulas:
//
//     z = fl(x + y).       Formula 4.1 (Dekker, 1971, p. 227)
//     w = fl(x - z),       Formula 4.13 (Dekker, 1971, p. 230)
//     zz = fl(w + y).
//
// fl(E) is the expression obtained by replacing the arithmetical operations "+", "-", "*"
// and "/" present in E by the corresponding floating-point operations.
//
// [1] Dekker, T. J. (1971). A floating-point technique for extending the available
//     precision. Numerische Mathematik, 18(3), 224–242. https://doi.org/10.1007/bf01397083

#define Fast2Sum413(x,y,z,zz,w) \
do { \
    z = x + y; \
    w = x - z; \
    zz = w + y; \
} while (0)

// Fast2Sum algorithm for exact addition of two floating-point numbers.
//
// The Fast2Sum algorithm, proposed by Dekker (1971), allows the computation of the error
// of a (rounded to nearest) floating-point addition. The algorithm calculates z and zz so
// that z is the closest floating-point number to x + y, and z + zz is exactly x + y. The
// algorithm requires |x| >= |y|.
//
// This implementation is based on the following formulas:
//
//     z = fl(x + y).       Formula 4.1 (Dekker, 1971, p. 227)
//     w = fl(z - x),       Formula 4.3 (Dekker, 1971, p. 228)
//     zz = fl(y - w).
//
// fl(E) is the expression obtained by replacing the arithmetical operations "+", "-", "*"
// and "/" present in E by the corresponding floating-point operations.
//
// [1] Dekker, T. J. (1971). A floating-point technique for extending the available
//     precision. Numerische Mathematik, 18(3), 224–242. https://doi.org/10.1007/bf01397083

#define Fast2Sum43(x,y,z,zz,w) \
do { \
    z = x + y; \
    w = z - x; \
    zz = y - w; \
} while (0)

// Slow2Sum algorithm for exact addition of two floating-point numbers.
//
// The Fast2Sum algorithm, proposed by Dekker (1971), allows the computation of the error
// of a (rounded to nearest) floating-point addition. The algorithm calculates z and zz so
// that z is the closest floating-point number to x + y, and z + zz is exactly x + y. The
// algorithm requires |x| >= |y|.
//
// This implementation, called Slow2Sum, conditionally ensures that |x| >= |y| and after
// that applies the following formulas:
//
//     z = fl(x + y).       Formula 4.1 (Dekker, 1971, p. 227)
//     w = fl(x - z),       Formula 4.13 (Dekker, 1971, p. 230)
//     zz = fl(w + y).
//
// fl(E) is the expression obtained by replacing the arithmetical operations "+", "-", "*"
// and "/" present in E by the corresponding floating-point operations.
//
// Beware of poor performance due to branching.
//
// [1] Dekker, T. J. (1971). A floating-point technique for extending the available
//     precision. Numerische Mathematik, 18(3), 224–242. https://doi.org/10.1007/bf01397083

#define Slow2Sum(x,y,z,zz) \
do { \
    z = x + y; \
    zz = (x > y) == (x > -y) ? (x - z) + y : (y - z) + x; \
} while (0)

// TwoSum algorithm for exact addition of two floating-point numbers.
//
// The conventional TwoSum algorithm, due to Møller (1965) and Knuth (1969), allows the
// computation of the error of a (rounded to nearest) floating-point addition. The
// algorithm calculates z and zz so that z is the closest floating-point number to x + y,
// and z + zz is exactly x + y.
//
// This implementation is based on the following Dekker's (1971) formulas:
//
//     z = fl(x + y).                   Formula 4.1 (Dekker, 1971, p. 227)
//     w = fl(z - x), z1 = fl(y - w),   Formula 4.16 (Dekker, 1971, p. 230)
//     v = fl(z - w), z2 = fl(v - x),
//     zz = fl(z1 - z2).
//
// fl(E) is the expression obtained by replacing the arithmetical operations "+", "-", "*"
// and "/" present in E by the corresponding floating-point operations.
//
// According to Dekker (1971), "this formula is equivalent to that given by Møller (1965,
// p. 42 process A) and Knuth (1969, p. 203 formula 48), the only difference being that
// there -z2 instead of z2 is calculated and added to zl" (p. 231).
//
// [1] Dekker, T. J. (1971). A floating-point technique for extending the available
//     precision. Numerische Mathematik, 18(3), 224–242. https://doi.org/10.1007/bf01397083
//
// [2] Knuth, D. E. (1969). The art of computer programming, volume 2: Seminumerical
//     algorithms. Addison-Wesley.
//
// [3] Møller, O. (1965). Quasi double-precision in floating point addition. BIT Numerical
//     Mathematics, 5(1), 37–50. https://doi.org/10.1007/BF01975722

#define TwoSum(x,y,z,zz) \
do { \
    z = x + y; \
    zz = (y - (z - x)) - ((z - (z - x)) - x); \
} while (0)

// TwoSum algorithm for exact addition of two floating-point numbers.
//
// The conventional TwoSum algorithm, due to Møller (1965) and Knuth (1969), allows the
// computation of the error of a (rounded to nearest) floating-point addition. The
// algorithm calculates z and zz so that z is the closest floating-point number to x + y,
// and z + zz is exactly x + y.
//
// This implementation is based on the following Dekker's (1971) formulas:
//
//     z = fl(x + y).                   Formula 4.1 (Dekker, 1971, p. 227)
//     w = fl(z - x), z1 = fl(y - w),   Formula 4.16 (Dekker, 1971, p. 230)
//     v = fl(z - w), z2 = fl(v - x),
//     zz = fl(z1 - z2).
//
// fl(E) is the expression obtained by replacing the arithmetical operations "+", "-", "*"
// and "/" present in E by the corresponding floating-point operations.
//
// According to Dekker (1971), "this formula is equivalent to that given by Møller (1965,
// p. 42 process A) and Knuth (1969, p. 203 formula 48), the only difference being that
// there -z2 instead of z2 is calculated and added to zl" (p. 231).
//
// [1] Dekker, T. J. (1971). A floating-point technique for extending the available
//     precision. Numerische Mathematik, 18(3), 224–242. https://doi.org/10.1007/bf01397083
//
// [2] Knuth, D. E. (1969). The art of computer programming, volume 2: Seminumerical
//     algorithms. Addison-Wesley.
//
// [3] Møller, O. (1965). Quasi double-precision in floating point addition. BIT Numerical
//     Mathematics, 5(1), 37–50. https://doi.org/10.1007/BF01975722

#define TwoSum2(x,y,z,zz,w) \
do { \
    z = x + y; \
    w = z - x; \
    zz = (y - w) - ((z - w) - x); \
} while (0)

#define TwoSum3(u,v,s,c) \
do { \
    s = u + v; \
    c = (v - (s - u)) + (u - (s - (s - u))); \
} while (0)

#define TwoSum4(u,v,s,c,w) \
do { \
    s = u + v; \
    w = s - u; \
    c = (v - w) + (u - (s - w)); \
} while (0)

// TwoSum algorithm for exact addition of two floating-point numbers.
//
// The conventional TwoSum algorithm, due to Møller (1965) and Knuth (1969), allows the
// computation of the error of a (rounded to nearest) floating-point addition. The
// algorithm calculates z and zz so that z is the closest floating-point number to x + y,
// and z + zz is exactly x + y.
//
// This implementation is based on the following Dekker's (1971) formulas:
//
//     z = fl(x + y).                   Formula 4.1 (Dekker, 1971, p. 227)
//     w = fl(z - x), z1 = fl(y - w),   Formula 4.16 (Dekker, 1971, p. 230)
//     v = fl(z - w), z2 = fl(v - x),
//     zz = fl(z1 - z2).
//
// fl(E) is the expression obtained by replacing the arithmetical operations "+", "-", "*"
// and "/" present in E by the corresponding floating-point operations.
//
// According to Dekker (1971), "this formula is equivalent to that given by Møller (1965,
// p. 42 process A) and Knuth (1969, p. 203 formula 48), the only difference being that
// there -z2 instead of z2 is calculated and added to zl" (p. 231).
//
// [1] Dekker, T. J. (1971). A floating-point technique for extending the available
//     precision. Numerische Mathematik, 18(3), 224–242. https://doi.org/10.1007/bf01397083
//
// [2] Knuth, D. E. (1969). The art of computer programming, volume 2: Seminumerical
//     algorithms. Addison-Wesley.
//
// [3] Møller, O. (1965). Quasi double-precision in floating point addition. BIT Numerical
//     Mathematics, 5(1), 37–50. https://doi.org/10.1007/BF01975722

#define TwoSum416(x,y,z,zz,w,v,z1,z2) \
do { \
    z = x + y; \
    w = z - x; \
    z1 = y - w; \
    v = z - w; \
    z2 = v - x; \
    zz = z1 - z2; \
} while (0)

#ifdef __cplusplus
extern "C" {
#endif

    float bsf_flt_sum(const float* pdata, int start, int length);
    float bsf_flt_sum_pairwise_linz(const float* pdata, int start, int length);
    float bsf_flt_sum_pairwise_wikipedia(const float* pdata, int start, int length);
    float bsf_flt_sum_pairwise_wikipedia2(const float* pdata, int start, int length);
    float bsf_flt_sum_kahan(const float* pdata, int start, int length);
    float bsf_flt_sum_kahan_muller(const float* pdata, int start, int length);
    float bsf_flt_sum_kahan_muller_fast2sum(const float* pdata, int start, int length);
    float bsf_flt_sum_priest_comp(const float* pdata, int start, int length);
    float bsf_flt_sum_priest_dcomp(const float* pdata, int start, int length);
    float bsf_flt_sum_priest_dcomp_fast2sum(const float* pdata, int start, int length);
    float bsf_flt_sum_neumaier(const float* pdata, int start, int length);
    float bsf_flt_sum_neumaier_fast2sum(const float* pdata, int start, int length);
    float bsf_flt_sum_neumaier_twosum(const float* pdata, int start, int length);
    float bsf_flt_sum_neumaier_twosum2(const float* pdata, int start, int length);

    int bsf_flt_test1(void);

#ifdef __cplusplus
}
#endif

#endif
