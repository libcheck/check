#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <assert.h>
#include "check_check.h"


TestResult **tr_fail_array;
TestResult **tr_all_array;


#define MAXSTR 300

typedef struct {
  const char *tcname;
  int line_nos;
  int failure_type;
  const char *msg;
} master_test_t;

#define SIG_STR_LEN 128
static char signal_11_str[SIG_STR_LEN];
static char signal_11_8_str[SIG_STR_LEN];
static char signal_8_str[SIG_STR_LEN];

static master_test_t master_tests[] = {
  { "Simple Tests",  18, CK_FAILURE, "Failure expected" },
  { "Simple Tests",  24, CK_ERROR,   "Early exit with return value 1" },
  { "Simple Tests",  -1, CK_PASS,    "Passed" },
  { "Simple Tests",  -1, CK_FAILURE, "This test should fail" },
  { "Simple Tests",  -1, CK_PASS,    "Passed" },
  { "Simple Tests",  -1, CK_FAILURE, "This test should fail" },
  { "Simple Tests",  -1, CK_FAILURE, "Assertion '2 == 3' failed" },
  { "Simple Tests",  -1, CK_FAILURE, "Assertion '4 == 5' failed" },
  { "Simple Tests",  -1, CK_FAILURE, "Failure '2 != 3' occured" },
  { "Simple Tests",  -1, CK_FAILURE, "Failure '4 != 5' occured" },
  { "Simple Tests",  -1, CK_FAILURE, "3 != 4" },
  { "Simple Tests",  -1, CK_FAILURE, "5 != 6" },
  { "Simple Tests",  -1, CK_FAILURE, "7 == 7" },
  { "Simple Tests",  -1, CK_FAILURE, "Failed" },
  { "Simple Tests", 139, CK_FAILURE, "Assertion 'x == y' failed" },
  { "Simple Tests", 145, CK_FAILURE, "Assertion '0' failed" },
  { "Simple Tests", 170, CK_FAILURE, "Assertion 'x==y' failed: x==3, y==4" },
  { "Simple Tests", 180, CK_FAILURE, "Assertion 'x!=y' failed: x==3, y==3" },
  { "Simple Tests", 188, CK_FAILURE, "Assertion '\"test1\"==s' failed: \"test1\"==\"test1\", s==\"test2\"" },
  { "Simple Tests", 198, CK_FAILURE, "Assertion 't!=s' failed: t==\"test2\", s==\"test2\"" },

  { "Signal Tests",  -1, CK_ERROR,   signal_11_str },
  { "Signal Tests",  -1, CK_PASS,    "Passed" },
  { "Signal Tests", 228, CK_ERROR,   signal_11_8_str },
  { "Signal Tests",  -1, CK_FAILURE, "Early exit with return value 0" },
  { "Signal Tests",  -1, CK_FAILURE, "Early exit with return value 1" },
  { "Signal Tests",  -1, CK_ERROR,   signal_8_str },
  { "Signal Tests",  -1, CK_ERROR,   signal_8_str },

#if TIMEOUT_TESTS_ENABLED
  { "Environment Timeout Tests", 256, CK_ERROR,  "Test timeout expired" },
  { "Environment Timeout Tests",  -1, CK_PASS,   "Passed" },
  { "Environment Timeout Tests",  -1, CK_PASS,   "Passed" },
  { "Environment Timeout Tests", 275, CK_ERROR,  "Test timeout expired" },
  { "Timeout Tests", 256, CK_ERROR,  "Test timeout expired" },
  { "Timeout Tests",  -1, CK_PASS,   "Passed" },
  { "Timeout Tests", 269, CK_ERROR,  "Test timeout expired" },
  { "Timeout Tests", 275, CK_ERROR,  "Test timeout expired" },
  { "User Timeout Tests", 2560, CK_ERROR,  "Test timeout expired" },
  { "User Timeout Tests",  -1, CK_PASS,   "Passed" },
  { "User Timeout Tests",  -1, CK_PASS,   "Passed" },
  { "User Timeout Tests", 275, CK_ERROR,  "Test timeout expired" },
  /* Timeout tests are run twice , see check_check_sub.c:make_sub_suite() */
  { "Timeout Tests", 256, CK_ERROR,  "Test timeout expired" },
  { "Timeout Tests",  -1, CK_PASS,   "Passed" },
  { "Timeout Tests", 269, CK_ERROR,  "Test timeout expired" },
  { "Timeout Tests", 275, CK_ERROR,  "Test timeout expired" },
#endif

  { "Limit Tests",   -1, CK_ERROR,   "Early exit with return value 1" },
  { "Limit Tests",   -1, CK_FAILURE, "Completed properly" },
  { "Limit Tests",   -1, CK_FAILURE, "Completed properly" },

  { "Msg and fork Tests",  -1, CK_PASS,       "Passed" },
  { "Msg and fork Tests",  -1, CK_FAILURE,    "Expected fail" },
  { "Msg and fork Tests",  -1, CK_PASS,       "Passed" },
  { "Msg and fork Tests",  -1, CK_FAILURE,    "Expected fail" },
  { "Msg and fork Tests",  -1, CK_PASS,       "Passed" },
  { "Msg and fork Tests",  -1, CK_FAILURE,    "Expected fail" },

  { "Core",          -1, CK_FAILURE, "We failed" }
};

