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

lyx_docs = tutorial.lyx
sgml_docs = tutorial.sgml
html_docs =\
	tutorial.html\
	tutorial-1.html\
	tutorial-2.html\
	tutorial-3.html\
	tutorial-4.html


EXTRA_DIST = $(lyx_docs)

if MAKE_DOC
doc_DATA = $(lyx_docs) $(sgml_docs) $(html_docs)
else
doc_DATA = $(lyx_docs)
endif

CLEANFILES = $(sgml_docs) $(html_docs) *.*~