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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "list.h"
#include "error.h"
#include "check.h"
#include "check_magic.h"
#include "check_impl.h"
#include "check_pack.h"


static void pack_int (char **buf, int val);
static int upack_int (char **buf);
static void pack_str (char **buf, char *str);
static char *upack_str (char **buf);

static void pack_ctx (char **buf, void *msg);
static void pack_loc (char **buf, void *msg);
static void pack_fail (char **buf, void *msg);
static void upack_ctx (char **buf, void *msg);
static void upack_loc (char **buf, void *msg);
static void upack_fail (char **buf, void *msg);

static void check_type (int type, char *file, int line);
static enum ck_msg_type upack_type (char **buf);
static void pack_type (char **buf, enum ck_msg_type type);

static int read_buf (int fdes, char **buf);
static int get_result (char *buf, RcvMsg *rmsg);
static void rcvmsg_update_ctx(RcvMsg *rmsg, enum ck_result_ctx ctx);
static void rcvmsg_update_loc(RcvMsg *rmsg, char *file, int line);
static RcvMsg *rcvmsg_create(void);

typedef void (*pfun) (char **, void *);

static pfun pftab [] = {
  pack_ctx,
  pack_fail,
  pack_loc
};

static pfun upftab [] = {
  upack_ctx,
  upack_fail,
  upack_loc
};

int pack (enum ck_msg_type type, char *buf, void *data)
{
  char *obuf;
  int nread;
  
  if (buf == NULL)
    return -1;
  if (data == NULL)
    return 0;
  obuf = buf;
  
  check_type(type, __FILE__, __LINE__);

  pack_type(&buf, type);
  
  pftab[type] (&buf, data);

  nread = buf - obuf;
  return nread;
}

int upack (char *buf, void *data, enum ck_msg_type *type)
{
  char *obuf;
  int nread;

  if (buf == NULL)
    return -1;

  obuf = buf;

  *type = upack_type (&buf);

  check_type(*type, __FILE__, __LINE__);
  
  upftab[*type] (&buf, data);

  nread = buf - obuf;
  return nread;
}

static void pack_int (char **buf, int val)
{
  int n;
  
  n = sprintf (*buf, "%d", val);

  *buf += n + 1;
}

static int upack_int (char **buf)
{
  int val;
  char *endptr;

  val = (int) strtol (*buf, &endptr, 10);

  *buf = endptr + 1;
  
  return val;
}

static void pack_str (char **buf, char *val)
{
  int strsz;
  int n;

  if (val == NULL)
    strsz = 0;
  else
    strsz = strlen (val) + 1;

  pack_int(buf,strsz);  
  n = sprintf (*buf, "%s", val);

  *buf += n + 1;
}  

static char *upack_str (char **buf)
{
  char *val;
  int strsz;

  val = emalloc (CK_MAXMSG);
  
  strsz = upack_int (buf);
  
  strncpy (val, *buf, strsz);
  *buf += strsz;

  return val;
}

static void pack_type (char **buf, enum ck_msg_type type)
{
  pack_int(buf, (int) type);
}

static enum ck_msg_type upack_type (char **buf)
{
  return (enum ck_msg_type) upack_int(buf);
}

  
static void pack_ctx (char **buf, void *msg)
{
  CtxMsg *cmsg = msg;
  pack_int(buf,(int) cmsg->ctx);
}

static void upack_ctx (char **buf, void *msg)
{
  CtxMsg *cmsg = msg;

  cmsg->ctx = upack_int (buf);
  
  return;
}

static void pack_loc (char **buf, void *msg)
{
  LocMsg *lmsg = msg;

  pack_str (buf, lmsg->file);
  pack_int (buf, lmsg->line);

}

static void upack_loc (char **buf, void *msg)
{

  LocMsg *lmsg = msg;

  lmsg->file = upack_str(buf);
  lmsg->line = upack_int(buf);
  return;
}

static void pack_fail (char **buf, void *msg)
{
  FailMsg *fmsg = msg;
  pack_str (buf, fmsg->msg);
}

