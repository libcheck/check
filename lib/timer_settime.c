#include "libcompat.h"

int timer_settime(timer_t timerid               CK_ATTRIBUTE_UNUSED, 
                  int flags                     CK_ATTRIBUTE_UNUSED, 
                  const struct itimerspec *new_value, 
                  struct itimerspec * old_value CK_ATTRIBUTE_UNUSED)
{
    int seconds = new_value->it_value.tv_sec;
    
    /* 
     * As the alarm() call has only second precision, if the caller
     * specifies partial seconds, we round up to the nearest second.
     */
    if(new_value->it_value.tv_nsec > 0)
    {
        seconds += 1;
    }
    
    alarm(seconds);
    
    return 0;
}
