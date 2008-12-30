#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "../src/check.h"

/* defines size_t */
#include <sys/types.h>

#if !HAVE_FILENO
int fileno(FILE *stream);
#endif /* !HAVE_FILENO */

#if !HAVE_LOCALTIME_R
struct tm *localtime_r(const time_t *clock, struct tm *result);
#endif /* !HAVE_LOCALTIME_R */

#if !HAVE_MALLOC
void *rpl_malloc (size_t n);
#endif /* !HAVE_MALLOC */

#if !HAVE_REALLOC
void *rpl_realloc (void *p, size_t n);
#endif /* !HAVE_REALLOC */

#if !HAVE_SETENV
int setenv(const char *name, const char *value, int overwrite);
#endif /* !HAVE_SETENV */

#if !HAVE_SLEEP
unsigned int sleep (unsigned int seconds);
#endif /* !HAVE_SLEEP */

#if !HAVE_STRDUP
char *strdup (const char *str);
#endif /* !HAVE_STRDUP */

#if !HAVE_STRSIGNAL
const char *strsignal(int sig);
#endif /* !HAVE_STRSIGNAL */

#if !HAVE_UNSETENV
void unsetenv(const char *name);
#endif /* !HAVE_UNSETENV */

/* silence warnings about an empty library */
void ck_do_nothing (void);
