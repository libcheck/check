#ifndef CHECK_IMPL_H
#define CHECK_IMPL_H

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

/* This header should be included by any module that needs
   to know the implementation details of the check structures
   Include list.h before this header
*/

/* magic values */

enum {
  MAXLINE = 9999 /* maximum line no */
};

typedef struct TF {
  TFun fn;
  char *name;
} TF;

struct Suite {
  char *name;
  List *tclst; /* List of test cases */
};

struct TCase {
  char *name;
  List *tflst; /* list of test functions */
  SFun setup;
  SFun teardown;
};


#endif /* CHECK_IMPL_H */
