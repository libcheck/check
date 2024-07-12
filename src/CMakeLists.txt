#
# Check: a unit test framework for C
# Copyright (C) 2011 Mateusz Loskot
# Copyright (C) 2001, 2002 Arien Malec
# Copyright (C) 2020 Mikko Koivunalho
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

# Add parts of libcompat as required
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/fpclassify.c)

if (NOT HAVE_LIBRT)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/clock_gettime.c)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/timer_create.c)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/timer_delete.c)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/timer_settime.c)
endif(NOT HAVE_LIBRT)

if(NOT HAVE_GETLINE)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/getline.c)
endif(NOT HAVE_GETLINE)

if(NOT HAVE_GETTIMEOFDAY)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/gettimeofday.c)
endif(NOT HAVE_GETTIMEOFDAY)

if(NOT HAVE_DECL_LOCALTIME_R)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/localtime_r.c)
endif(NOT HAVE_DECL_LOCALTIME_R)

if(NOT HAVE_MALLOC)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/malloc.c)
endif(NOT HAVE_MALLOC)

if(NOT HAVE_REALLOC)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/realloc.c)
endif(NOT HAVE_REALLOC)

if(NOT HAVE_SNPRINTF)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/snprintf.c)
endif(NOT HAVE_SNPRINTF)

if(NOT HAVE_DECL_STRDUP AND NOT HAVE__STRDUP)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/strdup.c)
endif(NOT HAVE_DECL_STRDUP AND NOT HAVE__STRDUP)

if(NOT HAVE_DECL_STRSIGNAL)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/strsignal.c)
endif(NOT HAVE_DECL_STRSIGNAL)

if(NOT HAVE_DECL_ALARM)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/alarm.c)
endif(NOT HAVE_DECL_ALARM)

if(NOT HAVE_PTHREAD)
  list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/pthread_mutex.c)
endif()

set(HEADERS
  ${CONFIG_HEADER}
  ${CMAKE_CURRENT_BINARY_DIR}/check.h
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

# To maintain compatibility with the Autotools installation
# we specifically create both shared and static libraries
# as that is what Autotools script has been doing.
# Normally CMake would create the system's native default library type.

add_library(check STATIC ${SOURCES} ${HEADERS})
add_library(Check::check ALIAS check)

# We would like to create an OBJECT library but currently they are
# too unreliable and cumbersome,
# especially with target_link_libraries and install(EXPORT...
# https://stackoverflow.com/questions/38832528/transitive-target-include-directories-on-object-libraries
# So we instead do the work twice.
if (BUILD_SHARED_LIBS)
  add_library(checkShared SHARED ${SOURCES} ${HEADERS})
  add_library(Check::checkShared ALIAS checkShared)
endif (BUILD_SHARED_LIBS)

# Include libraries if available
if (HAVE_LIBM)
  list(APPEND ADDITIONAL_LIBS m)
endif (HAVE_LIBM)
if (HAVE_LIBRT)
  list(APPEND ADDITIONAL_LIBS rt)
endif (HAVE_LIBRT)
if (HAVE_SUBUNIT)
  list(APPEND ADDITIONAL_LIBS subunit)
endif (HAVE_SUBUNIT)

target_link_libraries(check PUBLIC ${ADDITIONAL_LIBS})
if (BUILD_SHARED_LIBS)
  target_link_libraries(checkShared PUBLIC m)
endif (BUILD_SHARED_LIBS)


if(MSVC)
  target_compile_definitions(checkShared
    PRIVATE "CK_DLL_EXP=_declspec(dllexport)"
    INTERFACE "CK_DLL_EXP=_declspec(dllimport)"
  )
endif (MSVC)

# More configuration for exporting

set(LIBRARY_OUTPUT_NAME "check")
list(APPEND public_headers "${CMAKE_CURRENT_BINARY_DIR}/check.h")
list(APPEND public_headers "${CMAKE_CURRENT_BINARY_DIR}/../check_stdint.h")

set_target_properties(check PROPERTIES
  OUTPUT_NAME ${LIBRARY_OUTPUT_NAME}
  PUBLIC_HEADER "${public_headers}"
)

if (MSVC)
  # "On Windows you should probably give each library a different name,
  # since there is a ".lib" file for both shared and static".
  # https://stackoverflow.com/a/2152157/4716395
  # "Dynamic-Link Library" (DLL) is Microsoft terminology.
  # So we call it this:
  set(LIBRARY_OUTPUT_NAME "checkDynamic")
endif (MSVC)
if (BUILD_SHARED_LIBS)
set_target_properties(checkShared PROPERTIES
  OUTPUT_NAME ${LIBRARY_OUTPUT_NAME}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  PUBLIC_HEADER "${public_headers}"
)
endif (BUILD_SHARED_LIBS)
target_include_directories(check
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>
    $<INSTALL_INTERFACE:include>
)
if (BUILD_SHARED_LIBS)
target_include_directories(checkShared
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>
    $<INSTALL_INTERFACE:include>
 )
endif (BUILD_SHARED_LIBS)

if(NOT THIS_IS_SUBPROJECT)
  if (BUILD_SHARED_LIBS)
    set(SHARED_LIBNAME checkShared)
  endif ()

  install(TARGETS check ${SHARED_LIBNAME}
    EXPORT check-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endif()

# vim: shiftwidth=2:softtabstop=2:tabstop=2:expandtab:autoindent

