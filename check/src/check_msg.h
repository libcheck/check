#ifndef CHECK_MSG_H
#define CHECK_MSG_H

/* Functions implementing messaging during test runs */
/* check.h must be included before this header */


typedef struct LastLocMsg {
  long int message_type;
  char msg[CMAXMSG]; /* Format: filename\nlineno\0 */
} LastLocMsg;

typedef struct FailureMsg {
  long int message_type;
  char msg[CMAXMSG];
} FailureMsg;

int create_msq (void);
void delete_msq (int msqid);

void send_failure_msg (int msqid, char *msg);
void send_last_loc_msg (int msqid, char * file, int line);

/* malloc'd return value which caller is responsible for
   freeing in each of the next two functions */
FailureMsg *receive_failure_msg (int msqid);
LastLocMsg *receive_last_loc_msg (int msqid);

/* file name contained in the LastLocMsg */
/* return value is malloc'd, caller responsible for freeing */
char *last_loc_file(LastLocMsg *msg);
int last_loc_line(LastLocMsg *msg);

#endif /*CHECK_MSG_H */
