#ifndef CHECK_MSG_NEW_H
#define CHECK_MSG_NEW_H

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

void send_failure_info (int key, char *msg);
void send_loc_info (int key, char *file, int line);
void send_ctx_info (int key,enum ck_result_ctx ctx);

TestResult *receive_test_result (int key);
TestResult *new_receive_test_result (int key, int waserror);

void setup_messaging(void);
int get_send_key(void);
int get_recv_key(void);
void teardown_messaging(void);

/* for testing only */
void setup_test_messaging(void);
int get_test_key(void);
void teardown_test_messaging(void);

#endif /*CHECK_MSG_NEW_H */
