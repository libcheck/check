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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "error.h"
#include "list.h"
#include "check.h"
#include "check_impl.h"
#include "check_msg.h"
#include "check_log.h"


enum rinfo {
  CK_R_SIG,
  CK_R_PASS,
  CK_R_EXIT,
  CK_R_FAIL_TEST,
  CK_R_FAIL_FIXTURE
};

enum tf_type {
  CK_FORK_TEST,
  CK_NOFORK_TEST,
  CK_NOFORK_FIXTURE
};

static void srunner_run_init (SRunner *sr, enum print_output print_mode);
static void srunner_run_end (SRunner *sr, enum print_output print_mode);
static void srunner_iterate_suites (SRunner *sr,
				    enum print_output print_mode);
static void srunner_run_tcase (SRunner *sr, TCase *tc);
static int srunner_run_unchecked_setup (SRunner *sr, TCase *tc);
static void srunner_run_unchecked_teardown (SRunner *sr, TCase *tc);
static void tcase_run_checked_setup (TCase *tc);
static void tcase_run_checked_teardown (TCase *tc);
static void srunner_iterate_tcase_tfuns (SRunner *sr, TCase *tc);
static void srunner_add_failure (SRunner *sr, TestResult *tf);
static TestResult *tcase_run_tfun_fork (TCase *tc, TF *tf);
static TestResult *tcase_run_tfun_nofork (TCase *tc, TF *tf);
static TestResult *receive_result_info_fork (char *tcname, int status);
static TestResult *receive_result_info_nofork (char *tcname);
static void set_fork_info (TestResult *tr, int status);
static void set_nofork_info (TestResult *tr);
static char *signal_msg (int sig);
static char *pass_msg (void);
static char *exit_msg (int exitstatus);
static int waserror (int status);

static void srunner_run_init (SRunner *sr, enum print_output print_mode)
{
  set_fork_status(srunner_fork_status(sr));
  setup_messaging();
  srunner_init_logging (sr, print_mode);
  log_srunner_start (sr);
}

static void srunner_run_end (SRunner *sr, enum print_output print_mode)
{
  log_srunner_end (sr);
  srunner_end_logging (sr);
  teardown_messaging();
  set_fork_status(CK_FORK);  
}

static void srunner_iterate_suites (SRunner *sr,
				    enum print_output print_mode)
  
{
  List *slst;
  List *tcl;
  TCase *tc;

  slst = sr->slst;
  
  for (list_front(slst); !list_at_end(slst); list_advance(slst)) {
    Suite *s = list_val(slst);
    
    log_suite_start (sr, s);

    tcl = s->tclst;
  
    for (list_front(tcl);!list_at_end (tcl); list_advance (tcl)) {
      tc = list_val (tcl);
      srunner_run_tcase (sr, tc);
    }
  }
}

void srunner_run_all (SRunner *sr, enum print_output print_mode)
{
  if (sr == NULL)
    return;
  if (print_mode < 0 || print_mode >= CK_LAST)
    eprintf("Bad print_mode argument to srunner_run_all: %d",
	    __FILE__, __LINE__, print_mode);
      
  srunner_run_init (sr, print_mode);
  srunner_iterate_suites (sr, print_mode);
  srunner_run_end (sr, print_mode);
}

static void srunner_add_failure (SRunner *sr, TestResult *tr)
{  
  list_add_end (sr->resultlst, tr);
  if (tr->ctx == CK_CTX_TEST)
    sr->stats->n_checked++;
  if (tr->rtype == CK_FAILURE)
    sr->stats->n_failed++;
  else if (tr->rtype == CK_ERROR)
    sr->stats->n_errors++;
  
}

static void srunner_iterate_tcase_tfuns (SRunner *sr, TCase *tc)
{
  List *tfl;
  TF *tfun;
  TestResult *tr = NULL;

  tfl = tc->tflst;
  
  for (list_front(tfl); !list_at_end (tfl); list_advance (tfl)) {
    tfun = list_val (tfl);
    switch (srunner_fork_status(sr)) {
    case CK_FORK:
      tr = tcase_run_tfun_fork (tc, tfun);
      break;
    case CK_NOFORK:
      tr = tcase_run_tfun_nofork (tc, tfun);
      break;
    default:
      eprintf("Bad fork status in SRunner", __FILE__, __LINE__);
    }
    srunner_add_failure (sr, tr);
    log_test_end(sr, tr);
  }
}  

static int srunner_run_unchecked_setup (SRunner *sr, TCase *tc)
{
  TestResult *tr;
  List *l;
  Fixture *f;
  int rval = 1;

  set_fork_status(CK_NOFORK);

  l = tc->unch_sflst;

  for (list_front(l); !list_at_end(l); list_advance(l)) {
    
    f = list_val(l);
    send_ctx_info(get_send_key(),CK_CTX_SETUP);
    f->fun();

    tr = receive_result_info_nofork (tc->name);

    if (tr->rtype != CK_PASS) {
      srunner_add_failure(sr, tr);
      rval = 0;
      break;
    }
  } 

  set_fork_status(srunner_fork_status(sr));
  return rval;
}

