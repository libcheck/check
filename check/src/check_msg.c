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
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "list.h"
#include "error.h"
#include "check.h"
#include "check_impl.h"
#include "check_msg.h"
#include "check_pack.h"

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
  Pipe *p3;
  int cur;
} PipeEntry;

List *plst = NULL;

static PipeEntry *get_pe_by_key(int key);
static Pipe *get_pipe_by_key(int key);
static int get_setup_key (void);
static void setup_pipe (Pipe *p);
static void setup_messaging_with_key (int key);
static void teardown_messaging_with_key (int key);

void send_failure_info (int key, char *msg)
{
  FailMsg fmsg;
  Pipe *p;

  fmsg.msg = msg;
  p = get_pipe_by_key(key);
  if (p == NULL)
    eprintf("Couldn't find pipe with key %d",__FILE__, __LINE__, key);
  ppack(p->sendfd, CK_MSG_FAIL, &fmsg);
}

void send_loc_info (int key, char * file, int line)
{
  LocMsg lmsg;
  Pipe *p;

  lmsg.file = file;
  lmsg.line = line;
  p = get_pipe_by_key(key);
  if (p == NULL)
    eprintf("Couldn't find pipe with key %d",__FILE__, __LINE__, key);
  ppack(p->sendfd, CK_MSG_LOC, &lmsg);
}

void send_ctx_info (int key,enum ck_result_ctx ctx)
{
  CtxMsg cmsg;
  Pipe *p;

  cmsg.ctx = ctx;
  p = get_pipe_by_key(key);
  if (p == NULL)
    eprintf("Couldn't find pipe with key %d",__FILE__, __LINE__, key);

  ppack(p->sendfd, CK_MSG_CTX, &cmsg);
}

TestResult *receive_test_result (int key)
{
  Pipe *p;
  TestResult *tr;

  p = get_pipe_by_key(key);
  if (p == NULL)
    eprintf("Couldn't find pipe with key %d",__FILE__, __LINE__, key);
  close(p->sendfd);
  tr = punpack(p->recvfd);
  close(p->recvfd);
  setup_pipe(p);
  return tr;
}

void setup_messaging(void)
{
  setup_messaging_with_key(get_recv_key());
}

int get_send_key(void)
{
  if (cur_fork_status() == CK_FORK)
    return getppid();
  else
    return getpid();
}

int get_recv_key(void)
{
  return getpid();
}

static int get_setup_key (void)
{
  return get_recv_key();
}

void teardown_messaging(void)
{
  teardown_messaging_with_key(get_setup_key());
}

/* for testing only */
void setup_test_messaging(void)
{
  setup_messaging_with_key(get_test_key());
}

int get_test_key(void)
{
  return -1;
}

void teardown_test_messaging(void)
{
  teardown_messaging_with_key(get_test_key());
}

static PipeEntry *get_pe_by_key(key)
{
  PipeEntry *pe = NULL;
  
  for (list_front(plst); !list_at_end(plst); list_advance(plst)) {
    PipeEntry *p = list_val(plst);
    if (p->key == key) {
      pe = p;
      break;
    }
  }

  return pe;
}

static Pipe *get_pipe_by_key(int key)
{
  Pipe *pr = NULL;
  PipeEntry *pe = get_pe_by_key(key);

  if (pe == NULL || pe->cur == 0)
      eprintf ("No messaging setup", __FILE__, __LINE__);
  
  if (pe->cur == 1)
    pr = pe->p1;
  else if (pe->cur == 2)
    pr = pe->p2;
  else if (pe->cur == 3)
    pr = pe->p3;
  
  return pr;
}

static void setup_pipe (Pipe *p)
{
  int fd[2];

  pipe(fd);
  p->sendfd = fd[1];
  p->recvfd = fd[0];
}
      
void setup_messaging_with_key (int key)
{
  PipeEntry *pe;

  if (plst == NULL)
    plst = list_create();

  pe = get_pe_by_key(key);
  if (pe == NULL) {
    pe = emalloc(sizeof(PipeEntry));
    pe->cur = 0;
    pe->key = key;
    list_add_end(plst, pe);
  }
  if (pe->cur == 0) {
    pe->p1 = emalloc(sizeof(Pipe));
    pe->cur = 1;
    setup_pipe (pe->p1);
  } else if (pe->cur == 1) {
    pe->cur = 2;
    pe->p2 = emalloc(sizeof(Pipe));
    setup_pipe (pe->p2);
  } else if (pe->cur == 2) {
    pe->cur = 3;
    pe->p3 = emalloc(sizeof(Pipe));
    setup_pipe (pe->p3);
  } else
    eprintf("Only two nestings of suite runs supported", __FILE__, __LINE__);

}

void teardown_messaging_with_key (int key)
{
  PipeEntry *pe = get_pe_by_key(key);

  if (pe == NULL || pe->cur == 0)
    eprintf("Messaging not setup", __FILE__, __LINE__);
  else if (pe->cur == 1) {
    pe->cur = 0;
    free(pe->p1);
  } else if (pe->cur == 2) {
    pe->cur = 1;
    free(pe->p2);
  } else if (pe->cur == 3) {
    pe->cur = 2;
    free(pe->p3);
  }
  
}

