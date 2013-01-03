lib_LTLIBRARIES = \
	libxon-obj.la libxon-objxx.la \
	libxon-client.la \
	libxon-server.la

nobase_include_headers = \
	xon/constants.h \
	xon/object.h xon/object.hh \
	xon/client.h \
	xon/server.h

##################################################################
libxon_obj_la_SOURCES = \
	object.c xon/object.h macros.h align.h xon/constants.h

libxon_obj_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY_C \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_objxx_la_SOURCES = \
	object.cc xon/object.hh xon/object.h \
	macros.h align.h xon/constants.h

libxon_objxx_la_CXXFLAGS = \
	-DBUILDING_XON_LIBRARY_CPP \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_client_la_SOURCES = \
	client.c xon/client.h \
	xon/constants.h xon/object.h \
	macros.h align.h 

libxon_client_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY_C \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_server_la_SOURCES = \
	server.c xon/server.h \
	xon/constants.h xon/object.h \
	macros.h align.h 

libxon_server_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY_C \
	-DCONFIG_H $(CFLAG_VISIBILITY)
