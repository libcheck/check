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


static void receive_failure_info (MsgSys *msgsys,
				  TestResult *tr,
				  enum tf_type tf_type,
				  int status);


static void srunner_run_init (SRunner *sr, enum print_output print_mode);
static void srunner_run_end (SRunner *sr, enum print_output print_mode);
static void srunner_iterate_suites (SRunner *sr,
				    enum print_output print_mode);
static void srunner_run_tcase (SRunner *sr, TCase *tc);
static int srunner_run_setup (SRunner *sr, TCase *tc, MsgSys *msgsys);
static void srunner_iterate_tcase_tfuns (SRunner *sr, TCase *tc);
static void srunner_add_failure (SRunner *sr, TestResult *tf);
static TestResult *tfun_run_fork (char *tcname, TF *tf);
static TestResult *tfun_run_nofork (char *tcname, TF *tf);
static TestResult *receive_result_info (MsgSys *msgsys, char *tcname,
					enum tf_type, int status);
static TestResult *receive_result_info_nofork (MsgSys *msgsys, char *tcname);
static TestResult *receive_result_info_setup (MsgSys *msgsys, char *tcname);
static enum test_result result_info_rtype (enum rinfo rinfo);
static enum rinfo result_info (enum tf_type tf_type, char *msg, int status);
static char *result_info_msg (enum rinfo rinfo, char *fmsg, int status);
static void receive_last_loc_info (MsgSys *msgsys, TestResult *tr);

static char *signal_msg (int sig);
static char *exit_msg (int exitstatus);



static void srunner_run_init (SRunner *sr, enum print_output print_mode)
{
  set_fork_status(srunner_fork_status(sr));
  srunner_init_logging (sr, print_mode);
  log_srunner_start (sr);
}

static void srunner_run_end (SRunner *sr, enum print_output print_mode)
{
  log_srunner_end (sr);
  srunner_end_logging (sr);
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
  if (print_mode < 0 || print_mode >= CRLAST)
    eprintf("Bad print_mode argument to srunner_run_all: %d", print_mode);
      
  srunner_run_init (sr, print_mode);
  srunner_iterate_suites (sr, print_mode);
  srunner_run_end (sr, print_mode);
}

static void srunner_add_failure (SRunner *sr, TestResult *tr)
{  
  list_add_end (sr->resultlst, tr);
  switch (tr->rtype) {
    
  case CK_PASS:
    sr->stats->n_checked++;
    return;
  case CK_FAILURE:
    sr->stats->n_checked++;
    sr->stats->n_failed++;
    return;
  case CK_ERROR:
    sr->stats->n_checked++;
    sr->stats->n_errors++;
    return;
  case CK_FIXTURE:
    sr->stats->n_failed++;
    return;
  }
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
      tr = tfun_run_fork (tc->name, tfun);
      break;
    case CK_NOFORK:
      tr = tfun_run_nofork (tc->name, tfun);
      break;
    default:
      eprintf("Bad fork status in SRunner");
    }
    srunner_add_failure (sr, tr);
    log_test_end(sr, tr);
  }
}

static int srunner_run_setup (SRunner *sr, TCase *tc, MsgSys *msgsys)
{
  TestResult *tr;

  set_fork_status(CK_NOFORK);
  
  if (tc->setup)
    tc->setup();

  tr = receive_result_info_setup (msgsys, tc->name);

  set_fork_status(srunner_fork_status(sr));
  if (tr->rtype == CK_PASS) {
    return 1;
  } else {
    srunner_add_failure(sr, tr);
    return 0;
  }
  
}

static void srunner_run_tcase (SRunner *sr, TCase *tc)
{
  MsgSys *msgsys;

  msgsys = init_msgsys();

  if (srunner_run_setup (sr,tc,msgsys)) {  
  
    srunner_iterate_tcase_tfuns(sr,tc);
  
    if (tc->teardown)
      tc->teardown();
  }
  delete_msgsys();
}

static void receive_last_loc_info (MsgSys *msgsys, TestResult *tr)
{
  Loc *loc;
  loc = receive_last_loc_msg (msgsys);
  if (loc == NULL) {
    char *s = emalloc (strlen ("unknown") + 1);
    strcpy(s,"unknown");
    tr->file = s;
    tr->line = -1;
  } else {
    tr->file = loc->file;
    tr->line = loc->line;
    free (loc);
  }
}  


static TestResult *receive_result_info (MsgSys *msgsys, char *tcname,
					enum tf_type tf_type, int status)
{
  TestResult *tr = emalloc (sizeof(TestResult));

