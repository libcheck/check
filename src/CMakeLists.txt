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

CONFIGURE_FILE(check.h.in check.h @ONLY)

SET(LIBRARY_OUTPUT_NAME "check")

LIST(APPEND PUBLIC_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/check.h
    ${CMAKE_SOURCE_DIR}/check_stdint.h
)

ADD_LIBRARY(check STATIC
    check.c
    check_error.c
    check_list.c
    check_log.c
    check_msg.c
    check_pack.c
    check_print.c
    check_run.c
    check_str.c
    ${CMAKE_SOURCE_DIR}/lib/fpclassify.c
    $<$<NOT:$<BOOL:${HAVE_LIBRT}>>:${CMAKE_SOURCE_DIR}/lib/clock_gettime.c ${CMAKE_SOURCE_DIR}/lib/timer_create.c ${CMAKE_SOURCE_DIR}/lib/timer_delete.c ${CMAKE_SOURCE_DIR}/lib/timer_settime.c>
    $<$<NOT:$<BOOL:${HAVE_GETLINE}>>:${CMAKE_SOURCE_DIR}/lib/getline.c>
    $<$<NOT:$<BOOL:${HAVE_GETTIMEOFDAY}>>:${CMAKE_SOURCE_DIR}/lib/gettimeofday.c>
    $<$<NOT:$<BOOL:${HAVE_DECL_LOCALTIME_R}>>:${CMAKE_SOURCE_DIR}/lib/localtime_r.c>
    $<$<NOT:$<BOOL:${HAVE_MALLOC}>>:${CMAKE_SOURCE_DIR}/lib/malloc.c>
    $<$<NOT:$<BOOL:${HAVE_REALLOC}>>:${CMAKE_SOURCE_DIR}/lib/realloc.c>
    $<$<NOT:$<BOOL:${HAVE_SNPRINTF}>>:${CMAKE_SOURCE_DIR}/lib/snprintf.c>
    $<$<AND:$<NOT:$<BOOL:${HAVE_DECL_STRUP}>>,$<NOT:$<BOOL:${HAVE__STRDUP}>>>:${CMAKE_SOURCE_DIR}/lib/strdup.c>
    $<$<NOT:$<BOOL:${HAVE_DECL_STRSIGNAL}>>:${CMAKE_SOURCE_DIR}/lib/strsignal.c>
    $<$<NOT:$<BOOL:${HAVE_DECL_ALARM}>>:${CMAKE_SOURCE_DIR}/lib/alarm.c>
    $<$<NOT:$<BOOL:${HAVE_PTHREAD}>>:${CMAKE_SOURCE_DIR}/lib/pthread_mutex.c>
)

SET_TARGET_PROPERTIES(check
    PROPERTIES
        LANGUAGE
            C
        C_STANDARD
            99
        C_STANDARD_REQUIRED
            ON
        C_EXTENSIONS
            ON
        OUTPUT_NAME
            ${LIBRARY_OUTPUT_NAME}
        VERSION
            ${PROJECT_VERSION}
        PUBLIC_HEADER
            "${PUBLIC_HEADERS}"
)

TARGET_COMPILE_OPTIONS(check
    PUBLIC
        ####################################################
        #                   Clang  Options
        ####################################################
        #
        # Enable the standard diagnostic options if we
        # detect that the current C compiler is clang.
        #
        $<$<C_COMPILER_ID:Clang>:-Wall -Wextra>

        ####################################################
        #                   GCC Options
        ####################################################
        #
        # Enable the standard diagnostic options if we
        # detect that the current C compiler is GCC.
        #
        $<$<C_COMPILER_ID:GNU>:-Wall -Wextra>
        #
        # If the C compiler is GCC, we are building a Debug
        # build, and the version of our compiler is at least
        # 10.1.0, enable the static analyzer.
        #
        # This option was implemented in version 10.1.0, so
        # this is the minimum version required for this
        # option.
        #
        $<$<AND:$<C_COMPILER_ID:GNU>,$<VERSION_GREATER_EQUAL:$<C_COMPILER_VERSION>,10.1.0>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:-fanalyzer>

        ####################################################
        #                   MSVC Options
        ####################################################
        #
        $<$<AND:$<C_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:/Od /RTC1 /GS /Gy /Zi /analyze:512 /sdl /TC /Wall>
        $<$<AND:$<C_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_BUILD_TYPE},Release>>:/O2 /GF /Gy /Qpar /Qfast_transcendentals /TC>
)

TARGET_COMPILE_DEFINITIONS(check
    PUBLIC
        $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS>
)

TARGET_LINK_LIBRARIES(check
    PRIVATE
        $<$<AND:$<BOOL:${Threads_FOUND}>,$<BOOL:${CMAKE_USE_PTHREADS_INIT}>>:${CMAKE_THREAD_LIBS_INIT}>
    PUBLIC
        $<$<BOOL:${HAVE_LIBM}>:m>
        $<$<BOOL:${HAVE_LIBRT}>:rt>
        $<$<BOOL:${HAVE_SUBUNIT}>:subunit>
)

TARGET_INCLUDE_DIRECTORIES(check
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)

ADD_LIBRARY(Check::check ALIAS check)

IF(MSVC)
    SET(LIBRARY_OUTPUT_NAME "checkDynamic")
