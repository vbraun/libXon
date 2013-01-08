#!/usr/bin/env python

import os
os.putenv('CFLAGS', '-I../build/include  -fno-strict-aliasing')
os.putenv('LDFLAGS', '-L../build/lib -Wl,-rpath -Wl,'+os.path.abspath('../build/lib'))
#os.putenv('CFLAGS', 'aoeueuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu')
os.system('python setup.py build_ext --inplace')


import sys
sys.path.append('.')

from message_passing.message import binary_object


bob = binary_object({
        "key_string": "value",
        "key_number": 123,
        "key_bool": True
        })

print bob

print "number of entries:", len(bob)

print bob.has_key("key_string"), bob.has_key("key_number"), \
    bob.has_key("key_bool"), bob.has_key("foo_bar")
print bob["key_string"], bob["key_number"], bob["key_bool"]
print bob.get("key_string"), bob.get("key_number"), \
    bob.get("key_bool"), bob.get("foo_bar")

bob.hexdump()
