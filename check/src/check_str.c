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

#include <stdio.h>
#include <check.h>
#include "list.h"
#include "check_error.h"
#include "check_impl.h"
#include "check_str.h"

static char *tr_type_str (TestResult *tr);
static int percent_passed (TestStats *t);

char *tr_str (TestResult *tr) 
{
  char *exact_msg;
  char *rstr;
  
  exact_msg = (tr->rtype == CK_ERROR) ? "(after this point) ": "";
  rstr = emalloc(CMAXMSG);
  
  snprintf (rstr, CMAXMSG, "%s:%d:%s:%s: %s%s",
	    tr->file, tr->line,
	    tr_type_str(tr),  tr->tcname,
	    exact_msg, tr->msg);

  return rstr;
}

char *sr_stat_str (SRunner *sr)
{
  char *str;
  TestStats *ts;
  
  ts = sr->stats;
  str = emalloc (CMAXMSG);
  
  snprintf (str, CMAXMSG, "%d%%: Checks: %d, Failures: %d, Errors: %d",
	    percent_passed (ts), ts->n_checked, ts->n_failed,
	    ts->n_errors);
  return str;
}


static char *tr_type_str (TestResult *tr)
{
  char *str = NULL;
  if (tr->ctx == CK_CTX_TEST) {
    if (tr->rtype == CK_PASS)
      str = "P";
    else if (tr->rtype == CK_FAILURE)
      str = "F";
    else if (tr->rtype == CK_ERROR)
      str = "E";
  } else
    str = "S";

  return str;
}

static int percent_passed (TestStats *t)
{
  if (t->n_failed == 0 && t->n_errors == 0)
    return 100;
  else if (t->n_checked == 0)
    return 0;
  else
    return (int) ( (float) (t->n_checked - (t->n_failed + t->n_errors)) /
		   (float) t->n_checked * 100);
}
