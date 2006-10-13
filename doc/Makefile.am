## Process this file with automake to produce Makefile.in

info_TEXINFOS = check.texi
check_TEXINFOS = fdl.texi

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
                   example/src/money.h

example_tests_docs = example/tests/Makefile.am \
                     example/tests/check_money.c

## what to clean

CLEANFILES = *~

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