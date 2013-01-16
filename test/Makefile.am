AM_CPPFLAGS = -I$(top_srcdir)/src/

TESTS = \
	xon_obj_rw xon_objxx_rw \
	test_client_c test_client_cxx \
	speed_client \
	exec_pipe_client \
	preload_client

check_PROGRAMS = \
	xon_obj_rw xon_objxx_rw \
	test_client_c test_client_cxx test_server_c test_server_cxx \
	speed_client \
	exec_pipe_client \
	buffering \
	preload_victim preload_client

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

exec_pipe_client_SOURCES = exec_pipe.cc
exec_pipe_client_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-clientxx.la

speed_client_SOURCES = speed.cc
speed_client_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-clientxx.la

preload_victim_SOURCES = preload_victim.cc

preload_client_SOURCES = preload_client.cc
preload_client_LDADD = \
	@top_builddir@/src/libxon-objectxx.la \
	@top_builddir@/src/libxon-clientxx.la

buffering_SOURCES = buffering.cc


AM_CFLAGS = -Wall -g -O0
AM_CXXFLAGS = -Wall -g -O0
