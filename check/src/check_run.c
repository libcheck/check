#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "list.h"
#include "check.h"
#include "check_impl.h"
#include "check_msg.h"

typedef struct TestStats {
  int n_checked;
  int n_failed;
  int n_errors;
} TestStats;

struct SRunner {
  Suite *s;
  TestStats *stats;
  List *resultlst;
};

struct TestResult {
  int ftype;     /* Type of result */
  char *file;    /* File where the test occured */
  int line;      /* Line number where the test occurred */
  int exact_loc; /* Is the location given by file:line exact? */
  char *tcname;  /* Test case that generated the result */
  char *msg;     /* Failure message */
};

static int percent_passed (TestStats *t);
static void srunner_run_tcase (SRunner *sr, TCase *tc);
static void srunner_add_failure (SRunner *sr, TestResult *tf);
static TestResult *tfun_run (int msqid, char *tcname, TF *tf);
static List *srunner_resultlst (SRunner *sr);

static void print_failure (TestResult *tr);
static char *signal_msg (int sig);
static char *exit_msg (int exitstatus);
static int non_pass (int val);


SRunner *srunner_create (Suite *s)
{
  SRunner *sr = emalloc (sizeof(SRunner)); /* freed in srunner_free */
  sr->s = s;
  sr->stats = emalloc (sizeof(TestStats)); /* freed in srunner_free */
  sr->stats->n_checked = sr->stats->n_failed = sr->stats->n_errors = 0;
  sr->resultlst = list_create();
  return sr;
}

void srunner_free (SRunner *sr)
{
  List *l;
  TestResult *tr;
  if (sr == NULL)
    return;
  
  free (sr->stats);

  l = sr->resultlst;
  for (list_front(l); !list_at_end(l); list_advance(l)) {
    tr = list_val(l);
    free(tr->file);
    if (tr->ftype == CRFAILURE || tr->ftype == CRERROR)
      free(tr->msg);
    free(tr);
  }
  list_free (sr->resultlst);
  free (sr);
} 

void srunner_run_all (SRunner *sr, int print_mode)
{
  List *tcl;
  TCase *tc;
  if (sr == NULL)
    return;
  if (print_mode < 0 || print_mode >= CRLAST)
    eprintf("Bad print_mode argument to srunner_run_all: %d", print_mode);

  if (print_mode > CRSILENT) {
    printf ("Running suite: %s\n", sr->s->name);
    fflush (stdout);
  }

  tcl = sr->s->tclst;
  
  for (list_front(tcl);!list_at_end (tcl); list_advance (tcl)) {
    tc = list_val (tcl);
    srunner_run_tcase (sr, tc);
  }
  if (print_mode >= CRMINIMAL)
    print_summary_report (sr);
  if (print_mode >= CRNORMAL)
    print_failures (sr);
}

static void srunner_add_failure (SRunner *sr, TestResult *tr)
{
  sr->stats->n_checked++;
  list_add_end (sr->resultlst, tr);
  switch (tr->ftype) {
    
  case CRPASS:
    return;
  case CRFAILURE:
    sr->stats->n_failed++;
    return;
  case CRERROR:
    sr->stats->n_errors++;
    return;
  }
}
  
static void srunner_run_tcase (SRunner *sr, TCase *tc)
{
  List *tfl;
  TF *tfun;
  TestResult *tr;
  int msqid;

  if (tc->setup)
    tc->setup();
  msqid = create_msq();
  tfl = tc->tflst;
  
  for (list_front(tfl); !list_at_end (tfl); list_advance (tfl)) {
    tfun = list_val (tfl);
    tr = tfun_run (msqid, tc->name, tfun);
    srunner_add_failure (sr, tr);
  }
  delete_msq(msqid);
  if (tc->teardown)
    tc->teardown();
}

static TestResult *receive_failure_info (int msqid, int status, char *tcname)
{
  LastLocMsg *lmsg;
  FailureMsg *fmsg;
  TestResult *tr = emalloc (sizeof(TestResult));

  lmsg = receive_last_loc_msg (msqid);
  tr->file = last_loc_file (lmsg);
  tr->line = last_loc_line (lmsg);
  free (lmsg);
  tr->tcname = tcname;

  if (WIFSIGNALED(status)) {
    tr->ftype = CRERROR;
    tr->exact_loc = 0;
    tr->msg = signal_msg (WTERMSIG(status));
    return tr;
  }
  
  if (WIFEXITED(status)) {
    
    if (WEXITSTATUS(status) == 0) {
      tr->ftype = CRPASS;
      tr->exact_loc = 1;
    }
    else {
      
      fmsg = receive_failure_msg (msqid);
      if (fmsg == NULL) { /* implies early exit */
	tr->ftype = CRERROR;
	tr->msg =  exit_msg (WEXITSTATUS(status));
	tr->exact_loc = 0;
      }
      else {
	tr->ftype = CRFAILURE;
	tr->exact_loc = 1;
	tr->msg = emalloc(strlen(fmsg->msg) + 1);
	strcpy (tr->msg, fmsg->msg);
	free (fmsg);
      }
    }
  } else {
    eprintf ("Bad status from wait() call\n");
  }
  return tr;
}

static TestResult *tfun_run (int msqid, char *tcname, TF *tfun)
{
  pid_t pid;
  int status = 0;

  pid = fork();
  if (pid == -1)
     eprintf ("Unable to fork:");
  if (pid == 0) {
    tfun->fn(msqid);
    exit(0);
  }
  (void) wait(&status);
  return receive_failure_info(msqid, status, tcname);
}


/* Printing */

void print_summary_report (SRunner *sr)
{
  TestStats *ts = sr->stats;
  printf ("%d%%: Checks: %d, Failures: %d, Errors: %d\n",
	  percent_passed (ts), ts->n_checked, ts->n_failed, ts->n_errors);
  return;
}

void print_failures (SRunner *sr)
{
  List *resultlst;
  resultlst = sr->resultlst;
  
  for (list_front(resultlst); !list_at_end(resultlst); list_advance(resultlst)) {
    print_failure (list_val(resultlst));
  }
  return;
}

int srunner_nfailures (SRunner *sr)
{
  return sr->stats->n_failed + sr->stats->n_errors;
}

TestResult **srunner_failures (SRunner *sr)
{
  int i = 0;
  TestResult **trarray;
  List *rlst;
  trarray = malloc (sizeof(trarray[0]) * srunner_nfailures(sr));

  rlst = srunner_resultlst (sr);
  for (list_front(rlst); !list_at_end(rlst); list_advance(rlst)) {
    TestResult *tr = list_val(rlst);
    if (non_pass(tr->ftype))
      trarray[i++] = tr;
    
  }
  return trarray;
}

static List *srunner_resultlst (SRunner *sr)
{
  return sr->resultlst;
}

char *tr_msg (TestResult *tr)
{
  return tr->msg;
}

static int percent_passed (TestStats *t)
{
  if (t->n_failed == 0 && t->n_errors == 0)
    return 100;
  else
    return (int) ( (float) (t->n_checked - (t->n_failed + t->n_errors)) /
		   (float) t->n_checked * 100);
}

static void print_failure (TestResult *tr)
{
  char *exact_msg;
  if (tr->ftype == CRPASS)
    return;
  exact_msg = (tr->exact_loc) ? "" : "(after this point) ";
  printf ("%s:%d:%s: %s%s\n", tr->file, tr->line, tr->tcname,
	  exact_msg, tr->msg);
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

static int non_pass (int val)
{
  return val == CRFAILURE || CRERROR;
}
