if PLAIN_DOCDIR
DOCROOTDIR=$(prefix)/share/doc/$(PACKAGE)
else
DOCROOTDIR=$(prefix)/share/doc/${PACKAGE}-${VERSION}
endif
docdir = $(DOCROOTDIR)

doc_DATA = ChangeLog ChangeLogOld NEWS README COPYING

SUBDIRS=src tests doc rpm debian

EXTRA_DIST= ChangeLogOld autogen.sh

CLEANFILES=*~\
	${PACKAGE}-${VERSION}*.gz\
	${PACKAGE}-${VERSION}*.rpm\
	ChangeLog.bak

rpm: dist
	(cd rpm && ${MAKE} ${AM_MAKEFLAGS} rpm)

