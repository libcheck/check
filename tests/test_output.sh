#!/bin/sh

# For this test script, it is assumed that HAVE_FORK=0 implies
# running in a Windows environment. That means not only are
# fork-related tests not run, but also line endings are important.
# In *NIX environments line endings are \n, but in Windows they
# are expected to be \r\n. For this reason, HAVE_FORK=0 uses
# printf to generate the comparison strings, so the line endings
# can be controlled.

. ./test_vars

t0="x"

if [ $HAVE_FORK -eq 1 ]; then
t1="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1"
else
t1=`printf "xRunning suite(s): Master\r
50%%: Checks: 2, Failures: 1, Errors: 0\r
"`
fi

if [ $HAVE_FORK -eq 1 ]; then
t2="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1
ex_output.c:17:F:Core:test_fail:0: Failure
ex_output.c:26:E:Core:test_exit:0: (after this point) Early exit with return value 1"
else
t2=`printf "xRunning suite(s): Master\r
50%%: Checks: 2, Failures: 1, Errors: 0\r
ex_output.c:17:F:Core:test_fail:0: Failure\r
"`
fi

if [ $HAVE_FORK -eq 1 ]; then
t3="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1
ex_output.c:11:P:Core:test_pass:0: Passed
ex_output.c:17:F:Core:test_fail:0: Failure
ex_output.c:26:E:Core:test_exit:0: (after this point) Early exit with return value 1"
else
t3=`printf "xRunning suite(s): Master\r
50%%: Checks: 2, Failures: 1, Errors: 0\r
ex_output.c:11:P:Core:test_pass:0: Passed\r
ex_output.c:17:F:Core:test_fail:0: Failure\r
"`
fi

if [ $HAVE_FORK -eq 1 ]; then
t4="xtest: Core:test_pass
success: Core:test_pass
test: Core:test_fail
failure: Core:test_fail [
ex_output.c:17: Failure
]
test: Core:test_exit
error: Core:test_exit [
ex_output.c:26: (after this point) Early exit with return value 1
]"
else
t4=`printf "xtest: Core:test_pass\r
success: Core:test_pass\r
test: Core:test_fail\r
failure: Core:test_fail [\r
ex_output.c:17: Failure\r
]\r
"`
fi

op0=`./ex_output${EXEEXT} CK_SILENT`
op1=`./ex_output${EXEEXT} CK_MINIMAL`
op2=`./ex_output${EXEEXT} CK_NORMAL`
op3=`./ex_output${EXEEXT} CK_VERBOSE`
if test 1 -eq $ENABLE_SUBUNIT; then
op4=`./ex_output${EXEEXT} CK_SUBUNIT`
fi


test_output ( ) {
    if [ "${1}" != "${2}" ]; then
	echo "Problem with ex_output${EXEEXT} ${3}";
	echo "Expected:";
	echo "${1}";
	echo "Got:";
	echo "${2}";
	exit 1;
    fi
    
}

test_output "$t0" x"$op0" "CK_SILENT";
test_output "$t1" x"$op1" "CK_MINIMAL";
test_output "$t2" x"$op2" "CK_NORMAL";
test_output "$t3" x"$op3" "CK_VERBOSE";
if test 1 -eq $ENABLE_SUBUNIT; then
test_output "$t4" x"$op4" "CK_SUBUNIT";
fi
exit 0