  tr->tcname = tcname;
  receive_last_loc_info (msgsys, tr);
  receive_failure_info (msgsys, tr, tf_type, status);
  return tr;
}

static TestResult *receive_result_info_fork (MsgSys *msgsys, char *tcname,
					     int status)
{
  return receive_result_info (msgsys, tcname, CK_FORK_TEST, status);
}

static TestResult *receive_result_info_nofork (MsgSys *msgsys, char *tcname)
{
  return receive_result_info (msgsys,tcname, CK_NOFORK_TEST, 0);
}


static TestResult *receive_result_info_setup (MsgSys *msgsys, char *tcname)
{
  return receive_result_info (msgsys,tcname, CK_NOFORK_FIXTURE, 0);
}


static enum rinfo result_info (enum tf_type tf_type, char *msg, int status)
{
  enum rinfo rinfo = -1;
  
  if (tf_type == CK_FORK_TEST) {
    int was_sig = WIFSIGNALED(status);
    int was_exit = WIFEXITED(status);
    int exit_status = WEXITSTATUS(status);
    
    if (was_sig)
      rinfo = CK_R_SIG;
    else if (was_exit && exit_status == 0)
      rinfo = CK_R_PASS;
    else if (was_exit && exit_status != 0) {
      if (msg == NULL) /* early exit */
	rinfo = CK_R_EXIT;
      else
	rinfo = CK_R_FAIL_TEST;
    } else
      eprintf ("Bad status from wait() call\n");
  } else {
    if (msg == NULL)
      rinfo = CK_R_PASS;
    else if (tf_type == CK_NOFORK_FIXTURE)
      rinfo = CK_R_FAIL_FIXTURE;
    else
      rinfo = CK_R_FAIL_TEST;
  }

  return rinfo;
}

    
static enum test_result result_info_rtype (enum rinfo rinfo)
{
  enum test_result rtype = -1;
  switch (rinfo) {
  case CK_R_SIG:
    rtype = CK_ERROR;
    break;
  case CK_R_PASS:
    rtype = CK_PASS;
    break;
  case CK_R_EXIT:
    rtype = CK_ERROR;
    break;
  case CK_R_FAIL_TEST:
    rtype = CK_FAILURE;
    break;
  case CK_R_FAIL_FIXTURE:
    rtype = CK_FIXTURE;
    break;
  default: eprintf ("Bad parameter value in result_info_rtype");
  }
  return rtype;
}

static char * result_info_msg (enum rinfo rinfo, char *fmsg, int status)
{
  char *msg = NULL;
  
  switch (rinfo) {
  case CK_R_SIG:
    msg = signal_msg (WTERMSIG(status));
    break;
  case CK_R_PASS:
    msg = emalloc (sizeof("Test passed"));
    strcpy (msg,"Test passed");
    break;
  case CK_R_EXIT:
    msg = exit_msg(WEXITSTATUS(status));
    break;
  case CK_R_FAIL_TEST: /* fall through */
  case CK_R_FAIL_FIXTURE:
    msg = emalloc(strlen(fmsg) + 1);
    strcpy (msg, fmsg);
    break;
  default: eprintf ("Bad parameter value in result_info_rtype");
  }

  return msg;
}

static void receive_failure_info (MsgSys *msgsys,
				  TestResult *tr,
				  enum tf_type tf_type,
				  int status)
{
  char *fmsg;
  enum rinfo rinfo;

  fmsg = receive_failure_msg (msgsys);

  rinfo = result_info (tf_type, fmsg, status);
  tr->rtype = result_info_rtype (rinfo);
  tr->msg = result_info_msg (rinfo, fmsg, status);
  
  if (fmsg != NULL)
    free (fmsg);

}

static TestResult *tfun_run_nofork (char *tcname, TF *tfun)
{
  MsgSys  *msgsys;

  msgsys = get_recv_msgsys();
  tfun->fn();
  return receive_result_info_nofork (msgsys, tcname);
}

  
static TestResult *tfun_run_fork (char *tcname, TF *tfun)
{
  pid_t pid;
  int status = 0;
  MsgSys  *msgsys;

  msgsys = get_recv_msgsys();

  pid = fork();
  if (pid == -1)
     eprintf ("Unable to fork:");
  if (pid == 0) {
    tfun->fn();
    _exit(EXIT_SUCCESS);
  }
  (void) wait(&status);
  return receive_result_info_fork (msgsys, tcname, status);
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