static int nr_of_master_tests = sizeof master_tests /sizeof master_tests[0];

START_TEST(test_check_nfailures)
{
  int i;
  int failed = 0;
  
  for (i = 0; i < nr_of_master_tests; i++) {
    if (master_tests[i].failure_type != CK_PASS) {
      failed++;
    }
  }
  fail_unless (sub_nfailed == failed,
               "Unexpected number of failures received, %d, expected %d.",
               sub_nfailed, failed);
}
END_TEST

START_TEST(test_check_ntests_run)
{
  fail_unless (sub_ntests == nr_of_master_tests,
               "Unexpected number of tests run, %d.", sub_ntests);
}
END_TEST

START_TEST(test_check_failure_msgs)
{
  int i;
  int passed = 0;
  const char *got_msg;
  const char *expected_msg;
  TestResult *tr;

  for (i = 0; i < sub_ntests; i++) {
    if (master_tests[i].failure_type == CK_PASS) {
      passed++;
      continue;
    }

    fail_if(i - passed > sub_nfailed);
    tr = tr_fail_array[i - passed];
    fail_unless(tr != NULL);
    got_msg = tr_msg(tr);
    expected_msg = master_tests[i].msg;
    if (strcmp(got_msg, expected_msg) != 0) {      
      char *emsg = malloc(MAXSTR);
      snprintf(emsg, MAXSTR,"For test %d: Expected %s, got %s",
               i, expected_msg, got_msg);
      fail(emsg);
      free(emsg);
    }
  }
}
END_TEST
  
START_TEST(test_check_failure_lnos)
{
  int i;
  int line_no;
  int passed = 0;
  TestResult *tr;
  
  for (i = 0; i < sub_ntests; i++) {
    if (master_tests[i].failure_type == CK_PASS) {
      passed++;
      continue;
    }

    fail_if(i - passed > sub_nfailed);
    tr = tr_fail_array[i - passed];
    fail_unless(tr != NULL);
    line_no = master_tests[i].line_nos;
    if (line_no > 0 && tr_lno(tr) != line_no) {
      char *emsg = malloc(MAXSTR);
      snprintf(emsg, MAXSTR, "For test %d: Expected lno %d, got %d",
               i, line_no, tr_lno(tr));
      fail(emsg);
      free(emsg);
    }    
  }
}
END_TEST

START_TEST(test_check_failure_ftypes)
{
  int i;
  int passed = 0;
  TestResult *tr;
  
  for (i = 0; i < sub_ntests; i++) {
    if (master_tests[i].failure_type == CK_PASS) {
      passed++;
      continue;
    }

    fail_if(i - passed > sub_nfailed);
    tr = tr_fail_array[i - passed];
    fail_unless(tr != NULL);
    fail_unless(master_tests[i].failure_type == tr_rtype(tr),
                "Failure type wrong for test %d", i);
  }
}
END_TEST

