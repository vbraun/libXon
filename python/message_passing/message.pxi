from libcpp.string cimport string

cdef extern from "xon/object.hh" namespace "xon":
    
    cdef cppclass xon_object "xon::object"
    cdef cppclass xon_obj_builder "xon::obj_builder"
    cdef cppclass xon_obj_reader "xon::obj_reader"

    cdef cppclass xon_object:
        xon_object(void *data)
        size_t size()
        bint is_bson()
        bint is_host_arch()
        xon_obj_reader read_obj()
        void* pointer()
        string string()
        void hexdump()
