#!/bin/sh

if [ "${srcdir}" = "." ]; then
    lsrc=""
else
    lsrc="${srcdir}/"
fi

expected="Running suite S1
${lsrc}ex_log_output.c:8:P:Core:test_pass: Passed
${lsrc}ex_log_output.c:14:F:Core:test_fail: Failure
${lsrc}ex_log_output.c:18:E:Core:test_exit: (after this point) Early exit with return value 1
Running suite S2
${lsrc}ex_log_output.c:26:P:Core:test_pass2: Passed
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

test_log_output "CK_SILENT";
test_log_output "CK_MINIMAL";
test_log_output "CK_NORMAL";
test_log_output "CK_VERBOSE";
exit 0
