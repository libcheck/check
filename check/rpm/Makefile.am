WD=`pwd`
TOPDIRFLAGS=--define "_topdir ${WD}"
RPMFLAGS=

EXTRA_DIST=check.spec check.spec.in

SOURCE_FILE=${PACKAGE}-${VERSION}.tar.gz
RPM_I386=./RPMS/i386/${PACKAGE}-$(VERSION)-1.i386.rpm
RPM_SRC=./SRPMS/${PACKAGE}-$(VERSION)-1.src.rpm

RPMDIRS=BUILD SOURCES RPMS RPMS/i386 SRPMS

rpm: check.spec
	mkdir -p ${RPMDIRS}
	cp ../${SOURCE_FILE} SOURCES
	rpm ${TOPDIRFLAGS} ${RPMFLAGS} -ba check.spec
	cp ${RPM_I386} ..
	cp ${RPM_SRC} ..
	rm -rf ${RPMDIRS}

CLEANFILES=*.*~
