if HAVE_EASY_BINDINGS

EXTRA_DIST = libidmefv2cpp-lua.i libidmefv2.cxx

if HAVE_LUA

swigdatadir = @SWIG_DATA_DIR@/lua
dist_swigdata_DATA = libidmefv2cpp-lua.i

AM_CPPFLAGS = -I@top_srcdir@ -I@top_builddir@/src/include -I@top_srcdir@/src/include -I@top_builddir@/src/libidmefv2-error -I@top_srcdir@/bindings/c++/include @LUA_CFLAGS@ -I@top_srcdir@/libmissing -I@top_builddir@/libmissing

libidmefv2_la_LDFLAGS = -module -avoid-version @LUA_LIBS@
libidmefv2_la_LIBADD = $(top_builddir)/bindings/c++/.libs/libidmefv2cpp.la
libidmefv2_la_SOURCES = libidmefv2.cxx
libidmefv2dir = $(libdir)/lua/@LUA_VERSION@
libidmefv2_LTLIBRARIES = libidmefv2.la

libidmefv2.cxx: $(top_srcdir)/bindings/c++/include/*.hxx $(top_srcdir)/bindings/libidmefv2cpp.i libidmefv2cpp-lua.i 
if HAVE_SWIG
	$(SWIG) -c++ -DSWIG_COMPILE_LIBIDMEFV2 -I$(top_srcdir)/bindings -I$(top_srcdir)/bindings/c++/include -I$(top_srcdir)/src/include -I$(top_srcdir)/src/libidmefv2-error -lua -o $@ $(top_srcdir)/bindings/libidmefv2cpp.i
endif


endif

endif

-include $(top_srcdir)/git.mk
