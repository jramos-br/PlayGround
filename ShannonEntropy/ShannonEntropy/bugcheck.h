/*
 * The bugcheck C/C++ macro.
 *
 * This header file defines the C/C++ preprocessor macro bugcheck().
 *
 * bugcheck() implements a runtime assertion similar to assert(), which can
 * be used to verify a programmer assumption and print a diagnostic message
 * if the assumption is false.
 *
 * Unlike assert(), bugcheck() does not depend upon conditional compilation
 * and always works the same way, regardless of how the program is compiled.
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

#ifndef _BUGCHECK_H_6DE44ED5_F66A_48E7_BD12_218ADCC885A0_
#define _BUGCHECK_H_6DE44ED5_F66A_48E7_BD12_218ADCC885A0_

#ifdef _MSC_VER
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /* Writes diagnostic information to stderr and calls exit(EXIT_FAILURE). */
    void bugcheck_(const char* expression, const char* file, int line);

#ifdef __cplusplus
}
#endif

/* If "expression" is false, writes diagnostic information to stderr and calls exit(EXIT_FAILURE). */
#define bugcheck(expression) \
    (void)((!!(expression)) || (bugcheck_(#expression, __FILE__, __LINE__), 0))

#endif
