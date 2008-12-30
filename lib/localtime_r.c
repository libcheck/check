#include "libcompat.h"

struct tm *localtime_r (const time_t *clock CK_ATTRIBUTE_UNUSED, struct tm *result CK_ATTRIBUTE_UNUSED)
{
  return NULL;
}
