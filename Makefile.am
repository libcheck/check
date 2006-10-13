## Process this file with automake to produce Makefile.in

SUBDIRS=src tests doc rpm debian

## decide on appending version to installed doc dir
if PLAIN_DOCDIR
DOCROOTDIR=$(prefix)/share/doc/$(PACKAGE)
else
DOCROOTDIR=$(prefix)/share/doc/${PACKAGE}-${VERSION}
endif
docdir = $(DOCROOTDIR)

## install the following extra data with docs
doc_DATA = ChangeLog ChangeLogOld NEWS README COPYING.LESSER

## install check.m4 so clients can use it
m4datadir = $(datadir)/aclocal
m4data_DATA = check.m4

EXTRA_DIST= ChangeLogOld $(m4data_DATA)

CLEANFILES=*~\
	${PACKAGE}-${VERSION}*.gz\
	${PACKAGE}-${VERSION}*.rpm\
	ChangeLog.bak

rpm: dist
	(cd rpm && ${MAKE} ${AM_MAKEFLAGS} rpm)

debian: dist
	dpkg-buildpackage -rfakeroot
