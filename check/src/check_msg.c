/*
 * Check: a unit test framework for C
 * Copyright (C) 2001 2002, Arien Malec
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

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#include "check_error.h"
#include "check.h"
#include "check_list.h"
#include "check_impl.h"
#include "check_msg.h"
#include "check_pack.h"

struct MsgKey
{
  int key;
};

typedef struct Pipe 
{
  int sendfd;
  int recvfd;
} Pipe;

typedef struct PipeEntry
{
  int key;
  Pipe *p1;
  Pipe *p2;
  int cur;
} PipeEntry;

List *plst = NULL;

static PipeEntry *get_pe_by_key(MsgKey *key);
static Pipe *get_pipe_by_key(MsgKey *key);
static MsgKey *get_setup_key (void);
static void setup_pipe (Pipe *p);
static void setup_messaging_with_key (MsgKey *key);
static void teardown_messaging_with_key (MsgKey *key);
static TestResult *construct_test_result (RcvMsg *rmsg, int waserror);
static void tr_set_loc_by_ctx (TestResult *tr, enum ck_result_ctx ctx,
			       RcvMsg *rmsg);

void send_failure_info (MsgKey *key, const char *msg)
{
  FailMsg fmsg;
  Pipe *p;

  fmsg.msg = (char *) msg;
  p = get_pipe_by_key (key);
  if (p == NULL)
    eprintf ("Couldn't find pipe with key %d",__FILE__, __LINE__, key);
  ppack (p->sendfd, CK_MSG_FAIL, (CheckMsg *) &fmsg);
}

void send_loc_info (MsgKey *key, const char * file, int line)
{
  LocMsg lmsg;
  Pipe *p;

  lmsg.file = (char *) file;
  lmsg.line = line;
  p = get_pipe_by_key (key);
  if (p == NULL)
    eprintf ("Couldn't find pipe with key %d",__FILE__, __LINE__, key);
  ppack (p->sendfd, CK_MSG_LOC, (CheckMsg *) &lmsg);
}

void send_ctx_info (MsgKey *key,enum ck_result_ctx ctx)
{
  CtxMsg cmsg;
  Pipe *p;

  cmsg.ctx = ctx;
  p = get_pipe_by_key (key);
  if (p == NULL)
    eprintf ("Couldn't find pipe with key %d",__FILE__, __LINE__, key);

  ppack (p->sendfd, CK_MSG_CTX, (CheckMsg *) &cmsg);
}

TestResult *receive_test_result (MsgKey *key, int waserror)
{
  Pipe *p;
  RcvMsg *rmsg;

  p = get_pipe_by_key (key);
  if (p == NULL)
    eprintf ("Couldn't find pipe with key %d",__FILE__, __LINE__, key);
  close (p->sendfd);
  rmsg = punpack (p->recvfd);
  close (p->recvfd);
  setup_pipe (p);

  return construct_test_result (rmsg, waserror);
}

static void tr_set_loc_by_ctx (TestResult *tr, enum ck_result_ctx ctx,
			       RcvMsg *rmsg)
{
  if (ctx == CK_CTX_TEST) {
    tr->file = rmsg->test_file;
    tr->line = rmsg->test_line;
  } else {
    tr->file = rmsg->fixture_file;
    tr->line = rmsg->fixture_line;
  }
}

static TestResult *construct_test_result (RcvMsg *rmsg, int waserror)
{
  TestResult *tr;

  if (rmsg == NULL)
    return NULL;

  tr = emalloc (sizeof (TestResult));

  if (rmsg->msg != NULL || waserror) {
    tr->ctx = rmsg->lastctx;
    tr->msg = rmsg->msg;
    tr_set_loc_by_ctx (tr, rmsg->lastctx, rmsg);
  } else if (rmsg->lastctx == CK_CTX_SETUP) {
    tr->ctx = CK_CTX_SETUP;
    tr->msg = NULL;
    tr_set_loc_by_ctx (tr, CK_CTX_SETUP, rmsg);  
  } else {
    tr->ctx = CK_CTX_TEST;
    tr->msg = NULL;
    tr_set_loc_by_ctx (tr, CK_CTX_TEST, rmsg);
  }

  return tr;  
  
}

void setup_messaging (void)
{
  setup_messaging_with_key (get_recv_key());
}

MsgKey *get_send_key (void)
{
  MsgKey *key = emalloc (sizeof (MsgKey));
  
  if (cur_fork_status () == CK_FORK)
    key->key = getppid ();
  else
    key->key = getpid ();

  return key;
}

MsgKey *get_recv_key (void)
{
  MsgKey *key = emalloc (sizeof (MsgKey));

  key->key = getpid ();
  return key;
}

static MsgKey *get_setup_key (void)
{
  return get_recv_key ();
}

void teardown_messaging (void)
{
  teardown_messaging_with_key (get_setup_key ());
}

/* for testing only */
void setup_test_messaging (void)
{
  setup_messaging_with_key (get_test_key ());
}

