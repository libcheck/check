#include <stdlib.h>
#include <stdio.h>
#include <check.h>

START_TEST(test_pass)
{
  fail_unless (1==1, "Shouldn't see this");
}
END_TEST

START_TEST(test_fail)
{
  fail("Failure");
}
END_TEST

START_TEST(test_exit)
{
  exit(1);
}
END_TEST

Suite *make_suite (void)
{
  Suite *s;
  TCase *tc;

  s = suite_create("Master");
  tc = tcase_create ("Core");
  suite_add_tcase(s, tc);
  tcase_add_test (tc, test_pass);
  tcase_add_test (tc, test_fail);
  tcase_add_test (tc, test_exit);

  return s;
}

void run_tests (int printmode)
{
  SRunner *sr;
  Suite *s;

  s = make_suite();
  sr = srunner_create(s);
  srunner_run_all(sr, printmode);
}

int main (int argc, char **argv)
{
  
  if (argc != 2) {
    printf ("Usage: ex_output (CRSILENT | CRMINIMAL | CRNORMAL | CRVERBOSE)\n");
    return EXIT_FAILURE;
  }

  if (strcmp (argv[1], "CRSILENT") == 0)
    run_tests(CRSILENT);
  else if (strcmp (argv[1], "CRMINIMAL") == 0)
    run_tests(CRMINIMAL);
  else if (strcmp (argv[1], "CRNORMAL") == 0)
    run_tests(CRNORMAL);
  else if (strcmp (argv[1], "CRVERBOSE") == 0)
    run_tests(CRVERBOSE);
  else {
    printf ("Usage: ex_output (CRSILENT | CRMINIMAL | CRNORMAL | CRVERBOSE)\n");
    return EXIT_FAILURE;
  }    
    

  return EXIT_SUCCESS;
}
  
