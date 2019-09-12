/*
 * The stdc program.
 *
 * Prints the language conformance and available features according to the
 * predefined macros.
 *
 * Microsoft compilers:
 *
 * - Use /TP to compile a .c file as C++ code.
 * - Use /Za to disable Microsoft extensions that aren't compatible with ANSI
 *   C89/ISO C90.
 * - Use /std to enable C++ language features from a specific version of the
 *   C++ language standard.
 * - Use /Zc:__cplusplus to see the correct value of the __cplusplus macro
 *   (Visual Studio 2017 version 15.7).
 *
 * gcc compilers:
 *
 * - Use gcc to compile as C code.
 * - Use g++ to compile as C++ code.
 * - Use rung++.sh and rungcc.sh to compile and run.
 *
 * Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
 *
 * This is free software. Redistribution and use in source and binary forms,
 * with or without modification, for any purpose and with or without fee are
 * hereby permitted. Altered source versions must be plainly marked as such.
 *
 * If you find this software useful, an acknowledgment would be appreciated
 * but is not required.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
 * FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.
 */

#include <stdio.h>

#ifdef _MSC_VER /* MSVC compiler */

void identify()
{
    /* Microsoft Visual C++ */
    /* https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B */

    /* Predefined macros */
    /* https://docs.microsoft.com/pt-br/cpp/preprocessor/predefined-macros */

    switch (_MSC_VER)
    {
    case  100: printf("_MSC_VER %d - MSC 1.0\n", _MSC_VER); break;
    case  200: printf("_MSC_VER %d - MSC 2.0\n", _MSC_VER); break;
    case  300: printf("_MSC_VER %d - MSC 3.0\n", _MSC_VER); break;
    case  400: printf("_MSC_VER %d - MSC 4.0\n", _MSC_VER); break;
    case  500: printf("_MSC_VER %d - MSC 5.0\n", _MSC_VER); break;
    case  600: printf("_MSC_VER %d - MSC 6.0\n", _MSC_VER); break;
    case  700: printf("_MSC_VER %d - MSC 7.0\n", _MSC_VER); break;
    case  800: printf("_MSC_VER %d - MSVC++ 1.0\n", _MSC_VER); break;
    case  900: printf("_MSC_VER %d - MSVC++ 2.0\n", _MSC_VER); break;
    case 1000: printf("_MSC_VER %d - MSVC++ 4.0 (Developer Studio 4.0)\n", _MSC_VER); break;
    case 1020: printf("_MSC_VER %d - MSVC++ 4.2 (Developer Studio 4.2)\n", _MSC_VER); break;
    case 1100: printf("_MSC_VER %d - MSVC++ 5.0 (Visual Studio 97 version 5.0)\n", _MSC_VER); break;
    case 1200: printf("_MSC_VER %d - MSVC++ 6.0 (Visual Studio 6.0 version 6.0)\n", _MSC_VER); break;
    case 1300: printf("_MSC_VER %d - MSVC++ 7.0 (Visual Studio .NET 2002 version 7.0)\n", _MSC_VER); break;
    case 1310: printf("_MSC_VER %d - MSVC++ 7.1 (Visual Studio .NET 2003 version 7.1)\n", _MSC_VER); break;
    case 1400: printf("_MSC_VER %d - MSVC++ 8.0 (Visual Studio 2005 version 8.0)\n", _MSC_VER); break;
    case 1500: printf("_MSC_VER %d - MSVC++ 9.0 (Visual Studio 2008 version 9.0)\n", _MSC_VER); break;
    case 1600: printf("_MSC_VER %d - MSVC++ 10.0 (Visual Studio 2010 version 10.0)\n", _MSC_VER); break;
    case 1700: printf("_MSC_VER %d - MSVC++ 11.0 (Visual Studio 2012 version 11.0)\n", _MSC_VER); break;
    case 1800: printf("_MSC_VER %d - MSVC++ 12.0 (Visual Studio 2013 version 12.0)\n", _MSC_VER); break;
    case 1900: printf("_MSC_VER %d - MSVC++ 14.0 (Visual Studio 2015 version 14.0)\n", _MSC_VER); break;
    case 1910: printf("_MSC_VER %d - MSVC++ 14.1 (Visual Studio 2017 version 15.0)\n", _MSC_VER); break;
    case 1911: printf("_MSC_VER %d - MSVC++ 14.11 (Visual Studio 2017 version 15.3)\n", _MSC_VER); break;
    case 1912: printf("_MSC_VER %d - MSVC++ 14.12 (Visual Studio 2017 version 15.5)\n", _MSC_VER); break;
    case 1913: printf("_MSC_VER %d - MSVC++ 14.13 (Visual Studio 2017 version 15.6)\n", _MSC_VER); break;
    case 1914: printf("_MSC_VER %d - MSVC++ 14.14 (Visual Studio 2017 version 15.7)\n", _MSC_VER); break;
    case 1915: printf("_MSC_VER %d - MSVC++ 14.15 (Visual Studio 2017 version 15.8)\n", _MSC_VER); break;
    case 1916: printf("_MSC_VER %d - MSVC++ 14.16 (Visual Studio 2017 version 15.9)\n", _MSC_VER); break;
    case 1920: printf("_MSC_VER %d - MSVC++ 14.2 (Visual Studio 2019 Version 16.0)\n", _MSC_VER); break;
    case 1921: printf("_MSC_VER %d - MSVC++ 14.21 (Visual Studio 2019 Version 16.1)\n", _MSC_VER); break;
    case 1922: printf("_MSC_VER %d - MSVC++ 14.22 (Visual Studio 2019 Version 16.2)\n", _MSC_VER); break;

    default: printf("_MSC_VER %04d MSVC compiler\n", _MSC_VER); break;
    }

#ifdef __cplusplus

    /* C++ code */
    /* https://pt.wikipedia.org/wiki/C%2B%2B */

    /* MSVC now correctly reports __cplusplus */
    /* https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/ */

    /* The MSVC compiler does not, and never will, support a C++11, C++03, or */
    /*  C++98 standards version switch. */

#ifdef _MSVC_LANG
#if _MSVC_LANG == __cplusplus /* /Zc:__cplusplus */
#define USE_CPLUSPLUS
#else
#define USE_MSVC_LANG
#endif
#else
#define USE_CPLUSPLUS
#endif

#ifdef USE_MSVC_LANG

    switch (_MSVC_LANG)
    {
    case 199711L:
        /* The 1998 C++ standard */
        /* The 2003 C++ standard */
        /* !!! _MSVC_LANG started with C++14 !!! */
        printf("_MSVC_LANG %ld - C++98/C++03 language (unexpected value, _MSVC_LANG started with C++14)\n", _MSVC_LANG);
        break;
    case 201103L:
        /* The 2011 C++ standard (unused by MSVC compilers) */
        /* !!! _MSVC_LANG started with C++14 !!! */
        printf("_MSVC_LANG %ld - C++11 language (unexpected value - _MSVC_LANG started with C++14)\n", _MSVC_LANG);
        break;
    case 201402L:
        /* The 2014 C++ standard */
        printf("_MSVC_LANG %ld - C++14 language\n", _MSVC_LANG);
        break;
    case 201703L:
        /* The 2017 C++ standard */
        printf("_MSVC_LANG %ld - C++17 language\n", _MSVC_LANG);
        break;
    default:
        /* Other versions (experimental proposals, new features, etc) */
        printf("_MSVC_LANG %ld - C++ language\n", _MSVC_LANG);
        break;
    }

#endif /* USE_MSVC_LANG */

#ifdef USE_CPLUSPLUS

    switch (__cplusplus)
    {
    case 199711L:
        /* The 1998 C++ standard */
        /* The 2003 C++ standard */
#if _MSC_VER < 1600 /* VS2010 */
        printf("__cplusplus %ld - C++98/C++03 language\n", __cplusplus);
        break;
#else
        printf("__cplusplus %ld - C++98/C++03 language (partial support for C++11)\n", __cplusplus);
        break;
#endif
    case 201103L:
        /* The 2011 C++ standard (unused by MSVC compilers) */
        printf("__cplusplus %ld - C++11 language (unexpected value - unused by MSVC compilers)\n", __cplusplus);
        break;
    case 201402L:
        /* The 2014 C++ standard */
        printf("__cplusplus %ld - C++14 language\n", __cplusplus);
        break;
    case 201703L:
        /* The 2017 C++ standard */
        printf("__cplusplus %ld - C++17 language\n", __cplusplus);
        break;
    default:
        /* Other versions (experimental proposals, new features, etc) */
        printf("__cplusplus %ld - C++ language\n", __cplusplus);
        break;
    }

#endif /* USE_CPLUSPLUS */

#else /* __cplusplus */

    /* C code */
    /* https://en.wikipedia.org/wiki/C_(programming_language) */
    /* https://en.wikipedia.org/wiki/ANSI_C */

    /* No mention of C compliance */
    /* https://github.com/MicrosoftDocs/cpp-docs/issues/407 */

    /* Reader Q&A: What about VC++ and C99? */
    /* https://herbsutter.com/2012/05/03/reader-qa-what-about-vc-and-c99/ */

    /* The C compiler supports C89/C90 plus some C99 features. */

#ifdef __STDC_VERSION__

    /* Currently (VS2017) the C compiler supports only C89/C90, so */
    /*  __STDC_VERSION__ is not defined. Maybe VS2019 will support C11. */

    switch (__STDC_VERSION__)
    {
    case 199409L:
        /* The 1989/1990 C standard as amended in 1994 */
        printf("__STDC_VERSION__ %ld - C95 language\n", __STDC_VERSION__);
        break;
    case 199901L:
        /* The 1999 revision of the C standard */
        printf("__STDC_VERSION__ %ld - C99 language\n", __STDC_VERSION__);
        break;
    case 201112L:
        /* The 2011 revision of the C standard */
        printf("__STDC_VERSION__ %ld - C11 language\n", __STDC_VERSION__);
        break;
    case 201710L:
        /* The 2017 revision of the C standard */
        printf("__STDC_VERSION__ %ld - C18 language\n", __STDC_VERSION__);
        break;
    default:
        /* Other versions (experimental proposals, new features, etc) */
        printf("__STDC_VERSION__ %ld - C language\n", __STDC_VERSION__);
        break;
    }

#else /* __STDC_VERSION__ */

#ifdef __STDC__

    /* The ANSI C89/ISO C90 standard */
    printf("__STDC__ %d - C89/C90 language\n", __STDC__);

#else /* __STDC__ */

    /* The ANSI C89/ISO C90 standard with Microsoft extensions */
    printf("__STDC__ undefined - C89/C90 language plus some C99 features (Microsoft extensions)\n");

#endif /* __STDC__ */

#endif /* __STDC_VERSION__ */

#endif /* __cplusplus */

}

