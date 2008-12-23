/* AC_FUNC_MALLOC in configure defines malloc to rpl_malloc if
   malloc (0) is NULL to provide GNU compatibility */
#if HAVE_CONFIG_H
#include <config.h>
#endif
#undef malloc

#include <sys/types.h>

void *malloc (size_t n);

/* force malloc(0) to return a valid pointer */
void *
rpl_malloc (size_t n)
{
  if (n == 0)
    n = 1;
  return malloc (n);
}
