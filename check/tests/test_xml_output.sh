#!/bin/sh

if [ "${srcdir}" = "." ]; then
    lsrc=""
else
    lsrc="${srcdir}/"
fi

expected="<?xml version=\"1.0\"?>
<testsuites xmlns=\"http://check.sourceforge.net/ns\">
  <suite>
    <title>S1</title>
    <test result=\"success\">
      <fn>ex_xml_output.c:8</fn>
      <id>test_pass</id>
      <description>Core</description>
      <message>Passed</message>
    </test>
    <test result=\"failure\">
      <fn>ex_xml_output.c:14</fn>
      <id>test_fail</id>
      <description>Core</description>
      <message>Failure</message>
    </test>
    <test result=\"error\">
      <fn>ex_xml_output.c:18</fn>
      <id>test_exit</id>
      <description>Core</description>
      <message>Early exit with return value 1</message>
    </test>
  </suite>
  <suite>
    <title>S2</title>
    <test result=\"success\">
      <fn>ex_xml_output.c:26</fn>
      <id>test_pass2</id>
      <description>Core</description>
      <message>Passed</message>
    </test>
  </suite>
</testsuites>"

./ex_xml_output > /dev/null
actual=`cat test.log.xml | grep -v duration | grep -v datetime`
if [ x"${expected}" != x"${actual}" ]; then
    echo "Problem with ex_log_output ${3}";
    echo "Expected:";
    echo "${expected}";
    echo "Got:";
    echo "${actual}";
    exit 1;
fi
    
exit 0
