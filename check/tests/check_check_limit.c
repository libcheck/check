#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "check_check.h"
#include "check_str.h"


static SRunner *sr;

static void limit_setup (void)
{
  Suite *s = suite_create("Empty");
  sr = srunner_create(s);
  srunner_run_all(sr, CRSILENT);
}

static void limit_teardown (void)
{
  srunner_free(sr);
} 

START_TEST(test_summary)
{
  fail_unless(strcmp(sr_stat_str(sr),
		     "100%: Checks: 0, Failures: 0, Errors: 0") == 0,
	      "Bad statistics string for empty suite");
}
END_TEST

Suite *make_limit_suite (void)
{
  Suite *s = suite_create("Limit");
  TCase *tc = tcase_create("Empty");

  tcase_add_test(tc,test_summary);
  tcase_add_fixture(tc,limit_setup,limit_teardown, 0);
  
  suite_add_tcase(s, tc);

  return s;
}
