#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <check.h>
#include "error.h"
#include "check_magic.h"
#include "check_msg.h"
#include "check_check.h"

int msq;
static void msg_setup (void)
{
  msq = msgget(1, 0666 | IPC_CREAT);
  if (msq == -1)
    eprintf ("Unable to create message queue (%s):");
}

static void msg_teardown (void)
{
  if (msgctl (msq, IPC_RMID, NULL) == -1)
    eprintf ("Failed to free message queue:");
}

START_TEST(test_send_failure)
{
  FailureMsg *fmsg;
  /* Failure message sending and receiving need to be in equal pairs
     otherwise the world may fall off its axis...*/
  send_failure_msg (msq, "This is a test");
  fmsg = receive_failure_msg (msq);
  fail_unless (strcmp (fmsg->msg, "This is a test") == 0,
	       "Didn't receive the correct message");
  free(fmsg);
}
END_TEST

START_TEST(test_send_lastloc)
{
  LastLocMsg *lmsg;
  char *file;

  send_last_loc_msg (msq, "abc", 1);
  send_last_loc_msg (msq, "def", 2);
  lmsg = receive_last_loc_msg (msq);
  file = last_loc_file (lmsg);
  fail_unless (strcmp (file, "def") == 0,
	       "Didn't receive the correct file name");
  fail_unless (last_loc_line(lmsg) == 2,
	       "Didn't receive the correct line number");
  free(file);
  free(lmsg);
}
END_TEST

Suite *make_msg_suite (void)
{
  Suite *s;
  TCase *tc;
  s = suite_create("Msg");
  tc = tcase_create("Core Tests");
  tcase_set_fixture(tc, msg_setup, msg_teardown);
  tcase_add_test(tc, test_send_failure);
  tcase_add_test(tc, test_send_lastloc);
  suite_add_tcase(s, tc);
  return s;
}
