DOCROOTDIR= $(prefix)/share/doc/$(PACKAGE)-$(VERSION)
docdir = $(DOCROOTDIR)

doc_DATA = ChangeLog ChangeLogOld NEWS README COPYING

SUBDIRS=src tests doc rpm

EXTRA_DIST= ChangeLogOld

CLEANFILES=*~ ${PACKAGE}-${VERSION}*.gz ${PACKAGE}-${VERSION}*.rpm

rpm: dist
	(cd rpm && ${MAKE} ${AM_MAKEFLAGS} rpm)