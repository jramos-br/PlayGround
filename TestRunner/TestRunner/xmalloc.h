/*
 * Memory allocation functions, with error checking.
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    void* xmalloc(size_t size);
    void* xcalloc(size_t num, size_t size);
    void* xrealloc(void* ptr, size_t size);
    void xfree(void* ptr);
    char* xstrdup(const char* str);

#ifdef __cplusplus
}
#endif
