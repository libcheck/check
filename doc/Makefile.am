## Process this file with automake to produce Makefile.in

SUBDIRS = money

## source files

sgml_docs = tutorial.sgml

## built files

html_docs = *.html

$(html_docs): $(sgml_docs)
	docbook2html -o . $<

## what to clean

CLEANFILES = *~

## what to distribute

EXTRA_DIST = $(sgml_docs)

## what to install

datarootdir = $(prefix)/share

if HAVE_DOCBOOK2HTML
  tutorialdir=$(datarootdir)/doc/$(PACKAGE)/tutorial
  tutorial_DATA=$(html_docs)
  CLEANFILES += $(html_docs)
endif
