#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "error.h"
#include "check_str.h"
#include "check_check.h"
#include "check_magic.h"

static void fixture_sub_setup (void)
{
  fail("Test failure in fixture");
}


static Suite *fixture_s;
static SRunner *fixture_sr;
void setup_fixture (void)
{
  TCase *tc;
  
  fixture_s = suite_create("Fix Sub");
  tc = tcase_create("Core");
  tcase_add_unchecked_fixture(tc, fixture_sub_setup, NULL);
  suite_add_tcase (fixture_s, tc);
  fixture_sr = srunner_create(fixture_s);
  srunner_run_all(fixture_sr,CRSILENT);
}

void teardown_fixture (void)
{
  srunner_free(fixture_sr);
  suite_free(fixture_s);
}

START_TEST(test_fixture_fail_counts)
{
  int nrun, nfail;

  nrun = srunner_ntests_run(fixture_sr);
  nfail = srunner_ntests_failed(fixture_sr);

  fail_unless (nrun == 0 && nfail == 1,
	       "Counts for run and fail for fixture failure not correct");
}
END_TEST

START_TEST(test_print_counts)
{
  char *srstat = sr_stat_str(fixture_sr);
  char *exp = "0%: Checks: 0, Failures: 1, Errors: 0";

  fail_unless(strcmp(srstat, exp) == 0,
	      "SRunner stat string incorrect with setup failure");
}
END_TEST

START_TEST(test_setup_failure_msg)
{
  TestResult **tra;
  char *trm;
  char *trmexp = "check_check_fixture.c:11:S:Core: Test failure in fixture";

  tra = srunner_failures(fixture_sr);
  trm = tr_str(tra[0]);

  if (strcmp(trm, trmexp) != 0) {
    char *errm = emalloc(CK_MAXMSG);

    snprintf(errm,CK_MAXMSG,
	     "Bad setup tr msg (%s)", trm);
    
    fail (errm);
  }
  
}
END_TEST

Suite *make_fixture_suite (void)
{

  Suite *s;
  TCase *tc;

  s = suite_create("Fixture");
  tc = tcase_create("Core");

  suite_add_tcase (s, tc);
  tcase_add_test(tc,test_fixture_fail_counts);
  tcase_add_test(tc,test_print_counts);
  tcase_add_test(tc,test_setup_failure_msg);

  return s;
}
