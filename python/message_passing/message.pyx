


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
        cdef xon_obj_builder builder
        for key, value in dictionary.iteritems():
            if not isinstance(key, basestring):
                raise ValueError('all keys must be strings')
            if isinstance(value, basestring):
                builder.add_string(key, value);
            elif isinstance(value, bool):  # must be before int!
                builder.add_bool(key, value);
            elif isinstance(value, int):
                builder.add_int64(key, value);
            else:
                print "unsupported value type:", type(value)
        self.obj = builder.get_new()

    cdef xon_reader* _reader(self):
        if self.reader == NULL:
            self.reader = new xon_obj_reader(self.obj[0])
        return self.reader

    cdef int _find(self, string key):
        return self._reader().find(key)

    cdef _get_key(self, string key):
        cdef int pos = self._find(key)
        return self._get_pos(pos)

    cdef _get_pos(self, int position):
        cdef xon_reader* xr = self._reader()
        cdef int type = xr.type(position)
        if (type == XON_ELEMENT_STRING):
            return xr.get_string(position)
        elif (type == XON_ELEMENT_DOUBLE):
            return xr.get_double(position)
        elif (type == XON_ELEMENT_BOOLEAN):
            return xr.get_bool(position)
        elif (type == XON_ELEMENT_INT64):
            return xr.get_int64(position)
        elif (type == XON_ELEMENT_INT32):
            return xr.get_int32(position)
        else:
            raise ValueError('unsupported value type')

    def __len__(self):
        return self._reader().count()

    def __getitem__(self, key):
        return self._get_key(key)

    def __repr__(self):
        return self.obj.string(False)

    cpdef hexdump(self):
        self.obj.hexdump()

    cpdef bint has_key(self, key):
        return self._reader().has_key(key)

    cpdef get(self, key, default=None):
        cdef int pos = self._find(key)
        if pos < 0:
            return default
        return self._get_pos(pos)
        


