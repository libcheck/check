#!/bin/sh

OUTPUT_FILE=test.log

. ./test_vars

if [ $HAVE_FORK -eq 1 ]; then
expected="Running suite S1
${SRCDIR}ex_output.c:11:P:Core:test_pass:0: Passed
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
${SRCDIR}ex_output.c:26:E:Core:test_exit:0: (after this point) Early exit with return value 1
Running suite S2
${SRCDIR}ex_output.c:46:P:Core:test_pass2:0: Passed
${SRCDIR}ex_output.c:52:F:Core:test_loop:0: Iteration 0 failed
${SRCDIR}ex_output.c:52:P:Core:test_loop:1: Passed
${SRCDIR}ex_output.c:52:F:Core:test_loop:2: Iteration 2 failed
Running suite XML escape \" ' < > & tests
${SRCDIR}ex_output.c:58:F:description \" ' < > &:test_xml_esc_fail_msg:0: fail \" ' < > & message
Results for all suites run:
37%: Checks: 8, Failures: 4, Errors: 1"
else
expected="Running suite S1
${SRCDIR}ex_output.c:11:P:Core:test_pass:0: Passed
${SRCDIR}ex_output.c:17:F:Core:test_fail:0: Failure
Running suite S2
${SRCDIR}ex_output.c:46:P:Core:test_pass2:0: Passed
${SRCDIR}ex_output.c:52:F:Core:test_loop:0: Iteration 0 failed
${SRCDIR}ex_output.c:52:P:Core:test_loop:1: Passed
${SRCDIR}ex_output.c:52:F:Core:test_loop:2: Iteration 2 failed
Running suite XML escape \" ' < > & tests
${SRCDIR}ex_output.c:58:F:description \" ' < > &:test_xml_esc_fail_msg:0: fail \" ' < > & message
Results for all suites run:
42%: Checks: 7, Failures: 4, Errors: 0"
fi

test_log_output ( ) {
    rm -f ${OUTPUT_FILE}
    ./ex_output${EXEEXT} "${1}" "LOG" "NORMAL" > /dev/null
    actual=`cat ${OUTPUT_FILE} | tr -d "\r"`
    if [ x"${expected}" != x"${actual}" ]; then
	echo "Problem with ex_log_output${EXEEXT} ${1} LOG NORMAL";
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
