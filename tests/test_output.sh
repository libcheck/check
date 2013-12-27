#!/bin/sh

. ./test_vars

if [ x"${SRCDIR}" != x"." ]; then
    SRCDIR="${SRCDIR}/"
else
    SRCDIR=""
fi

t0="x"

if [ $HAVE_FORK -eq 1 ]; then
t1="xRunning suite(s): S1
 S2
 XML escape \" ' < > & tests
37%: Checks: 8, Failures: 4, Errors: 1"
else
t1="xRunning suite(s): S1
 S2
 XML escape \" ' < > & tests
42%: Checks: 7, Failures: 4, Errors: 0"
fi

if [ $HAVE_FORK -eq 1 ]; then
t2="xRunning suite(s): S1
 S2
 XML escape \" ' < > & tests
37%: Checks: 8, Failures: 4, Errors: 1
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:26:E:Core:test_exit:0: (after this point) Early exit with return value 1
${SRCDIR}ex_output.c:52:F:Core:test_loop:0: Iteration 0 failed
${SRCDIR}ex_output.c:52:F:Core:test_loop:2: Iteration 2 failed
${SRCDIR}ex_output.c:58:F:description \" ' < > &:test_xml_esc_fail_msg:0: fail \" ' < > & message"
else
t2="xRunning suite(s): S1
 S2
 XML escape \" ' < > & tests
42%: Checks: 7, Failures: 4, Errors: 0
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:52:F:Core:test_loop:0: Iteration 0 failed
${SRCDIR}ex_output.c:52:F:Core:test_loop:2: Iteration 2 failed
${SRCDIR}ex_output.c:58:F:description \" ' < > &:test_xml_esc_fail_msg:0: fail \" ' < > & message"
fi

if [ $HAVE_FORK -eq 1 ]; then
t3="xRunning suite(s): S1
 S2
 XML escape \" ' < > & tests
37%: Checks: 8, Failures: 4, Errors: 1
${SRCDIR}ex_output.c:11:P:Core:test_pass:0: Passed
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:26:E:Core:test_exit:0: (after this point) Early exit with return value 1
${SRCDIR}ex_output.c:46:P:Core:test_pass2:0: Passed
${SRCDIR}ex_output.c:52:F:Core:test_loop:0: Iteration 0 failed
${SRCDIR}ex_output.c:52:P:Core:test_loop:1: Passed
${SRCDIR}ex_output.c:52:F:Core:test_loop:2: Iteration 2 failed
${SRCDIR}ex_output.c:58:F:description \" ' < > &:test_xml_esc_fail_msg:0: fail \" ' < > & message"
else
t3="xRunning suite(s): S1
 S2
 XML escape \" ' < > & tests
42%: Checks: 7, Failures: 4, Errors: 0
${SRCDIR}ex_output.c:11:P:Core:test_pass:0: Passed
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:46:P:Core:test_pass2:0: Passed
${SRCDIR}ex_output.c:52:F:Core:test_loop:0: Iteration 0 failed
${SRCDIR}ex_output.c:52:P:Core:test_loop:1: Passed
${SRCDIR}ex_output.c:52:F:Core:test_loop:2: Iteration 2 failed
${SRCDIR}ex_output.c:58:F:description \" ' < > &:test_xml_esc_fail_msg:0: fail \" ' < > & message"
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
]
test: Core:test_pass2
success: Core:test_pass2
test: Core:test_loop
failure: Core:test_loop [
${SRCDIR}ex_output.c:52: Iteration 0 failed
]
test: Core:test_loop
success: Core:test_loop
test: Core:test_loop
failure: Core:test_loop [
${SRCDIR}ex_output.c:52: Iteration 2 failed
]
test: description \" ' < > &:test_xml_esc_fail_msg
failure: description \" ' < > &:test_xml_esc_fail_msg [
${SRCDIR}ex_output.c:58: fail \" ' < > & message
]"
else
t4="xtest: Core:test_pass
success: Core:test_pass
test: Core:test_fail
failure: Core:test_fail [
${SRCDIR}ex_output.c:17: Failure
]
test: Core:test_pass2
success: Core:test_pass2
test: Core:test_loop
failure: Core:test_loop [
${SRCDIR}ex_output.c:52: Iteration 0 failed
]
test: Core:test_loop
success: Core:test_loop
test: Core:test_loop
failure: Core:test_loop [
${SRCDIR}ex_output.c:52: Iteration 2 failed
]
test: description \" ' < > &:test_xml_esc_fail_msg
failure: description \" ' < > &:test_xml_esc_fail_msg [
${SRCDIR}ex_output.c:58: fail \" ' < > & message
]"
fi

op0=`./ex_output${EXEEXT} CK_SILENT STDOUT NORMAL  | tr -d "\r"`
op1=`./ex_output${EXEEXT} CK_MINIMAL STDOUT NORMAL | tr -d "\r"`
op2=`./ex_output${EXEEXT} CK_NORMAL  STDOUT NORMAL | tr -d "\r"`
op3=`./ex_output${EXEEXT} CK_VERBOSE STDOUT NORMAL | tr -d "\r"`
if test 1 -eq $ENABLE_SUBUNIT; then
op4=`./ex_output${EXEEXT} CK_SUBUNIT STDOUT NORMAL | tr -d "\r"`
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

test_output "$t0" x"$op0" "CK_SILENT STDOUT NORMAL";
test_output "$t1" x"$op1" "CK_MINIMAL STDOUT NORMAL";
test_output "$t2" x"$op2" "CK_NORMAL STDOUT NORMAL";
test_output "$t3" x"$op3" "CK_VERBOSE STDOUT NORMAL";
if test 1 -eq $ENABLE_SUBUNIT; then
test_output "$t4" x"$op4" "CK_SUBUNIT STDOUT NORMAL";
fi
exit 0
