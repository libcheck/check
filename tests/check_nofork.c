#include "../lib/libcompat.h"

#include <stdlib.h>
#include <stdio.h>
#include <check.h>


Suite *s;
TCase *tc;
SRunner *sr;

START_TEST(test_nofork_exit)
{
  char* s = NULL;

  ck_assert(NULL != s);

  /* this test should not crash in nofork mode */
  ck_assert_str_eq("test", s);
}
END_TEST

int main(void)
{
  s = suite_create("NoFork");
  tc = tcase_create("Exit");
  sr = srunner_create(s);

  suite_add_tcase(s, tc);
  tcase_add_test(tc, test_nofork_exit);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_MINIMAL);
  srunner_free(sr);

  return 0;
}
