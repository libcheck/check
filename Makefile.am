## Process this file with automake to produce Makefile.in

## run tests after everything else

SUBDIRS = src doc . tests

## FIXME: maybe we don't need this line

AM_MAKEINFOFLAGS = -I$(top_srcdir)/doc/example

## what to clean

CLEANFILES = *~\
	$(PACKAGE)-$(VERSION).tar.gz\
	ChangeLog.bak

## what additional things to distribute

EXTRA_DIST = SVNChangeLog ChangeLogOld check.pc.in $(m4data_DATA)

## install docs
docdir = $(datadir)/doc/$(PACKAGE)
doc_DATA = SVNChangeLog ChangeLog ChangeLogOld NEWS README COPYING.LESSER

## install check.m4 with AM_PATH_CHECK in it
m4datadir = $(datadir)/aclocal
m4data_DATA = check.m4

## install check.pc
pcdatadir = $(libdir)/pkgconfig
pcdata_DATA = check.pc

SVNChangeLog:
	svn log -v @abs_top_srcdir@ > $@
