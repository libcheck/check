## Process this file with automake to produce Makefile.in

if PLAIN_DOCDIR
DOCROOTDIR=$(prefix)/share/doc/$(PACKAGE)
else
DOCROOTDIR=$(prefix)/share/doc/${PACKAGE}-${VERSION}
endif
docdir = $(DOCROOTDIR)

SUBDIRS=money

SUFFIXES = .html

sgml_docs = tutorial.sgml
html_docs = *.html

$(html_docs): $(sgml_docs)
	docbook2html -o . $<

EXTRA_DIST = $(sgml_docs) $(html_docs)

if MAKE_DOC
doc_DATA = $(sgml_docs) $(html_docs)
CLEANFILES = $(html_docs) *.*~
else
doc_DATA = $(sgml_docs)
CLEANFILES=*.*~
endif

