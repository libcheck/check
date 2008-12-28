/* This file gets included if AC_REPLACE_FUNCS([sleep]) cannot find
   the function. */

#include "libcompat.h"

unsigned int
sleep (unsigned int seconds CK_ATTRIBUTE_UNUSED)
{
  return 0;
}
