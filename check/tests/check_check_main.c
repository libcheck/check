#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "check_check.h"

int main (void)
{
  int n;


  Suite *s;
  SRunner *sr;
  s = main_make_suite();
  sr = srunner_create (s);
  
  setup();
  printf ("Ran %d tests in subordinate suite\n", sub_nfailed);
  srunner_run_all (sr, CRNORMAL);
  cleanup();
  n = srunner_ntests_failed(sr);
  srunner_free(sr);
  suite_free(s);
  return (n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
