#ifndef CHECK_IMPL_H
#define CHECK_IMPL_H

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
