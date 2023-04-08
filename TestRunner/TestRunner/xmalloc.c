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

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#define strdup _strdup
#endif

#include "xmalloc.h"

/* Writes diagnostic information to stderr and calls exit(EXIT_FAILURE). */
static void xabort(void)
{
    if (errno == 0)
        errno = ENOMEM;

    perror(NULL);
    exit(EXIT_FAILURE);
}

/* Allocates "size" bytes and returns a pointer to the allocated memory. */
void* xmalloc(size_t size)
{
    errno = 0;
    void* ptr = malloc(size);

    if (ptr == NULL && size != 0)
        xabort();

    return ptr;
}

/* Allocates memory for an array of "num" elements of "size" bytes each and
   returns a pointer to the allocated memory. */
void* xcalloc(size_t num, size_t size)
{
    errno = 0;
    void* ptr = calloc(num, size);

    if (ptr == NULL && num != 0 && size != 0)
        xabort();

    return ptr;
}

/* Changes the size of the memory block pointed to by "ptr" to "size" bytes. */
void* xrealloc(void* ptr, size_t size)
{
    errno = 0;
    void* newptr = realloc(ptr, size);

    if (newptr == NULL && size != 0)
        xabort();

    return newptr;
}

/* Frees the memory space pointed to by "ptr". */
void xfree(void* ptr)
{
    free(ptr);
}

/* Returns a pointer to a new string which is a duplicate of the string "str".
   Memory for the new string is obtained with "malloc()". */
char* xstrdup(const char* str)
{
    errno = 0;
    char* newstr = strdup(str);

    if (newstr == NULL)
        xabort();

    return newstr;
}
