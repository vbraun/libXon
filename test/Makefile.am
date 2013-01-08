DEFAULT_INCLUDES = -I@top_builddir@/src/

TESTS = \
	xon_obj_rw xon_objxx_rw \
	test_client_c test_client_cxx \
	speed_client \
	exec_pipe

check_PROGRAMS = \
	xon_obj_rw xon_objxx_rw \
	test_client_c test_client_cxx test_server_c test_server_cxx \
	speed_client speed_server \
	exec_pipe

xon_obj_rw_SOURCES = xon_obj_rw.c
xon_obj_rw_LDADD = @top_builddir@/src/libxon-object.la

xon_objxx_rw_SOURCES = xon_objxx_rw.cc
xon_objxx_rw_LDADD = @top_builddir@/src/libxon-objectxx.la

test_client_c_SOURCES = test_client_c.c
test_client_c_LDADD = \
	@top_builddir@/src/libxon-object.la \
	@top_builddir@/src/libxon-client.la

test_client_cxx_SOURCES = test_client_cxx.cc
test_client_cxx_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-clientxx.la

test_server_c_SOURCES = test_server_c.c
test_server_c_LDADD = \
	@top_builddir@/src/libxon-object.la \
	@top_builddir@/src/libxon-server.la

test_server_cxx_SOURCES = test_server_cxx.cc
test_server_cxx_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-serverxx.la

test_server_cxx_SOURCES = test_server_cxx.cc
test_server_cxx_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-serverxx.la

exec_pipe_SOURCES = exec_pipe.cc
exec_pipe_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-clientxx.la

speed_client_SOURCES = speed_client.cc
speed_client_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-clientxx.la

speed_server_SOURCES = speed_server.cc
speed_server_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-serverxx.la

AM_CFLAGS = -Wall -g -O0
