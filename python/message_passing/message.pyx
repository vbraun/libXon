

cdef class binary_object(object):

    def __cinit__(self):
        self.obj = NULL
        self.reader = NULL

    def __dealloc__(self):
        del self.obj
        del self.reader

    def __init__(self, dictionary):
        self._init_from_dict(dictionary)
    
    cdef _init_from_dict(self, dict dictionary):
        pass

    def __len__(self):
        pass

    def __getitem__(self, key):
        pass

    def __repr__(self):
        return "binary object"

    cpdef hexdump(self):
        pass

    cpdef bint has_key(self, key):
        pass

    cpdef get(self, key):
        pass


