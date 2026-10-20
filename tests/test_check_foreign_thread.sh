#!/bin/sh

fail()
{
    printf '%s: %s\n' "$mode" "$1" >&2
    cat foreign-thread.out >&2
    exit 1
}

# Assertions on another thread must be reported without invoking exit handlers.
for mode in body setup teardown; do
    ./check_foreign_thread "$mode" > foreign-thread.out 2>&1
    status=$?
    test "$status" = 77 && exit 77
    test "$status" = 1 || fail "expected exit status 1, got $status"
    grep -q 'assertion without an active test jump target' foreign-thread.out ||
        fail 'missing fatal diagnostic'
    grep -q 'foreign-thread diagnostic probe' foreign-thread.out ||
        fail 'missing assertion message'
    if grep -q ATEXIT_RAN foreign-thread.out; then
        fail 'exit handler unexpectedly ran'
    fi
done

# These modes must retain normal runner reporting and process termination.
for mode in same fork; do
    ./check_foreign_thread "$mode" > foreign-thread.out 2>&1
    status=$?
    if test "$mode" = fork && test "$status" = 77; then
        continue
    fi
    test "$status" = 1 || fail "expected exit status 1, got $status"
    grep -q 'Failures: 1, Errors: 0' foreign-thread.out ||
        fail 'missing normal failure summary'
    grep -q ATEXIT_RAN foreign-thread.out ||
        fail 'exit handler did not run'
done
rm -f foreign-thread.out
