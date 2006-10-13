dnl Process this file with autoconf to produce a configure script.
AC_INIT(money.h)
AM_INIT_AUTOMAKE(money,0.2)

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

dnl This macro is defined in check.m4 and tests if check.h and libcheck.a
dnl can be found. It sets CHECK_CFLAGS and CHECK_LIBS accordingly.
dnl AM_PATH_CHECK([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
AM_PATH_CHECK()

dnl Checks for header files.
AM_CONFIG_HEADER(config.h)

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

AC_OUTPUT(Makefile)
