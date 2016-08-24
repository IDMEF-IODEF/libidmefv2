if HAVE_EASY_BINDINGS
if HAVE_RUBY

swigdatadir = @SWIG_DATA_DIR@/ruby
dist_swigdata_DATA = libidmefv2cpp-ruby.i

EXTRA_DIST = libidmefv2cpp-ruby.i LibIdmefv2.cxx

rbexec_LTLIBRARIES = LibIdmefv2.la
LibIdmefv2_la_CPPFLAGS = $(RUBY_INCLUDES) $(RUBY_CCFLAGS) -I@top_builddir@ -I$(top_srcdir)/src/include -I$(top_builddir)/src/include -I@top_builddir@/src/libidmefv2-error -I$(top_srcdir)/bindings/c++/include
LibIdmefv2_la_LDFLAGS = -module -avoid-version
LibIdmefv2_la_LIBADD = $(top_builddir)/bindings/c++/.libs/libidmefv2cpp.la $(RUBY_LIBS)
nodist_LibIdmefv2_la_SOURCES = LibIdmefv2.cxx

LibIdmefv2.cxx: $(top_srcdir)/bindings/c++/include/*.hxx $(top_srcdir)/bindings/libidmefv2cpp.i libidmefv2cpp-ruby.i 
if HAVE_SWIG
	$(SWIG) -c++ -DSWIG_COMPILE_LIBIDMEFV2 -I$(top_srcdir)/bindings -I$(top_srcdir)/bindings/c++/include -I$(top_srcdir)/src/include -I$(top_srcdir)/src/libidmefv2-error -ruby -o $@ $(top_srcdir)/bindings/libidmefv2cpp.i
endif

endif
endif

-include $(top_srcdir)/git.mk
