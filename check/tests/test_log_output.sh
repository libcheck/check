#!/bin/sh

expected="Running suite S1
ex_log_output.c:7:P:Core: Test passed
ex_log_output.c:13:F:Core: Failure
ex_log_output.c:17:E:Core: (after this point) Early exit with return value 1
Running suite S2
ex_log_output.c:21:P:Core: Test passed
Results for all suites run:
25%: Checks: 4, Failures: 1, Errors: 1"


test_log_output ( ) {
    
    ./ex_log_output "${1}"
    actual=`cat test_logfile`
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
