#
# Check: a unit test framework for C
# Copyright (C) 2020 Jose Fernando Lopez Fernandez
# 
# This library is free software; you can redistribute
# it and/or modify it under the terms of the GNU Lesser
# General Public License as published by the Free Software
# Foundation; either version 2.1 of the License, or (at
# your option) any later version.
# 
# This library is distributed in the hope that it will
# be useful, but WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
# 
# You should have received a copy of the GNU Lesser General
# Public License along with this library; if not, write
# to the Free Software Foundation, Inc., 51 Franklin St,
# Fifth Floor, Boston, MA  02110-1301  USA
#

# Define CMake variable for the current date.
#
# This command is a cross platform way of constructing a
# string usable by the CONFIGURE_FILE CMake command, and
# while limited beyond the simple functionality which it
# provides here, it is exactly what we need.
#
# The LIBCHECK_BUILD_DATE variable is defined to be the
# current (locale-dependent, not UTC) date formatted as an
# ISO date string.
#
STRING(TIMESTAMP LIBCHECK_BUILD_DATE "%Y-%m-%d")

# Process each of the following man page directories once we
# have successfully configured the necessary variable(s)
# defined above.
#
ADD_SUBDIRECTORY(man3)
ADD_SUBDIRECTORY(man7)
