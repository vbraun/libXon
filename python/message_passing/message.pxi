from libc.stdint cimport int32_t, int64_t
from libcpp.string cimport string
from libcpp cimport bool as cppbool


cdef extern from "xon/constants.h":
    cdef int XON_ELEMENT_DOUBLE
    cdef int XON_ELEMENT_STRING
    cdef int XON_ELEMENT_DOCUMENT
    cdef int XON_ELEMENT_ARRAY
    cdef int XON_ELEMENT_BINARY
    cdef int XON_ELEMENT_UNDEF_DEPRECATED
    cdef int XON_ELEMENT_OBJECT_ID
    cdef int XON_ELEMENT_BOOLEAN
    cdef int XON_ELEMENT_UTC_DATETIME
    cdef int XON_ELEMENT_NULL
    cdef int XON_ELEMENT_REGEX
    cdef int XON_ELEMENT_DBPOINTER_DEPRECATED
    cdef int XON_ELEMENT_JAVASCRIPT
    cdef int XON_ELEMENT_SYMBOL_DEPRECATED
    cdef int XON_ELEMENT_JAVASCRIPT_SCOPE
    cdef int XON_ELEMENT_INT32
    cdef int XON_ELEMENT_INT64
    cdef int XON_ELEMENT_TIME
    cdef int XON_ELEMENT_MIN_KEY
    cdef int XON_ELEMENT_MAX_KEY


cdef extern from "xon/object.hh" namespace "xon":
    
    cdef cppclass xon_object "xon::object"

    cdef cppclass xon_builder "xon::builder"
    cdef cppclass xon_obj_builder "xon::obj_builder"

    cdef cppclass xon_reader "xon::reader"
    cdef cppclass xon_obj_reader "xon::obj_reader"

    cdef cppclass xon_object:
        xon_object(void *data)
        xon_object(xon_object)
        size_t size()
        bint is_bson()
        bint is_host_arch()
        xon_obj_reader read_obj()
        void* pointer()
        string string(bint show_address)
        void hexdump()

    cdef cppclass xon_builder:
        xon_builder& add_cstr(string key, char* value)
        xon_builder& add_string(string key, string value)
        xon_builder& add_double(string key, float value)
        xon_builder& add_int32(string key, int32_t value)
        xon_builder& add_int64(string key, int64_t value)
        xon_builder& add_bool(string key, bint value)
        xon_object get()
        xon_object* get_new()

    cdef cppclass xon_obj_builder(xon_builder):
        pass
        
    cdef cppclass xon_reader:
        xon_reader()
        xon_reader(xon_object&)
        int count()
        bint has_key(string key)
        string key(int pos)
        int find(string key)
        int type(string key)
        int type(int pos)
        string  get_string(string key)
        string  get_string(int pos)
        float   get_double(string key)
        float   get_double(int pos)
        int32_t get_int32 (string key)
        int32_t get_int32 (int pos)
        int64_t get_int64 (string key)
        int64_t get_int64 (int pos)
        bint    get_bool  (string key)
        bint    get_bool  (int pos)
        
    cdef cppclass xon_obj_reader(xon_reader):
        xon_obj_reader(xon_object&)
        



cdef extern from "xon/client.hh" namespace "xon":
    
    cdef cppclass xon_client "xon::client":
        xon_client(string command)
        void send(xon_object obj)
        xon_object receive()
        void wait(float timeout)
        void kill()

    
