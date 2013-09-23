#!/bin/sh

# For this test script, it is assumed that HAVE_FORK=0 implies
# running in a Windows environment. That means not only are
# fork-related tests not run, but also line endings are important.
# In *NIX environments line endings are \n, but in Windows they
# are expected to be \r\n. For this reason, HAVE_FORK=0 uses
# printf to generate the comparison strings, so the line endings
# can be controlled.

. ./test_vars

if [ $HAVE_FORK -eq 1 ]; then
expected="Running suite S1
ex_log_output.c:10:P:Core:test_pass:0: Passed
ex_log_output.c:16:F:Core:test_fail:0: Failure
ex_log_output.c:25:E:Core:test_exit:0: (after this point) Early exit with return value 1
Running suite S2
ex_log_output.c:34:P:Core:test_pass2:0: Passed
Results for all suites run:
50%: Checks: 4, Failures: 1, Errors: 1"
else
expected=`printf "Running suite S1\r
ex_log_output.c:10:P:Core:test_pass:0: Passed\r
ex_log_output.c:16:F:Core:test_fail:0: Failure\r
Running suite S2\r
ex_log_output.c:34:P:Core:test_pass2:0: Passed\r
Results for all suites run:\r
66%%: Checks: 3, Failures: 1, Errors: 0\r
"`
fi

test_log_output ( ) {
    
    ./ex_log_output${EXEEXT} "${1}" > /dev/null
    actual=`cat test.log`
    if [ x"${expected}" != x"${actual}" ]; then
	echo "Problem with ex_log_output${EXEEXT} ${3}";
	echo "Expected:";
	echo "${expected}";
	echo "Got:";
	echo "${actual}";
	exit 1;
    fi
    
}

test_log_output "CK_SILENT";
test_log_output "CK_MINIMAL";
test_log_output "CK_NORMAL";
test_log_output "CK_VERBOSE";
exit 0