static void upack_fail (char **buf, void *msg)
{
  FailMsg *fmsg = msg;
  fmsg->msg = upack_str(buf);
}

static void check_type (int type, char *file, int line)
{
  if (type >= CK_MSG_LAST)
    eprintf ("%s:%d:Bad message type arg", file, line);
}

void ppack (int fdes, enum ck_msg_type type, void *data)
{
  char *buf;
  int n;
  ssize_t r;

  buf = emalloc (CK_MAXMSGBUF);
  n = pack(type, buf, data);
  r = write (fdes, buf, n);
  if (r == -1)
    eprintf("Error in ppack:",__FILE__,__LINE__);

  free(buf);
}

static int read_buf (int fdes, char **buf)
{
  char *readloc;
  int n;
  int nread = 0;
  int size = 1;
  int grow = 2;
  
  *buf = emalloc(size);
  readloc = *buf;
  while (1) {
    n = read(fdes,readloc,size - nread);
    if (n == 0)
      break;
    if (n == -1)
      eprintf("Error in read_buf:", __FILE__, __LINE__);

    nread += n;
    size *= grow;
    *buf = erealloc(*buf,size);
    readloc = *buf + nread;
  }
  return nread;
}    


static int get_result (char *buf, RcvMsg *rmsg)
{
  enum ck_msg_type type;
  void *data;
  int n;

  data = emalloc(CK_MAXMSGBUF);
  
  n = upack(buf,data,&type);
  if (n == -1)
    eprintf("Error in upack", __FILE__, __LINE__);
  
  if (type == CK_MSG_CTX) {
    CtxMsg *cmsg = data;
    rcvmsg_update_ctx(rmsg, cmsg->ctx);
  } else if (type == CK_MSG_LOC) {
    LocMsg *lmsg = data;
    rcvmsg_update_loc(rmsg, lmsg->file, lmsg->line);
  } else if (type == CK_MSG_FAIL) {      
    FailMsg *fmsg = data;
    rmsg->msg = emalloc (strlen(fmsg->msg) + 1);
    strcpy(rmsg->msg, fmsg->msg);
  } else
    check_type(type, __FILE__, __LINE__);

  free(data);
  return n;
  
}



static void reset_rcv_test (RcvMsg *rmsg)
{
  rmsg->test_line = -1;
  rmsg->test_file = NULL;
}

static void reset_rcv_fixture (RcvMsg *rmsg)
{
  rmsg->fixture_line = -1;
  rmsg->fixture_file = NULL;
}

static RcvMsg *rcvmsg_create(void)
{
  RcvMsg *rmsg;

  rmsg = emalloc (sizeof (RcvMsg));
  rmsg->lastctx = -1;
  rmsg->msg = NULL;
  reset_rcv_test(rmsg);
  reset_rcv_fixture(rmsg);
  return rmsg;
}

static void rcvmsg_update_ctx(RcvMsg *rmsg, enum ck_result_ctx ctx)
{
  if (rmsg->lastctx != -1)
    reset_rcv_fixture(rmsg);

  rmsg->lastctx = ctx;
}

static void rcvmsg_update_loc (RcvMsg *rmsg, char *file, int line)
{
  int flen = strlen(file);
  
  if (rmsg->lastctx == CK_CTX_TEST) {
    rmsg->test_line = line;
    rmsg->test_file = emalloc(flen + 1);
    strcpy(rmsg->test_file, file);
  } else {
    rmsg->fixture_line = line;
    rmsg->fixture_file = emalloc(flen + 1);
    strcpy(rmsg->fixture_file, file);
  }
}
  
RcvMsg *punpack(int fdes)
{
  int nread, n;
  char *buf;
  char *obuf;
  RcvMsg *rmsg;

  nread = read_buf (fdes, &buf);
  obuf = buf;
  rmsg = rcvmsg_create();
  
  while (nread > 0) {
    n = get_result(buf, rmsg);
    nread -= n;
    buf += n;
  }

  free(obuf);
  if (rmsg->lastctx == -1) {
    free (rmsg);
    rmsg = NULL;
  }

  return rmsg;
}
