#include <stdlib.h>
#include "list.h"
#include "error.h"

enum {
  LINIT = 1,
  LGROW = 2
};

struct List {
  int n_elts;
  int max_elts;
  int current; /* pointer to the current node */
  int last; /* pointer to the node before END */
  void **data;
};

static void maybe_grow (List *lp)
{
  if (lp->n_elts >= lp->max_elts) {
    lp->max_elts *= LGROW;
    lp->data = erealloc (lp->data, lp->max_elts * sizeof(lp->data[0]));
  }
}

List *list_create (void)
{
  List *lp;
  lp = emalloc (sizeof(List));
  lp->n_elts = 0;
  lp->max_elts = LINIT;
  lp->data = emalloc(sizeof(lp->data[0]) * LINIT);
  lp->current = lp->last = -1;
  return lp;
}

void list_add_end (List *lp, void *val)
{
  if (lp == NULL)
    return;
  maybe_grow(lp);
  lp->last++;
  lp->n_elts++;
  lp->current = lp->last;
  lp->data[lp->current] = val;
}

int list_at_end (List *lp)
{
  if (lp->current == -1)
    return 1;
  else
    return (lp->current > lp->last);
}

void list_front (List *lp)
{
  if (lp->current == -1)
    return;
  lp->current = 0;
}


void list_free (List *lp)
{
  if (lp == NULL)
    return;
  
  free(lp->data);
  free (lp);
}

void *list_val (List *lp)
{
  if (lp == NULL)
    return NULL;
  if (lp->current == -1 || lp->current > lp->last)
    return NULL;
  
  return lp->data[lp->current];
}

void list_advance (List *lp)
{
  if (lp == NULL)
    return;
  if (list_at_end(lp))
    return;
  lp->current++;
}



  
