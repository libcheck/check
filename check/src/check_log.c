#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <check_log.h>
#include "list.h"
#include "check_impl.h"

/*
  Check: a unit test framework for C
  Copyright (C) 2001, Arien Malec

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

void srunner_set_log (SRunner *sr, char *fname)
{
  if (sr->log_fname)
    return;
  sr->log_fname = fname;
}

int srunner_has_log (SRunner *sr)
{
  return sr->log_fname != NULL;
}

char *srunner_log_fname (SRunner *sr)
{
  return sr->log_fname;
}
