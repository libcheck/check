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

#include <stdio.h>
#include <check.h>
#include "list.h"
#include "error.h"
#include "check_impl.h"
#include "check_str.h"

static char *rtype_str (int rtype);
static int percent_passed (TestStats *t);

char *tr_str (TestResult *tr) 
{
  char *exact_msg;
  char *rstr;
  
  exact_msg = (tr->rtype == CRERROR) ? "(after this point) ": "";
  rstr = emalloc(CMAXMSG);
  
  snprintf (rstr, CMAXMSG, "%s:%d:%s:%s: %s%s",
	    tr->file, tr->line,
	    rtype_str(tr->rtype),  tr->tcname,
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


static char *rtype_str (int rtype)
{
  switch (rtype) {
  case CRPASS:
    return "P";
    break;
  case CRFAILURE:
    return "F";
    break;
  case CRERROR:
    return "E";
    break;
  default:
    eprintf("Bad argument %d to rtype_to_string", rtype);
    return NULL;
  }
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
