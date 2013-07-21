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

/* provides assert */
#include <assert.h>

/* defines FILE */
#include <stdio.h>

/* provides localtime and struct tm */
#include <sys/time.h>
#include <time.h>

/* declares fork(), _POSIX_VERSION.  according to Autoconf.info,
   unistd.h defines _POSIX_VERSION if the system is POSIX-compliant,
   so we will use this as a test for all things uniquely provided by
   POSIX like sigaction() and fork() */
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

/* declares pthread_create and friends */
#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif

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

/* our setenv implementation is currently broken */
#if !HAVE_SETENV
#define HAVE_WORKING_SETENV 0
#else
#define HAVE_WORKING_SETENV 1
#endif

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
int unsetenv (const char *name);
#endif /* !HAVE_DECL_UNSETENV */

#ifndef HAVE_LIBRT
/* 
 * On systems where clock_gettime() is not available, the
 * definition for CLOCK_MONOTONIC will also not be available.
 * This variable should define which type of clock clock_gettime()
 * should use. We define it here if it is not defined simply
 * so the reimplementation can ignore it.
 */
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif

#ifdef STRUCT_ITIMERSPEC_DEFINITION_MISSING
/* 
 * The following structure is defined in POSIX.1b for timer start values and intervals.
 * If it is not defined in time.h, then we need to define it here.
 */
struct itimerspec
{
    struct timespec it_interval;
    struct timespec it_value;
};
#endif /* STRUCT_ITIMERSPEC_DEFINITION_MISSING */

/* 
 * As the functions which use timer_t are not defined on the system, 
 * the timer_t type probably also is not defined.
 */
typedef int timer_t;

int clock_gettime(int clk_id, struct timespec *ts);
int timer_create(int clockid, struct sigevent *sevp, timer_t *timerid);
int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec * old_value);
int timer_delete(timer_t timerid);
#endif /* HAVE_LIBRT */

/*
 * The following checks are to determine if the system's
 * snprintf (or its variants) should be replaced with
 * the C99 compliant version in libcompat.
 */
#if HAVE_CONFIG_H
#include <config.h>
#endif
#if HAVE_STDARG_H
#include <stdarg.h>
#if !HAVE_VSNPRINTF
int rpl_vsnprintf(char *, size_t, const char *, va_list);
#endif
#if !HAVE_SNPRINTF
int rpl_snprintf(char *, size_t, const char *, ...);
#endif
#if !HAVE_VASPRINTF
int rpl_vasprintf(char **, const char *, va_list);
#endif
#if !HAVE_ASPRINTF
int rpl_asprintf(char **, const char *, ...);
#endif
#endif /* HAVE_STDARG_H */

/* silence warnings about an empty library */
void ck_do_nothing (void);

#endif /* !LIBCOMPAT_H */
