#include "config.h"
#include "../lib/libcompat.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif

Suite *s;
TCase *tc;
SRunner *sr;

static void *sendinfo_thread(void *userdata CK_ATTRIBUTE_UNUSED)
{
	unsigned int i;
	for (i=0; i < 100; i++) {
  		fail_unless(1,"Shouldn't see this message");
	}

	return NULL;
}


static void *sendinfo_fail_thread(void *userdata CK_ATTRIBUTE_UNUSED)
{
	unsigned int i;
	for (i=0; i < 100; i++) {
		fail("This test fails");
	}

	return NULL;
}

START_TEST(test_pass)
{
#ifdef HAVE_PTHREAD
	pthread_t a, b;
	pthread_create(&a, NULL, sendinfo_thread, (void *) 0xa);
	pthread_create(&b, NULL, sendinfo_thread, (void *) 0xb);

	pthread_join(a, NULL);
	pthread_join(b, NULL);
#else
	sendinfo_thread((void *) 0xa);
	sendinfo_thread((void *) 0xb);
#endif
}
END_TEST

START_TEST(test_fail)
{
#ifdef HAVE_PTHREAD
	pthread_t a, b;
	pthread_create(&a, NULL, sendinfo_fail_thread, (void *) 0xa);
	pthread_create(&b, NULL, sendinfo_fail_thread, (void *) 0xb);

	pthread_join(a, NULL);
	pthread_join(b, NULL);
#else
	sendinfo_fail_thread((void *) 0xa);
	sendinfo_fail_thread((void *) 0xb);
#endif
}
END_TEST

static void run (int num_iters)
{
  int i;
  s = suite_create ("Stress");
  tc = tcase_create ("Stress");
  sr = srunner_create (s);
  suite_add_tcase(s, tc);

  for (i = 0; i < num_iters; i++) {
    tcase_add_test (tc, test_pass);
    tcase_add_test (tc, test_fail);
  }

  srunner_run_all(sr, CK_SILENT);
  if (srunner_ntests_failed (sr) != num_iters) {
    printf ("Error: expected %d failures, got %d\n",
	    num_iters, srunner_ntests_failed(sr));
    return;
  }

  srunner_free(sr);
}

  
int main(void)
{
  int i;
  time_t t1;
  int iters[] = {1, 100, 1000, -1};

  for (i = 0; iters[i] != -1; i++) {
    t1 = time(NULL);
    run(iters[i]);
    printf ("%d, %d\n", iters[i], (int) difftime(time(NULL), t1));
  }
  return 0;
}
