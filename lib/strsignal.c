/* This file gets included if AC_REPLACE_FUNCS([strsignal]) cannot find the function. */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>

#if !HAVE_STRSIGNAL
static const char *strsignal(int sig);
#endif /* !HAVE_STRSIGNAL */

/* Note that Gnulib has a much more advanced version of strsignal */
static const char *
strsignal (int sig)
{
  static char signame[40];
  sprintf(signame, "SIG #%d", sig);
  return signame;
}
