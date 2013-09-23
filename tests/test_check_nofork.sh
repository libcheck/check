#!/bin/sh

# For this test script, it is assumed that HAVE_FORK=0 implies
# running in a Windows environment. That means not only are
# fork-related tests not run, but also line endings are important.
# In *NIX environments line endings are \n, but in Windows they
# are expected to be \r\n. For this reason, HAVE_FORK=0 uses
# printf to generate the comparison strings, so the line endings
# can be controlled.

. ./test_vars

if [ $HAVE_FORK -eq 1 ]; then
expected="Running suite(s): NoFork
0%: Checks: 1, Failures: 1, Errors: 0"
else
expected=`printf "Running suite(s): NoFork\r
0%%: Checks: 1, Failures: 1, Errors: 0\r
"`
fi

actual=`./check_nofork${EXEEXT}`
if [ x"${expected}" = x"${actual}" ]; then
  exit 0
else
  echo "Problem with check_nofork${EXEEXT}"
  echo "Expected: "
  echo "${expected}"
  echo "Got: "
  echo "${actual}"
  
  echo xxd expected
  echo -n "${expected}" | xxd
  echo xxd actual
  echo -n "${actual}" | xxd
  exit 1
fi
