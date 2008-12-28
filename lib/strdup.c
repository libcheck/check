/* This file gets included if AC_REPLACE_FUNCS([strdup]) cannot
   find the function. */

#include "libcompat.h"
#include <stdio.h>

/* Note that Gnulib has a much more advanced version of strdup */
char *
strdup (const char *str)
{
  /* FIXME: obviously this is broken */
  return NULL;
}