static void tcase_run_checked_setup (TCase *tc)
{
  List *l;
  Fixture *f;

  l = tc->ch_sflst;
  
  send_ctx_info(get_send_key(),CK_CTX_SETUP);

  for (list_front(l); !list_at_end(l); list_advance(l)) {
    f = list_val(l);
    f->fun();
  }
}

static void tcase_run_checked_teardown (TCase *tc)
{
  List *l;
  Fixture *f;

  l = tc->ch_tflst;
  
  send_ctx_info(get_send_key(),CK_CTX_TEARDOWN);

  for (list_front(l); !list_at_end(l); list_advance(l)) {
    f = list_val(l);
    f->fun();
  }
}

static void srunner_run_unchecked_teardown (SRunner *sr, TCase *tc)
{
  List *l;
  Fixture *f;
  
  set_fork_status(CK_NOFORK);
  l = tc->unch_tflst;
  
  for (list_front(l); !list_at_end(l); list_advance(l)) {
    
    f = list_val(l);
    send_ctx_info(get_send_key(),CK_CTX_TEARDOWN);
    f->fun ();
  }
  set_fork_status(srunner_fork_status(sr));
}

static void srunner_run_tcase (SRunner *sr, TCase *tc)
{
  
  if (srunner_run_unchecked_setup (sr,tc)) {  
  
    srunner_iterate_tcase_tfuns(sr,tc);
  
    srunner_run_unchecked_teardown (sr, tc);
  }
}

static TestResult *receive_result_info_fork (char *tcname, int status)
{
  TestResult *tr;

  tr = receive_test_result (get_recv_key(), waserror(status));
  if (tr == NULL)
    eprintf("Failed to receive test result", __FILE__, __LINE__);
  tr->tcname = tcname;
  set_fork_info(tr, status);

  return tr;
}

static TestResult *receive_result_info_nofork (char *tcname)
{
  TestResult *tr;

  tr = receive_test_result(get_recv_key(), 0);
  if (tr == NULL)
    eprintf("Failed to receive test result", __FILE__, __LINE__);
  tr->tcname = tcname;
  set_nofork_info(tr);

  return tr;
}

static void set_fork_info (TestResult *tr, int status)
{
  int was_sig = WIFSIGNALED(status);
  int was_exit = WIFEXITED(status);
  int exit_status = WEXITSTATUS(status);

  if (was_sig) {
    tr->rtype = CK_ERROR;
    tr->msg = signal_msg(WTERMSIG(status));
  } else if (was_exit && exit_status == 0) {
    tr->rtype = CK_PASS;
    tr->msg = pass_msg();
  } else if (was_exit && exit_status != 0) {
    if (tr->msg == NULL) { /* early exit */
      tr->rtype = CK_ERROR;
      tr->msg = exit_msg(exit_status);
    } else {
      tr->rtype = CK_FAILURE;
    }
  }
}

static void set_nofork_info (TestResult *tr)
{
  if (tr->msg == NULL) {
    tr->rtype = CK_PASS;
    tr->msg = pass_msg();
  } else {
    tr->rtype = CK_FAILURE;
  }
}

static TestResult *tcase_run_tfun_nofork (TCase *tc, TF *tfun)
{
  tfun->fn();
  return receive_result_info_nofork (tc->name);
}

  
static TestResult *tcase_run_tfun_fork (TCase *tc, TF *tfun)
{
  pid_t pid;
  int status = 0;

  pid = fork();
  if (pid == -1)
     eprintf ("Unable to fork:",__FILE__,__LINE__);
  if (pid == 0) {
    tcase_run_checked_setup(tc);
    tfun->fn();
    tcase_run_checked_teardown(tc);
    _exit(EXIT_SUCCESS);
  }
  (void) wait(&status);
  return receive_result_info_fork (tc->name, status);
}

static char *signal_msg (int signal)
{
  char *msg = emalloc (CMAXMSG); /* free'd by caller */
  snprintf(msg, CMAXMSG, "Received signal %d", signal);
  return msg;
}

static char *exit_msg (int exitval)
{
  char *msg = emalloc(CMAXMSG); /* free'd by caller */
  snprintf(msg, CMAXMSG,
	   "Early exit with return value %d", exitval);
  return msg;
}

static char *pass_msg (void)
{
  char *msg = emalloc(sizeof("Passed"));
  strcpy (msg, "Passed");
  return msg;
}

enum fork_status srunner_fork_status (SRunner *sr)
{
  if (sr->fstat == CK_FORK_UNSPECIFIED) {
    char *env = getenv("CK_FORK");
    if (env == NULL)
      return CK_FORK;
    if (strcmp(env,"no") == 0)
      return CK_NOFORK;
    else
      return CK_FORK;
  } else
    return sr->fstat;
}

void srunner_set_fork_status (SRunner *sr, enum fork_status fstat)
{
  sr->fstat = fstat;
}

static int waserror (int status)
{
  int was_sig = WIFSIGNALED(status);
  int was_exit = WIFEXITED(status);
  int exit_status = WEXITSTATUS(status);

  return (was_sig || (was_exit && exit_status != 0));
}
