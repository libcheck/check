#ifndef LIST_H
#define LIST_H

typedef struct List List;

/* Create an empty list */
List * list_create (void);

/* Is list at end? */
int list_at_end (List * lp);

/* Position list at front */
void list_front(List *lp);

/* Add a value to the front of the list,
   positioning newly added value as current value */
void list_add_front (List *lp, void *val);

/* Add a value to the end of the list,
   positioning newly added value as current value */
void list_add_end (List *lp, void *val);

/* Give the value of the current node */
void *list_val (List * lp);

/* Position the list at the next node */
void list_advance (List * lp);

/* Free a list, but don't free values */
void list_free (List * lp);

/* Free a list, freeing values using a freeing function */
/* void list_vfree (List * lp, void (*fp) (void *)); */

#endif /*LIST_H*/
