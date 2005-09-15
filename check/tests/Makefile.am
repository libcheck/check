TESTS = \
	check_check		\
	test_output.sh		\
	test_xml_output.sh	\
	test_log_output.sh

noinst_PROGRAMS = \
	check_check	\
	check_stress	\
	ex_output	\
	ex_xml_output	\
	ex_log_output

EXTRA_DIST = test_output.sh test_log_output.sh test_xml_output.sh

if NO_TIMEOUT_TESTS
check_check_CFLAGS = -DTIMEOUT_TESTS_ENABLED=0
endif

check_check_SOURCES = \
	check_check.h		\
	check_list.c		\
	check_check_sub.c	\
	check_check_master.c	\
	check_check_msg.c	\
	check_check_log.c	\
	check_check_limit.c	\
	check_check_fork.c	\
	check_check_fixture.c	\
	check_check_pack.c	\
	check_check_main.c

check_stress_SOURCES = check_stress.c

ex_output_SOURCES = ex_output.c

ex_log_output_SOURCES = ex_log_output.c

ex_xml_output_SOURCES = ex_xml_output.c

LIBCHECK_A=../src/libcheck.a
INCLUDES = -I$(srcdir)/../src
LDADD = $(LIBCHECK_A)

CLEANFILES = *~ *.log test_logfile

$(LIBCHECK_A):
	$(MAKE) -C ../src
