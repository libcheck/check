#include "libcompat.h"

int setenv (const char *name CK_ATTRIBUTE_UNUSED, const char *value CK_ATTRIBUTE_UNUSED, int overwrite CK_ATTRIBUTE_UNUSED)
{
  return 0;
}
