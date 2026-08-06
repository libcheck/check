#!/usr/bin/env sh

OUTPUT_FILE=junit_test.xml
CK_DEFAULT_TIMEOUT=4

. ./test_vars
. $(dirname $0)/test_output_strings

rm -f ${OUTPUT_FILE}
export CK_DEFAULT_TIMEOUT
./ex_output${EXEEXT} CK_MINIMAL JUNIT_XML NORMAL > /dev/null
actual_junit_xml=`cat ${OUTPUT_FILE} | tr -d "\r" | grep -v \<duration\> | grep -v \<datetime\> | grep -v \<path\>`
if [ x"${expected_junit_xml}" != x"${actual_junit_xml}" ]; then
    echo "Problem with ex_xml_output${EXEEXT}";
    echo "Expected:";
    echo "${expected_junit_xml}";
    echo "Got:";
    echo "${actual_junit_xml}";
    exit 1;
fi

if [ ! -z `which xmllint` ]; then
    xmllint_output=`xmllint ${OUTPUT_FILE}`
    if [ $? -ne 0 ]; then
        echo "xmllint found an issue"
        echo ${xmllint_output}
        exit 1
    fi
fi

exit 0
