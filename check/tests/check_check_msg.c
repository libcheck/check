#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "error.h"
#include "check_magic.h"
#include "check_msg.h"
#include "check_check.h"

START_TEST(test_send)
{
  TestResult *tr;
  setup_test_messaging();
  send_ctx_info(get_test_key(),CK_CTX_SETUP);
  send_loc_info(get_test_key(),"abc123.c", 10);
  send_ctx_info(get_test_key(),CK_CTX_TEST);
  send_loc_info(get_test_key(),"abc123.c", 22);
  send_loc_info(get_test_key(),"abc123.c", 25);
  send_failure_info(get_test_key(),"Oops");
  tr = receive_test_result(get_test_key());
  teardown_test_messaging();

  fail_unless (tr_ctx(tr) == CK_CTX_TEST,
	       "Bad CTX received");
  fail_unless (strcmp(tr_msg(tr), "Oops") == 0,
	       "Bad failure msg received");
  fail_unless (strcmp(tr_lfile(tr), "abc123.c") == 0,
	       "Bad loc file received");
  fail_unless (tr_lno(tr) == 25,
	       "Bad loc line received");
}
END_TEST

Suite *make_msg_suite (void)
{
  Suite *s;
  TCase *tc;
  s = suite_create("Msg");
  tc = tcase_create("Core Tests");
  tcase_add_test(tc, test_send);
  suite_add_tcase(s, tc);
  return s;
}
