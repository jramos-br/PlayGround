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

echo "Tests of g++ on `hostname` with `uname`"
date
echo

echo 'Which version is running?'
echo 'g++ --version'
echo
g++ --version
echo

echo '----- g++ with default options -----'
echo
echo 'g++ -Wall -Wextra -o stdc stdc.c'
if g++ -Wall -Wextra -o stdc stdc.c
then
    echo
    ./stdc
    echo
else
    echo '\nfailed\n\n'
fi

for opt in -ansi \
           -std=c++98 \
           -std=c++03 \
           -std=c++11 \
           -std=c++14 \
           -std=c++17 \
           -gnu=c++98 \
           -gnu=c++03 \
           -gnu=c++11 \
           -gnu=c++14 \
           -gnu=c++17
do
    echo '----- g++ with $opt -----'
    echo
    echo 'g++ $opt -Wall -Wextra -o stdc stdc.c'
    if g++ $opt -Wall -Wextra -o stdc stdc.c
    then
        echo
        ./stdc
        echo
    else
        echo '\nfailed\n\n'
    fi
done
