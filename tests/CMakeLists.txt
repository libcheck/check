#
# Check: a unit test framework for C
#
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

CONFIGURE_FILE(test_vars.in test_vars @ONLY)

ADD_EXECUTABLE(check_check
    check_check_exit.c
    check_check_fixture.c
    check_check_fork.c
    check_check_limit.c
    check_check_log.c
    check_check_log_internal.c
    check_check_main.c
    check_check_master.c
    check_check_msg.c
    check_check_pack.c
    check_check_selective.c
    check_check_sub.c
    check_check_tags.c
    check_list.c
)

ADD_EXECUTABLE(check_check_export
    check_check_sub.c
    check_check_master.c
    check_check_log.c
    check_check_fork.c
    check_check_export_main.c
)

ADD_EXECUTABLE(ex_output
    ex_output.c
)

ADD_EXECUTABLE(check_nofork
    check_nofork.c
)

ADD_EXECUTABLE(check_nofork_teardown
    check_nofork_teardown.c
)

ADD_EXECUTABLE(check_set_max_msg_size
    check_set_max_msg_size.c
)

SET(CHECK_TESTS
    check_check
    check_check_export
    ex_output
    check_nofork
    check_nofork_teardown
    check_set_max_msg_size
)

SET_TARGET_PROPERTIES(${CHECK_TESTS}
    PROPERTIES
        LANGUAGE
            C
        C_STANDARD
            99
        C_STANDARD_REQUIRED
            ON
        C_EXTENSIONS
            ON
)

FOREACH(CHECK_TEST ${CHECK_TESTS})
    TARGET_COMPILE_OPTIONS(${CHECK_TEST}
        PUBLIC
            ################################################
            #               Clang  Options
            ################################################
            #
            # Enable the standard diagnostic options if we
            # detect that the current C compiler is clang.
            #
            $<$<C_COMPILER_ID:Clang>:-Wall -Wextra>

            ################################################
            #               GCC Options
            ################################################
            #
            # Enable the standard diagnostic options if we
            # detect that the current C compiler is GCC.
            #
            $<$<C_COMPILER_ID:GNU>:-Wall -Wextra>
            #
            # If the C compiler is GCC, we are building a
            # Debug build, and the version of our compiler
            # is at least 10.1.0 enable the static analyzer.
            #
            # This option was implemented in version 10.1.0,
            # so this is the minimum version required for
            # this option.
            #
            $<$<AND:$<C_COMPILER_ID:GNU>,$<VERSION_GREATER_EQUAL:$<C_COMPILER_VERSION>,10.1.0>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:-fanalyzer>

            ################################################
            #               MSVC Options
            ################################################
            #
            $<$<AND:$<C_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>>:/Od /RTC1 /GS /Gy /Zi /analyze:512 /sdl /TC /Wall>
            $<$<AND:$<C_COMPILER_ID:MSVC>,$<STREQUAL:${CMAKE_BUILD_TYPE},Release>>:/O2 /GF /Gy /Qpar /Qfast_transcendentals /TC>
    )

    TARGET_COMPILE_DEFINITIONS(${CHECK_TEST}
        PUBLIC
            $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS>
            $<$<BOOL:${ENABLE_MEMORY_LEAKING_TESTS}>:MEMORY_LEAKING_TESTS_ENABLED=1>
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

    TARGET_INCLUDE_DIRECTORIES(${CHECK_TEST} PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/src
    )

    TARGET_LINK_LIBRARIES(${CHECK_TEST}
        PUBLIC
            $<$<AND:$<BOOL:${Threads_FOUND}>,$<BOOL:${CMAKE_USE_PTHREADS_INIT}>>:${CMAKE_THREAD_LIBS_INIT}>
            check
    )
ENDFOREACH()

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
# # Enable finding check.h
# include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src)

# # For the test_vars.in script, to give the unit test shell script
# # runners the location of the source files
# set(srcdir "${CMAKE_CURRENT_SOURCE_DIR}")

# if(WIN32)
#   # CMake uses Unix slashes for everything, but the tests that
#   # read srcdir expect platform specific slashes. There are two
#   # slashes because the shell scripts will consume srcdir.
#   string(REPLACE "/" "\\\\" srcdir "${srcdir}")
#   set(EXEEXT ".exe")
#   set(IS_MSVC "1")
# endif(WIN32)

# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test_vars.in
#   ${CMAKE_CURRENT_BINARY_DIR}/test_vars @ONLY)

# if(ENABLE_MEMORY_LEAKING_TESTS)
#   add_definitions(-DMEMORY_LEAKING_TESTS_ENABLED=1)
# else(ENABLE_MEMORY_LEAKING_TESTS)
#   add_definitions(-DMEMORY_LEAKING_TESTS_ENABLED=0)
# endif(ENABLE_MEMORY_LEAKING_TESTS)

# set(CHECK_CHECK_SOURCES
#   check_check_exit.c
#   check_check_fixture.c
#   check_check_fork.c
#   check_check_limit.c
#   check_check_log.c
#   check_check_log_internal.c
#   check_check_main.c
#   check_check_master.c
#   check_check_msg.c
#   check_check_pack.c
#   check_check_selective.c
#   check_check_sub.c
#   check_check_tags.c
#   check_list.c)
# set(CHECK_CHECK_HEADERS check_check.h)
# add_executable(check_check ${CHECK_CHECK_HEADERS} ${CHECK_CHECK_SOURCES})
# target_link_libraries(check_check check)

# set(CHECK_CHECK_EXPORT_SOURCES
#   check_check_sub.c
#   check_check_master.c
#   check_check_log.c
#   check_check_fork.c
#   check_check_export_main.c
# )
# set(CHECK_CHECK_EXPORT_HEADERS check_check.h)
# add_executable(check_check_export
#   ${CHECK_CHECK_EXPORT_HEADERS}
#   ${CHECK_CHECK_EXPORT_SOURCES})
# target_link_libraries(check_check_export check)

# set(EX_OUTPUT_SOURCES ex_output.c)
# add_executable(ex_output ${EX_OUTPUT_SOURCES})
# target_link_libraries(ex_output check)

# set(CHECK_NOFORK_SOURCES check_nofork.c)
# add_executable(check_nofork ${CHECK_NOFORK_SOURCES})
# target_link_libraries(check_nofork check)

# set(CHECK_NOFORK_TEARDOWN_SOURCES check_nofork_teardown.c)
# add_executable(check_nofork_teardown ${CHECK_NOFORK_TEARDOWN_SOURCES})
# target_link_libraries(check_nofork_teardown check)

# set(CHECK_SET_MAX_MSG_SIZE_SOURCES check_set_max_msg_size.c)
# add_executable(check_set_max_msg_size ${CHECK_SET_MAX_MSG_SIZE_SOURCES})
# target_link_libraries(check_set_max_msg_size check)
