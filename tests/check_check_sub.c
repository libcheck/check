#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <check.h>
#include "check_check.h"








START_TEST(test_lno)
{
  fail("Failure expected"); /* line 18*/
}
END_TEST

START_TEST(test_mark_lno)
{
  mark_point(); /* line 24*/
  exit(EXIT_FAILURE); /* should fail at line 24*/
}
END_TEST

START_TEST(test_pass)
{
  fail_unless(1 == 1, "This test should pass");
  fail_unless(9999, "This test should pass");
}
END_TEST
/* FIXME: this should really be called test_fail_unless */
START_TEST(test_fail)
{
  fail_unless(1 == 2, "This test should fail");
}
END_TEST

START_TEST(test_fail_if_pass)
{
  fail_if(1 == 2, "This test should pass");
  fail_if(0, "This test should pass");
}
END_TEST

START_TEST(test_fail_if_fail)
{
  fail_if(1 == 1, "This test should fail");
}
END_TEST

START_TEST(test_fail_null_msg)
{
  fail_unless(2 == 3, NULL);
}
END_TEST

#if defined(__GNUC__)
START_TEST(test_fail_no_msg)
{
  fail_unless(4 == 5);
}
END_TEST
#endif /* __GNUC__ */
START_TEST(test_fail_if_null_msg)
{
  fail_if(2 != 3, NULL);
}
END_TEST

#if defined(__GNUC__)
START_TEST(test_fail_if_no_msg)
{
  fail_if(4 != 5);
}
END_TEST
#endif /* __GNUC__ */
START_TEST(test_fail_vararg_msg_1)
{
  int x = 3;
  int y = 4;
  fail_unless(x == y, "%d != %d", x, y);
}
END_TEST

START_TEST(test_fail_vararg_msg_2)
{
  int x = 5;
  int y = 6;
  fail_if(x != y, "%d != %d", x, y);
}
END_TEST

START_TEST(test_fail_vararg_msg_3)
{
  int x = 7;
  int y = 7;
  fail("%d == %d", x, y);
}
END_TEST
#if defined(__GNUC__)
START_TEST(test_fail_empty)
{ /* plain fail() doesn't compile with xlc in C mode because of `, ## __VA_ARGS__' problem */
  fail();
}
END_TEST
#endif /* __GNUC__ */
/* FIXME: all these line numbers are kind of hard to maintain */
START_TEST(test_ck_abort)
{
  ck_abort(); /* line 114 */
}
END_TEST

START_TEST(test_ck_abort_msg)
{
  ck_abort_msg("Failure expected"); /* line 120 */
}
END_TEST

/* FIXME: perhaps passing NULL to ck_abort_msg should be an error. */
START_TEST(test_ck_abort_msg_null)
{
  ck_abort_msg(NULL); /* line 127 */
}
END_TEST

START_TEST(test_ck_assert)
{
  int x = 3;
  int y = 3;
  ck_assert(1);
  ck_assert(x == y);
  y++;
  ck_assert(x != y);
  ck_assert(x == y); /* line 139 */
}
END_TEST

START_TEST(test_ck_assert_null)
{
  ck_assert(0); /* line 145 */
}
END_TEST
















START_TEST(test_ck_assert_int_eq)
{
  int x = 3;
  int y = 3;
  ck_assert_int_eq(x, y);
  y++;
  ck_assert_int_eq(x, y); /* line 170 */
}
END_TEST

START_TEST(test_ck_assert_int_ne)
{
  int x = 3;
  int y = 2;
  ck_assert_int_ne(x, y);
  y++;
  ck_assert_int_ne(x, y); /* line 180 */
}
END_TEST

START_TEST(test_ck_assert_str_eq)
{
  const char *s = "test2";
  ck_assert_str_eq("test2", s);
  ck_assert_str_eq("test1", s); /* line 188 */
}
END_TEST

START_TEST(test_ck_assert_str_ne)
{
  const char *s = "test2";
  const char *t = "test1";
  ck_assert_str_ne(t, s);
  t = "test2";
  ck_assert_str_ne(t, s); /* line 198 */
}
END_TEST



























