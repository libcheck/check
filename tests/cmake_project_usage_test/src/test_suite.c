#include <stdlib.h>
#include <check.h>
#include "test_suite.h"

static Suite * test_suite(void) {
    Suite *s;
    TCase *tc_core;
    TTest const * tests[MAX_TESTS_IN_SUITE] = { 0 };
    add_tests(MAX_TESTS_IN_SUITE, tests);

    s = suite_create("ProjectUsageTest");

    /* Core test case */
    tc_core = tcase_create("Core");

    for(size_t i = 0; i < MAX_TESTS_IN_SUITE; ++i) {
        TTest const * test = tests[i];
        if(test) {
            tcase_add_test(tc_core, test);
        }
    }
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = test_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

