## Process this file with automake to produce Makefile.in

## run tests after everything else

SUBDIRS = src doc . tests

AM_MAKEINFOFLAGS = -I$(top_srcdir)/doc/example

## what to clean

CLEANFILES = *~\
	$(PACKAGE)-$(VERSION).tar.gz\
	ChangeLog.bak

## what additional things to distribute

EXTRA_DIST = ChangeLogOld $(m4data_DATA) $(pcdata_DATA)

## install docs
docdir = $(datadir)/doc/$(PACKAGE)
doc_DATA = SVNChangeLog ChangeLog ChangeLogOld NEWS README COPYING.LESSER

## install check.m4 with AM_PATH_CHECK in it
m4datadir = $(datadir)/aclocal
m4data_DATA = check.m4

## install check.pc
pcdata_DATA = check.pc
pcdatadir = $(libdir)/pkgconfig

ChangeLog:
	svn log -v $(top_srcdir) > SVNChangeLog
