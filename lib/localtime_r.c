#include "libcompat.h"

#if !defined(localtime_r)

struct tm * localtime_r (const time_t *clock, struct tm *result)
{
    struct tm *now = NULL;

#if HAVE_LOCALTIME_S
    localtime_s (now, clock)
#elif HAVE__LOCALTIME64_S
    _localtime64_s (now, clock);
#else
    now = localtime (clock);
#endif /* !HAVE_LOCALTIME_S */

    if (now != NULL)
    {
      *result = *now;
    }
    return result;
}

#endif /* !defined(localtime_r) */
