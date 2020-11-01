#
# Check: a unit test framework for C
# Copyright (C) 2011 Mateusz Loskot
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

ADD_LIBRARY(compat STATIC
    libcompat.c
    $<$<NOT:$<BOOL:${HAVE_LIBRT}>>:clock_gettime.c timer_create.c timer_delete.c timer_settime.c>
    $<$<NOT:$<BOOL:${HAVE_GETLINE}>>:getline.c>
    $<$<NOT:$<BOOL:${HAVE_GETTIMEOFDATE}>>:gettimeofday.c>
    $<$<NOT:$<BOOL:${HAVE_DECL_LOCALTIME_R}>>:localtime_r.c>
    $<$<NOT:$<BOOL:${HAVE_MALLOC}>>:malloc.c>
    $<$<NOT:$<BOOL:${HAVE_REALLOC}>>:realloc.c>
    $<$<NOT:$<BOOL:${HAVE_SNPRINTF}>>:snprintf.c>
    $<$<AND:$<NOT:$<BOOL:${HAVE_DECL_STRDUP}>>,$<NOT:$<BOOL:${HAVE__STRDUP}>>>:strdup.c>
    $<$<NOT:$<BOOL:${HAVE_STRSIGNAL}>>:strsignal.c>
    $<$<NOT:$<BOOL:${HAVE_DECL_ALARM}>>:alarm.c>
    $<$<NOT:$<BOOL:${HAVE_PTHREAD}>>:pthread_mutex.c>
)

SET_TARGET_PROPERTIES(compat
    PROPERTIES
        LANGUAGE C
        C_STANDARD 99
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS ON
)

TARGET_COMPILE_FEATURES(compat
    PUBLIC
        c_function_prototypes
        c_variadic_macros
        c_std_99
)

