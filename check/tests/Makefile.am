TESTS=\
	check_check\
	check_list\
	check_check_msg\
	check_check_log\
	test_output.sh

noinst_PROGRAMS=\
	check_check\
	check_list\
	check_stress\
	check_check_msg\
	check_check_log\
	ex_output

EXTRA_DIST=test_output.sh

check_check_log_SOURCES=\
	check_log.c

check_check_SOURCES= \
	check_check.h check_check_sub.c check_check_master.c check_check_main.c

check_list_SOURCES=\
	check_list.c

check_stress_SOURCES=\
	check_stress.c

check_check_msg_SOURCES=\
	check_check_msg.c

ex_output_SOURCES=\
	ex_output.c

INCLUDES= -I$(srcdir)/../src
LDADD= ../src/libcheck.a

CLEANFILES=*.*~