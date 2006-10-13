## Process this file with automake to produce Makefile.in

## source files

examples =\
	money.h\
	money.c\
	check_money.c\
	configure.in.money\
	Makefile.am.money

## what to clean

CLEANFILES = *~

## what to distribute

EXTRA_DIST = $(examples)

## what to install

datarootdir = $(prefix)/share
examplesdir = $(datarootdir)/doc/$(PACKAGE)/examples
examples_DATA = $(examples)
