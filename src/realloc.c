/* AC_FUNC_REALLOC in configure defines realloc to rpl_realloc if
   realloc (p, 0) is NULL to provide GNU compatibility */
#if HAVE_CONFIG_H
#include <config.h>
#endif
#undef realloc
     
#include <sys/types.h>
     
void *realloc (void *p, size_t n);
     
/* force realloc(p, 0) to return a valid pointer */
void *
rpl_realloc (void *p, size_t n)
{
  if (n == 0)
    n = 1;
  if (p == 0)
    return malloc (n);
  return realloc (p, n);
}
