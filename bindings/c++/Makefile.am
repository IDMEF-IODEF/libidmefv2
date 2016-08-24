if HAVE_EASY_BINDINGS

SUBDIRS = include

lib_LTLIBRARIES = libidmefv2cpp.la
libidmefv2cpp_ladir = ${prefix}/lib
libidmefv2cpp_la_SOURCES = \
	libidmefv2-error.cxx \
	idmefv2.cxx \
	idmefv2-class.cxx \
	idmefv2-criteria.cxx \
	idmefv2-path.cxx \
	idmefv2-time.cxx \
	idmefv2-value.cxx

libidmefv2cpp_la_LDFLAGS = -no-undefined -version-info @LIBIDMEFV2CPP_SONAME@
libidmefv2cpp_la_LIBADD  = $(top_builddir)/src/.libs/libidmefv2.la
libidmefv2cpp_la_CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/src/include -I$(top_srcdir)/src/include -I$(top_builddir)/src/libidmefv2-error -I$(top_builddir)/bindings/c++/include -I$(top_srcdir)/bindings/c++/include 

endif

-include $(top_srcdir)/git.mk
