
include 'message.pxi'

cdef class BinaryObject(object):
    cdef xon_object *obj
    cdef xon_obj_reader *reader

    cdef _init_from_dict(self, dict dictionary)
    cdef _init_from_object(self, xon_object obj)
    cdef xon_reader* _reader(self)
    cdef int _find(self, string key)
    cdef _get_key(self, string key)
    cdef _get_pos(self, int position)

    cpdef hexdump(self)
    cpdef bint has_key(self, key)
    cpdef get(self, key, default=?)


    # keys        
    # copy
    # fromkeys    
    # items
    # iterkeys
    # itervalues  
    # iteritems   
    # __iter__

    # values
    # viewitems
    # viewkeys
    # viewvalues
    # __cmp__
    # __contains__
    # __getattribute__  
    # __hash__



cdef class Client(object):
    cdef xon_client *thisptr
    cpdef send(self, dict dictionary)
    cpdef BinaryObject receive(self)
    cpdef wait(self, float timeout=?)
    cpdef kill(self)
