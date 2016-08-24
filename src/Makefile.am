SUBDIRS = libidmefv2-error include

AM_CPPFLAGS = @PCFLAGS@ -I$(srcdir)/include -I$(top_builddir) -I$(top_builddir)/src/include -I$(srcdir)/libidmefv2-error \
-I$(top_builddir)/libmissing -I$(top_srcdir)/libmissing $(LTDLINCL) @LIBIDMEFV2_CFLAGS@ @LIBGCRYPT_CFLAGS@ 

pkgconfigdir = $(libdir)/pkgconfig

libidmefv2includedir = $(includedir)/
libidmefv2_la_LIBADD = @LIBIDMEFV2_LIBS@ $(LIBLTDL) $(top_builddir)/libmissing/libmissing.la libidmefv2-error/libidmefv2-error.la
libidmefv2_la_LDFLAGS = $(GETADDRINFO_LIB) $(GETHOSTNAME_LIB) $(HOSTENT_LIB) $(INET_NTOP_LIB) $(INET_PTON_LIB) $(LIBSOCKET) $(LIB_POLL) $(LIB_SELECT) $(LTLIBINTL) $(LTLIBTHREAD) $(SERVENT_LIB) -no-undefined -version-info @LIBIDMEFV2_SONAME@ -export-symbols-regex "^(idmefv2|_?libidmefv2)_"
lib_LTLIBRARIES = libidmefv2.la
BUILT_SOURCES = $(LTDLDEPS) 

AM_YFLAGS = -d
LEX_OUTPUT_ROOT = lex.yy

libidmefv2_la_SOURCES = 		\
	common.c			\
	ntp.c				\
	variable.c			\
	libidmefv2-hash.c			\
	libidmefv2-ident.c			\
	libidmefv2-io.c			\
	libidmefv2-log.c			\
	libidmefv2-string.c		\
	idmefv2-additional-data.c		\
	idmefv2-class.c			\
	idmefv2-criteria.c		\
	idmefv2-criteria-string.yac.y	\
	idmefv2-criteria-string.lex.l	\
	idmefv2-criterion-value.c		\
	idmefv2-data.c			\
	idmefv2-message-helpers.c		\
	idmefv2-message-read-json.c		\
	idmefv2-message-print.c		\
	idmefv2-message-print-json.c		\
	idmefv2-message-write.c		\
	idmefv2-object.c			\
	idmefv2-path.c			\
	idmefv2-time.c			\
	idmefv2-tree-wrap.c		\
	idmefv2-value.c			\
	idmefv2-value-type.c		

-include $(top_srcdir)/git.mk
