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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "list.h"
#include "check.h"
#include "check_impl.h"
#include "check_msg.h"

static int non_pass (int val);

Suite *suite_create (char *name)
{
  Suite *s;
  s = emalloc (sizeof(Suite)); /* freed in suite_free */
  if (name == NULL)
    s->name = "";
  else
    s->name = name;
  s->tclst = list_create();
  return s;
}

void suite_free (Suite *s)
{
  List *l;
  if (s == NULL)
    return;
  for (l = s->tclst; !list_at_end(l); list_advance (l)) {
    tcase_free (list_val(l));
  }
  list_free (s->tclst);
  free(s);
}

TCase *tcase_create (char *name)
{
  TCase *tc = emalloc (sizeof(TCase)); /*freed in tcase_free */
  if (name == NULL)
    tc->name = "";
  else
    tc->name = name;
  tc->tflst = list_create();
  tc->setup = tc->teardown = NULL;

  return tc;
}


void tcase_free (TCase *tc)
{
  List *l;
  l = tc->tflst;
  for (list_front(l); !list_at_end(l); list_advance(l)) {
    free (list_val(l));
  }
  list_free(tc->tflst);
  free(tc);
}

void suite_add_tcase (Suite *s, TCase *tc)
{
  if (s == NULL || tc == NULL)
    return;
  list_add_end (s->tclst, tc);
}

void _tcase_add_test (TCase *tc, TFun fn, char *name)
{
  TF * tf;
  if (tc == NULL || fn == NULL || name == NULL)
    return;
  tf = emalloc (sizeof(TF)); /* freed in tcase_free */
  tf->fn = fn;
  tf->name = name;
  list_add_end (tc->tflst, tf);
}

void tcase_set_fixture (TCase *tc, SFun setup, SFun teardown)
{
  tc->setup = setup;
  tc->teardown = teardown;
}


void tcase_fn_start (char *fname, char *file, int line)
{
  MsgSys *msgsys;

  msgsys = get_send_msgsys();
  send_last_loc_msg (msgsys, file, line);
}

void _mark_point (char *file, int line)
{
  MsgSys *msgsys;

  msgsys = get_send_msgsys();
  send_last_loc_msg (msgsys, file, line);
}

void _fail_unless (int result, char *file, int line, char * msg)
{
  MsgSys *msgsys;

  msgsys = get_send_msgsys();
  if (line > MAXLINE)
    eprintf ("Line number %d too large to use", line);

  send_last_loc_msg (msgsys, file, line);
  if (!result) {
    send_failure_msg (msgsys, msg);
    if (cur_fork_status() == CK_FORK)
      exit(1);
  }
}

SRunner *srunner_create (Suite *s)
{
  SRunner *sr = emalloc (sizeof(SRunner)); /* freed in srunner_free */
  sr->slst = list_create();
  list_add_end(sr->slst, s);
  sr->stats = emalloc (sizeof(TestStats)); /* freed in srunner_free */
  sr->stats->n_checked = sr->stats->n_failed = sr->stats->n_errors = 0;
  sr->resultlst = list_create();
  sr->log_fname = NULL;
  sr->loglst = NULL;
  sr->fstat = CK_FORK_UNSPECIFIED;
  return sr;
}

void srunner_add_suite (SRunner *sr, Suite *s)
{
  list_add_end(sr->slst, s);
}

void srunner_free (SRunner *sr)
{
  List *l;
  TestResult *tr;
  if (sr == NULL)
    return;
  
  free (sr->stats);
  list_free(sr->slst);

  l = sr->resultlst;
  for (list_front(l); !list_at_end(l); list_advance(l)) {
    tr = list_val(l);
    free(tr->file);
    free(tr->msg);
    free(tr);
  }
  list_free (sr->resultlst);

  free (sr);
}

int srunner_ntests_failed (SRunner *sr)
{
  return sr->stats->n_failed + sr->stats->n_errors;
}

int srunner_ntests_run (SRunner *sr)
{
  return sr->stats->n_checked;
}

TestResult **srunner_failures (SRunner *sr)
{
  int i = 0;
  TestResult **trarray;
  List *rlst;
  trarray = malloc (sizeof(trarray[0]) * srunner_ntests_failed (sr));

  rlst = sr->resultlst;
  for (list_front(rlst); !list_at_end(rlst); list_advance(rlst)) {
    TestResult *tr = list_val(rlst);
    if (non_pass(tr->rtype))
      trarray[i++] = tr;
    
  }
  return trarray;
}

TestResult **srunner_results (SRunner *sr)
{
  int i = 0;
  TestResult **trarray;
  List *rlst;

  trarray = malloc (sizeof(trarray[0]) * srunner_ntests_run (sr));

  rlst = sr->resultlst;
  for (list_front(rlst); !list_at_end(rlst); list_advance(rlst)) {
    trarray[i++] = list_val(rlst);
  }
  return trarray;
}

static int non_pass (int val)
{
  return val != CK_PASS;
}

char *tr_msg (TestResult *tr)
{
  return tr->msg;
}

int tr_lno (TestResult *tr)
{
  return tr->line;
}

char *tr_lfile (TestResult *tr)
{
  return tr->file;
}

int tr_rtype (TestResult *tr)
{
  return tr->rtype;
}

char *tr_tcname (TestResult *tr)
{
  return tr->tcname;
}

static int _fstat = CK_FORK;

void set_fork_status (enum fork_status fstat)
{
  if (fstat == CK_FORK || fstat == CK_NOFORK)
    _fstat = fstat;
  else
    eprintf ("Bad status in set_fork_status");
}

enum fork_status cur_fork_status (void)
{
  return _fstat;
}
