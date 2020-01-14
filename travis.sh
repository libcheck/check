#!/bin/bash
# Copyright (C) 2016 Branden Archer <b.m.archer4@gmail.com>
# Copyright (C) 2016 Joshua D. Boyd <jdboyd@jdboyd.net>
# Copyright (C) 2020 Mikko Koivunalho <mikko.koivunalho@iki.fi>
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
   cmake --version
   cmake . || exit 1
   make || exit 1
   ctest -V || exit 1
   sudo make install || exit 1
fi

if [ "${USE_CMAKE}" = 'NO' ] ; then
   autoreconf -i || exit 1
   ./configure ${EXTRA_ARGS} --disable-build-docs || exit 1
   make || exit 1

   if [ -f doc/version.texi ]; then
      echo "Documentation was generated (doc/version.texi), though disabled";
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

   pushd doc/example
   autoreconf -i || exit 1
   ./configure || exit 1
   make || exit 1
   export LD_LIBRARY_PATH=/usr/local/lib
   make check
   test_result=$?
   cat tests/test-suite.log
   cat tests/check_money.trs
   if [ $test_result -ne 0 ]; then
      exit 1
   fi
   popd
fi

if [ "${PRE_RELEASE_CHECK}" = 'YES' ]; then
   autoreconf --install || exit 1
   ./configure || exit 1
   make prereleasecheck || exit 1
   tar xf check-*.tar.gz
   cd check-*
   cmake --version
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

if [ "${CMAKE_PROJECT_USAGE_TEST}" = 'YES' ] ; then
   cmake --version || exit 1
   project_dir="project_cmake"
   install_dir="${PWD}/install_cmake"
   build_dir="${PWD}/build_cmake"

   # For FetchContent we copy everything to a subdir.
   # Otherwise the copying would be recursive and forever.
   mkdir "$project_dir" || exit 1
   find . -maxdepth 1 -print | grep -v -E "^(\.|\./${project_dir})\$" | grep -v '^\./\.git.*$' | xargs cp -R -t "$project_dir" || exit 1

   # For find_package we need the project built and installed
   mkdir "$build_dir" && cd "$build_dir" || exit 1
   cmake -D BUILD_TESTING=OFF -D CMAKE_INSTALL_PREFIX="${install_dir}" ../project_cmake || exit 1
   make && make install || exit 1
   cd .. || exit 1

   cp -R tests/cmake_project_usage_test . || exit 1
   proj_dir="$PWD/${project_dir}"
   build_dir="build_fetch_content"
   mkdir $build_dir && cd $build_dir || exit 1
   INCLUDE_CHECK_WITH='FetchContent' INCLUDE_CHECK_FROM="file://${proj_dir}" cmake -D CMAKE_BUILD_TYPE=Debug ../cmake_project_usage_test || exit 1
   make && src/tests.test || exit 1
   cd .. || exit 1

   build_dir='build_find_package_config'
   mkdir $build_dir && cd $build_dir || exit 1
   INCLUDE_CHECK_WITH='find_package_config' Check_ROOT="${install_dir}" cmake -D CMAKE_BUILD_TYPE=Debug ../cmake_project_usage_test || exit 1
   make && src/tests.test || exit 1
   cd .. || exit 1
fi

