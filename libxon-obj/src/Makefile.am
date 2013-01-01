lib_LTLIBRARIES = libxon-obj.la

nobase_include_headers = \
	xon/object.h


libxon_obj_la_SOURCES = \
	object.c xon/object.h macros.h

libxon_obj_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY \
	-DCONFIG_H $(CFLAG_VISIBILITY)
