#!/bin/sh

t0="x"
t1="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1"
t2="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1
ex_output.c:13:F:Core: Failure
ex_output.c:17:E:Core: (after this point) Early exit with return value 1"
t3="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1
ex_output.c:7:P:Core: Passed
ex_output.c:13:F:Core: Failure
ex_output.c:17:E:Core: (after this point) Early exit with return value 1"

op0=`./ex_output CRSILENT`
op1=`./ex_output CRMINIMAL`
op2=`./ex_output CRNORMAL`
op3=`./ex_output CRVERBOSE`


test_output ( ) {
    if [ "${1}" != "${2}" ]; then
	echo "Problem with ex_output ${3}";
	echo "Expected:";
	echo "${1}";
	echo "Got:";
	echo "${2}";
	exit 1;
    fi
    
}

test_output "$t0" x"$op0" "CRSILENT";
test_output "$t1" x"$op1" "CRMINIMAL";
test_output "$t2" x"$op2" "CRNORMAL";
test_output "$t3" x"$op3" "CRVERBOSE";
exit 0
