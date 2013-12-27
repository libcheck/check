#!/bin/sh

OUTPUT_FILE=test.tap

. ./test_vars

if [ x"${SRCDIR}" != x"." ]; then
    SRCDIR="${SRCDIR}/"
else
    SRCDIR=""
fi

if [ $HAVE_FORK -eq 1 ]; then
expected_normal_tap="ok 1 - ${SRCDIR}ex_output.c:Core:test_pass: Passed
not ok 2 - ${SRCDIR}ex_output.c:Core:test_fail: Failure
not ok 3 - ${SRCDIR}ex_output.c:Core:test_exit: Early exit with return value 1
ok 4 - ${SRCDIR}ex_output.c:Core:test_pass2: Passed
not ok 5 - ${SRCDIR}ex_output.c:Core:test_loop: Iteration 0 failed
ok 6 - ${SRCDIR}ex_output.c:Core:test_loop: Passed
not ok 7 - ${SRCDIR}ex_output.c:Core:test_loop: Iteration 2 failed
not ok 8 - ${SRCDIR}ex_output.c:description \" ' < > &:test_xml_esc_fail_msg: fail \" ' < > & message
1..8"
expected_aborted_tap="ok 1 - ${SRCDIR}ex_output.c:Core:test_pass: Passed
not ok 2 - ${SRCDIR}ex_output.c:Core:test_fail: Failure
not ok 3 - ${SRCDIR}ex_output.c:Core:test_exit: Early exit with return value 1
not ok 4 - ${SRCDIR}ex_output.c:Core:test_abort: Early exit with return value 1
ok 5 - ${SRCDIR}ex_output.c:Core:test_pass2: Passed
not ok 6 - ${SRCDIR}ex_output.c:Core:test_loop: Iteration 0 failed
ok 7 - ${SRCDIR}ex_output.c:Core:test_loop: Passed
not ok 8 - ${SRCDIR}ex_output.c:Core:test_loop: Iteration 2 failed
not ok 9 - ${SRCDIR}ex_output.c:description \" ' < > &:test_xml_esc_fail_msg: fail \" ' < > & message
1..9"
else
expected_normal_tap="ok 1 - ${SRCDIR}ex_output.c:Core:test_pass: Passed
not ok 2 - ${SRCDIR}ex_output.c:Core:test_fail: Failure
ok 3 - ${SRCDIR}ex_output.c:Core:test_pass2: Passed
not ok 4 - ${SRCDIR}ex_output.c:Core:test_loop: Iteration 0 failed
ok 5 - ${SRCDIR}ex_output.c:Core:test_loop: Passed
not ok 6 - ${SRCDIR}ex_output.c:Core:test_loop: Iteration 2 failed
not ok 7 - ${SRCDIR}ex_output.c:description \" ' < > &:test_xml_esc_fail_msg: fail \" ' < > & message
1..7"
# When fork() is unavailable, one of the tests
# will invoke exit() which will terminate the
# unit testing program. In that case, the tap
# results will be incomplete, but the required
# test plan will be missing, signaling that
# something bad happened.
expected_aborted_tap="ok 1 - ${SRCDIR}ex_output.c:Core:test_pass: Passed
not ok 2 - ${SRCDIR}ex_output.c:Core:test_fail: Failure"
fi

test_tap_output ( ) {
    rm -f ${OUTPUT_FILE}
    ./ex_output${EXEEXT} "CK_SILENT" "TAP" "${1}" > /dev/null
    actual_tap=`cat ${OUTPUT_FILE} | tr -d "\r"`
    expected_tap="${2}"
    if [ x"${expected_tap}" != x"${actual_tap}" ]; then
        echo "Problem with ex_tap_output${EXEEXT}";
        echo "Expected:";
        echo "${expected_tap}";
        echo
        echo "Got:";
        echo "${actual_tap}";
        exit 1;
    fi
}

test_tap_output "NORMAL"     "${expected_normal_tap}"
test_tap_output "EXIT_TEST" "${expected_aborted_tap}"

exit 0