START_TEST(test_segv) /* line 228 */
{
  raise (SIGSEGV);
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

#if TIMEOUT_TESTS_ENABLED
START_TEST(test_eternal) /* line 256 */
{
  for (;;)
    ;
}
END_TEST

START_TEST(test_sleep2)
{
  sleep(2);
}
END_TEST

START_TEST(test_sleep5) /* line 269 */
{
  sleep(5);
}
END_TEST

START_TEST(test_sleep8) /* line 275 */
{
  sleep(8);
}
END_TEST
#endif

START_TEST(test_early_exit)
{
  exit(EXIT_FAILURE);
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
  fail("Completed properly");
}
END_TEST

START_TEST(test_null_2)
{
  SRunner *sr = srunner_create(NULL);
  srunner_run_all (sr, CK_NORMAL);
  srunner_free (sr);
  fail("Completed properly");
}
END_TEST

START_TEST(test_fork1p_pass)
{
  pid_t pid;
  
  if((pid = fork()) < 0) {
    fail("Failed to fork new process");
  } else if (pid > 0) {
    fail_unless(1, NULL);
    kill(pid, SIGKILL);
  } else {
    for (;;) {
      sleep(1);
    }
  }
}
END_TEST

START_TEST(test_fork1p_fail)
{
  pid_t pid;
  
  if((pid = fork()) < 0) {
    fail("Failed to fork new process");
  } else if (pid > 0) {
    fail("Expected fail");
    kill(pid, SIGKILL);
  } else {
    for (;;) {
      sleep(1);
    }
  }
}
END_TEST

START_TEST(test_fork1c_pass)
{
  pid_t pid;
  
  if((pid = check_fork()) < 0) {
    fail("Failed to fork new process");
  } else if (pid > 0) {
    check_waitpid_and_exit(pid);
  } else {
    fail_unless(1, NULL);
    check_waitpid_and_exit(0);
  }
}
END_TEST

START_TEST(test_fork1c_fail)
{
  pid_t pid;
  
  if((pid = check_fork()) < 0) {
    fail("Failed to fork new process");
  } else if (pid == 0) {
    fail("Expected fail");
    check_waitpid_and_exit(0);
  }
  check_waitpid_and_exit(pid);
}
END_TEST

START_TEST(test_fork2_pass)
{
  pid_t pid;
  pid_t pid2;
  
  if((pid = check_fork()) < 0) {
    fail("Failed to fork new process");
  } else if (pid > 0) {
    if((pid2 = check_fork()) < 0) {
      fail("Failed to fork new process");
    } else if (pid2 == 0) {
      fail_unless(1, NULL);
      check_waitpid_and_exit(0);
    }
    check_waitpid_and_exit(pid2);
  }
  check_waitpid_and_exit(pid);
}
END_TEST

START_TEST(test_fork2_fail)
{
  pid_t pid;
  pid_t pid2;
  
  if((pid = check_fork()) < 0) {
    fail("Failed to fork new process");
  } else if (pid > 0) {
    if((pid2 = check_fork()) < 0) {
      fail("Failed to fork new process");
    } else if (pid2 == 0) {
      fail("Expected fail");
      check_waitpid_and_exit(0);
    }
    check_waitpid_and_exit(pid2);
    fail("Expected fail");
  }
  check_waitpid_and_exit(pid);
}
END_TEST

START_TEST(test_srunner)
{
  Suite *s;
  SRunner *sr;

  s = suite_create("Check Servant3");
  fail_unless(s != NULL, NULL);
  sr = srunner_create(NULL);
  fail_unless(sr != NULL, NULL);
  srunner_add_suite(sr, s);
  srunner_free(sr);

  sr = srunner_create(NULL);
  fail_unless(sr != NULL, NULL);
  srunner_add_suite(sr, NULL);
  srunner_free(sr);

  s = suite_create("Check Servant3");
  fail_unless(s != NULL, NULL);
  sr = srunner_create(s);
  fail_unless(sr != NULL, NULL);
  srunner_free(sr);
}
END_TEST

START_TEST(test_2nd_suite)
{
  fail("We failed");
}
END_TEST

Suite *make_sub2_suite(void)
{
  Suite *s = suite_create("Check Servant2");
  TCase *tc = tcase_create("Core");
  suite_add_tcase(s, tc);
  tcase_add_test(tc, test_srunner);
  tcase_add_test(tc, test_2nd_suite);

  return s;
}

Suite *make_sub_suite(void)
{
  Suite *s;

  TCase *tc_simple;
  TCase *tc_signal;
#if TIMEOUT_TESTS_ENABLED
  TCase *tc_timeout_env;
  TCase *tc_timeout;
  TCase *tc_timeout_usr;
#endif
  TCase *tc_limit;
  TCase *tc_messaging_and_fork;

  s = suite_create("Check Servant");

  tc_simple = tcase_create("Simple Tests");
  tc_signal = tcase_create("Signal Tests");
#if TIMEOUT_TESTS_ENABLED
  setenv("CK_DEFAULT_TIMEOUT", "6", 1);
  tc_timeout_env = tcase_create("Environment Timeout Tests");
  unsetenv("CK_DEFAULT_TIMEOUT");
  tc_timeout = tcase_create("Timeout Tests");
  tc_timeout_usr = tcase_create("User Timeout Tests");
#endif
  tc_limit = tcase_create("Limit Tests");
  tc_messaging_and_fork = tcase_create("Msg and fork Tests");

  suite_add_tcase (s, tc_simple);
  suite_add_tcase (s, tc_signal);
#if TIMEOUT_TESTS_ENABLED
  suite_add_tcase (s, tc_timeout_env);
  suite_add_tcase (s, tc_timeout);
  suite_add_tcase (s, tc_timeout_usr);
  /* Add a second time to make sure tcase_set_timeout doesn't contaminate it. */
  suite_add_tcase (s, tc_timeout);
#endif
  suite_add_tcase (s, tc_limit);
  suite_add_tcase (s, tc_messaging_and_fork);

  tcase_add_test (tc_simple, test_lno);
  tcase_add_test (tc_simple, test_mark_lno);
  tcase_add_test (tc_simple, test_pass);
  tcase_add_test (tc_simple, test_fail);
  tcase_add_test (tc_simple, test_fail_if_pass);
  tcase_add_test (tc_simple, test_fail_if_fail);
  tcase_add_test (tc_simple, test_fail_null_msg);
#if defined(__GNUC__)
  tcase_add_test (tc_simple, test_fail_no_msg);
#endif /* __GNUC__ */
  tcase_add_test (tc_simple, test_fail_if_null_msg);
#if defined(__GNUC__)
  tcase_add_test (tc_simple, test_fail_if_no_msg);
#endif /* __GNUC__ */
  tcase_add_test (tc_simple, test_fail_vararg_msg_1);
  tcase_add_test (tc_simple, test_fail_vararg_msg_2);
  tcase_add_test (tc_simple, test_fail_vararg_msg_3);
#if defined(__GNUC__)
  tcase_add_test (tc_simple, test_fail_empty);
#endif /* __GNUC__ */

  tcase_add_test (tc_simple, test_ck_abort);
  tcase_add_test (tc_simple, test_ck_abort_msg);
  tcase_add_test (tc_simple, test_ck_abort_msg_null);
  tcase_add_test (tc_simple, test_ck_assert);
  tcase_add_test (tc_simple, test_ck_assert_null);
  tcase_add_test (tc_simple, test_ck_assert_int_eq);
  tcase_add_test (tc_simple, test_ck_assert_int_ne);
  tcase_add_test (tc_simple, test_ck_assert_str_eq);
  tcase_add_test (tc_simple, test_ck_assert_str_ne);

  tcase_add_test (tc_signal, test_segv);
  tcase_add_test_raise_signal (tc_signal, test_segv, 11); /* pass  */
  tcase_add_test_raise_signal (tc_signal, test_segv, 8);  /* error */
  tcase_add_test_raise_signal (tc_signal, test_pass, 8);  /* fail  */
  tcase_add_test_raise_signal (tc_signal, test_fail, 8);  /* fail  */
  tcase_add_test (tc_signal, test_fpe);
  tcase_add_test (tc_signal, test_mark_point);

#if TIMEOUT_TESTS_ENABLED
  tcase_add_test (tc_timeout_env, test_eternal);
  tcase_add_test (tc_timeout_env, test_sleep2);
  tcase_add_test (tc_timeout_env, test_sleep5);
  tcase_add_test (tc_timeout_env, test_sleep8);

  tcase_add_test (tc_timeout, test_eternal);
  tcase_add_test (tc_timeout, test_sleep2);
  tcase_add_test (tc_timeout, test_sleep5);
  tcase_add_test (tc_timeout, test_sleep8);

  tcase_set_timeout (tc_timeout_usr, 6);
  tcase_add_test (tc_timeout_usr, test_eternal);
  tcase_add_test (tc_timeout_usr, test_sleep2);
  tcase_add_test (tc_timeout_usr, test_sleep5);
  tcase_add_test (tc_timeout_usr, test_sleep8);
#endif

  tcase_add_test (tc_limit, test_early_exit);
  tcase_add_test (tc_limit, test_null);
  tcase_add_test (tc_limit, test_null_2);

  tcase_add_test (tc_messaging_and_fork, test_fork1p_pass);
  tcase_add_test (tc_messaging_and_fork, test_fork1p_fail);
  tcase_add_test (tc_messaging_and_fork, test_fork1c_pass);
  tcase_add_test (tc_messaging_and_fork, test_fork1c_fail);
  tcase_add_test (tc_messaging_and_fork, test_fork2_pass);
  tcase_add_test (tc_messaging_and_fork, test_fork2_fail);

  return s;
}
