#!/bin/sh

set -e
autoreconf -fiv
./configure --prefix=`pwd`/build CFLAGS="-g -O0 -Wall" CXXFLAGS="-g -O0 -Wall" 
make
make check
make install

