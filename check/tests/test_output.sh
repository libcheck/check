#!/bin/sh

t0="x"
t1="xRunning suite: Master
33%: Checks: 3, Failures: 1, Errors: 1"
t2="xRunning suite: Master
33%: Checks: 3, Failures: 1, Errors: 1
ex_output.c:13:F:Core: Failure
ex_output.c:17:E:Core: (after this point) Early exit with return value 1"
t3="xRunning suite: Master
33%: Checks: 3, Failures: 1, Errors: 1
ex_output.c:7:P:Core: Test passed
ex_output.c:13:F:Core: Failure
ex_output.c:17:E:Core: (after this point) Early exit with return value 1"

op0=`./ex_output CRSILENT`
op1=`./ex_output CRMINIMAL`
op2=`./ex_output CRNORMAL`
op3=`./ex_output CRVERBOSE`

if [ "$t0" != x"$op0" ]; then
    echo "Problem with ex_output CRSILENT";
    exit 1
fi
if [ "$t1" != x"$op1" ]; then
    echo "Problem with ex_output CRMINIMAL";
    exit 1
fi
if [ "$t2" != x"$op2" ]; then
    echo "Problem with ex_output CRNORMAL";
    exit 1
fi
if [ "$t3" != x"$op3" ]; then
    echo "Problem with ex_output CRVERBOSE";
    exit 1
fi

exit 0
