## Process this file with automake to produce Makefile.in

WD=`pwd`
TOPDIRFLAGS=--define "_topdir ${WD}"
RPMFLAGS=
RPM_RELEASE=1

EXTRA_DIST=check.spec check.spec.in

SOURCE_FILE=${PACKAGE}-${VERSION}.tar.gz
RPM_I386=./RPMS/i386/${PACKAGE}-$(VERSION)-$(RPM_RELEASE).i386.rpm
RPM_SRC=./SRPMS/${PACKAGE}-$(VERSION)-$(RPM_RELEASE).src.rpm

RPMDIRS=BUILD SOURCES RPMS RPMS/i386 SRPMS

rpm: check.spec
	mkdir -p ${RPMDIRS}
	cp ../${SOURCE_FILE} SOURCES
	rpm ${TOPDIRFLAGS} ${RPMFLAGS} -ba check.spec
	cp ${RPM_I386} ..
	cp ${RPM_SRC} ..
	rm -rf ${RPMDIRS}

CLEANFILES=*.*~ check.spec
