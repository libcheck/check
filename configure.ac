#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Prelude.
AC_PREREQ([2.59])
AC_INIT([Check], [0.9.4], [check-devel@lists.sourceforge.net])

# unique source file --- primitive safety check 
AC_CONFIG_SRCDIR([doc/money/check_money.c])

# place to put some extra build scripts installed
AC_CONFIG_AUX_DIR([build-aux])

# really severe build strictness
AM_INIT_AUTOMAKE([-Wall -Werror gnits 1.9.6])

# FIXME: find the other places this stuff is used

CHECK_MAJOR_VERSION=0
CHECK_MINOR_VERSION=9
CHECK_MICRO_VERSION=4
CHECK_VERSION=$CHECK_MAJOR_VERSION.$CHECK_MINOR_VERSION.$CHECK_MICRO_VERSION

AC_SUBST(CHECK_MAJOR_VERSION)
AC_SUBST(CHECK_MINOR_VERSION)
AC_SUBST(CHECK_MICRO_VERSION)
AC_SUBST(CHECK_VERSION)

# Configure options.

AC_ARG_ENABLE(gcov,
AC_HELP_STRING([--enable-gcov],[turn on code coverage analysis tools]),
[case "${enableval}" in
  yes) enable_gcov=true ;;
  no)  enable_gcov=false ;;
  *)   AC_MSG_ERROR(bad value ${enableval} for --enable-gcov) ;;
esac], [enable_gcov=false ])

AM_CONDITIONAL(ENABLE_GCOV, test x"$enable_gcov" = "xtrue")

AC_ARG_ENABLE(timeout-tests,
AC_HELP_STRING([--enable-timeout-tests],[turn off timeout tests]),
[case "${enableval}" in
  yes) enable_timeout_tests=true ;;
  no)  enable_timeout_tests=false ;;
  *)   AC_MSG_ERROR(bad value ${enableval} for --enable-timeout-tests) ;;
esac], [enable_timeout_tests=true ])

AM_CONDITIONAL(NO_TIMEOUT_TESTS, test x"$enable_timeout_tests" = "xfalse")

AC_ARG_ENABLE(plain_docdir,
[  --enable-plain-docdir        don't append version information to docdir],
[case "${enableval}" in
  yes) plain_docdir=true ;;
  no)  plain_docdir=false ;;
  *)   AC_MSG_ERROR(bad value ${enableval} for --enable-plain-docdir) ;;
esac], [plain_docdir=false ])

AM_CONDITIONAL(PLAIN_DOCDIR, test x"$plain_docdir" = "xtrue")

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
if test -n "$GCC"; then
   CFLAGS="$CFLAGS -Wall -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings"
fi
AC_CHECK_PROGS(GCOV, gcov, false)
AC_CHECK_PROGS(LCOV, lcov, false)
AC_CHECK_PROGS(GENHTML, genhtml, false)
AC_CHECK_PROG(have_docbook2html,docbook2html,yes)
if test x"$have_docbook2html" != "xyes" ; then
   AC_MSG_WARN([required tools not found, not building docs])
fi
AM_CONDITIONAL(MAKE_DOC, test x"$have_docbook2html" = "xyes")

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])


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
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([alarm gettimeofday localtime_r memmove memset putenv setenv strdup strerror strrchr strstr])

# Output files
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile
                 debian/Makefile
                 doc/Makefile
                 doc/money/Makefile
                 rpm/check.spec
                 rpm/Makefile
                 src/check.h
                 src/Makefile
                 tests/Makefile])

AC_OUTPUT