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

START_TEST(test_pass2)
{
  ck_assert_msg (1==1, "Shouldn't see this");
}
END_TEST

START_TEST(test_loop)
{
  ck_assert_msg (_i==1, "Iteration %d failed", _i);
}
END_TEST

START_TEST(test_xml_esc_fail_msg)
{
  ck_abort_msg("fail \" ' < > & message");
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

static Suite *make_s2_suite (void)
{
  Suite *s;
  TCase *tc;

  s = suite_create("S2");
  tc = tcase_create ("Core");
  suite_add_tcase(s, tc);
  tcase_add_test (tc, test_pass2);
  tcase_add_loop_test(tc, test_loop, 0, 3);

  return s;
}

/* check that XML special characters are properly escaped in XML log file */
static Suite *make_xml_esc_suite (void)
{
  Suite *s;
  TCase *tc;

  s = suite_create("XML escape \" ' < > & tests");
  tc = tcase_create ("description \" ' < > &");
  suite_add_tcase(s, tc);

  tcase_add_test (tc, test_xml_esc_fail_msg);

  return s;
}

static void run_tests (int printmode)
{
  SRunner *sr;

  sr = srunner_create(make_s1_suite());
  srunner_add_suite(sr, make_s2_suite());
  srunner_add_suite(sr, make_xml_esc_suite());

  srunner_set_xml(sr, "test.log.xml");
  srunner_run_all(sr, printmode);
  srunner_free(sr);
}


int main (int argc CK_ATTRIBUTE_UNUSED, char **argv CK_ATTRIBUTE_UNUSED)
{
  run_tests(CK_SILENT);		/* not considered in XML output */

  return EXIT_SUCCESS;
}
  
