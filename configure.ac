#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Prelude.
AC_PREREQ([2.59])

AC_INIT([Check], [0.9.9], [check-devel at lists dot sourceforge dot net])
CHECK_MAJOR_VERSION=0
CHECK_MINOR_VERSION=9
CHECK_MICRO_VERSION=9
CHECK_VERSION=$CHECK_MAJOR_VERSION.$CHECK_MINOR_VERSION.$CHECK_MICRO_VERSION

# unique source file --- primitive safety check 
AC_CONFIG_SRCDIR([src/check.c])

# place where extra autoconf macros are kept
AC_CONFIG_MACRO_DIR([m4])

# place where portability library functions are kept
AC_CONFIG_LIBOBJ_DIR([lib])

# place to put some extra build scripts installed
AC_CONFIG_AUX_DIR([build-aux])

# define things like _GNU_SOURCE appropriately
AC_USE_SYSTEM_EXTENSIONS

# really severe build strictness
AM_INIT_AUTOMAKE([-Wall -Werror gnits 1.9.6])

# From patch 2803433, request system extensions to generate 64-bit safe code
AC_USE_SYSTEM_EXTENSIONS

AC_SUBST(CHECK_MAJOR_VERSION)
AC_SUBST(CHECK_MINOR_VERSION)
AC_SUBST(CHECK_MICRO_VERSION)
AC_SUBST(CHECK_VERSION)

# Configure options.

AC_ARG_ENABLE(gcov,
AC_HELP_STRING([--enable-gcov],
	       [turn on test coverage @<:@default=no@:>@]),
[case "${enableval}" in
  yes) enable_gcov=true ;;
  no)  enable_gcov=false ;;
  *)   AC_MSG_ERROR(bad value ${enableval} for --enable-gcov) ;;
esac], [enable_gcov=false ])

if test x$enable_gcov = xtrue ; then
  if test -n "$GCC"; then
    AC_MSG_ERROR([gcov only works if gcc is used])
  fi

  GCOV_CFLAGS="-fprofile-arcs -ftest-coverage"
  AC_SUBST(GCOV_CFLAGS)

  dnl libtool 1.5.22 and lower strip -fprofile-arcs from the flags
  dnl passed to the linker, which is a bug; -fprofile-arcs implicitly
  dnl links in -lgcov, so we do it explicitly here for the same effect
  GCOV_LIBS=-lgcov
  AC_SUBST(GCOV_LIBS)
fi

AM_CONDITIONAL(ENABLE_GCOV, test x"$enable_gcov" = "xtrue")

AC_ARG_ENABLE(timeout-tests,
AC_HELP_STRING([--enable-timeout-tests],
	       [turn on timeout tests @<:@default=yes@:>@]),
[case "${enableval}" in
  yes) enable_timeout_tests=true ;;
  no)  enable_timeout_tests=false ;;
  *)   AC_MSG_ERROR(bad value ${enableval} for --enable-timeout-tests) ;;
esac], [enable_timeout_tests=true ])

AM_CONDITIONAL(NO_TIMEOUT_TESTS, test x"$enable_timeout_tests" = "xfalse")

AC_ARG_ENABLE(subunit,
AC_HELP_STRING([--enable-subunit],
	       [enable support for the subunit test protocol @<:@default=autodetect@:>@]),
[case "${enableval}" in
  yes)
    enable_subunit=true
    echo "Enabled subunit support"
    ;;
  no)
    enable_subunit=false
    echo "Disabled subunit support"
    ;;
  autodetect)
    echo "Subunit support will enable automatically."
    ;;
  *)   AC_MSG_ERROR(bad value ${enableval} for --enable-subunit) ;;
esac], 
[echo "Subunit support will enable automatically."
 enable_subunit=autodetect])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
# Automake wants this for per-target CFLAGS
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_LIBTOOL

# add these options to CFLAGS if the compiler supports them
AC_DEFUN([AX_CFLAGS_ADD],[AX_C_CHECK_FLAG($1, , , CFLAGS="$CFLAGS $1")])
AX_CFLAGS_WARN_ALL_ANSI
AX_CFLAGS_ADD([-Wextra])
AX_CFLAGS_ADD([-Wstrict-prototypes])
AX_CFLAGS_ADD([-Wmissing-prototypes])
AX_CFLAGS_ADD([-Wwrite-strings])
AX_CFLAGS_ADD([-Wno-variadic-macros])

