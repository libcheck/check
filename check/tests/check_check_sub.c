#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <check.h>
#include "check_check.h"








START_TEST(test_lno)
{
  fail("Failure expected"); /*line 18*/
}
END_TEST

START_TEST(test_mark_lno)
{
  mark_point(); /*line 24*/
  exit(EXIT_FAILURE); /*should fail at line 24*/
}
END_TEST

START_TEST(test_pass)
{
  fail_unless(1 == 1, "This test should pass");
  fail_unless(9999, "This test should pass");
}
END_TEST

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


START_TEST(test_fail_no_msg)
{
  fail_unless(4 == 5);
}
END_TEST

START_TEST(test_fail_if_null_msg)
{
  fail_if(2 != 3, NULL);
}
END_TEST


START_TEST(test_fail_if_no_msg)
{
  fail_if(4 != 5);
}
END_TEST

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

START_TEST(test_fail_empty)
{
  fail();
}
END_TEST

START_TEST(test_segv) /* line 111 */
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

START_TEST(test_eternal) /* line 139 */
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

START_TEST(test_sleep5) /* line 152 */
{
  sleep(5);
}
END_TEST

START_TEST(test_sleep8) /* line 158 */
{
  sleep(8);
}
END_TEST


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
    fail_unless(1);
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
    fail_unless(1);
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
      fail_unless(1);
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
  tcase_add_test(tc, test_2nd_suite);

  return s;
}

Suite *make_sub_suite(void)
{
  Suite *s = suite_create("Check Servant");

  TCase *tc_simple = tcase_create("Simple Tests");
  TCase *tc_signal = tcase_create("Signal Tests");
  TCase *tc_timeout = tcase_create("Timeout Tests");
  TCase *tc_timeout_ext = tcase_create("Extended Timeout Tests");
  TCase *tc_limit = tcase_create("Limit Tests");
  TCase *tc_messaging_and_fork = tcase_create("Msg and fork Tests");

  suite_add_tcase (s, tc_simple);
  suite_add_tcase (s, tc_signal);
  suite_add_tcase (s, tc_timeout);
  suite_add_tcase (s, tc_timeout_ext);
  /* Add a second time to make sure tcase_set_timeout doesn't contaminate it. */
  suite_add_tcase (s, tc_timeout);
  suite_add_tcase (s, tc_limit);
  suite_add_tcase (s, tc_messaging_and_fork);

  tcase_add_test (tc_simple, test_lno);
  tcase_add_test (tc_simple, test_mark_lno);
  tcase_add_test (tc_simple, test_pass);
  tcase_add_test (tc_simple, test_fail);
  tcase_add_test (tc_simple, test_fail_if_pass);
  tcase_add_test (tc_simple, test_fail_if_fail);
  tcase_add_test (tc_simple, test_fail_null_msg);
  tcase_add_test (tc_simple, test_fail_no_msg);
  tcase_add_test (tc_simple, test_fail_if_null_msg);
  tcase_add_test (tc_simple, test_fail_if_no_msg);
  tcase_add_test (tc_simple, test_fail_vararg_msg_1);
  tcase_add_test (tc_simple, test_fail_vararg_msg_2);
  tcase_add_test (tc_simple, test_fail_vararg_msg_3);
  tcase_add_test (tc_simple, test_fail_empty);

  tcase_add_test (tc_signal, test_segv);
  tcase_add_test_raise_signal (tc_signal, test_segv, 11); /* pass  */
  tcase_add_test_raise_signal (tc_signal, test_segv, 8);  /* error */
  tcase_add_test_raise_signal (tc_signal, test_pass, 8);  /* fail  */
  tcase_add_test_raise_signal (tc_signal, test_fail, 8);  /* fail  */
  tcase_add_test (tc_signal, test_fpe);
  tcase_add_test (tc_signal, test_mark_point);

  tcase_add_test (tc_timeout, test_eternal);
  tcase_add_test (tc_timeout, test_sleep2);
  tcase_add_test (tc_timeout, test_sleep5);
  tcase_add_test (tc_timeout, test_sleep8);

  tcase_set_timeout (tc_timeout_ext, 6);
  tcase_add_test (tc_timeout_ext, test_eternal);
  tcase_add_test (tc_timeout_ext, test_sleep2);
  tcase_add_test (tc_timeout_ext, test_sleep5);
  tcase_add_test (tc_timeout_ext, test_sleep8);

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