#endif /* _MSC_VER */

#ifdef __GNUC__

void identify()
{
    /* GNU Compiler Collection */
    /* https://en.wikipedia.org/wiki/GNU_Compiler_Collection */
    /* https://www.gnu.org/software/gcc/releases.html */

    /* Predefined macros */
    /* https://gcc.gnu.org/onlinedocs/cpp/Predefined-Macros.html */

#ifdef __GNUC_MINOR__
#ifdef __GNUC_PATCHLEVEL__
    printf("GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif
#endif

#ifdef __cplusplus

    /* C++ code */
    /* https://pt.wikipedia.org/wiki/C%2B%2B */
    /* https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html */

    /* Bug 1773 - __cplusplus defined to 1, should be 199711L */
    /* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=1773 */

    switch (__cplusplus)
    {
    case 1:
        /* Unspecified version (see Bug 1773) */
        printf("__cplusplus %d - C++98 language (not fully conforming)\n", __cplusplus);
        break;
    case 199707L:
        /* Earlier proposal of the C++98 standard */
        printf("__cplusplus %d - C++98 language (earlier proposal)\n", __cplusplus);
        break;
    case 199711L:
        /* The 1998 C++ standard */
        /* The 2003 C++ standard */
        printf("__cplusplus %d - C++98/C++03 language\n", __cplusplus);
        break;
    case 201103L:
        /* The 2011 C++ standard */
        printf("__cplusplus %d - C++11 language\n", __cplusplus);
        break;
    case 201402L:
        /* The 2014 C++ standard */
        printf("__cplusplus %d - C++14 language\n", __cplusplus);
        break;
    case 201703L:
        /* The 2017 C++ standard */
        printf("__cplusplus %d - C++17 language\n", __cplusplus);
        break;
    default:
        /* Other versions (experimental proposals, new features, etc) */
        printf("__cplusplus %d - C++ language\n", __cplusplus);
        break;
    }

#else /* __cplusplus */

    /* C code */
    /* https://en.wikipedia.org/wiki/C_(programming_language) */
    /* https://en.wikipedia.org/wiki/ANSI_C */

#ifdef __STDC_VERSION__

    switch (__STDC_VERSION__)
    {
    case 1:
        /* Unspecified version */
        printf("__STDC_VERSION__ %ld - C language\n", __STDC_VERSION__);
        break;
    case 199409L:
        /* The 1989/1990 C standard as amended in 1994 */
        printf("__STDC_VERSION__ %ld - C95 language\n", __STDC_VERSION__);
        break;
    case 199901L:
        /* The 1999 revision of the C standard */
        printf("__STDC_VERSION__ %ld - C99 language\n", __STDC_VERSION__);
        break;
    case 201112L:
        /* The 2011 revision of the C standard */
        printf("__STDC_VERSION__ %ld - C11 language\n", __STDC_VERSION__);
        break;
    case 201710L:
        /* The 2017 revision of the C standard */
        printf("__STDC_VERSION__ %ld - C18 language\n", __STDC_VERSION__);
        break;
    default:
        /* Other versions (experimental proposals, new features, etc) */
        printf("__STDC_VERSION__ %ld - C language\n", __STDC_VERSION__);
        break;
    }

#else /* __STDC_VERSION__ */

#ifdef __STDC__

    switch (__STDC__)
    {
    case 0:
        /* The ANSI C89/ISO C90 standard (non-strict conformance to the C Standard) */
        printf("__STDC__ %d - C89/C90 language (non-strict conformance)\n", __STDC__);
        break;
    case 1:
        /* The ANSI C89/ISO C90 standard */
        printf("__STDC__ %d - C89/C90 language\n", __STDC__);
        break;
    default:
        /* The ANSI C89/ISO C90 standard */
        printf("__STDC__ %d - C89/C90 language\n", __STDC__);
        break;
    }

#else /* __STDC__ */

    /* The K&R C */
    printf("__STDC__ undefined - C program\n");

#endif /* __STDC__ */

#endif /* __STDC_VERSION__ */

#endif /* __cplusplus*/

}

#endif /* __GNUC__ */

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    identify();
    return 0;
}
