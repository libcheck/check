#ifndef CHECK_CHECK_H
#define CHECK_CHECK_H

int sub_nfailed;
int sub_ntests;

void setup (void);
void cleanup (void);
Suite *sub_make_suite(void);
Suite *main_make_suite(void);

#endif /* CHECK_CHECK_H */
