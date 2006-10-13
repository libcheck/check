## Process this file with automake to produce Makefile.in

info_TEXINFOS = check.texi
check_TEXINFOS = fdl.texi
check.texi: example/src/*.c example/src/*.h example/tests/*.c
	cd $(top_srcdir)/doc/example/src; \
	diff -u money.1.h money.2.h > money.1-2.h.diff; \
	diff -u money.1.c money.3.c > money.1-3.c.diff; \
	diff -u money.3.c money.4.c > money.3-4.c.diff; \
	diff -u money.4.c money.5.c > money.4-5.c.diff; \
	diff -u money.5.c money.6.c > money.5-6.c.diff; \
	cd -;\
	cd $(top_srcdir)/doc/example/tests; \
	diff -u check_money.1.c check_money.2.c > check_money.1-2.c.diff; \
	diff -u check_money.2.c check_money.3.c > check_money.2-3.c.diff; \
	diff -u check_money.3.c check_money.6.c > check_money.3-6.c.diff; \
	diff -u check_money.6.c check_money.7.c > check_money.6-7.c.diff; \
	cd -;

## source files

sgml_docs = tutorial.sgml

## built files

html_docs = tutorial/*.html

$(html_docs): $(sgml_docs)
	docbook2html -o tutorial $<

# example files

example_docs = example/Makefile.am \
               example/README \
               example/configure.ac

example_src_docs = example/src/Makefile.am \
                   example/src/main.c \
                   example/src/money.c \
                   example/src/money.h \
		   example/src/money.1.h \
		   example/src/money.2.h \
		   example/src/money.1.c \
		   example/src/money.3.c \
		   example/src/money.4.c \
		   example/src/money.5.c \
		   example/src/money.6.c

example_tests_docs = example/tests/Makefile.am \
                     example/tests/check_money.c \
                     example/tests/check_money.1.c \
                     example/tests/check_money.2.c \
                     example/tests/check_money.3.c \
                     example/tests/check_money.6.c \
                     example/tests/check_money.7.c

## what to clean

CLEANFILES = *~ \
             $(top_srcdir)/doc/example/src/*.diff \
             $(top_srcdir)/doc/example/tests/*.diff

## what to distribute

EXTRA_DIST = $(sgml_docs) \
             $(example_docs) \
             $(example_src_docs) \
             $(example_tests_docs)

## what to install

docdir = $(datadir)/doc/$(PACKAGE)

# install money example

exampledir = $(docdir)/example
example_DATA = $(example_docs)

examplesrcdir = $(docdir)/example/src
examplesrc_DATA = $(example_src_docs)

exampletestsdir = $(docdir)/example/tests
exampletests_DATA = $(example_tests_docs)


# install tutorial docs

if HAVE_DOCBOOK2HTML
  tutorialdir = $(docdir)/tutorial
  tutorial_DATA = $(html_docs)
  CLEANFILES += $(html_docs)
endif