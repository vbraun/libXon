#!/bin/sh

set -e
autoreconf -fiv
./configure CFLAGS="-g -O0" CXXFLAGS="-g -O0"
make
make check

