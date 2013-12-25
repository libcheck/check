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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
# Enable finding check.h
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src)

set(CHECK_CHECK_SOURCES
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
  check_list.c)
set(CHECK_CHECK_HEADERS check_check.h)
add_executable(check_check ${CHECK_CHECK_HEADERS} ${CHECK_CHECK_SOURCES})
target_link_libraries(check_check check compat)

set(EX_OUTPUT_SOURCES ex_output.c)
add_executable(ex_output ${EX_OUTPUT_SOURCES})
target_link_libraries(ex_output check compat)

set(EX_LOG_OUTPUT_SOURCES ex_log_output.c)
add_executable(ex_log_output ${EX_LOG_OUTPUT_SOURCES})
target_link_libraries(ex_log_output check compat)

set(EX_XML_OUTPUT_SOURCES ex_xml_output.c)
add_executable(ex_xml_output ${EX_XML_OUTPUT_SOURCES})
target_link_libraries(ex_xml_output check compat)

set(EX_TAP_OUTPUT_SOURCES ex_tap_output.c)
add_executable(ex_tap_output ${EX_TAP_OUTPUT_SOURCES})
target_link_libraries(ex_tap_output check compat)