START_TEST(test_check_failure_lfiles)
{
  int i;
  for (i = 0; i < sub_nfailed; i++) {
    TestResult *tr = tr_fail_array[i];
    fail_unless(tr != NULL);
    fail_unless(tr_lfile(tr) != NULL, "Bad file name for test %d", i);
    fail_unless(strstr(tr_lfile(tr), "check_check_sub.c") != 0,
                "Bad file name for test %d", i);
  }
}
END_TEST

START_TEST(test_check_tcnames)
{
  const char *tcname;   
  tcname = tr_tcname(tr_all_array[_i]);
  if (strcmp(tcname, master_tests[_i].tcname) != 0) {
    char *emsg = malloc (MAXSTR);
    snprintf(emsg, MAXSTR,"Expected %s, got %s",
             master_tests[_i].tcname, tcname);
    fail(emsg);
    free(emsg);
  } 
}
END_TEST


START_TEST(test_check_all_msgs)
{
  const char *msg;
  msg = tr_msg(tr_all_array[_i]);
  if (strcmp(msg, master_tests[_i].msg) != 0) {
    char *emsg = malloc (MAXSTR);
    snprintf(emsg, MAXSTR,"Expected %s, got %s",
             master_tests[_i].msg, msg);
    fail(emsg);
    free(emsg);
  }
}
END_TEST  

START_TEST(test_check_all_ftypes)
{
  fail_unless(master_tests[_i].failure_type == tr_rtype(tr_all_array[_i]),
              "Failure type wrong for test %d", _i);
}
END_TEST

int test_fixture_val = 0;
static void test_fixture_setup(void)
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

static void test_fixture_teardown (void)
{
  test_fixture_val = 3;
}

START_TEST(test_teardown)
{
  fail_unless (test_fixture_val == 3,
	       "Value not changed correctly in teardown");
}
END_TEST  


Suite *make_master_suite (void)
{
  Suite *s;
  TCase *tc_core;
  TCase *tc_fixture;
  TCase *tc_post_teardown;
  
  s = suite_create("Master");
  tc_core = tcase_create("Core Tests");
  tc_fixture = tcase_create("Fixture Setup Tests");
  suite_add_tcase (s, tc_core);
  tcase_add_test (tc_core, test_check_nfailures);
  tcase_add_test (tc_core, test_check_ntests_run);
  tcase_add_test (tc_core, test_check_failure_msgs);
  tcase_add_test (tc_core, test_check_failure_ftypes);
  tcase_add_test (tc_core, test_check_failure_lnos);
  tcase_add_test (tc_core, test_check_failure_lfiles);
  tcase_add_loop_test (tc_core, test_check_tcnames, 0, sub_ntests);
  tcase_add_loop_test (tc_core, test_check_all_msgs, 0, sub_ntests);
  tcase_add_loop_test (tc_core, test_check_all_ftypes, 0, nr_of_master_tests);
  tcase_add_unchecked_fixture(tc_fixture, test_fixture_setup,
			      test_fixture_teardown);
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
  return s;
}

static void init_signal_strings(void)
{
  int n;
  n = snprintf(signal_11_str, SIG_STR_LEN, "Received signal 11 (%s)", strsignal(11));
  assert(n < SIG_STR_LEN);
  n = snprintf(signal_11_8_str, SIG_STR_LEN, "Received signal 11 (%s), expected 8 (%s)", strsignal(11), strsignal(8));
  assert(n < SIG_STR_LEN);
  n = snprintf(signal_8_str, SIG_STR_LEN, "Received signal 8 (%s)", strsignal(8));
  assert(n < SIG_STR_LEN);
}

void setup (void)
{
  Suite *s = make_sub_suite();
  SRunner *sr = srunner_create(s);

  init_signal_strings();

  srunner_add_suite(sr, make_sub2_suite());
  srunner_run_all(sr, CK_VERBOSE);
  tr_fail_array = srunner_failures(sr);
  tr_all_array = srunner_results(sr);
  sub_nfailed = srunner_ntests_failed(sr);
  sub_ntests = srunner_ntests_run(sr);
}

void cleanup (void)
{
}
