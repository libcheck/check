#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include <check_log.h>


START_TEST(test_set_log)
{
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  srunner_set_log (sr, "test_log");

  fail_unless (srunner_has_log (sr), "SRunner not logging");
}
END_TEST  

Suite *make_log_suite(void)
{

  Suite *s;
  TCase *tc_core;

  s = suite_create("Log Suite");
  tc_core = tcase_create("Core");

  suite_add_tcase(s, tc_core);
  tcase_add_test(tc_core, test_set_log);

  return s;
}

int main(void)
{
  int n;

  Suite *s;
  SRunner *sr;
  
  s = make_log_suite();
  sr = srunner_create (s);
  
  srunner_run_all (sr, CRNORMAL);
  n = srunner_ntests_failed(sr);
  srunner_free(sr);
  suite_free(s);
  return (n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

  
  
