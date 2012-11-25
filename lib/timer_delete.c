#include "libcompat.h"

int timer_delete(timer_t timerid CK_ATTRIBUTE_UNUSED)
{
    /*
     * There is only one timer, that used by alarm.
     * Setting alarm(0) will not set a new alarm, and
     * will kill the previous timer.
     */
    
    alarm(0);
    
    return 0;
}
