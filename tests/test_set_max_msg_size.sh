#!/usr/bin/env sh

UNIT_TEST=./check_set_max_msg_size
MAX_MESSAGE_LOG_FILE=${UNIT_TEST}.set.output
TOO_LONG_MESSAGE="Message string too long"

# This test reduces the maximal message size using the provided function,
# so that the assertion message becomes too long.
# We set the maximal size to 32, which is shorter than the message.

rm -f ${MAX_MESSAGE_LOG_FILE}
${UNIT_TEST} 32 2>&1 | tee ${MAX_MESSAGE_LOG_FILE}

NUM_TOO_LONG_MESSAGES=$(grep "${TOO_LONG_MESSAGE}" ${MAX_MESSAGE_LOG_FILE} | wc -l)

if test ${NUM_TOO_LONG_MESSAGES} -gt 0; then
    echo "Maximal message size was reduced."
    rm -f ${MAX_MESSAGE_LOG_FILE}
    exit 0
else
    echo "ERROR: Maximal message size was not reduced."
    echo "Test output was preserved in ${MAX_MESSAGE_LOG_FILE}"
    exit 1
fi