TARGET_COMPILE_OPTIONS(compat
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

TARGET_COMPILE_DEFINITIONS(compat
    PUBLIC
        $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS>
        $<$<BOOL:${HAVE_SYS_TYPES_H}>:HAVE_SYS_TYPES_H>
        $<$<BOOL:${HAVE_ERRNO_H}>:HAVE_ERRNO_H>
        $<$<BOOL:${HAVE_INTTYPES_H}>:HAVE_INTTYPES_H>
        $<$<BOOL:${HAVE_LIMITS_H}>:HAVE_LIMITS_H>
        $<$<BOOL:${HAVE_REGEX_H}>:HAVE_REGEX_H>
        $<$<BOOL:${HAVE_SIGNAL_H}>:HAVE_SIGNAL_H>
        $<$<BOOL:${HAVE_STDARG_H}>:HAVE_STDARG_H>
        $<$<BOOL:${HAVE_STDINT_H}>:HAVE_STDINT_H>
        $<$<BOOL:${HAVE_STDLIB_H}>:HAVE_STDLIB_H>
        $<$<BOOL:${HAVE_STRING_H}>:HAVE_STRING_H>
        $<$<BOOL:${HAVE_STRINGS_H}>:HAVE_STRINGS_H>
        $<$<BOOL:${HAVE_SYS_TIME_H}>:HAVE_SYS_TIME_H>
        $<$<BOOL:${HAVE_TIME_H}>:HAVE_TIME_H>
        $<$<BOOL:${HAVE_UNISTD_H}>:HAVE_UNISTD_H>
        $<$<BOOL:${HAVE_PTHREAD}>:HAVE_PTHREAD>
        $<$<BOOL:${HAVE_WINDOWS_H}>:HAVE_WINDOWS_H>
        $<$<BOOL:${HAVE_FORK}>:HAVE_FORK>
        $<$<BOOL:${HAVE_GETLINE}>:HAVE_GETLINE>
        $<$<BOOL:${HAVE_GETPID}>:HAVE_GETPID>
        $<$<BOOL:${HAVE_GETTIMEOFDAY}>:HAVE_GETTIMEOFDAY>
        $<$<BOOL:${HAVE_DECL_LOCALTIME_R}>:HAVE_DECL_LOCALTIME_R>
        $<$<BOOL:${HAVE_MALLOC}>:HAVE_MALLOC>
        $<$<BOOL:${HAVE_MKSTEMP}>:HAVE_MKSTEMP>
        $<$<BOOL:${HAVE_REALLOC}>:HAVE_REALLOC>
        $<$<BOOL:${HAVE_DECL_SETENV}>:HAVE_DECL_SETENV>
        $<$<BOOL:${HAVE_SIGACTION}>:HAVE_SIGACTION>
        $<$<BOOL:${HAVE_DECL_STRDUP}>:HAVE_DECL_STRDUP>
        $<$<BOOL:${HAVE_DECL_STRSIGNAL}>:HAVE_DECL_STRSIGNAL>
        $<$<BOOL:${HAVE__GETPID}>:HAVE__GETPID>
        $<$<BOOL:${HAVE__STRDUP}>:HAVE__STRDUP>
        $<$<BOOL:${HAVE_DECL_ALARM}>:HAVE_DECL_ALARM>
        $<$<BOOL:${HAVE_INIT_ONCE_BEGIN_INITIALIZE}>:HAVE_INIT_ONCE_BEGIN_INITIALIZE>
        $<$<BOOL:${HAVE_INIT_ONCE_COMPLETE}>:HAVE_INIT_ONCE_COMPLETE>
        $<$<BOOL:${HAVE_REGCOMP}>:HAVE_REGCOMP>
        $<$<BOOL:${HAVE_REGEXEC}>:HAVE_REGEXEC>
        $<$<BOOL:${HAVE_SNPRINTF_FUNCTION}>:HAVE_SNPRINTF_FUNCTION>
        $<$<BOOL:${HAVE_VSNPRINTF_FUNCTION}>:HAVE_VSNPRINTF_FUNCTION>
)

TARGET_INCLUDE_DIRECTORIES(compat
    PUBLIC
        ${CMAKE_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

# set(SOURCES libcompat.c)

# set(SOURCES ${SOURCES} fpclassify.c)

# if (NOT HAVE_LIBRT)
#   set(SOURCES ${SOURCES} clock_gettime.c)
#   set(SOURCES ${SOURCES} timer_create.c)
#   set(SOURCES ${SOURCES} timer_delete.c)
#   set(SOURCES ${SOURCES} timer_settime.c)
# endif(NOT HAVE_LIBRT)

# if(NOT HAVE_GETLINE)
#   set(SOURCES ${SOURCES} getline.c)
# endif(NOT HAVE_GETLINE)

# if(NOT HAVE_GETTIMEOFDAY)
#   set(SOURCES ${SOURCES} gettimeofday.c)
# endif(NOT HAVE_GETTIMEOFDAY)

# if(NOT HAVE_DECL_LOCALTIME_R)
#   set(SOURCES ${SOURCES} localtime_r.c)
# endif(NOT HAVE_DECL_LOCALTIME_R)

# if(NOT HAVE_MALLOC)
#   set(SOURCES ${SOURCES} malloc.c)
# endif(NOT HAVE_MALLOC)

# if(NOT HAVE_REALLOC)
#   set(SOURCES ${SOURCES} realloc.c)
# endif(NOT HAVE_REALLOC)

# if(NOT HAVE_SNPRINTF)
#     set(SOURCES ${SOURCES} snprintf.c)
# endif(NOT HAVE_SNPRINTF)

# if(NOT HAVE_DECL_STRDUP AND NOT HAVE__STRDUP)
#   set(SOURCES ${SOURCES} strdup.c)
# endif(NOT HAVE_DECL_STRDUP AND NOT HAVE__STRDUP)

# if(NOT HAVE_DECL_STRSIGNAL)
#   set(SOURCES ${SOURCES} strsignal.c)
# endif(NOT HAVE_DECL_STRSIGNAL)

# if(NOT HAVE_DECL_ALARM)
#   set(SOURCES ${SOURCES} alarm.c)
# endif(NOT HAVE_DECL_ALARM)

# if (NOT HAVE_PTHREAD)
#   set(SOURCES ${SOURCES} pthread_mutex.c)
# endif()

# set(HEADERS libcompat.h)

#add_library(compat STATIC ${SOURCES} ${HEADERS})

# vim: shiftwidth=2:softtabstop=2:tabstop=2:expandtab:autoindent

