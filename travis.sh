#!/bin/bash
# Copyright (C) 2016 Branden Archer <b.m.archer4@gmail.com>
# Copyright (C) 2016 Joshua D. Boyd <jdboyd@jdboyd.net>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

set -x

if [ -d doc/check_html ]; then
   echo "The HTML output folder already exists";
   exit 1;
fi

if [ -d doc/doxygen ]; then
   echo "The doxygen output folder already exists";
   exit 1;
fi

if [ "${USE_CMAKE}" = 'YES' ] ; then
   cmake . || exit 1
   make || exit 1
   ctest -V || exit 1
   sudo make install || exit 1
fi

if [ "${USE_CMAKE}" = 'NO' ] ; then
   autoreconf -i || exit 1
   ./configure ${EXTRA_ARGS} || exit 1
   make || exit 1

   if [ ! -f doc/version.texi ]; then
      echo "version.texi not generated";
      exit 1;
   fi

   make check || exit 1
   sudo make install || exit 1

   make doc/check_html || exit 1
   if [ ! -d doc/check_html ]; then
      echo "HTML documentation not generated"
      exit 1;
   fi

   make doc/doxygen || exit 1
   if [ ! -d doc/doxygen ]; then
      echo "Doxygen documentation not generated";
      exit 1;
   fi
fi

if [ "${PRE_RELEASE_CHECK}" = 'YES' ]; then
   autoreconf --install || exit 1
   ./configure || exit 1
   make prereleasecheck || exit 1
   tar xf check-*.tar.gz
   cd check-*
   cmake . || exit 1
   make || exit 1
fi

if [ "${SCAN_BUILD}" = 'YES' ]; then
   autoreconf --install || exit 1
   scan-build ./configure --enable-snprintf-replacement --enable-timer-replacement || exit 1
   scan-build -o clang make || exit 1
   if [ -n "$(find clang -type f)" ]; then
      echo "scan-build found potential issues"
      find clang -type f -print -exec cat \{} \;
      exit 1
   fi
fi

