if PLAIN_DOCDIR
DOCROOTDIR=$(prefix)/share/doc/$(PACKAGE)
else
DOCROOTDIR=$(prefix)/share/doc/${PACKAGE}-${VERSION}
endif
docdir = $(DOCROOTDIR)

SUBDIRS=money

SUFFIXES = .sgml
%.sgml: %.lyx
	lyx -e linuxdoc $<

%.html : %.sgml
	sgml2html --split=1 $<

lyx_docs = example.lyx
sgml_docs = example.sgml
html_docs =\
	example.html\
	example-1.html\
	example-2.html\
	example-3.html\
	example-4.html


EXTRA_DIST = $(lyx_docs)

if MAKE_DOC
doc_DATA = $(lyx_docs) $(sgml_docs) $(html_docs)
else
doc_DATA = $(lyx_docs)
endif

CLEANFILES = $(sgml_docs) $(html_docs) *.*~