MsgKey *get_test_key (void)
{
  MsgKey *key = emalloc (sizeof (MsgKey));
  key->key = -1;
  return key;

}

void teardown_test_messaging (void)
{
  teardown_messaging_with_key (get_test_key ());
}

static PipeEntry *get_pe_by_key (MsgKey *key)
{
  PipeEntry *pe = NULL;
  
  for (list_front(plst); !list_at_end(plst); list_advance(plst)) {
    PipeEntry *p = list_val(plst);
    if (p->key == key->key) {
      pe = p;
      break;
    }
  }

  return pe;
}

static Pipe *get_pipe_by_key (MsgKey *key)
{
  Pipe *pr = NULL;
  PipeEntry *pe = get_pe_by_key (key);

  if (pe == NULL || pe->cur == 0)
      eprintf ("No messaging setup", __FILE__, __LINE__);
  
  if (pe->cur == 1)
    pr = pe->p1;
  else if (pe->cur == 2)
    pr = pe->p2;
  
  return pr;
}

static void setup_pipe (Pipe *p)
{
  int fd[2];

  pipe (fd);

  p->sendfd = fd[1];
  p->recvfd = fd[0];

  /* 
   *  Make the pipe nonblocking so we don't block when too many
   *  messages are sent while the other end of the pipe waits for the
   *  test to exit (see bug #482012).  This doesn't solve our problem,
   *  but it makes it more obvious what is happening since instead of
   *  blocking the test exits with "Resource temporarily unavailable".
   */
  fcntl (p->sendfd, F_SETFL, O_NONBLOCK);
}

static void setup_messaging_with_key (MsgKey *key)
{
  PipeEntry *pe;

  if (plst == NULL)
    plst = list_create();

  pe = get_pe_by_key (key);
  if (pe == NULL) {
    pe = emalloc (sizeof (PipeEntry));
    pe->cur = 0;
    pe->key = key->key;
    list_add_end (plst, pe);
  }
  if (pe->cur == 0) {
    pe->p1 = emalloc (sizeof(Pipe));
    pe->cur = 1;
    setup_pipe (pe->p1);
  } else if (pe->cur == 1) {
    pe->cur = 2;
    pe->p2 = emalloc (sizeof(Pipe));
    setup_pipe (pe->p2);
  } else
    eprintf ("Only one nesting of suite runs supported", __FILE__, __LINE__);
}

static void teardown_messaging_with_key (MsgKey *key)
{
  PipeEntry *pe = get_pe_by_key (key);

  if (pe == NULL || pe->cur == 0)
    eprintf ("Messaging not setup", __FILE__, __LINE__);
  else if (pe->cur == 1) {
    pe->cur = 0;
    free (pe->p1);
  } else if (pe->cur == 2) {
    pe->cur = 1;
    free (pe->p2);
  }
}

