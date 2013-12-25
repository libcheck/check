#
# Check: a unit test framework for C
# Copyright (C) 2011 Mateusz Loskot
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


set(SOURCES libcompat.c)

if(NOT HAVE_DECL_FILENO OR HAVE__FILENO)
  set(SOURCES ${SOURCES} fileno.c)
endif(NOT HAVE_DECL_FILENO OR HAVE__FILENO)

if(NOT HAVE_GETTIMEOFDAY)
  set(SOURCES ${SOURCES} gettimeofday.c)
endif(NOT HAVE_GETTIMEOFDAY)

if(NOT HAVE_DECL_LOCALTIME_R)
  set(SOURCES ${SOURCES} localtime_r.c)
endif(NOT HAVE_DECL_LOCALTIME_R)

if(NOT HAVE_MALLOC)
  set(SOURCES ${SOURCES} malloc.c)
endif(NOT HAVE_MALLOC)

if(NOT HAVE_DECL_PIPE)
  set(SOURCES ${SOURCES} pipe.c)
endif(NOT HAVE_DECL_PIPE)

if(NOT HAVE_DECL_PUTENV)
  set(SOURCES ${SOURCES} putenv.c)
endif(NOT HAVE_DECL_PUTENV)

if(NOT HAVE_REALLOC)
  set(SOURCES ${SOURCES} realloc.c)
endif(NOT HAVE_REALLOC)

if(NOT HAVE_DECL_SETENV)
  set(SOURCES ${SOURCES} setenv.c)
endif(NOT HAVE_DECL_SETENV)

if(NOT HAVE_DECL_SLEEP)
  set(SOURCES ${SOURCES} sleep.c)
endif(NOT HAVE_DECL_SLEEP)

if(NOT HAVE_DECL_STRDUP)
  set(SOURCES ${SOURCES} strdup.c)
endif(NOT HAVE_DECL_STRDUP)

if(NOT HAVE_DECL_STRSIGNAL)
  set(SOURCES ${SOURCES} strsignal.c)
endif(NOT HAVE_DECL_STRSIGNAL)

if(NOT HAVE_DECL_UNSETENV)
  set(SOURCES ${SOURCES} unsetenv.c)
endif(NOT HAVE_DECL_UNSETENV)



set(HEADERS libcompat.h)

add_library(compat STATIC ${SOURCES} ${HEADERS})

install(TARGETS compat
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib) 

install(FILES ${CMAKE_BINARY_DIR}/lib/libcompat.h DESTINATION include)
