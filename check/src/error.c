#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "error.h"

void eprintf (char *fmt, ...)
{
  va_list args;
  fflush(stdout);

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  /*include system error information if format ends in colon */
  if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':')
    fprintf(stderr, " %s", strerror(errno));
  fprintf(stderr, "\n");

  exit(2);
}

void *emalloc (size_t n)
{
  void *p;
  p = malloc(n);
  if (p == NULL)
    eprintf("malloc of %u bytes failed:", n);
  return p;
}

void *erealloc (void * ptr, size_t n)
{
  void *p;
  p = realloc (ptr, n);
  if (p == NULL)
    eprintf("realloc of %u bytes failed:", n);
  return p;
}
