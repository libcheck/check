#include "libcompat.h"

int
putenv (const char *string CK_ATTRIBUTE_UNUSED);
{
  return 0;
}
