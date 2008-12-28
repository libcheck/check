/* AC_FUNC_MALLOC in configure defines malloc to rpl_malloc if
   malloc (0) is NULL to provide GNU compatibility */
#if HAVE_CONFIG_H
#include <config.h>
#endif

/* malloc has been defined to rpl_malloc, so first undo that */
#undef malloc

/* defines size_t */
#include <sys/types.h>

/* this gives us the real malloc to use below */
void *malloc (size_t n);

/* force malloc(0) to return a valid pointer */
void *
rpl_malloc (size_t n)
{
  if (n == 0)
    n = 1;
  return malloc (n);
}
