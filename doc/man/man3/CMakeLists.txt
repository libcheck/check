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

# Define the library's section 3 man pages.
#
SET(MAN3_PAGES
    suite_create.3
)

# Configure all of the section 3 man pages defined above.
#
# This command will process each of the man pages, setting
# the project version and build date automatically.
#
FOREACH(MAN3_PAGE ${MAN3_PAGES})
    # Configure the man pages.
    #
    # The configuration of each man page involves only the
    # addition of the library version number defined in the
    # configure.ac file to each man page.
    #
    CONFIGURE_FILE(${MAN3_PAGE}.in ${MAN3_PAGE} @ONLY)

    # Install Section 3 Man Pages (Library Calls)
    #
    # This directive describes what, where, how, and when the
    # section 3 man pages should be installed. Specifically, the
    # ${CMAKE_INSTALL_MANDIR} variable provided by the
    # GNUInstallDirs module specifies the system-specific man
    # page installation directory, the permissions set the
    # desired installed file(s) permissions, the configurations
    # directive specifies that this directive applies for both
    # Debug and Release configurations, and the component
    # directive allows for the installation of only those items
    # belonging to the docs component.
    #
    # For example, to install only the 'docs' component, the
    # following command can be used after generating the project
    # build files.
    #
    #   $ cmake --install . --component docs
    #
    # This example assumes the project build files were
    # generated in the current working directory.
    #
    INSTALL(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${MAN3_PAGE}
        DESTINATION
            ${CMAKE_INSTALL_MANDIR}/man3
        PERMISSIONS
            OWNER_READ
            OWNER_WRITE
            GROUP_READ
            WORLD_READ
        CONFIGURATIONS
            Debug
            Release
        COMPONENT
            docs
        OPTIONAL
    )
ENDFOREACH()
