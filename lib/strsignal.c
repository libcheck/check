/* This file gets included if AC_REPLACE_FUNCS([strsignal]) cannot find the function. */

#include <stdio.h>

/* Note that Gnulib has a much more advanced version of strsignal */
static const char *
strsignal (int sig)
{
  static char signame[40];
  sprintf(signame, "SIG #%d", sig);
  return signame;
}
