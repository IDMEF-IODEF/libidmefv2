includedir = $(prefix)/include/libidmefv2
AM_CPPFLAGS = @PCFLAGS@ -I$(top_srcdir)/libmissing -I$(top_srcdir)/src/include -I$(top_builddir)/src/include

EXTRA_DIST = mkstrtable.awk err-sources.h.in err-codes.h.in \
	mkerrnos.awk errnos.in mkerrcodes.awk mkerrcodes1.awk mkerrcodes2.awk mkerrcodes.c \
	mkheader.awk libidmefv2-error.h.in err-sources.h err-codes.h

BUILT_SOURCES = err-sources.h err-codes.h code-from-errno.h code-to-errno.h err-sources-sym.h err-codes-sym.h errnos-sym.h libidmefv2-error.h 

CLEANFILES = err-sources.h err-codes.h code-to-errno.h code-from-errno.h \
        libidmefv2-error.h mkerrcodes mkerrcodes.h \
        err-sources-sym.h err-codes-sym.h errnos-sym.h

nodist_include_HEADERS = libidmefv2-error.h

noinst_LTLIBRARIES = libidmefv2-error.la
libidmefv2_error_la_LIBADD  = @LTLIBTHREAD@
libidmefv2_error_la_LDFLAGS = -no-undefined
libidmefv2_error_la_SOURCES = libidmefv2-error.h libidmefv2-error.c strsource.c strerror.c

err-sources.h: Makefile mkstrtable.awk err-sources.h.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=3 \
		$(srcdir)/err-sources.h.in >$@

err-sources-sym.h: Makefile mkstrtable.awk err-sources.h.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=2 -v nogettext=1 \
	$(srcdir)/err-sources.h.in >$@

err-codes.h: Makefile mkstrtable.awk err-codes.h.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=3 \
	$(srcdir)/err-codes.h.in >$@

err-codes-sym.h: Makefile mkstrtable.awk err-codes.h.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=2 -v nogettext=1 \
		$(srcdir)/err-codes.h.in >$@

code-to-errno.h: Makefile mkerrnos.awk errnos.in
	LANG="" $(AWK) -f $(srcdir)/mkerrnos.awk $(srcdir)/errnos.in >$@


mkerrcodes.h: Makefile mkerrcodes.awk
	LANG="" $(AWK) -f $(srcdir)/mkerrcodes1.awk $(srcdir)/errnos.in >_$@
	$(CPP) -P _$@ | grep LIBIDMEFV2_ERROR_ | LANG="" $(AWK) -f $(srcdir)/mkerrcodes.awk >$@
	-rm _$@

# It is correct to use $(CC_FOR_BUILD) here.  We want to run the
# program at build time.
mkerrcodes: mkerrcodes.c mkerrcodes.h Makefile
	$(CC_FOR_BUILD) -I. -I$(srcdir) -o $@ $(srcdir)/mkerrcodes.c

code-from-errno.h: mkerrcodes Makefile
	./mkerrcodes | LANG="" $(AWK) -f $(srcdir)/mkerrcodes2.awk >$@

errnos-sym.h: Makefile mkstrtable.awk errnos.in
	LANG="" $(AWK) -f $(srcdir)/mkstrtable.awk -v textidx=2 -v nogettext=1 \
	-v prefix=LIBIDMEFV2_ERROR_ -v namespace=errnos_ \
	$(srcdir)/errnos.in >$@

libidmefv2-error.h: Makefile mkheader.awk err-sources.h.in err-codes.h.in errnos.in libidmefv2-error.h.in
	LANG="" $(AWK) -f $(srcdir)/mkheader.awk \
		$(srcdir)/err-sources.h.in \
		$(srcdir)/err-codes.h.in \
		$(srcdir)/errnos.in \
		$(srcdir)/libidmefv2-error.h.in > $@


-include $(top_srcdir)/git.mk
