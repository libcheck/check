#!/bin/sh

expected="Running suite S1
ex_log_output.c:7:P:Core: Passed
ex_log_output.c:13:F:Core: Failure
ex_log_output.c:17:E:Core: (after this point) Early exit with return value 1
Running suite S2
ex_log_output.c:25:P:Core: Passed
Results for all suites run:
50%: Checks: 4, Failures: 1, Errors: 1"


test_log_output ( ) {
    
    ./ex_log_output "${1}" > /dev/null
    actual=`cat test.log`
    if [ x"${expected}" != x"${actual}" ]; then
	echo "Problem with ex_log_output ${3}";
	echo "Expected:";
	echo "${expected}";
	echo "Got:";
	echo "${actual}";
	exit 1;
    fi
    
}

test_log_output "CRSILENT";
test_log_output "CRMINIMAL";
test_log_output "CRNORMAL";
test_log_output "CRVERBOSE";
exit 0
