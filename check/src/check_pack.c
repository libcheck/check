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

#include "error.h"
#include "check_pack.h"
#include "check_magic.h"


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
  
  if (buf == NULL)
    return -1;
  if (data == NULL)
    return 0;
  obuf = buf;
  
  check_type(type, __FILE__, __LINE__);

  pack_type(&buf, type);
  
  pftab[type] (&buf, data);

  return buf - obuf;
}

enum ck_msg_type upack (char *buf, void *data)
{
  enum ck_msg_type type;

  type = upack_type (&buf);

  check_type(type, __FILE__, __LINE__);
  
  upftab[type] (&buf, data);

  return type;
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

