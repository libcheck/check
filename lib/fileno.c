#include "libcompat.h"

int fileno(FILE *stream CK_ATTRIBUTE_UNUSED)
{
  return 0;
}

