#include <stdlib.h>
#include <signal.h>
#include <check.h>
#include "check_check.h"

START_TEST(test_lno)
{
  fail("Failure expected"); /*line 8*/
}
END_TEST

START_TEST(test_mark_lno)
{
  mark_point(); /*line 14*/
  exit(1); /*should fail at line 14*/
}
END_TEST

START_TEST(test_pass)
{
  fail_unless (1==1, "This test should pass");
  fail_unless (9999, "This test should pass");
}
END_TEST

START_TEST(test_fail)
{
  fail_unless (1==2, "This test should fail");
}
END_TEST

START_TEST(test_segv)
{
  char arr[1];
  int i = 0;

  while (1) {
    arr[i] = 'a';
    i++;
  }
}
END_TEST


START_TEST(test_fpe)
{
  raise (SIGFPE);
}
END_TEST


/* TODO:
   unit test running the same suite in succession */

START_TEST(test_mark_point)
{
  int i;
  i = 0;
  i++;
  mark_point();
  raise(SIGFPE);
  fail("Shouldn't reach here");
}
END_TEST

START_TEST(test_early_exit)
{
  exit(1);
}
END_TEST

START_TEST(test_null)
{  
  Suite *s;
  TCase *tc;
  SRunner *sr;
  
  s = suite_create(NULL);
  tc = tcase_create(NULL);
  suite_add_tcase (s, NULL);
  tcase_add_test (tc, NULL);
  sr = srunner_create(NULL);
  srunner_run_all (NULL, -1);
  srunner_free (NULL);
  suite_free (NULL);
  fail("Completed properly");
}
END_TEST

Suite *make_sub_suite(void)
{
  Suite *s = suite_create("Check Servant");
  TCase *tc_simple = tcase_create("Simple Tests");
  TCase *tc_signal = tcase_create("Signal Tests");
  TCase *tc_limit = tcase_create("Limit Tests");
  suite_add_tcase (s, tc_simple);
  suite_add_tcase (s, tc_signal);
  suite_add_tcase (s, tc_limit);
  tcase_add_test (tc_simple, test_lno);
  tcase_add_test (tc_simple, test_mark_lno);
  tcase_add_test (tc_simple, test_pass);
  tcase_add_test (tc_simple, test_fail);
  tcase_add_test (tc_signal, test_segv);
  tcase_add_test (tc_signal, test_fpe);
  tcase_add_test (tc_signal, test_mark_point);
  tcase_add_test (tc_limit, test_early_exit);
  tcase_add_test (tc_limit, test_null);
  return s;
}
