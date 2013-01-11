AM_CFLAGS = -Wall

lib_LTLIBRARIES = \
	libxon-object.la libxon-objectxx.la \
	libxon-server.la libxon-serverxx.la \
	libxon-client.la libxon-clientxx.la

nobase_include_HEADERS = \
	xon/support.h \
	xon/constants.h \
	xon/status.h \
	xon/object.h xon/object.hh \
	xon/client.h xon/client.hh \
	xon/server.h xon/server.hh \
	xon/exceptions.hh

bin_PROGRAMS = \
	xon_server_exec_pipe \
	xon_server_speed_test \
	xon_server_mathlink


##################################################################
libxon_object_la_SOURCES = \
	object.c xon/object.h macros.h align.h xon/constants.h \
	buffer.h buffer.c

libxon_object_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY_C \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_objectxx_la_SOURCES = \
	object.cc xon/object.hh	xon/object.h \
	macros.h align.h xon/constants.h

libxon_objectxx_la_CXXFLAGS = \
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

libxon_client_la_LIBADD = \
	libxon-object.la

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

libxon_clientxx_la_LIBADD = \
	libxon-objectxx.la

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

libxon_server_la_LIBADD = \
	libxon-object.la

libxon_server_la_CFLAGS = \
	-DBUILDING_XON_LIBRARY_C \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
libxon_serverxx_la_SOURCES = \
	server.cc xon/server.h \
	xon/constants.h xon/object.h \
	macros.h align.h \
	cookie.h cookie.cc \
	socket_comm.h socket_comm.cc

libxon_serverxx_la_LIBADD = \
	libxon-objectxx.la

libxon_serverxx_la_CXXFLAGS = \
	-DBUILDING_XON_LIBRARY_CPP \
	-DCONFIG_H $(CFLAG_VISIBILITY)


##################################################################
xon_server_exec_pipe_SOURCES = \
	subprocess.hh subprocess.cc \
	cookie.h cookie.cc \
	exec_pipe.cc 

xon_server_exec_pipe_LDADD = \
	libxon-objectxx.la \
	libxon-serverxx.la

xon_server_exec_pipe_CXXFLAGS = $(AM_CXXFLAGS)


##################################################################
xon_server_speed_test_SOURCES = speed_server.cc

xon_server_speed_test_LDADD = \
	libxon-objectxx.la \
	libxon-serverxx.la

xon_server_speed_test_CXXFLAGS = $(AM_CXXFLAGS)


##################################################################
xon_server_mathlink_SOURCES = \
	cookie.h cookie.cc \
	subprocess.hh subprocess.cc \
	mathlink_interface.hh mathlink_interface.cc \
	mathlink_server.cc

xon_server_mathlink_LDADD = \
	libxon-objectxx.la \
	libxon-serverxx.la \
	-ldl \
	-lpthread    # gdb gets unhappy if the dlopen() pulls in pthread

xon_server_mathlink_CXXFLAGS = $(AM_CXXFLAGS)
