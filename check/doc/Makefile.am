DOCROOTDIR= $(prefix)/doc/$(PACKAGE)-$(VERSION)
# Fake out automake to make the money subdirectory...
afakedir = $(DOCROOTDIR)/money
docdir = $(DOCROOTDIR)

SUFFIXES = .sgml
%.sgml: %.lyx
	lyx -e linuxdoc $<

%.html : %.sgml
	sgml2html --split=1 $<

sgml_docs = example.sgml
html_docs =\
	example.html\
	example-1.html\
	example-2.html\
	example-3.html\
	example-4.html\
	example-5.html

example_docs =\
	money\
	money/aclocal.m4\
	money/check_money.c\
	money/configure.in\
	money/INSTALL\
	money/money.c\
	money/README\
	money/AUTHORS\
	money/config.h.in\
	money/COPYING\
	money/Makefile.am\
	money/money.h\
	money/stamp-h.in\
	money/ChangeLog\
	money/configure\
	money/Makefile.in\
	money/NEWS

afake_DATA =

doc_DATA = example.lyx $(sgml_docs) $(html_docs) $(example_docs)

CLEANFILES = $(sgml_docs) $(html_docs) *.*~