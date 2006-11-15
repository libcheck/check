## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES=libcheck.la

include_HEADERS=check.h

AM_CFLAGS=@GCOV_CFLAGS@

libcheck_la_LIBADD=@GCOV_LIBS@

libcheck_la_SOURCES=\
	check.h		\
	check.c		\
	check_error.h	\
	check_error.c	\
	check_impl.h	\
	check_list.h	\
	check_list.c	\
	check_log.h	\
	check_log.c	\
	check_msg.h	\
	check_msg.c	\
	check_pack.h	\
	check_pack.c	\
	check_print.h	\
	check_print.c	\
	check_run.c	\
	check_str.h	\
	check_str.c

EXTRA_DIST=check.h.in

CLEANFILES=*~ *.gcno

LCOV_OUTPUT  = lcov.info
LCOV_HTML    = lcov_html
LCOV_LCOV    = @LCOV@
LCOV_GENHTML = @GENHTML@

lcov: $(LCOV_HTML)

$(LCOV_OUTPUT): libcheck.la
	@$(MAKE) -C $(top_builddir)/tests check
	$(LCOV_LCOV) --capture --directory . --base-directory . --output-file $(LCOV_OUTPUT)

$(LCOV_HTML): $(LCOV_OUTPUT)
	-$(RM) -r $(LCOV_HTML)
	LANG=C $(LCOV_GENHTML) --output-directory $(LCOV_HTML) --title "Check Code Coverage" --show-details $(LCOV_OUTPUT)
	@echo "Point a web browser at $(LCOV_HTML)/index.html to see results."

clean-local: lcov-clean

.PHONY: lcov-clean
lcov-clean:
	-$(RM) -r $(LCOV_HTML) $(LCOV_OUTPUT)
