## Process this file with automake to produce Makefile.in

## source files

sgml_docs = tutorial.sgml

## built files

html_docs = *.html

$(html_docs): $(sgml_docs)
	docbook2html -o tutorial $<

# example files

example_docs = example/Makefile.am \
	       example/configure.ac \
               example/src/Makefile.am \
               example/src/main.c \
               example/src/money.c \
               example/src/money.h \
               example/tests/Makefile.am \
               example/tests/check_money.c

## what to clean

CLEANFILES = *~

## what to distribute

EXTRA_DIST = $(sgml_docs) $(example_docs)

## what to install

docdir = $(datadir)/doc/$(PACKAGE)

# install money example

exampledir = $(docdir)/example
example_DATA = $(example_docs)

# install tutorial docs

if HAVE_DOCBOOK2HTML
  tutorialdir = $(docdir)/tutorial
  tutorial_DATA = $(html_docs)
  CLEANFILES += $(html_docs)
endif