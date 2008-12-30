#ifndef LIBCOMPAT_H
#define LIBCOMPAT_H

#if HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define GCC_VERSION_AT_LEAST(major, minor) \
((__GNUC__ > (major)) || \
 (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))
#else
#define GCC_VERSION_AT_LEAST(major, minor) 0
#endif

#if GCC_VERSION_AT_LEAST(2,95)
#define CK_ATTRIBUTE_UNUSED __attribute__ ((unused))
#else
#define CK_ATTRIBUTE_UNUSED              
#endif /* GCC 2.95 */

/* defines size_t */
#include <sys/types.h>

/* defines FILE */
#include <stdio.h>

/* replacement functions for broken originals */
#if !HAVE_MALLOC
void *rpl_malloc (size_t n);
#endif /* !HAVE_MALLOC */

#if !HAVE_REALLOC
void *rpl_realloc (void *p, size_t n);
#endif /* !HAVE_REALLOC */

/* functions that may be undeclared */
#if !HAVE_DECL_FILENO
int fileno (FILE *stream);
#endif /* !HAVE_DECL_FILENO */

#if !HAVE_DECL_LOCALTIME_R
struct tm *localtime_r (const time_t *clock, struct tm *result);
#endif /* !HAVE_DECL_LOCALTIME_R */

#if !HAVE_DECL_PIPE
int pipe (int *fildes);
#endif /* !HAVE_DECL_PIPE */

#if !HAVE_DECL_PUTENV
int putenv (const char *string);
#endif /* !HAVE_DECL_PUTENV */

#if !HAVE_DECL_SETENV
int setenv (const char *name, const char *value, int overwrite);
#endif /* !HAVE_DECL_SETENV */

#if !HAVE_DECL_SLEEP
unsigned int sleep (unsigned int seconds);
#endif /* !HAVE_DECL_SLEEP */

#if !HAVE_DECL_STRDUP
char *strdup (const char *str);
#endif /* !HAVE_DECL_STRDUP */

#if !HAVE_DECL_STRSIGNAL
const char *strsignal (int sig);
#endif /* !HAVE_DECL_STRSIGNAL */

#if !HAVE_DECL_UNSETENV
void unsetenv (const char *name);
#endif /* !HAVE_DECL_UNSETENV */

/* silence warnings about an empty library */
void ck_do_nothing (void);

#endif /* !LIBCOMPAT_H */
