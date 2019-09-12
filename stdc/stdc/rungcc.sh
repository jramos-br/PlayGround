#!/bin/sh

# This file is part of the stdc program.
#
# Prints the language conformance and available features according to the
# predefined macros.
#
# Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
#
# This is free software. Redistribution and use in source and binary forms,
# with or without modification, for any purpose and with or without fee are
# hereby permitted. Altered source versions must be plainly marked as such.
#
# If you find this software useful, an acknowledgment would be appreciated
# but is not required.
#
# THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
# FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.

echo "Tests of gcc on `hostname` with `uname`"
date
echo

echo 'Which version is running?'
echo 'gcc --version'
echo
gcc --version
echo

echo '----- gcc with default options -----'
echo
echo 'gcc -Wall -Wextra -o stdc stdc.c'
if gcc -Wall -Wextra -o stdc stdc.c
then
    echo
    ./stdc
    echo
else
    echo '\nfailed\n\n'
fi

for opt in -ansi \
           -std=c89 \
           -std=gnu89 \
           -std=c90 \
           -std=gnu90 \
           -std=iso9899:1990 \
           -std=iso9899:199409 \
           -std=c99 \
           -std=gnu99 \
           -std=iso9899:1999 \
           -std=c11 \
           -std=iso9899:2011 \
           -std=c17 \
           -std=iso9899:2017 \
           -std=gnu17
do
    echo "----- gcc with $opt -----"
    echo
    echo gcc $opt -Wall -Wextra -o stdc stdc.c
    if gcc $opt -Wall -Wextra -o stdc stdc.c
    then
        echo
        ./stdc
        echo
    else
        echo '\nfailed\n\n'
    fi
done
