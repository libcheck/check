#ifndef CHECK_MSG_H
#define CHECK_MSG_H

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

/* Functions implementing messaging during test runs */
/* check.h must be included before this header */

/* Abstract type for a messaging system
   Hides the details (IPC msg vs. pipes vs...*/
typedef struct MsgSys MsgSys;

typedef struct loc 
{
  int line;
  char *file;
} Loc;

void send_failure_msg (MsgSys *msgsys, char *fmsg);
void send_last_loc_msg (MsgSys *msgsys, char * file, int line);

/* malloc'd return value which caller is responsible for
   freeing in each of the next two functions */
char *receive_failure_msg (MsgSys *msgsys);
Loc *receive_last_loc_msg (MsgSys *msgsys);

MsgSys *init_msgsys (void);
void delete_msgsys (void);

MsgSys *get_recv_msgsys (void);
MsgSys *get_send_msgsys (void);

/* Used externally only for testing */
MsgSys *create_msgsys_with_key(int key);
void delete_msgsys_with_key(int key);
MsgSys *get_msgsys_with_key(int key);

#endif /*CHECK_MSG_H */
