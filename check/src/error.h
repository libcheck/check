#ifndef ERROR_H
#define ERROR_H

/* Include stdlib.h beforehand */

/* Print error message and die
   If fmt ends in colon, include system error information */
void eprintf (char *fmt, ...);
/* malloc or die */
void *emalloc(size_t n);
void *erealloc(void *, size_t n);

#endif /*ERROR_H*/
