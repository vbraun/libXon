DEFAULT_INCLUDES = -I@top_builddir@/src/

TESTS = \
	xon_obj_rw xon_objxx_rw \
	test_client_c

check_PROGRAMS = \
	xon_obj_rw xon_objxx_rw \
	test_client_c test_server_c

xon_obj_rw_SOURCES = xon_obj_rw.c
xon_obj_rw_LDADD = @top_builddir@/src/libxon-obj.la

xon_objxx_rw_SOURCES = xon_objxx_rw.cc
xon_objxx_rw_LDADD = @top_builddir@/src/libxon-objxx.la
# xon_objxx_rw_LDFLAGS = 

test_client_c_SOURCES = test_client_c.c
test_client_c_LDADD = \
	@top_builddir@/src/libxon-obj.la \
	@top_builddir@/src/libxon-client.la

test_server_c_SOURCES = test_server_c.c
test_server_c_LDADD = \
	@top_builddir@/src/libxon-obj.la \
	@top_builddir@/src/libxon-server.la



AM_CFLAGS = -Wall -g -O0