AC_CHECK_PROGS(GCOV, gcov, false)
AC_CHECK_PROGS(LCOV, lcov, false)
AC_CHECK_PROGS(GENHTML, genhtml, false)
AC_CHECK_PROGS(TEX, tex, false)
if test "$TEX" = "false"; then
    # Make it [somewhat] clear to maintainers that tex is missing. Not an error
    # though because 'make install' (which users need) does not build the docs
    # anyway.
    AC_MSG_WARN(tex not installed: cannot rebuild HTML documentation.)
fi

# Add math library
LIBS=-lm
AC_SUBST(LIBS)

# Check if clock_gettime, timer_create, timer_settime, and timer_delete are available in lib rt, and if so,
# add -lrt to LIBS
AC_CHECK_LIB([rt], [clock_gettime, timer_create, timer_settime, timer_delete])

# check that struct itimerspec is defined in time.h. If not, we need to
# define it in libcompat.h
AC_CHECK_MEMBERS([struct itimerspec.it_interval, struct itimerspec.it_value], [], [AC_DEFINE_UNQUOTED(STRUCT_ITIMERSPEC_DEFINITION_MISSING, 1, "Need to define the itimerspec structure")], [#include <time.h>])

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])

AS_IF([test x"$enable_subunit" != "xfalse" && test x"$enable_subunit" != "xtrue"], [
	PKG_CHECK_EXISTS([libsubunit], [:], [enable_subunit=false])
	])
AS_IF([test x"$enable_subunit" != "xfalse"], [
	PKG_CHECK_MODULES([LIBSUBUNIT], [libsubunit])
	])
if test "xfalse" = x"$enable_subunit"; then
ENABLE_SUBUNIT="0"
LIBSUBUNIT_PC=""
else
ENABLE_SUBUNIT="1"
LIBSUBUNIT_PC="libsubunit"
fi
AC_SUBST(ENABLE_SUBUNIT)
AC_SUBST([LIBSUBUNIT_PC])
AC_DEFINE_UNQUOTED(ENABLE_SUBUNIT, $ENABLE_SUBUNIT, [Subunit protocol result output])

AM_CONDITIONAL(SUBUNIT, test x"$enable_subunit" != "xfalse")


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(short, 2)
AC_CHECK_SIZEOF(long, 4)

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_REPLACE_FUNCS([clock_gettime timer_create timer_settime timer_delete fileno localtime_r pipe putenv setenv sleep strdup strsignal unsetenv])
AC_CHECK_DECLS([clock_gettime, timer_create, timer_settime, timer_delete, fileno, localtime_r, pipe, putenv, setenv, sleep, strdup, strsignal, unsetenv])

# Checks for pthread implementation.
ACX_PTHREAD
CC="$PTHREAD_CC"

# Check for whether we can install checkmk (we have a usable awk)
AC_ARG_VAR([AWK_PATH],[Awk interpreter command])
AC_PATH_PROG(AWK_PATH, $AWK, [*NO AWK*])
AM_CONDITIONAL([INSTALL_CHECKMK], [test "x$AWK_PATH" != 'x*NO AWK*'])

# Certain awk implementations disagree with each other on how to
# substitute doubled backslashes in gsub()
AC_SUBST(AWK_GSUB_DBL_BSLASH, '\\\\')

AS_IF([test "x$AWK_PATH" = 'x*NO AWK*'],
      [AC_MSG_WARN([Couldn't find a usable awk; won't install checkmk.])],

      # Determine correct number of backslashes for gsub's replacement
      # value.
      [AS_IF([echo '\' |
              "$AWK_PATH" '{ gsub("\\\\", "\\\\", $0); print }' |
              grep '^\\$' >/dev/null 2>&1], AWK_GSUB_DBL_BSLASH='\\\\\\\\')
       AC_CONFIG_FILES(checkmk/checkmk)
       AC_CONFIG_COMMANDS([checkmk-x], [chmod +x checkmk/checkmk])])

# Output files
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([check.pc
                 Makefile
                 checkmk/Makefile
		 doc/Makefile
		 lib/Makefile
                 src/check.h
                 src/Makefile
                 tests/Makefile
		 tests/test_vars])

AC_OUTPUT
