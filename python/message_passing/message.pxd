
include 'message.pxi'

cdef class binary_object(object):
    cdef xon_object *obj
    cdef xon_obj_reader *reader

    cdef _init_from_dict(self, dict dictionary)

    cpdef hexdump(self)
    cpdef bint has_key(self, key)
    cpdef get(self, key)

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
