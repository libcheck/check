#!/bin/sh

. ./test_vars

if [ x"${SRCDIR}" != x"." ]; then
    SRCDIR="${SRCDIR}/"
else
    SRCDIR=""
fi

t0="x"

if [ $HAVE_FORK -eq 1 ]; then
t1="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1"
else
t1="xRunning suite(s): Master
50%: Checks: 2, Failures: 1, Errors: 0"
fi

if [ $HAVE_FORK -eq 1 ]; then
t2="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:26:E:Core:test_exit:0: (after this point) Early exit with return value 1"
else
t2="xRunning suite(s): Master
50%: Checks: 2, Failures: 1, Errors: 0
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure"
fi

if [ $HAVE_FORK -eq 1 ]; then
t3="xRunning suite(s): Master
33%: Checks: 3, Failures: 1, Errors: 1
${SRCDIR}ex_output.c:11:P:Core:test_pass:0: Passed
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:26:E:Core:test_exit:0: (after this point) Early exit with return value 1"
else
t3="xRunning suite(s): Master
50%: Checks: 2, Failures: 1, Errors: 0
${SRCDIR}ex_output.c:11:P:Core:test_pass:0: Passed
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure"
fi

if [ $HAVE_FORK -eq 1 ]; then
t4="xtest: Core:test_pass
success: Core:test_pass
test: Core:test_fail
failure: Core:test_fail [
${SRCDIR}ex_output.c:17: Failure
]
test: Core:test_exit
error: Core:test_exit [
${SRCDIR}ex_output.c:26: (after this point) Early exit with return value 1
]"
else
t4="xtest: Core:test_pass
success: Core:test_pass
test: Core:test_fail
failure: Core:test_fail [
${SRCDIR}ex_output.c:17: Failure
]"
fi

op0=`./ex_output${EXEEXT} CK_SILENT   | tr -d "\r"`
op1=`./ex_output${EXEEXT} CK_MINIMAL  | tr -d "\r"`
op2=`./ex_output${EXEEXT} CK_NORMAL   | tr -d "\r"`
op3=`./ex_output${EXEEXT} CK_VERBOSE  | tr -d "\r"`
if test 1 -eq $ENABLE_SUBUNIT; then
op4=`./ex_output${EXEEXT} CK_SUBUNIT  | tr -d "\r"`
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
