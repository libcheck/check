#ifndef CHECK_PACK_H
#define CHECK_PACK_H

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

enum ck_msg_type {
  CK_MSG_CTX,
  CK_MSG_FAIL,
  CK_MSG_LOC,
  CK_MSG_LAST
};


typedef struct CtxMsg
{
  enum ck_result_ctx ctx;
} CtxMsg;

typedef struct LocMsg 
{
  int line;
  char *file;
} LocMsg;

typedef struct FailMsg
{
  char *msg;
} FailMsg;

  
int pack (enum ck_msg_type type, char *buf, void *data);
int upack (char *buf, void *data, enum ck_msg_type *type);

void ppack (int fdes, enum ck_msg_type type, void *data);
TestResult *punpack(int fdes);


#endif /*CHECK_PACK_H */
