TESTS=check_check check_list check_check_msg

noinst_PROGRAMS=check_check check_list check_stress check_check_msg

check_check_SOURCES= \
	check_check.c

check_list_SOURCES=\
	check_list.c

check_stress_SOURCES=\
	check_stress.c

check_check_msg_SOURCES=\
	check_check_msg.c

INCLUDES= -I$(srcdir)/../src
LDADD= ../src/libcheck.a

CLEANFILES=*.*~