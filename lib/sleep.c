/* This file gets included if AC_REPLACE_FUNCS([sleep]) cannot find the function. */
#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>

#if !HAVE_SLEEP
unsigned int sleep (unsigned int seconds);
#endif /* !HAVE_SLEEP */

unsigned int
sleep (unsigned int seconds)
{
  return 0;
}
