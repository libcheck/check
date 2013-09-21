#!/bin/sh

. ./test_vars

expected="Running suite(s): NoFork
0%: Checks: 1, Failures: 1, Errors: 0"

actual=`./check_nofork${EXEEXT} 2>&1`
if [ x"${expected}" = x"${actual}" ]; then
  exit 0
else
  echo "Problem with check_nofork${EXEEXT}"
  echo "Expected: Failure"
  echo "Got: Segmentation fault"
  exit 1
fi
