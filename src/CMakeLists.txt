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

add_library(check OBJECT ${SOURCES} ${HEADERS})
set_target_properties(check PROPERTIES POSITION_INDEPENDENT_CODE 1)
if (HAVE_LIBM)
    target_link_libraries(check PRIVATE m)
endif (HAVE_LIBM)
if (HAVE_LIBRT)
    target_link_libraries(check PRIVATE rt)
    # target_compile_definitions(check PUBLIC HAVE_LIBRT 1)
    # FIXME Remove global setting in main CMakeLists.txt
endif (HAVE_LIBRT)
if (HAVE_SUBUNIT)
    target_link_libraries(check PUBLIC subunit)
    # target_compile_definitions(check PUBLIC ENABLE_SUBUNIT 1)
else(HAVE_SUBUNIT)
    # target_compile_definitions(check PUBLIC ENABLE_SUBUNIT 0)
endif (HAVE_SUBUNIT)
# target_link_libraries(check PRIVATE compat)

# Enable finding check.h
target_include_directories(check
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>)
if(MSVC)
  target_compile_definitions(check PRIVATE CK_DLL_EXP _declspec\(dllexport\))
endif (MSVC)

get_target_property(INTERFACES check INTERFACE_LINK_LIBRARIES)
get_target_property(check_link_libs check LINK_LIBRARIES)
message(STATUS "INTERFACES:${INTERFACES}")
message(STATUS "check_link_libs:${check_link_libs}")

# Installable libraries
add_library(checkShared SHARED $<TARGET_OBJECTS:check>)
target_link_libraries(checkShared PUBLIC ${check_link_libs})
set_target_properties(checkShared PROPERTIES
  OUTPUT_NAME check
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  PUBLIC_HEADER ${CMAKE_CURRENT_BINARY_DIR}/check.h
)
add_library(checkStatic STATIC $<TARGET_OBJECTS:check>)
target_link_libraries(checkStatic PUBLIC ${check_link_libs})
set_target_properties(checkStatic PROPERTIES
  OUTPUT_NAME check
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  PUBLIC_HEADER ${CMAKE_CURRENT_BINARY_DIR}/check.h
)

include(GNUInstallDirs)
install(TARGETS checkShared checkStatic 
  EXPORT check-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/check.h
#     TYPE INCLUDE)

