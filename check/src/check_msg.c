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
#include "check_run.h"

enum {
  LASTLOCMSG = 1,
  FAILUREMSG = 2
};
static LastLocMsg *create_last_loc_msg (char *file, int line);
static FailureMsg *create_failure_msg (char *msg);
static int get_msq (key_t key);
static char *ipcerrstr (int ipcerr);

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

char *last_loc_file (LastLocMsg *msg)
{
  int i;
  char *rmsg = emalloc (CMAXMSG); /* caller responsible for freeing */
  char *mmsg = msg->msg;
  if (msg == NULL)
    return NULL;
  for (i = 0; mmsg[i] != '\n'; i++) {
    if (mmsg[i] == '\0')
      eprintf ("Badly formated last loc message");
    rmsg[i] = mmsg[i];
  }
  rmsg[i] = '\0';
  return rmsg;
}

int last_loc_line (LastLocMsg *msg)
{
  char *rmsg;
  if (msg == NULL)
    return -1;
  rmsg  = msg->msg;
  while (*rmsg != '\n') {
    if (*rmsg == '\0')
      eprintf ("Badly formated last loc message");
    rmsg++;
  }
  rmsg++; /*advance past \n */
  return atoi (rmsg);
}


void send_last_loc_msg (int msqid, char * file, int line)
{
  int rval;
  LastLocMsg *rmsg = create_last_loc_msg(file, line);
  rval = msgsnd(msqid, (void *) rmsg, CMAXMSG, IPC_NOWAIT);
  if (rval == -1) {
    eprintf ("send_last_loc_msg:Failed to send message, msqid = %d:",msqid);
  }
  free(rmsg);
}

int init_msq (void) {
  int msqid;

  msqid = msgget(init_key(), 0666 | IPC_CREAT);
  if (msqid == -1)
    eprintf ("Unable to create message queue (%s):", ipcerrstr(errno));
  return msqid;
}

int get_recv_msq (void)
{
  return get_msq ((key_t) recv_key());
}

int get_send_msq (void)
{
  return get_msq ((key_t) send_key());
}


static int get_msq (key_t key)
{
  int msqid;
  
  msqid = msgget (key, 0666);
  if (msqid == -1)
    eprintf ("Unable to get message queue (%s):", ipcerrstr(errno));
  return msqid;
}

void delete_msq (void)
{
  int msqid;

  msqid = get_msq((key_t) init_key());
  if (msgctl (msqid, IPC_RMID, NULL) == -1)
    eprintf ("Failed to free message queue:");
}


void send_failure_msg (int msqid, char *msg)
{
  int rval;
  
  FailureMsg *rmsg = create_failure_msg(msg);
  
  rval = msgsnd(msqid, (void *) rmsg, CMAXMSG, IPC_NOWAIT);
  if (rval == -1)
    eprintf ("send_failure_msg:Failed to send message:");
  free(rmsg);
}

LastLocMsg *receive_last_loc_msg (int msqid)
{
  LastLocMsg *rmsg = emalloc(sizeof(LastLocMsg)); /* caller responsible for freeing */
  while (1) {
    int rval;
    rval = msgrcv(msqid, (void *) rmsg, CMAXMSG, LASTLOCMSG, IPC_NOWAIT);
    if (rval == -1) {
      if (errno == ENOMSG)
	break;
      eprintf ("receive_last_loc_msg:Failed to receive message:");
    }
  }
  return rmsg;
}
  
FailureMsg *receive_failure_msg (int msqid)
{ 
  FailureMsg *rmsg = emalloc(sizeof(FailureMsg));
  int rval;
  rval = msgrcv(msqid, (void *) rmsg, CMAXMSG, FAILUREMSG, IPC_NOWAIT);
  if (rval == -1) {
    if (errno == ENOMSG)
      return NULL;
    eprintf ("receive_failure_msg:Failed to receive message:");
  }
  return rmsg;
}

int init_key(void)
{
  return getpid();
}

int send_key(void)
{
  return getppid();
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

    
