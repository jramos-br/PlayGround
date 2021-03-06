// The Combinatorics program.
//
// This program demonstrates several algorithms to deal with combinations
// and permutations.
//
// Algorithms and portions of the source code used in this software are
// publicly available on the Internet and at any major bookstore. Where
// applicable, links are provided to online resources.
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

#include <tchar.h>
#include "choose.h"
#include "broadhurst.h"
#include "nlucaroni.h"
#include "cppitertools.h"

int _tmain(int argc, _TCHAR* argv[])
{
    broadhurst();
    nlucaroni();
    cppitertools();
    return 0;
}
