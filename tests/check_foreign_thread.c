/*
 * Check: a unit test framework for C
 * Copyright (C) 2001, 2002 Arien Malec
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "../lib/libcompat.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Invoked by test_check_foreign_thread.sh with one of these modes:
 *
 * body (default), setup, teardown:
 *   Fail an assertion on a newly created thread while the test body or the
 *   corresponding checked fixture is running, with forking disabled. Each
 *   must print the fatal diagnostic and terminate the process with status 1,
 *   without returning to the runner or invoking the atexit handler.
 * same:
 *   Fail on the thread running the test, with forking disabled. This must
 *   retain normal failure reporting through longjmp and let the runner
 *   finish. Normal process termination invokes the atexit handler.
 * fork:
 *   Fail on a newly created thread in a forked test child. This must retain
 *   normal failure reporting to the parent runner. The parent finishes and
 *   invokes its atexit handler on normal process termination.
 *
 * The normal reporting modes also return status 1 because the test failed.
 * Status 77 skips the probe without pthreads or compiler TLS, or the fork
 * mode when fork is unavailable. Status 3 indicates a pthread API failure.
 */

#if defined(HAVE_PTHREAD) && defined(CK_THREAD_LOCAL)
static const char *mode;

static void at_exit(void)
{
    fprintf(stderr, "ATEXIT_RAN\n");
}

static void *worker(void *unused)
{
    (void)unused;
    ck_abort_msg("foreign-thread diagnostic probe");
    return NULL;
}

static void trigger(void)
{
    pthread_t thread;
    if(pthread_create(&thread, NULL, worker, NULL) != 0)
        exit(3);
    if(pthread_join(thread, NULL) != 0)
        exit(3);
}

static void setup(void)
{
    if(strcmp(mode, "setup") == 0)
        trigger();
}

static void teardown(void)
{
    if(strcmp(mode, "teardown") == 0)
        trigger();
}

START_TEST(probe)
{
    if(strcmp(mode, "body") == 0 || strcmp(mode, "fork") == 0)
        trigger();
    if(strcmp(mode, "same") == 0)
        ck_abort_msg("same-thread diagnostic probe");
}
END_TEST

int main(int argc, char **argv)
{
    Suite *s = suite_create("foreign-thread");
    TCase *tc = tcase_create("probe");
    SRunner *sr = srunner_create(s);
    int failed;

    mode = argc > 1 ? argv[1] : "body";
#if !defined(HAVE_FORK) || HAVE_FORK == 0
    if(strcmp(mode, "fork") == 0)
        return 77;
#endif
    atexit(at_exit);
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, probe);
    suite_add_tcase(s, tc);
    srunner_set_fork_status(sr, strcmp(mode, "fork") == 0 ? CK_FORK : CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed ? 1 : 0;
}
#else
int main(void)
{
    return 77;
}
#endif
