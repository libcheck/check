#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <check.h>
#include "error.h"
#include "check_magic.h"
#include "check_msg.h"
#include "check_check.h"

MsgSys *msgsys;

enum {
  KEY = 1
};

static void msg_setup (void)
{
  msgsys = create_msgsys_with_key(KEY);
}

static void msg_teardown (void)
{
  delete_msgsys_with_key(KEY);
}

START_TEST(test_send_failure)
{
  char *fmsg;
  /* Failure message sending and receiving need to be in equal pairs
     otherwise the world may fall off its axis...*/
  send_failure_msg (msgsys, "This is a test");
  fmsg = receive_failure_msg (msgsys);
  fail_unless (strcmp (fmsg, "This is a test") == 0,
	       "Didn't receive the correct message");
  free(fmsg);
}
END_TEST

START_TEST(test_send_lastloc)
{
  Loc *loc;

  send_last_loc_msg (msgsys, "abc", 1);
  send_last_loc_msg (msgsys, "def", 2);
  loc = receive_last_loc_msg (msgsys);
  fail_unless (strcmp (loc->file, "def") == 0,
	       "Didn't receive the correct file name");
  fail_unless (loc->line == 2,
	       "Didn't receive the correct line number");
  free(loc);
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
