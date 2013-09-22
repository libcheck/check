#include "../lib/libcompat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "check_check.h"


/* save environment variable's value and set new value */
static int save_set_env(const char *name, const char *value,
                        const char **old_value)
{
  *old_value = getenv(name);
  return setenv(name, value, 1);
}

/* restore environment variable's old value, handle cases where
   variable must be unset (old value is NULL) */
static int restore_env(const char *name, const char *old_value)
{
  int res;
  if (old_value == NULL) {
     res = unsetenv(name);
  } else {
     res = setenv(name, old_value, 1);
  }
  return res;
}

START_TEST(test_set_log)
{
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  srunner_set_log (sr, "test_log");

  ck_assert_msg (srunner_has_log (sr), "SRunner not logging");
  ck_assert_msg (strcmp(srunner_log_fname(sr), "test_log") == 0,
	       "Bad file name returned");

  srunner_free(sr);
}
END_TEST

/* Test enabling logging via environment variable */
START_TEST(test_set_log_env)
{
  const char *old_val;
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  /* check that setting log file via environment variable works */
  ck_assert_msg(save_set_env("CK_LOG_FILE_NAME", "test_log", &old_val) == 0,
              "Failed to set environment variable");

  ck_assert_msg (srunner_has_log (sr), "SRunner not logging");
  ck_assert_msg (strcmp(srunner_log_fname(sr), "test_log") == 0,
	       "Bad file name returned");

  /* check that explicit call to srunner_set_log()
     overrides environment variable */
  srunner_set_log (sr, "test2_log");

  ck_assert_msg (srunner_has_log (sr), "SRunner not logging");
  ck_assert_msg (strcmp(srunner_log_fname(sr), "test2_log") == 0,
	       "Bad file name returned");

  /* restore old environment */
  ck_assert_msg(restore_env("CK_LOG_FILE_NAME", old_val) == 0,
              "Failed to restore environment variable");

  srunner_free(sr);
}
END_TEST

START_TEST(test_no_set_log)
{
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  ck_assert_msg (!srunner_has_log (sr), "SRunner not logging");
  ck_assert_msg (srunner_log_fname(sr) == NULL, "Bad file name returned");

  srunner_free(sr);
}
END_TEST

START_TEST(test_double_set_log)
{
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  srunner_set_log (sr, "test_log");
  srunner_set_log (sr, "test2_log");

  ck_assert_msg(strcmp(srunner_log_fname(sr), "test_log") == 0,
	      "Log file is initialize only and shouldn't be changeable once set");

  srunner_free(sr);
}
END_TEST


START_TEST(test_set_xml)
{
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  srunner_set_xml (sr, "test_log.xml");

  ck_assert_msg (srunner_has_xml (sr), "SRunner not logging XML");
  ck_assert_msg (strcmp(srunner_xml_fname(sr), "test_log.xml") == 0,
	       "Bad file name returned");
         
  srunner_free(sr);
}
END_TEST

/* Test enabling XML logging via environment variable */
START_TEST(test_set_xml_env)
{
  const char *old_val;
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  /* check that setting XML log file via environment variable works */
  ck_assert_msg(save_set_env("CK_XML_LOG_FILE_NAME", "test_log.xml", &old_val) == 0,
              "Failed to set environment variable");

  ck_assert_msg (srunner_has_xml (sr), "SRunner not logging XML");
  ck_assert_msg (strcmp(srunner_xml_fname(sr), "test_log.xml") == 0,
	       "Bad file name returned");

  /* check that explicit call to srunner_set_xml()
     overrides environment variable */
  srunner_set_xml (sr, "test2_log.xml");

  ck_assert_msg (srunner_has_xml (sr), "SRunner not logging XML");
  ck_assert_msg (strcmp(srunner_xml_fname(sr), "test2_log.xml") == 0,
	       "Bad file name returned");

  /* restore old environment */
  ck_assert_msg(restore_env("CK_XML_LOG_FILE_NAME", old_val) == 0,
              "Failed to restore environment variable");
  
  srunner_free(sr);
}
END_TEST

START_TEST(test_no_set_xml)
{
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  ck_assert_msg (!srunner_has_xml (sr), "SRunner not logging XML");
  ck_assert_msg (srunner_xml_fname(sr) == NULL, "Bad file name returned");
  
  srunner_free(sr);
}
END_TEST

START_TEST(test_double_set_xml)
{
  Suite *s = suite_create("Suite");
  SRunner *sr = srunner_create(s);

  srunner_set_xml (sr, "test_log.xml");
  srunner_set_xml (sr, "test2_log.xml");

  ck_assert_msg(strcmp(srunner_xml_fname(sr), "test_log.xml") == 0,
	      "XML Log file is initialize only and shouldn't be changeable once set");
  
  srunner_free(sr);
}
END_TEST

Suite *make_log_suite(void)
{

  Suite *s;
  TCase *tc_core, *tc_core_xml;

  s = suite_create("Log");
  tc_core = tcase_create("Core");
  tc_core_xml = tcase_create("Core XML");

  suite_add_tcase(s, tc_core);
  tcase_add_test(tc_core, test_set_log);
#if HAVE_WORKING_SETENV
  tcase_add_test(tc_core, test_set_log_env);
#endif /* HAVE_WORKING_SETENV */
  tcase_add_test(tc_core, test_no_set_log);
  tcase_add_test(tc_core, test_double_set_log);

  suite_add_tcase(s, tc_core_xml);
  tcase_add_test(tc_core_xml, test_set_xml);
#if HAVE_WORKING_SETENV
  tcase_add_test(tc_core_xml, test_set_xml_env);
#endif /* HAVE_WORKING_SETENV */
  tcase_add_test(tc_core_xml, test_no_set_xml);
  tcase_add_test(tc_core_xml, test_double_set_xml);

  return s;
}

