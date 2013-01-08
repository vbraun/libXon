#!/usr/bin/env python

import os
os.putenv('CFLAGS', '-I../build/include')
os.putenv('LDFLAGS', '-L../build/lib')
#os.putenv('CFLAGS', 'aoeueuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu')
os.system('python setup.py build_ext --inplace')

import sys
sys.path.append('.')

from message_passing.message import binary_object
