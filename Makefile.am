## Process this file with automake to produce Makefile.in

## run tests after everything else

SUBDIRS = src doc rpm . tests

## what to clean

CLEANFILES=*~\
	$(PACKAGE)-$(VERSION).tar.gz\
	$(PACKAGE)-$(VERSION).rpm\
	ChangeLog.bak

## what additional things to distribute

EXTRA_DIST= ChangeLogOld $(m4data_DATA)

## install docs
datarootdir = $(prefix)/share
docdir = $(datarootdir)/doc/$(PACKAGE)
doc_DATA = ChangeLog ChangeLogOld NEWS README COPYING.LESSER

## install check.m4 with AM_PATH_CHECK in it
m4datadir = $(datadir)/aclocal
m4data_DATA = check.m4

rpm: dist
	(cd rpm && ${MAKE} ${AM_MAKEFLAGS} rpm)
