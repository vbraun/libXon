#!/bin/sh

set -e
autoreconf -fiv
./configure CFLAGS="-g -O0 -Wall" CXXFLAGS="-g -O0 -Wall"
make
make check

