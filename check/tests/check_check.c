#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "check.h"
#include "error.h"

void setup (void);
void cleanup (void);
Suite *make_suite(void);

START_TEST(test_lno)
{
  fail("Failure expected"); /*line 14*/
}
END_TEST

START_TEST(test_mark_lno)
{
  mark_point(); /*line 20*/
  exit(1); /*should fail at line 20*/
}
END_TEST

START_TEST(test_pass)
{
  fail_unless (1==1, "This test should pass");
  fail_unless (9999, "This test should pass");
  fail("Completed properly");
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

int nfailures;
TestResult **trarray;

START_TEST(test_check_nfailures)
{
  fail_unless (nfailures == 9, "Unexpected number of failures received");
}
END_TEST

START_TEST(test_check_failures)
{
  int i;
  char *msgar[] = {
    "Failure expected",
    "Early exit with return value 1",
    "Completed properly",
    "This test should fail",
    "Received signal 11",
    "Received signal 8",
    "Received signal 8",
    "Early exit with return value 1",
    "Completed properly"};
  int lnos[] = {
    14,
    20,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1};
  
  for (i = 0; i < nfailures; i++) {
    char *msg;

    if (lnos[i] > 0 && tr_lno(trarray[i]) != lnos[i]) {
      char *emsg = emalloc (CMAXMSG);
      snprintf (emsg, CMAXMSG, "Expected lno %d, got %d",
		lnos[i], tr_lno(trarray[i]));
      fail (emsg);
      free (emsg);
    }
    
    msg = tr_msg(trarray[i]);
    if (strcmp (msg, msgar[i]) != 0) {
      char *emsg = emalloc (CMAXMSG);
      snprintf (emsg, CMAXMSG,"Expected %s, got %s", msgar[i], msg);
      fail (emsg);
    }
  }
}
END_TEST

Suite *make_suite(void)
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

int test_fixture_val = 0;
void test_fixture_setup (void)
{
  test_fixture_val = 1;
}

START_TEST(test_setup)
{
  fail_unless (test_fixture_val == 1,
	       "Value not setup or changed across tests correctly");
  test_fixture_val = 2;
}
END_TEST

void test_fixture_teardown (void)
{
  test_fixture_val = 3;
}

START_TEST(test_teardown)
{
  fail_unless (test_fixture_val == 3, "Value not changed correctly in teardown");
}
END_TEST  

int main (void)
{
  int n;


  Suite *s;
  TCase *tc;
  TCase *tc_fixture;
  TCase *tc_post_teardown;
  SRunner *sr;
  s = suite_create("Master");
  tc = tcase_create("Core Tests");
  suite_add_tcase (s, tc);
  tcase_add_test (tc, test_check_nfailures);
  tcase_add_test (tc, test_check_failures);
  tc_fixture = tcase_create("Fixture Setup Tests");
  tcase_set_fixture(tc_fixture, test_fixture_setup, test_fixture_teardown);
  /* add the test 3 times to make sure we adequately test
     preservation of fixture values across tests, regardless
     of the order in which tests are added to the test case */
  tcase_add_test (tc_fixture, test_setup);
  tcase_add_test (tc_fixture, test_setup);
  tcase_add_test (tc_fixture, test_setup);
  suite_add_tcase (s, tc_fixture);
  tc_post_teardown = tcase_create ("Fixture Teardown Tests");
  tcase_add_test (tc_post_teardown, test_teardown);
  suite_add_tcase (s, tc_post_teardown);
  sr = srunner_create (s);
  
  setup();
  printf ("Ran %d tests in subordinate suite\n", nfailures);
  srunner_run_all (sr, CRNORMAL);
  cleanup();
  n = srunner_nfailures(sr);
  srunner_free(sr);
  suite_free(s);
  return (n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void setup (void)
{
  char *msg;
  Suite *s = make_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CRSILENT);
  trarray = srunner_failures(sr);
  nfailures = srunner_nfailures(sr);
}

void cleanup (void)
{
  return;
}
