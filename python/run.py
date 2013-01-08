#!/usr/bin/env python

import os
os.putenv('CFLAGS', '-I../build/include  -fno-strict-aliasing')
os.putenv('LDFLAGS', '-L../build/lib -Wl,-rpath -Wl,'+os.path.abspath('../build/lib'))
#os.putenv('CFLAGS', 'aoeueuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu')
os.system('python setup.py build_ext --inplace')


import sys
sys.path.append('.')

from message_passing.message import BinaryObject, Client


bob = BinaryObject({
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


exec_pipe = Client(os.path.abspath('../build/bin/xon_server_exec_pipe'))

exec_pipe.send({
            "command": "cat",
            "stdin": "hello world"})
print exec_pipe.receive()

exec_pipe.send({
            "command": "bc",
            "stdin": "2^3^4\n"})
print exec_pipe.receive()

exec_pipe.send({
            "command": "bc",
            "stdin": "2^3^4^5\n"})
print exec_pipe.receive()

exec_pipe.send({"quit": True})
exec_pipe.wait()




speed_test = Client(os.path.abspath('../build/bin/xon_server_speed_test'))
for i in xrange(1000):
    speed_test.send({"x": i})
    reply = speed_test.receive()
    assert reply["result"] == i**2

speed_test.send({"quit": True})
speed_test.wait()
