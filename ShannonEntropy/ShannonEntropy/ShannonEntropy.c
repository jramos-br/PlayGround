// The ShannonEntropy program.
//
// This program calculates the Shannon Entropy of a given string.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shannon_entropy.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    char s[1024];

    while (fgets(s, _countof(s), stdin) != NULL)
    {
        double e, m;
        size_t len = strlen(s);

        if (len > 0 && s[len - 1] == '\n')
        {
            s[--len] = '\0';
        }

        printf("s=[%s] len=%zu\n", s, len);

        e = specific_entropy_nats(s);
        m = specific_entropy_nats_max(len);
        printf("specific_entropy_nats=%f max=%f\n", e, m);

        e = specific_entropy_bits(s);
        m = specific_entropy_bits_max(len);
        printf("specific_entropy_bits=%f max=%f\n", e, m);

        e = specific_entropy_normalized(s);
        m = specific_entropy_normalized_max(len);
        printf("specific_entropy_normalized=%f max=%f\n", e, m);

        e = absolute_entropy_nats(s);
        m = absolute_entropy_nats_max(len);
        printf("absolute_entropy_nats=%f max=%f\n", e, m);

        e = absolute_entropy_bits(s);
        m = absolute_entropy_bits_max(len);
        printf("absolute_entropy_bits=%f max=%f\n", e, m);

        e = absolute_entropy_normalized(s);
        m = absolute_entropy_normalized_max(len);
        printf("absolute_entropy_normalized=%f max=%f\n", e, m);
    }

    return 0;
}
