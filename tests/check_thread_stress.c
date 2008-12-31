#include "config.h"
#include "../lib/libcompat.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

Suite *s;
TCase *tc;
SRunner *sr;

static void *
sendinfo (void *userdata CK_ATTRIBUTE_UNUSED)
{
  unsigned int i;
  for (i = 0; i < 999; i++)
    {
      fail_unless (1, "Shouldn't see this message");
    }

  return NULL;
}

START_TEST (test_stress_threads)
{
#ifdef HAVE_PTHREAD
  pthread_t a, b;
  pthread_create (&a, NULL, sendinfo, (void *) 0xa);
  pthread_create (&b, NULL, sendinfo, (void *) 0xb);

  pthread_join (a, NULL);
  pthread_join (b, NULL);
#else
  sendinfo ((void *) 0xa);
  sendinfo ((void *) 0xb);
#endif
}
END_TEST

START_TEST (test_stress_forks)
{
  pid_t cpid = fork ();
  if (cpid == 0)
    {				
      /* child */
      sendinfo ((void *) 0x1);
    }
  else
    {
      sendinfo ((void *) 0x2);
    }
}
END_TEST 

int
main (void)
{
  int nf;
  s = suite_create ("ForkThreadStress");
  tc = tcase_create ("ForkThreadStress");
  sr = srunner_create (s);
  suite_add_tcase (s, tc);

  tcase_add_loop_test (tc, test_stress_threads, 0, 100);
  tcase_add_loop_test (tc, test_stress_forks, 0, 100);

  srunner_run_all (sr, CK_SILENT);
  nf = srunner_ntests_failed (sr);
  srunner_free (sr);
  return nf ? EXIT_FAILURE : EXIT_SUCCESS;
}
