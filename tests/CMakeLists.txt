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
