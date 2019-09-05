// The shannon_entropy.c program.
//
// This program calculates the Shannon Entropy of a given string.
//
// References:
// http://en.wikipedia.org/wiki/Entropy_(information_theory)
// https://stackoverflow.com/a/34924195/1320384
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

#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

// Calculates the specific entropy of a 8-bit character string.
// Returns nats/symbol (uses natural log), histogram length and string length.
static double specific_entropy_nats_base(const char* s, int* phistlen, size_t* pstrlen)
{
    size_t len = strlen(s);

    if (pstrlen != NULL)
    {
        *pstrlen = len;
    }

    int map[256] = { 0 };
    int hist[257];
    int histlen = 0;

    for (size_t i = 0; i < len; ++i)
    {
        int ch = (unsigned char)s[i];

        if (map[ch] == 0)
        {
            hist[map[ch] = ++histlen] = 1;
        }
        else
        {
            ++hist[map[ch]];
        }
    }

    if (phistlen != NULL)
    {
        *phistlen = histlen;
    }

    double result = 0.0;

    for (int i = 1; i <= histlen; ++i)
    {
        double frequency = (double)hist[i] / (double)len;
        result -= frequency * log(frequency);
    }

    return result;
}

// Calculates the specific entropy of a 8-bit character string.
// Returns nats/symbol (uses natural log).
double specific_entropy_nats(const char* s)
{
    return specific_entropy_nats_base(s, NULL, NULL);
}

// Calculates the maximum specific entropy of a 8-bit character string.
// Returns nats/symbol (uses natural log).
double specific_entropy_nats_max(size_t len)
{
    if (len > 1)
    {
        double frequency = 1.0 / len;
        double result = -(double)len * frequency * log(frequency);
        return result;
    }

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

// Calculates the specific entropy of a 8-bit character string.
// Returns bits/symbol (uses base 2 log), histogram length and string length.
static double specific_entropy_bits_base(const char* s, int* phistlen, size_t* pstrlen)
{
    return specific_entropy_nats_base(s, phistlen, pstrlen) * M_LOG2E;
}

// Calculates the specific entropy of a 8-bit character string.
// Returns bits/symbol (uses base 2 log).
double specific_entropy_bits(const char* s)
{
    return specific_entropy_nats_base(s, NULL, NULL) * M_LOG2E;
}

// Calculates the maximum specific entropy of a 8-bit character string.
// Returns bits/symbol (uses base 2 log).
double specific_entropy_bits_max(size_t len)
{
    return specific_entropy_nats_max(len) * M_LOG2E;
}

////////////////////////////////////////////////////////////////////////////////

// Calculates the normalized specific entropy of a 8-bit character string.
// Returns entropy/symbol, ranging from 0 to 1, where 1 means each symbol
// occurred equally often.
double specific_entropy_normalized(const char* s)
{
    int histlen;
    double result = specific_entropy_nats_base(s, &histlen, NULL);
    return histlen > 1 ? result / log((double)histlen) : 0.0;
}

// Calculates the maximum normalized specific entropy of a 8-bit character string.
// Returns 0 if len is zero else 1.
double specific_entropy_normalized_max(size_t len)
{
    return len > 1 ? specific_entropy_nats_max(len) / log((double)len) : len > 0 ? 1.0 : 0.0;
}

////////////////////////////////////////////////////////////////////////////////

// Calculates the absolute entropy of a 8-bit character string.
// Returns nats (uses natural log) and histogram length.
static double absolute_entropy_nats_base(const char* s, int* phistlen)
{
    size_t len;
    double result = specific_entropy_nats_base(s, phistlen, &len);
    return result * len;
}

// Calculates the absolute entropy of a 8-bit character string.
// Returns nats (uses natural log).
double absolute_entropy_nats(const char* s)
{
    size_t len;
    double result = specific_entropy_nats_base(s, NULL, &len);
    return result * len;
}

// Calculates the maximum absolute entropy of a 8-bit character string.
// Returns nats (uses natural log).
double absolute_entropy_nats_max(size_t len)
{
    return specific_entropy_nats_max(len) * len;
}

////////////////////////////////////////////////////////////////////////////////

// Calculates the absolute entropy of a 8-bit character string.
// Returns bits (uses base 2 log) and histogram length.
static double absolute_entropy_bits_base(const char* s, int* phistlen)
{
    size_t len;
    double result = specific_entropy_bits_base(s, phistlen, &len);
    return result * len;
}

// Calculates the absolute entropy of a 8-bit character string.
// Returns bits (uses base 2 log).
double absolute_entropy_bits(const char* s)
{
    size_t len;
    double result = specific_entropy_bits_base(s, NULL, &len);
    return result * len;
}

// Calculates the maximum absolute entropy of a 8-bit character string.
// Returns bits (uses base 2 log).
double absolute_entropy_bits_max(size_t len)
{
    return specific_entropy_bits_max(len) * len;
}

////////////////////////////////////////////////////////////////////////////////

// Calculates the normalized absolute entropy of a 8-bit character string.
// Returns entropy, ranging from 0 to the string length.
double absolute_entropy_normalized(const char* s)
{
    int histlen;
    double result = absolute_entropy_nats_base(s, &histlen);
    return histlen > 1 ? result / log((double)histlen) : 0.0;
}

// Calculates the maximum normalized absolute entropy of a 8-bit character string.
// Returns the string length.
double absolute_entropy_normalized_max(size_t len)
{
    return len > 1 ? absolute_entropy_nats_max(len) / log((double)len) : len > 0 ? 1.0 : 0.0;
}
