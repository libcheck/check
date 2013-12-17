#!/bin/sh

OUTPUT_FILE=test.tap

. ./test_vars

if [ $HAVE_FORK -eq 1 ]; then
expected_normal_tap="ok 1
not ok 2
not ok 3
ok 4
1..4"
expected_aborted_tap="ok 1
not ok 2
not ok 3
not ok 4
ok 5
1..5"
else
expected_normal_tap="ok 1
not ok 2
ok 3
1..3"
# When fork() is unavailable, one of the tests
# will invoke exit() which will terminate the
# unit testing program. In that case, the tap
# results will be incomplete, but the required
# test plan will be missing, signaling that
# something bad happened.
expected_aborted_tap="ok 1
not ok 2"
fi

test_tap_output ( ) {
    ./ex_tap_output${EXEEXT} "${1}" > /dev/null
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
test_tap_output "TEST_ABORT" "${expected_aborted_tap}"

exit 0
