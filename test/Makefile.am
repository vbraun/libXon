DEFAULT_INCLUDES = -I@top_builddir@/libxon-obj/src/

TESTS = \
	xon_obj_rw xon_objxx_rw

check_PROGRAMS = \
	xon_obj_rw xon_objxx_rw

xon_obj_rw_SOURCES = xon_obj_rw.c
xon_obj_rw_LDADD = @top_builddir@/libxon-obj/src/libxon-obj.la

xon_objxx_rw_SOURCES = xon_objxx_rw.cc
xon_objxx_rw_LDADD = @top_builddir@/libxon-obj/src/libxon-objxx.la
# xon_objxx_rw_LDFLAGS = 



CFLAGS = -Wall -g -O0