ENDIF()

ADD_LIBRARY(checkShared SHARED
    check.c
    check_error.c
    check_list.c
    check_log.c
    check_msg.c
    check_pack.c
    check_print.c
    check_run.c
    check_str.c
    ${CMAKE_SOURCE_DIR}/lib/fpclassify.c
    $<$<NOT:$<BOOL:${HAVE_LIBRT}>>:${CMAKE_SOURCE_DIR}/lib/clock_gettime.c ${CMAKE_SOURCE_DIR}/lib/timer_create.c ${CMAKE_SOURCE_DIR}/lib/timer_delete.c ${CMAKE_SOURCE_DIR}/lib/timer_settime.c>
    $<$<NOT:$<BOOL:${HAVE_GETLINE}>>:${CMAKE_SOURCE_DIR}/lib/getline.c>
    $<$<NOT:$<BOOL:${HAVE_GETTIMEOFDAY}>>:${CMAKE_SOURCE_DIR}/lib/gettimeofday.c>
    $<$<NOT:$<BOOL:${HAVE_DECL_LOCALTIME_R}>>:${CMAKE_SOURCE_DIR}/lib/localtime_r.c>
    $<$<NOT:$<BOOL:${HAVE_MALLOC}>>:${CMAKE_SOURCE_DIR}/lib/malloc.c>
    $<$<NOT:$<BOOL:${HAVE_REALLOC}>>:${CMAKE_SOURCE_DIR}/lib/realloc.c>
    $<$<NOT:$<BOOL:${HAVE_SNPRINTF}>>:${CMAKE_SOURCE_DIR}/lib/snprintf.c>
    $<$<AND:$<NOT:$<BOOL:${HAVE_DECL_STRUP}>>,$<NOT:$<BOOL:${HAVE__STRDUP}>>>:${CMAKE_SOURCE_DIR}/lib/strdup.c>
    $<$<NOT:$<BOOL:${HAVE_DECL_STRSIGNAL}>>:${CMAKE_SOURCE_DIR}/lib/strsignal.c>
    $<$<NOT:$<BOOL:${HAVE_DECL_ALARM}>>:${CMAKE_SOURCE_DIR}/lib/alarm.c>
    $<$<NOT:$<BOOL:${HAVE_PTHREAD}>>:${CMAKE_SOURCE_DIR}/lib/pthread_mutex.c>
)

SET_TARGET_PROPERTIES(checkShared
    PROPERTIES
        LANGUAGE
            C
        C_STANDARD
            99
        C_STANDARD_REQUIRED
            ON
        C_EXTENSIONS
            ON
        OUTPUT_NAME
            ${LIBRARY_OUTPUT_NAME}
        VERSION
            ${PROJECT_VERSION}
        SOVERSION
            ${PROJECT_VERSION_MAJOR}
        PUBLIC_HEADER
            "${PUBLIC_HEADERS}"
)

TARGET_COMPILE_OPTIONS(checkShared
    PUBLIC
        ####################################################
        #                   Clang  Options
        ####################################################
        #
        # Enable the standard diagnostic options if we
        # detect that the current C compiler is clang.
        #
        $<$<C_COMPILER_ID:Clang>:-Wall -Wextra>

        ####################################################
        #                   GCC Options
        ####################################################
        #
        # Enable the standard diagnostic options if we
        # detect that the current C compiler is GCC.
        #
        $<$<C_COMPILER_ID:GNU>:-Wall -Wextra>
        #
        # If the C compiler is GCC, we are building a Debug
        # build, and the version of our compiler is at least
        # 10.1.0, enable the static analyzer.
        #
        # This option was implemented in version 10.1.0, so
        # this is the minimum version required for this
        # option.
        #
        $<$<AND:$<C_COMPILER_ID:GNU>,$<VERSION_GREATER_EQUAL:$<C_COMPILER_VERSION>,10.1.0>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:-fanalyzer>

        ####################################################
        #                   MSVC Options
        ####################################################
        #
        $<$<AND:$<C_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:/Od /RTC1 /GS /Gy /Zi /analyze:512 /sdl /TC /Wall>
        $<$<AND:$<C_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_BUILD_TYPE},Release>>:/O2 /GF /Gy /Qpar /Qfast_transcendentals /TC>
)

TARGET_COMPILE_DEFINITIONS(checkShared
    PUBLIC
        $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS>
    PRIVATE
        $<$<BOOL:${MSVC}>: "CK_DLL_EXP=_declspec(dllexport)" >
    INTERFACE
        $<$<BOOL:${MSVC}>: "CK_DLL_EXP=_declspec(dllimport)" >
)

TARGET_LINK_LIBRARIES(checkShared
    PUBLIC
        $<$<BOOL:${HAVE_LIBM}>:m>
        $<$<BOOL:${HAVE_LIBRT}>:rt>
        $<$<BOOL:${HAVE_SUBUNIT}>:subunit>
)

TARGET_INCLUDE_DIRECTORIES(checkShared
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)

ADD_LIBRARY(Check::checkShared ALIAS checkShared)

IF(NOT THIS_IS_SUBPROJECT)
    INSTALL(
        TARGETS
            check
            checkShared
        EXPORT
            check-targets
        ARCHIVE DESTINATION
            ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION
            ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION
            ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}
    )
ENDIF()
