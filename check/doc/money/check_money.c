#include <stdlib.h>
#include <check.h>
#include "money.h"

Money *five_dollars;
void setup (void)
{
  five_dollars = money_create(5, "USD");
}

void teardown (void)
{
  money_free (five_dollars);
}


START_TEST(test_create)
{
  fail_unless (money_amount(five_dollars) == 5,
	       "Amount not set correctly on creation");
  fail_unless (strcmp(money_currency(five_dollars),"USD") == 0,
	       "Currency not set correctly on creation");
}
END_TEST

START_TEST(test_neg_create)
{
  Money *m = money_create(-1, "USD");
  fail_unless (m == NULL, "NULL should be returned on attempt to create with a negative amount");
}
END_TEST

START_TEST(test_zero_create)
{
  Money *m = money_create(0, "USD");
  fail_unless (money_amount(m) == 0,
	       "Zero is a valid amount of money");
}
END_TEST

Suite *money_suite (void)
{
  Suite *s = suite_create("Money");
  TCase *tc_core = tcase_create("Core");
  TCase *tc_limits = tcase_create("Limits");
  suite_add_tcase (s, tc_core);
  suite_add_tcase (s, tc_limits);
  tcase_add_test (tc_core, test_create);
  tcase_set_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_limits, test_neg_create);
  tcase_add_test (tc_limits, test_zero_create);
  return s;
}

int main (void)
{
  int nf;
  Suite *s = money_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all (sr, CRNORMAL);
  nf = srunner_nfailures(sr);
  srunner_free(sr);
  suite_free(s);
  return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
