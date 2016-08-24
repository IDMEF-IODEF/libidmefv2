dnl Autoconf macros for libidmefv2
dnl $id$

# Modified for LIBIDMEFV2 -- Yoann Vandoorselaere
# Modified for LIBGNUTLS -- nmav
# Configure paths for LIBGCRYPT
# Shamelessly stolen from the one of XDELTA by Owen Taylor
# Werner Koch   99-12-09

dnl AM_PATH_LIBIDMEFV2([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND ]]], THREAD_SUPPORT)
dnl Test for libidmefv2, and define LIBIDMEFV2_PREFIX, LIBIDMEFV2_CFLAGS, LIBIDMEFV2_PTHREAD_CFLAGS,
dnl LIBIDMEFV2_LDFLAGS, and LIBIDMEFV2_LIBS
dnl
AC_DEFUN([AM_PATH_LIBIDMEFV2],
[dnl
dnl Get the cflags and libraries from the libidmefv2-config script
dnl
AC_ARG_WITH(libidmefv2-prefix, AC_HELP_STRING(--with-libidmefv2-prefix=PFX,
            Prefix where libidmefv2 is installed (optional)),
            libidmefv2_config_prefix="$withval", libidmefv2_config_prefix="")

  if test x$libidmefv2_config_prefix != x ; then
     if test x${LIBIDMEFV2_CONFIG+set} != xset ; then
        LIBIDMEFV2_CONFIG=$libidmefv2_config_prefix/bin/libidmefv2-config
     fi
  fi

  AC_PATH_PROG(LIBIDMEFV2_CONFIG, libidmefv2-config, no)
  if test "$LIBIDMEFV2_CONFIG" != "no"; then
  	if $($LIBIDMEFV2_CONFIG --thread > /dev/null 2>&1); then
        	LIBIDMEFV2_PTHREAD_CFLAGS=`$LIBIDMEFV2_CONFIG --thread --cflags`

        	if test x$4 = xtrue || test x$4 = xyes; then
                	libidmefv2_config_args="--thread"
        	else
                	libidmefv2_config_args="--no-thread"
        	fi
  	else
        	LIBIDMEFV2_PTHREAD_CFLAGS=`$LIBIDMEFV2_CONFIG --pthread-cflags`
  	fi
  fi

  min_libidmefv2_version=ifelse([$1], ,0.1.0,$1)
  AC_MSG_CHECKING(for libidmefv2 - version >= $min_libidmefv2_version)
  no_libidmefv2=""
  if test "$LIBIDMEFV2_CONFIG" = "no" ; then
    no_libidmefv2=yes
  else
    LIBIDMEFV2_CFLAGS=`$LIBIDMEFV2_CONFIG $libidmefv2_config_args --cflags`
    LIBIDMEFV2_LDFLAGS=`$LIBIDMEFV2_CONFIG $libidmefv2_config_args --ldflags`
    LIBIDMEFV2_LIBS=`$LIBIDMEFV2_CONFIG $libidmefv2_config_args --libs`
    LIBIDMEFV2_PREFIX=`$LIBIDMEFV2_CONFIG $libidmefv2_config_args --prefix`
    LIBIDMEFV2_CONFIG_PREFIX=`$LIBIDMEFV2_CONFIG $libidmefv2_config_args --config-prefix`
    libidmefv2_config_version=`$LIBIDMEFV2_CONFIG $libidmefv2_config_args --version`


      ac_save_CFLAGS="$CFLAGS"
      ac_save_LDFLAGS="$LDFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $LIBIDMEFV2_CFLAGS"
      LDFLAGS="$LDFLAGS $LIBIDMEFV2_LDFLAGS"
      LIBS="$LIBS $LIBIDMEFV2_LIBS"
dnl
dnl Now check if the installed libidmefv2 is sufficiently new. Also sanity
dnl checks the results of libidmefv2-config to some extent
dnl
      rm -f conf.libidmefv2test
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libidmefv2/libidmefv2.h>

int
main ()
{
    system ("touch conf.libidmefv2test");

    if( strcmp( libidmefv2_check_version(NULL), "$libidmefv2_config_version" ) )
    {
      printf("\n*** 'libidmefv2-config --version' returned %s, but LIBIDMEFV2 (%s)\n",
             "$libidmefv2_config_version", libidmefv2_check_version(NULL) );
      printf("*** was found! If libidmefv2-config was correct, then it is best\n");
      printf("*** to remove the old version of LIBIDMEFV2. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If libidmefv2-config was wrong, set the environment variable LIBIDMEFV2_CONFIG\n");
      printf("*** to point to the correct copy of libidmefv2-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    }
    else if ( strcmp(libidmefv2_check_version(NULL), LIBIDMEFV2_VERSION ) ) {
        printf("\n*** LIBIDMEFV2 header file (version %s) does not match\n", LIBIDMEFV2_VERSION);
        printf("*** library (version %s)\n", libidmefv2_check_version(NULL) );
    }
    else {
      if ( libidmefv2_check_version( "$min_libidmefv2_version" ) )
        return 0;
      else {
        printf("no\n*** An old version of LIBIDMEFV2 (%s) was found.\n",
                libidmefv2_check_version(NULL) );
        printf("*** You need a version of LIBIDMEFV2 newer than %s. The latest version of\n",
               "$min_libidmefv2_version" );
        printf("*** LIBIDMEFV2 is always available from https://www.libidmefv2-siem.org/project/libidmefv2/files\n");
        printf("*** \n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the libidmefv2-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of LIBIDMEFV2, but you can also set the LIBIDMEFV2_CONFIG environment to point to the\n");
        printf("*** correct copy of libidmefv2-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
    return 1;
}
],, no_libidmefv2=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
       LDFLAGS="$ac_save_LDFLAGS"
  fi

  if test "x$no_libidmefv2" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])
  else
     if test -f conf.libidmefv2test ; then
        :
     else
        AC_MSG_RESULT(no)
     fi
     if test "$LIBIDMEFV2_CONFIG" = "no" ; then
       echo "*** The libidmefv2-config script installed by LIBIDMEFV2 could not be found"
       echo "*** If LIBIDMEFV2 was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the LIBIDMEFV2_CONFIG environment variable to the"
       echo "*** full path to libidmefv2-config."
     else
       if test -f conf.libidmefv2test ; then
        :
       else
          echo "*** Could not run libidmefv2 test program, checking why..."
          CFLAGS="$CFLAGS $LIBIDMEFV2_CFLAGS"
          LDFLAGS="$LDFLAGS $LIBIDMEFV2_LDFLAGS"
          LIBS="$LIBS $LIBIDMEFV2_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libidmefv2/libidmefv2.h>
],      [ return !!libidmefv2_check_version(NULL); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding LIBIDMEFV2 or finding the wrong"
          echo "*** version of LIBIDMEFV2. If it is not finding LIBIDMEFV2, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
          echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means LIBIDMEFV2 was incorrectly installed"
          echo "*** or that you have moved LIBIDMEFV2 since it was installed. In the latter case, you"
          echo "*** may want to edit the libidmefv2-config script: $LIBIDMEFV2_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LDFLAGS="$ac_save_LDFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     LIBIDMEFV2_CFLAGS=""
     LIBIDMEFV2_LDFLAGS=""
     LIBIDMEFV2_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  rm -f conf.libidmefv2test
  AC_SUBST(LIBIDMEFV2_CFLAGS)
  AC_SUBST(LIBIDMEFV2_PTHREAD_CFLAGS)
  AC_SUBST(LIBIDMEFV2_LDFLAGS)
  AC_SUBST(LIBIDMEFV2_LIBS)
  AC_SUBST(LIBIDMEFV2_PREFIX)
  AC_SUBST(LIBIDMEFV2_CONFIG_PREFIX)

  m4_ifdef([LT_INIT],
           [AC_DEFINE([LIBIDMEFV2_APPLICATION_USE_LIBTOOL2], [], [Define whether application use libtool >= 2.0])],
           [])

])

dnl *-*wedit:notab*-*  Please keep this as the last line.
