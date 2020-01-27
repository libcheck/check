#include <stdlib.h>
#include <assert.h>
#include <check.h>
#include "test_suite.h"

START_TEST (testSimple) {
    ck_assert(1 == 1);
    ck_assert(1 + 1 == 2);
    ck_assert(strlen("Test") == 4);
}
END_TEST

void add_tests(size_t const n, TTest const * tests[n]) {
    assert(n > 0);
    size_t i = 0;
    tests[i++] = testSimple;
    assert(i <= n);
}


