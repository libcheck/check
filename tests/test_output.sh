#!/bin/sh

. ./test_vars

if [ x"${SRCDIR}" != x"." ]; then
    SRCDIR="${SRCDIR}/"
else
    SRCDIR=""
fi

exp_silent=""

if [ $HAVE_FORK -eq 1 ]; then
exp_minimal="Running suite(s): S1
 S2
 XML escape \" ' < > & tests
37%: Checks: 8, Failures: 4, Errors: 1"
else
exp_minimal="Running suite(s): S1
 S2
 XML escape \" ' < > & tests
42%: Checks: 7, Failures: 4, Errors: 0"
fi

if [ $HAVE_FORK -eq 1 ]; then
exp_normal="Running suite(s): S1
 S2
 XML escape \" ' < > & tests
37%: Checks: 8, Failures: 4, Errors: 1
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:26:E:Core:test_exit:0: (after this point) Early exit with return value 1
${SRCDIR}ex_output.c:52:F:Core:test_loop:0: Iteration 0 failed
${SRCDIR}ex_output.c:52:F:Core:test_loop:2: Iteration 2 failed
${SRCDIR}ex_output.c:58:F:description \" ' < > &:test_xml_esc_fail_msg:0: fail \" ' < > & message"
else
exp_normal="Running suite(s): S1
 S2
 XML escape \" ' < > & tests
42%: Checks: 7, Failures: 4, Errors: 0
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:52:F:Core:test_loop:0: Iteration 0 failed
${SRCDIR}ex_output.c:52:F:Core:test_loop:2: Iteration 2 failed
${SRCDIR}ex_output.c:58:F:description \" ' < > &:test_xml_esc_fail_msg:0: fail \" ' < > & message"
fi

if [ $HAVE_FORK -eq 1 ]; then
exp_verbose="Running suite(s): S1
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
exp_verbose="Running suite(s): S1
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
exp_subunit="test: Core:test_pass
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
exp_subunit="test: Core:test_pass
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

act_silent=`./ex_output${EXEEXT} CK_SILENT STDOUT NORMAL  | tr -d "\r"`
act_minimal=`./ex_output${EXEEXT} CK_MINIMAL STDOUT NORMAL | tr -d "\r"`
act_normal=`./ex_output${EXEEXT} CK_NORMAL  STDOUT NORMAL | tr -d "\r"`
act_verbose=`./ex_output${EXEEXT} CK_VERBOSE STDOUT NORMAL | tr -d "\r"`
if test 1 -eq $ENABLE_SUBUNIT; then
act_subunit=`./ex_output${EXEEXT} CK_SUBUNIT STDOUT NORMAL | tr -d "\r"`
fi


test_output ( ) {
    if [ "x${1}" != "x${2}" ]; then
	echo "Problem with ex_output${EXEEXT} ${3}";
	echo "Expected:";
	echo "${1}";
	echo "Got:";
	echo "${2}";
	exit 1;
    fi
    
}

test_output "$exp_silent" "$act_silent" "CK_SILENT STDOUT NORMAL";
test_output "$exp_minimal" "$act_minimal" "CK_MINIMAL STDOUT NORMAL";
test_output "$exp_normal" "$act_normal" "CK_NORMAL STDOUT NORMAL";
test_output "$exp_verbose" "$act_verbose" "CK_VERBOSE STDOUT NORMAL";
if test 1 -eq $ENABLE_SUBUNIT; then
test_output "$exp_subunit" "$act_subunit" "CK_SUBUNIT STDOUT NORMAL";
fi
exit 0
