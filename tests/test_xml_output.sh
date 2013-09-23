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
expected_xml="<?xml version=\"1.0\"?>
<?xml-stylesheet type=\"text/xsl\" href=\"http://check.sourceforge.net/xml/check_unittest.xslt\"?>
<testsuites xmlns=\"http://check.sourceforge.net/ns\">
  <suite>
    <title>S1</title>
    <test result=\"success\">
      <fn>ex_xml_output.c:10</fn>
      <id>test_pass</id>
      <iteration>0</iteration>
      <description>Core</description>
      <message>Passed</message>
    </test>
    <test result=\"failure\">
      <fn>ex_xml_output.c:16</fn>
      <id>test_fail</id>
      <iteration>0</iteration>
      <description>Core</description>
      <message>Failure</message>
    </test>
    <test result=\"error\">
      <fn>ex_xml_output.c:25</fn>
      <id>test_exit</id>
      <iteration>0</iteration>
      <description>Core</description>
      <message>Early exit with return value 1</message>
    </test>
  </suite>
  <suite>
    <title>S2</title>
    <test result=\"success\">
      <fn>ex_xml_output.c:34</fn>
      <id>test_pass2</id>
      <iteration>0</iteration>
      <description>Core</description>
      <message>Passed</message>
    </test>
    <test result=\"failure\">
      <fn>ex_xml_output.c:40</fn>
      <id>test_loop</id>
      <iteration>0</iteration>
      <description>Core</description>
      <message>Iteration 0 failed</message>
    </test>
    <test result=\"success\">
      <fn>ex_xml_output.c:40</fn>
      <id>test_loop</id>
      <iteration>1</iteration>
      <description>Core</description>
      <message>Passed</message>
    </test>
    <test result=\"failure\">
      <fn>ex_xml_output.c:40</fn>
      <id>test_loop</id>
      <iteration>2</iteration>
      <description>Core</description>
      <message>Iteration 2 failed</message>
    </test>
  </suite>
  <suite>
    <title>XML escape &quot; &apos; &lt; &gt; &amp; tests</title>
    <test result=\"failure\">
      <fn>ex_xml_output.c:46</fn>
      <id>test_xml_esc_fail_msg</id>
      <iteration>0</iteration>
      <description>description &quot; &apos; &lt; &gt; &amp;</description>
      <message>fail &quot; &apos; &lt; &gt; &amp; message</message>
    </test>
  </suite>
</testsuites>"
expected_duration_count=9
expected_duration_error=5
else
expected_xml=`printf "<?xml version=\"1.0\"?>\r
<?xml-stylesheet type=\"text/xsl\" href=\"http://check.sourceforge.net/xml/check_unittest.xslt\"?>\r
<testsuites xmlns=\"http://check.sourceforge.net/ns\">\r
  <suite>\r
    <title>S1</title>\r
    <test result=\"success\">\r
      <fn>ex_xml_output.c:10</fn>\r
      <id>test_pass</id>\r
      <iteration>0</iteration>\r
      <description>Core</description>\r
      <message>Passed</message>\r
    </test>\r
    <test result=\"failure\">\r
      <fn>ex_xml_output.c:16</fn>\r
      <id>test_fail</id>\r
      <iteration>0</iteration>\r
      <description>Core</description>\r
      <message>Failure</message>\r
    </test>\r
  </suite>\r
  <suite>\r
    <title>S2</title>\r
    <test result=\"success\">\r
      <fn>ex_xml_output.c:34</fn>\r
      <id>test_pass2</id>\r
      <iteration>0</iteration>\r
      <description>Core</description>\r
      <message>Passed</message>\r
    </test>\r
    <test result=\"failure\">\r
      <fn>ex_xml_output.c:40</fn>\r
      <id>test_loop</id>\r
      <iteration>0</iteration>\r
      <description>Core</description>\r
      <message>Iteration 0 failed</message>\r
    </test>\r
    <test result=\"success\">\r
      <fn>ex_xml_output.c:40</fn>\r
      <id>test_loop</id>\r
      <iteration>1</iteration>\r
      <description>Core</description>\r
      <message>Passed</message>\r
    </test>\r
    <test result=\"failure\">\r
      <fn>ex_xml_output.c:40</fn>\r
      <id>test_loop</id>\r
      <iteration>2</iteration>\r
      <description>Core</description>\r
      <message>Iteration 2 failed</message>\r
    </test>\r
  </suite>\r
  <suite>\r
    <title>XML escape &quot; &apos; &lt; &gt; &amp; tests</title>\r
    <test result=\"failure\">\r
      <fn>ex_xml_output.c:46</fn>\r
      <id>test_xml_esc_fail_msg</id>\r
      <iteration>0</iteration>\r
      <description>description &quot; &apos; &lt; &gt; &amp;</description>\r
      <message>fail &quot; &apos; &lt; &gt; &amp; message</message>\r
    </test>\r
  </suite>\r
</testsuites>\r
"`
expected_duration_count=8
expected_duration_error=4
fi

./ex_xml_output${EXEEXT} > /dev/null
actual_xml=`cat test.log.xml | grep -v \<duration\> | grep -v \<datetime\> | grep -v \<path\>`
if [ x"${expected_xml}" != x"${actual_xml}" ]; then
    echo "Problem with ex_xml_output${EXEEXT} ${3}";
    echo "Expected:";
    echo "${expected_xml}";
    echo "Got:";
    echo "${actual_xml}";
    exit 1;
fi

actual_duration_count=`grep -c \<duration\> test.log.xml`
if [ x"${expected_duration_count}" != x"${actual_duration_count}" ]; then
    echo "Wrong number of <duration> elements in test.log.xml, ${expected_duration_count} vs ${actual_duration_count}";
    exit 1;
fi

actual_duration_error=`cat test.log.xml | grep \<duration\> | grep -c "\-1\.000000"`
if [ x"${expected_duration_error}" != x"${actual_duration_error}" ]; then
    echo "Wrong format for (or number of) error <duration> elements in test.log.xml, ${expected_duration_error} vs ${actual_duration_error}";
    exit 1;
fi

exit 0
