/* This file gets included if AC_REPLACE_FUNCS([sleep]) cannot find
   the function. */

#include "libcompat.h"

unsigned int
sleep (unsigned int CK_ATTRIBUTE_UNUSED seconds)
{
  return 0;
}
