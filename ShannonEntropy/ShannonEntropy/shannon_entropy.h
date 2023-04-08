// The shannon_entropy.h header file.
//
// This file is part of the shannon_entropy.c program.
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

#ifndef _SHANNON_ENTROPY_H_E8F81456_C7FD_4332_9DBE_B9AA7005C61C_
#define _SHANNON_ENTROPY_H_E8F81456_C7FD_4332_9DBE_B9AA7005C61C_

#ifdef _MSC_VER
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

    double specific_entropy_nats(const char* s);
    double specific_entropy_nats_max(size_t len);

    double specific_entropy_bits(const char* s);
    double specific_entropy_bits_max(size_t len);

    double specific_entropy_normalized(const char* s);
    double specific_entropy_normalized_max(size_t len);

    double absolute_entropy_nats(const char* s);
    double absolute_entropy_nats_max(size_t len);

    double absolute_entropy_bits(const char* s);
    double absolute_entropy_bits_max(size_t len);

    double absolute_entropy_normalized(const char* s);
    double absolute_entropy_normalized_max(size_t len);

#ifdef __cplusplus
}
#endif

#endif
