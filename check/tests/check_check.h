#ifndef CHECK_CHECK_H
#define CHECK_CHECK_H

int sub_nfailed;
int sub_ntests;

void setup (void);
void cleanup (void);
Suite *make_sub_suite(void);
Suite *make_sub2_suite(void);
Suite *make_master_suite(void);
Suite *make_list_suite(void);
Suite *make_msg_suite(void);
Suite *make_log_suite(void);
Suite *make_limit_suite(void);
Suite *make_fork_suite(void);

#endif /* CHECK_CHECK_H */
