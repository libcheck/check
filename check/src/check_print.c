/*
 * Check: a unit test framework for C
 * Copyright (C) 2001, 2002 Arien Malec
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "check.h"
#include "check_list.h"
#include "check_impl.h"
#include "check_str.h"
#include "check_print.h"

static void srunner_fprint_summary (FILE *file, SRunner *sr,
				    enum print_output print_mode);
static void srunner_fprint_results (FILE *file, SRunner *sr,
				    enum print_output print_mode);


void srunner_print (SRunner *sr, enum print_output print_mode)
{
  srunner_fprint (stdout, sr, print_mode);
}

void srunner_fprint (FILE *file, SRunner *sr, enum print_output print_mode)
{
  srunner_fprint_summary (file, sr, print_mode);
  srunner_fprint_results (file, sr, print_mode);
}

static void srunner_fprint_summary (FILE *file, SRunner *sr,
				    enum print_output print_mode)
{
  if (print_mode >= CK_MINIMAL) {
    char *str;

    str = sr_stat_str (sr);
    fprintf (file, "%s\n", str);
    free(str);
  }
  return;
}

static void srunner_fprint_results (FILE *file, SRunner *sr,
				    enum print_output print_mode)
{
  List *resultlst;
  
  resultlst = sr->resultlst;
  
  for (list_front(resultlst); !list_at_end(resultlst); list_advance(resultlst)) {
    TestResult *tr = list_val(resultlst);
    tr_fprint (file, tr, print_mode);
  }
  return;
}

void tr_fprint (FILE *file, TestResult *tr, enum print_output print_mode)
{
  if ((print_mode >= CK_VERBOSE && tr->rtype == CK_PASS) ||
      (tr->rtype != CK_PASS && print_mode >= CK_NORMAL)) {
    char *trstr = tr_str (tr);
    fprintf (file,"%s\n", trstr);
    free(trstr);
  }
}
