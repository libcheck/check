if PLAIN_DOCDIR
DOCROOTDIR=$(prefix)/share/doc/$(PACKAGE)
else
DOCROOTDIR=$(prefix)/share/doc/${PACKAGE}-${VERSION}
endif
docdir = $(DOCROOTDIR)

doc_DATA = ChangeLog ChangeLogOld NEWS README COPYING

SUBDIRS=src tests doc rpm debian

m4datadir = $(datadir)/aclocal
m4data_DATA = check.m4

EXTRA_DIST= ChangeLogOld autogen.sh $(m4data_DATA)

CLEANFILES=*~\
	${PACKAGE}-${VERSION}*.gz\
	${PACKAGE}-${VERSION}*.rpm\
	ChangeLog.bak

rpm: dist
	(cd rpm && ${MAKE} ${AM_MAKEFLAGS} rpm)

debian: dist
	dpkg-buildpackage -rfakeroot
