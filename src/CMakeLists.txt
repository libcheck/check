#
# Check: a unit test framework for C
# Copyright (C) 2011 Mateusz Loskot
# Copyright (C) 2001, 2002 Arien Malec
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
#

set(SOURCES
  check.c
  check_error.c
  check_list.c
  check_log.c
  check_msg.c
  check_pack.c
  check_print.c
  check_run.c
  check_str.c)

set(HEADERS 
  ${CONFIG_HEADER}
  # ${CMAKE_CURRENT_BINARY_DIR}/check.h
  check.h.in
  check_error.h
  check_impl.h
  check_list.h
  check_log.h
  check_msg.h
  check_pack.h
  check_print.h
  check_str.h)

configure_file(check.h.in check.h @ONLY)

# We would like to create an OBJECT library but currently they are
# too unreliable and cumbersome, 
# especially with target_link_libraries and install(EXPORT...
# https://stackoverflow.com/questions/38832528/transitive-target-include-directories-on-object-libraries
# So we instead do the work twice.

# The shared library is also for internal use.
add_library(check SHARED ${SOURCES} ${HEADERS})
add_library(checkStatic STATIC ${SOURCES} ${HEADERS})
if (HAVE_LIBM)
    target_link_libraries(check PRIVATE m)
    target_link_libraries(checkStatic PRIVATE m)
endif (HAVE_LIBM)
if (HAVE_LIBRT)
    target_link_libraries(check PRIVATE rt)
    target_link_libraries(checkStatic PRIVATE rt)
    # target_compile_definitions(check PUBLIC HAVE_LIBRT 1)
    # FIXME Remove global setting in main CMakeLists.txt
endif (HAVE_LIBRT)
if (HAVE_SUBUNIT)
    target_link_libraries(check PUBLIC subunit)
    target_link_libraries(checkStatic PUBLIC subunit)
    # target_compile_definitions(check PUBLIC ENABLE_SUBUNIT 1)
else(HAVE_SUBUNIT)
    # target_compile_definitions(check PUBLIC ENABLE_SUBUNIT 0)
endif (HAVE_SUBUNIT)
target_link_libraries(check PRIVATE compat)
# target_link_libraries(checkStatic PUBLIC compat)
set_target_properties(check PROPERTIES
  OUTPUT_NAME check
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  PUBLIC_HEADER ${CMAKE_CURRENT_BINARY_DIR}/check.h
)
set_target_properties(checkStatic PROPERTIES
  OUTPUT_NAME check
  PUBLIC_HEADER ${CMAKE_CURRENT_BINARY_DIR}/check.h
)

# Enable finding check.h
target_include_directories(check
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>)
target_include_directories(checkStatic
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>)
if(MSVC)
  target_compile_definitions(check PRIVATE CK_DLL_EXP _declspec\(dllexport\))
  target_compile_definitions(checkStatic PRIVATE CK_DLL_EXP _declspec\(dllexport\))
endif (MSVC)

install(
  TARGETS check checkStatic 
  EXPORT check-targets
  INCLUDES DESTINATION include
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

