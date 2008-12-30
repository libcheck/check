#include "libcompat.h"

int
pipe (int *fildes CK_ATTRIBUTE_UNUSED)
{
  return 0;
}
