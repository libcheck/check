## Process this file with automake to produce Makefile.in

TESTS = \
	check_check_export	\
	check_check		\
	test_output.sh		\
	test_xml_output.sh	\
	test_log_output.sh

noinst_PROGRAMS = \
	check_check_export \
	check_check	\
	check_stress	\
	ex_output	\
	ex_xml_output	\
	ex_log_output

LDADD = ../lib/libcompat.la

EXTRA_DIST = test_output.sh test_log_output.sh test_xml_output.sh

if NO_TIMEOUT_TESTS
check_check_CFLAGS = -DTIMEOUT_TESTS_ENABLED=0
check_check_export_CFLAGS = -DTIMEOUT_TESTS_ENABLED=0
endif

check_check_export_SOURCES = \
	check_check.h		\
	check_check_sub.c	\
	check_check_master.c	\
	check_check_log.c	\
	check_check_fork.c	\
	check_check_export_main.c
check_check_export_LDADD = $(top_builddir)/src/libcheck.la

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
check_check_LDADD = $(top_builddir)/src/libcheckinternal.la

check_stress_SOURCES = check_stress.c
check_stress_LDADD = $(top_builddir)/src/libcheck.la

ex_output_SOURCES = ex_output.c
ex_output_LDADD = $(top_builddir)/src/libcheck.la

ex_log_output_SOURCES = ex_log_output.c
ex_log_output_LDADD = $(top_builddir)/src/libcheck.la

ex_xml_output_SOURCES = ex_xml_output.c
ex_xml_output_LDADD = $(top_builddir)/src/libcheck.la

AM_CPPFLAGS = -I$(top_srcdir)/src

CLEANFILES = *~ *.log *.xml test_logfile
