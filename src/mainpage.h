/** @mainpage LibXon Documentation
*
* @authors Volker Braun <vbraun.name@gmail.com>
*
*
* @section intro Introduction
*
* LibXon (pronounce as "lib-Axon") is a message passing library
* together with some functionality to launch remote processes. Its
* fundamental goals are portability and minimal dependencies on the
* server side. Communication is over TCP sockets and communication
* overhead and latency is of that order; about 50 microseconds
* round-trip.
*
*
* @section msg Binary Messages and the Wire Data Format
*
* A message in libXon is very much similar to <a
* href="http://bsonspec.org/#/specification">BSON</a> and consists of
* a set of key/value pairs. The keys are zero-terminated strings and
* the allowed values are the few data types that BSON
* understands. LibXon's native object format, named XON in the
* following, is BSON with all fields aligned and padded to machine bit
* width. Integers in BSON are big endian, integers in XON are host
* order.
*
* Special significance have the first four bytes, which are a network
* byte order (big endian) integer, that is, C99 int32_t. In BSON, it
* is positive and equal to the length of the BSON object in bytes. In
* the native XON format, it is a negative integer unique to the
* machine architecture, that is, it encodes endianness and bit
* width. This is then padded to machine bit width. The following
* integer is the length in bytes.
*
* Clearly, the XON format is meant to transfer data between the same
* architectures. An error is guaranteed to be raised if you input a
* XON message from the wrong architecture. For cross-platform
* communication, BSON is also an allowed message format. 
*
* @todo BSON is not yet implemented
* 
*
* @section writer Creating Binary Messages
*
* A message is always an obpaque pointer of type xon_obj in the C API
* and of type xon::object in the C++ API. Messages are immutable. To
* create them, you need to use a special object builder and to decode
* them, an object reader. In the C API, you create an object as follows:
*
* \code{.c}
*   xon_obj_builder xb = xon_obj_builder_new();
*   xon_obj_builder_add_string(xb, "key",  "value");
*   xon_obj_builder_add_string(xb, "key2", "value2");
*   xon_obj_builder_add_double(xb, "float", 3.1415);
*   xon_obj_builder_add_int32 (xb, "int",   123);
*   xon_obj_builder_add_int64 (xb, "large", (int64_t)(1) << 40);
*   xon_obj obj = xon_obj_builder_get(xb);
*   xon_obj_builder_free(xb);
*   printf("First object\n");
*   xon_obj_print(obj);
*   free(obj);
* \endcode
*
* Note that all C API functions start with %xon_obj. This is the
* equivalent of the C++ namespace xon. You can use the builder to
* create any number of objects, it resets after you call
* xon_obj_builder_get to get the binary object. But you must call
* xon_obj_builder_free on it when you are finished or you will be
* leaking memory. The object itself is just a malloc'ed memory block,
* and you have to free it with the libc free(). The C++ analog of the
* above code is:
* 
* \code{.cpp}
*   xon::obj_builder xb;
*   xb.add("key",  "value");
*   xb.add("key2", "value2");
*   xb.add("float", 3.1415);
*   xb.add("int",   123);
*   xb.add("large", (int64_t)(1) << 40);
*   xon::object obj(xb);
*   std::cout << "First object" << std::endl << obj;
* \endcode
*
* The C++ xon::object class is a reference-counted pointer to the
* memory block of the libXon message. Copies of the object will refer
* to the same (immutable) binary object. You do not have to delete
* anything manually (unless you use C++ new, of course), the
* destructors clean up for you.
*
* 
* @section reader Decoding Binary Messages
*
* @section client Sending Messages to a Server 
*
*
* @section server Existing Servers
*
* @section server Implementing Your Own Server
*
*
*
* @section notes Release Notes
* - Version 1.0:
*   Not released yet
*
* @section requirements Requirements
*
* - C99 compiler (with support for e.g. int32_t integer types)
* - C++98 compiler (standard library, exceptions, 
*
* 
*/

