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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include "list.h"
#include "error.h"
#include "check.h"
#include "check_impl.h"
#include "check_msg.h"


typedef struct LastLocMsg {
  long int message_type;
  char msg[CMAXMSG]; /* Format: filename\nlineno\0 */
} LastLocMsg;

typedef struct FailureMsg {
  long int message_type;
  char msg[CMAXMSG];
} FailureMsg;

struct MsgSys 
{
  int msqid;
};


enum {
  LASTLOCMSG = 1,
  FAILUREMSG = 2
};
static LastLocMsg *create_last_loc_msg (char *file, int line);
static char *last_loc_file (LastLocMsg *msg);
static int last_loc_line (LastLocMsg *msg);
static FailureMsg *create_failure_msg (char *msg);
static char *ipcerrstr (int ipcerr);
static int init_key (void);
static int send_key (void);
static int recv_key (void);


static FailureMsg *create_failure_msg (char *msg)
{
  FailureMsg *m = emalloc (sizeof(FailureMsg));
  m->message_type = (long int) FAILUREMSG;
  strncpy(m->msg, msg, CMAXMSG);
  return m;
}


static LastLocMsg *create_last_loc_msg (char *file, int line)
{
  LastLocMsg *m = emalloc (sizeof(LastLocMsg));
  m->message_type = (long int) LASTLOCMSG;
  snprintf(m->msg, CMAXMSG, "%s\n%d", file, line);

  return m;
}

static char *last_loc_file (LastLocMsg *msg)
{
  int i;
  char *rmsg = emalloc (CMAXMSG); /* caller responsible for freeing */
  char *mmsg = msg->msg;
  if (msg == NULL)
    return NULL;
  for (i = 0; mmsg[i] != '\n'; i++) {
    if (mmsg[i] == '\0')
      eprintf ("Badly formated last loc message", __FILE__, __LINE__);
    rmsg[i] = mmsg[i];
  }
  rmsg[i] = '\0';
  return rmsg;
}

static int last_loc_line (LastLocMsg *msg)
{
  char *rmsg;
  if (msg == NULL)
    return -1;
  rmsg  = msg->msg;
  while (*rmsg != '\n') {
    if (*rmsg == '\0')
      eprintf ("Badly formated last loc message", __FILE__, __LINE__);
    rmsg++;
  }
  rmsg++; /*advance past \n */
  return atoi (rmsg);
}


void send_last_loc_msg (MsgSys *msgsys, char *file, int line)
{
  int rval;
  LastLocMsg *rmsg = create_last_loc_msg(file, line);
  rval = msgsnd(msgsys->msqid, (void *) rmsg, CMAXMSG, IPC_NOWAIT);
  if (rval == -1) {
    eprintf ("send_last_loc_msg:Failed to send message, msqid = %d:",
	     __FILE__, __LINE__, msgsys->msqid);
  }
  free(rmsg);
}

MsgSys *init_msgsys (void) {
  return create_msgsys_with_key(init_key());
}

MsgSys *get_recv_msgsys (void)
{
  return get_msgsys_with_key (recv_key());
}

MsgSys *get_send_msgsys (void)
{
  return get_msgsys_with_key (send_key());
}

MsgSys *create_msgsys_with_key (int key)
{
  MsgSys *msgsys;

  msgsys = emalloc(sizeof(MsgSys));

  msgsys->msqid = msgget((key_t) key, 0666 | IPC_CREAT);
  if (msgsys->msqid == -1)
    eprintf ("Unable to create message queue (%s):",
	     __FILE__, __LINE__, ipcerrstr(errno));
  return msgsys;
}


MsgSys *get_msgsys_with_key (int key)
{
  MsgSys *msgsys;

  msgsys = emalloc(sizeof(MsgSys));
  
  msgsys->msqid = msgget ((key_t) key, 0666);
  if (msgsys->msqid == -1)
    eprintf ("Unable to get message queue (%s):",
	     __FILE__, __LINE__, ipcerrstr(errno));
  return msgsys;
}

void delete_msgsys_with_key (int key)
{
  MsgSys *msgsys;

  msgsys = get_msgsys_with_key ((key_t) key);
  if (msgctl (msgsys->msqid, IPC_RMID, NULL) == -1)
    eprintf ("Failed to free message queue:", __FILE__, __LINE__);
  free(msgsys);
}

void delete_msgsys (void)
{
  delete_msgsys_with_key(init_key());
}


void send_failure_msg (MsgSys *msgsys, char *msg)
{
  int rval;
  
  FailureMsg *rmsg = create_failure_msg(msg);
  
  rval = msgsnd(msgsys->msqid, (void *) rmsg, CMAXMSG, IPC_NOWAIT);
  if (rval == -1)
    eprintf ("send_failure_msg:Failed to send message:", __FILE__, __LINE__);
  free(rmsg);
}

Loc *receive_last_loc_msg (MsgSys *msgsys)
{
  LastLocMsg *lmsg;
  Loc *loc;
  int got_msg = 0;

  lmsg = emalloc(sizeof(LastLocMsg)); /* caller responsible for freeing */
  while (1) {
    int rval;
    rval = msgrcv(msgsys->msqid,
		  (void *) lmsg, CMAXMSG, LASTLOCMSG, IPC_NOWAIT);
    if (rval == -1) {
      if (errno == ENOMSG)
	break;
      eprintf ("receive_last_loc_msg:Failed to receive message:",
	       __FILE__, __LINE__);
    }
    got_msg = 1;
  }
  
  if (got_msg) {
    loc = emalloc(sizeof(Loc));
    loc->file = last_loc_file(lmsg);
    loc->line = last_loc_line(lmsg);
  } else {
    loc = NULL;
  }
  
  free(lmsg);
  return loc;
}
  
char *receive_failure_msg (MsgSys *msgsys)
{ 
  FailureMsg *fmsg;
  char *msg;
  int rval;

  fmsg = emalloc(sizeof(FailureMsg));
  rval = msgrcv(msgsys->msqid,
		(void *) fmsg, CMAXMSG, FAILUREMSG, IPC_NOWAIT);
  if (rval == -1) {
    if (errno == ENOMSG)
      return NULL;
    eprintf ("receive_failure_msg:Failed to receive message:",
	     __FILE__, __LINE__);
  }
  msg = emalloc(strlen(fmsg->msg) + 1);
  strcpy(msg,fmsg->msg);
  free(fmsg);
  return msg;
}

int init_key(void)
{
  return getpid();
}

int send_key(void)
{
  int key;
  enum fork_status fstat;

  fstat = cur_fork_status();
  
  if (fstat == CK_FORK)
    key = (int) getppid();
  else if (fstat == CK_NOFORK)
    key = (int) getpid();
  else
    key = -1, eprintf ("Bad _fstat", __FILE__, __LINE__);

  return key;
}

int recv_key(void)
{
  return getpid();
}

static char *ipcerrstr (int ipcerrno)
{
  /* Debian sid has broken error reporting for IPC */
  switch (errno) {
  case EACCES: return "EACCES";
  case EEXIST: return "EEXIST";
  case EIDRM: return "EIDRM";
  case ENOENT: return "ENOENT";
  case ENOMEM: return "ENOMEM";
  case ENOSPC: return "ENOSPC";
  default: return "Other";
  }
}

    
