if PLAIN_DOCDIR
DOCROOTDIR=$(prefix)/share/doc/$(PACKAGE)
else
DOCROOTDIR=$(prefix)/share/doc/${PACKAGE}-${VERSION}
endif
exampledocdir=$(DOCROOTDIR)/examples

example_docs =\
	money.h\
	money.c\
	check_money.c\
	configure.in.money\
	Makefile.am.money

EXTRA_DIST = $(example_docs)

exampledoc_DATA = $(example_docs)