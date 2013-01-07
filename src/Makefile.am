AM_CFLAGS = -Wall

lib_LTLIBRARIES = \
	libxon-obj.la libxon-objxx.la \
	libxon-server.la \
	libxon-client.la libxon-clientxx.la

nobase_include_headers = \
	xon/constants.h \
	xon/status.h \
	xon/object.h xon/object.hh \
	xon/client.h xon/client.hh \
	xon/server.h \
	xon/exceptions.hh

bin_PROGRAMS = xon_server_exec_pipe


##################################################################
libxon_obj_la_SOURCES = \
	object.c xon/object.h macros.h align.h xon/constants.h \
	buffer.h buffer.c

libxon_obj_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY_C \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_objxx_la_SOURCES = \
	object.cc xon/object.hh	xon/object.h \
	macros.h align.h xon/constants.h

libxon_objxx_la_CXXFLAGS = \
	-DBUILDING_XON_LIBRARY_CPP \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_client_la_SOURCES = \
	client.c xon/client.h \
	xon/constants.h xon/object.h \
	macros.h align.h \
	subprocess.h subprocess.c \
	cookie.h cookie.c \
	socket_comm.h socket_comm.c

libxon_client_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY_C \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_clientxx_la_SOURCES = \
	client.cc xon/client.hh xon/client.h \
	xon/constants.h xon/object.h \
	macros.h align.h \
	subprocess.h subprocess.cc \
	cookie.h cookie.cc \
	socket_comm.h socket_comm.cc \
	xon/exceptions.hh

libxon_clientxx_la_CXXFLAGS = \
	-DBUILDING_XON_LIBRARY_CPP \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_server_la_SOURCES = \
	server.c xon/server.h \
	xon/constants.h xon/object.h \
	macros.h align.h \
	cookie.h cookie.c \
	socket_comm.h socket_comm.c

libxon_server_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY_C \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
xon_server_exec_pipe_SOURCES = \
	subprocess.h subprocess.cc \
	cookie.h cookie.cc \
	exec_pipe.cc 
