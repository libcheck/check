#include "../lib/libcompat.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>

START_TEST(test_pass)
{
  ck_assert_msg (1==1, "Shouldn't see this");
}
END_TEST

START_TEST(test_fail)
{
  ck_abort_msg("Failure");
}
END_TEST

/*
 * This test will fail without fork, as it will result in the
 * unit test runniner exiting early.
 */
#if defined(HAVE_FORK) && HAVE_FORK==1
START_TEST(test_exit)
{
  exit(1);
}
END_TEST
#endif /* HAVE_FORK */

/*
 * This test will intentionally mess up the unit testing program
 * when fork is unavailable. The purpose of including it is to
 * ensure that the tap output is correct when a test crashes.
 */
START_TEST(test_abort)
{
  exit(1);
}
END_TEST

START_TEST(test_pass2)
{
  ck_assert_msg (1==1, "Shouldn't see this");
}
END_TEST

static Suite *make_s1_suite (void)
{
  Suite *s;
  TCase *tc;

  s = suite_create("S1");
  tc = tcase_create ("Core");
  suite_add_tcase(s, tc);
  tcase_add_test (tc, test_pass);
  tcase_add_test (tc, test_fail);
#if defined(HAVE_FORK) && HAVE_FORK==1
  tcase_add_test (tc, test_exit);
#endif /* HAVE_FORK */

  return s;
}

static Suite *make_s2_suite (int include_abort_test)
{
  Suite *s;
  TCase *tc;

  s = suite_create("S2");
  tc = tcase_create ("Core");
  suite_add_tcase(s, tc);
  if(include_abort_test == 1)
  {
     tcase_add_test (tc, test_abort);
  }
  tcase_add_test (tc, test_pass2);

  return s;
}

static void run_tests (int include_abort_test)
{
  SRunner *sr;

  sr = srunner_create(make_s1_suite());
  srunner_add_suite(sr, make_s2_suite(include_abort_test));
  srunner_set_tap(sr, "test.tap");
  srunner_run_all(sr, CK_SILENT);
  srunner_free(sr);
}

static void usage(void)
{
  printf ("Usage: ex_tap_output (NORMAL | TEST_ABORT)\n");
}

int main (int argc, char **argv)
{

  if (argc != 2)
  {
    usage();
    return EXIT_FAILURE;
  }

  if (strcmp (argv[1], "NORMAL") == 0)
  {
    run_tests(0 /* run only tests that will behave properly*/);
  }
  else if (strcmp (argv[1], "TEST_ABORT") == 0)
  {
    run_tests(1 /* Include a test that will fail without fork()*/);
  }
  else
  {
    usage();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
