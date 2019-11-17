// The CreateGuid program.
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

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "Rpcrt4.lib")

// Calculates the Shannon entropy of a string
// http://en.wikipedia.org/wiki/Entropy_(information_theory)
double shannon_entropy(const char* s)
{
    int len = (int)strlen(s);
    int map[256] = { 0 };
    int hist[257];
    int histlen = 0;

    for (int i = 0; i < len; ++i)
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

    double result = 0;

    for (int i = 1; i <= histlen; ++i)
    {
        double frequency = (double)hist[i] / len;
        result -= frequency * log(frequency);
    }

    return result * M_LOG2E;
}

// Calculates the ideal Shannon entropy of a string with given length
double shannon_entropy_max(const char* s)
{
    int len = (int)strlen(s);
    double prob = 1.0 / len;
    double result = -1.0 * len * prob * log(prob) * M_LOG2E;
    return result;
}

int _tmain(int argc, LPTSTR argv[])
{
    RPC_WSTR lpszStringUuid;
    double entropy = 0;
    UUID uuid;
    RPC_STATUS status;
    char szUuid[40];

    for (;;)
    {
        status = UuidCreate(&uuid);

        if (status != RPC_S_OK)
        {
            _ftprintf_s(stderr, _T("UuidCreate: ERROR %d 0x%08X\n"), status, status);
            return EXIT_FAILURE;
        }

        status = UuidToString(&uuid, &lpszStringUuid);

        if (status != RPC_S_OK)
        {
            _ftprintf_s(stderr, _T("UuidToString: ERROR %d 0x%08X\n"), status, status);
            return EXIT_FAILURE;
        }

        _snprintf_s(szUuid, _countof(szUuid), "%ls", (LPCWSTR)lpszStringUuid);

        status = RpcStringFree(&lpszStringUuid);

        if (status != RPC_S_OK)
        {
            _ftprintf_s(stderr, _T("RpcStringFree: ERROR %d 0x%08X\n"), status, status);
            return EXIT_FAILURE;
        }

        double e = shannon_entropy(szUuid);

        if (e > entropy)
        {
            entropy = e;
            _ftprintf_s(stdout, _T("%hs %f %f\n"), szUuid, e, shannon_entropy_max(szUuid));
        }
    }

    return EXIT_SUCCESS;
}
