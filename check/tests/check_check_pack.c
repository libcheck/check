#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <check.h>
#include "check_magic.h"
#include "check_pack.h"
#include "error.h"
#include "check_check.h"

START_TEST(test_pack_fmsg)
{
  FailMsg *fmsg;
  char *buf;
  enum ck_msg_type type;

  fmsg = emalloc (sizeof(FailMsg));
  buf = emalloc (CK_MAXMSGBUF);
  fmsg->msg = "Hello, world!";

  pack (CK_MSG_FAIL, buf, fmsg);
  fmsg->msg = "";
  upack (buf, fmsg, &type);

  fail_unless (type == CK_MSG_FAIL,
	       "Bad type unpacked for FailMsg");

  if (strcmp (fmsg->msg, "Hello, world!") != 0) {
    char *msg = emalloc(CK_MAXMSG);
    snprintf (msg,CK_MAXMSG,
	      "Unpacked string is %s, should be Hello, World!",
	      fmsg->msg);
    
    fail(msg);
  }

  free(fmsg->msg);
  free(fmsg);
  free(buf);
}
END_TEST

START_TEST(test_pack_loc)
{
  LocMsg *lmsg;
  char *buf;
  enum ck_msg_type type;

  lmsg = emalloc (sizeof(LocMsg));
  buf = emalloc (CK_MAXMSGBUF);
  lmsg->file = "abc123.c";
  lmsg->line = 125;

  pack (CK_MSG_LOC, buf, lmsg);
  lmsg->file = "";
  lmsg->line = 0;
  upack (buf, lmsg, &type);

  fail_unless (type == CK_MSG_LOC,
	       "Bad type unpacked for LocMsg");

  if (lmsg->line != 125) {
    char *errm = emalloc (CK_MAXMSG);
    snprintf(errm, CK_MAXMSG,
	     "LocMsg line was %d, should be %d",
	     lmsg->line, 125);
    fail (errm);
  }
  
  if (strcmp (lmsg->file, "abc123.c") != 0) {
    char *errm = emalloc (CK_MAXMSG);
    snprintf(errm, CK_MAXMSG,
	     "LocMsg file was %s, should be abc123.c",
	     lmsg->file);
    fail (errm);
  }

  free (lmsg->file);
  free (lmsg);
  free (buf);
}
END_TEST

START_TEST(test_pack_ctx)
{
  CtxMsg cmsg;
  char *buf;
  enum ck_msg_type type;
  int npk, nupk;

  buf = emalloc (CK_MAXMSGBUF);
  cmsg.ctx = CK_CTX_SETUP;

  npk = pack (CK_MSG_CTX, buf, &cmsg);
  cmsg.ctx = CK_CTX_TEARDOWN;
  nupk = upack (buf, &cmsg, &type);

  fail_unless (type == CK_MSG_CTX,
	       "Bad type unpacked for CtxMsg");

  if (cmsg.ctx != CK_CTX_SETUP) {
    char *errm = emalloc (CK_MAXMSG);
    snprintf(errm, CK_MAXMSG,
	     "CtxMsg ctx got %d, expected %d",
	     cmsg.ctx, CK_CTX_SETUP);
    fail (errm);
  }

  free (buf);
    
}
END_TEST


START_TEST(test_pack_len)
{
  CtxMsg cmsg;
  char *buf;
  int n = 0;
  enum ck_msg_type type;

  buf = emalloc (CK_MAXMSGBUF);
  cmsg.ctx = CK_CTX_TEST;
  n = pack(CK_MSG_CTX,buf,&cmsg);
  fail_unless (n > 0, "Return val from pack not set correctly");

  /* Value below may change with different implementations of pack */
  fail_unless (n == 4, "Return val from pack not correct");
  n = 0;
  n = upack(buf,&cmsg,&type);
  if (n != 4) {
    char *msg = emalloc (CK_MAXMSG);
    snprintf(msg,CK_MAXMSG, "%d bytes read from upack, should be 4",n);
    fail (msg);
  }
  
  free(buf);
}
END_TEST

START_TEST(test_pack_ctx_limit)
{
  CtxMsg cmsg;
  CtxMsg *cmsgp = NULL;
  char *buf;

  cmsg.ctx = -1;
  buf = emalloc (CK_MAXMSGBUF);
  pack(CK_MSG_CTX,buf,&cmsg);
  pack(CK_MSG_CTX,buf,cmsgp);
  
}
END_TEST

START_TEST(test_pack_fail_limit)
{
  FailMsg fmsg;
  FailMsg *fmsgp = NULL;
  char *buf;
  enum ck_msg_type type;

  buf = emalloc (CK_MAXMSGBUF);
  fmsg.msg = "";
  pack(CK_MSG_FAIL,buf,&fmsg);
  fmsg.msg = "abc";
  upack(buf,&fmsg,&type);
  fail_unless (strcmp(fmsg.msg, "") == 0, "Empty string not handled properly");
  free(fmsg.msg);
  fmsg.msg = NULL;
  pack(CK_MSG_FAIL,buf,&fmsg);
  pack(CK_MSG_FAIL,buf,fmsgp);
}
END_TEST

START_TEST(test_pack_loc_limit)
{
  LocMsg lmsg;
  LocMsg *lmsgp = NULL;
  char *buf;
  enum ck_msg_type type;

  buf = emalloc (CK_MAXMSGBUF);
  lmsg.file = "";
  lmsg.line = 0;
  pack(CK_MSG_LOC,buf,&lmsg);
  lmsg.file = "abc";
  upack(buf,&lmsg,&type);
  fail_unless (strcmp(lmsg.file, "") == 0,
	       "Empty string not handled properly");
  free(lmsg.file);
  lmsg.file = NULL;
  pack(CK_MSG_LOC,buf,&lmsg);
  pack(CK_MSG_LOC,buf,lmsgp);
}
END_TEST

START_TEST(test_ppack)
{
  int filedes[2];
  CtxMsg cmsg;
  LocMsg lmsg;
  FailMsg fmsg;
  RcvMsg *rmsg;

  cmsg.ctx = CK_CTX_TEST;
  lmsg.file = "abc123.c";
  lmsg.line = 10;
  fmsg.msg = "oops";
  pipe(filedes);
  ppack(filedes[1],CK_MSG_CTX, &cmsg);
  ppack(filedes[1],CK_MSG_LOC, &lmsg);
  ppack(filedes[1],CK_MSG_FAIL, &fmsg);
  close(filedes[1]);
  rmsg = punpack(filedes[0]);

  fail_unless (rmsg != NULL,
	       "Return value from ppack should always be malloc'ed");
  fail_unless (rmsg->lastctx == CK_CTX_TEST,
	       "CTX not set correctly in ppack");
  fail_unless (rmsg->fixture_line == -1,
	       "Default fixture loc not correct");
  fail_unless (rmsg->fixture_file == NULL,
	       "Default fixture loc not correct");
  fail_unless (rmsg->test_line == 10,
	       "Test line not received correctly");
  fail_unless (strcmp(rmsg->test_file,"abc123.c") == 0,
	       "Test file not received correctly");
  fail_unless (strcmp(rmsg->msg, "oops") == 0,
	       "Failure message not received correctly");
  
  free(rmsg);
}
END_TEST

START_TEST(test_ppack_noctx)
{
  int filedes[2];
  LocMsg lmsg;
  FailMsg fmsg;
  RcvMsg *rmsg;

  lmsg.file = "abc123.c";
  lmsg.line = 10;
  fmsg.msg = "oops";
  pipe(filedes);
  ppack(filedes[1],CK_MSG_LOC, &lmsg);
  ppack(filedes[1],CK_MSG_FAIL, &fmsg);
  close(filedes[1]);
  rmsg = punpack(filedes[0]);

  fail_unless (rmsg == NULL,
	       "Result should be NULL with no CTX");

  if (rmsg != NULL)
    free(rmsg);
}
END_TEST

START_TEST(test_ppack_onlyctx)
{
  int filedes[2];
  CtxMsg cmsg;
  RcvMsg *rmsg;

  cmsg.ctx = CK_CTX_SETUP;
  pipe(filedes);
  ppack(filedes[1],CK_MSG_CTX, &cmsg);
  close(filedes[1]);
  rmsg = punpack(filedes[0]);

  fail_unless (rmsg->msg == NULL,
	       "Result message should be NULL with only CTX");
  fail_unless (rmsg->fixture_line == -1,
	       "Result loc line should be -1 with only CTX");
  fail_unless (rmsg->test_line == -1,
	       "Result loc line should be -1 with only CTX");

  if (rmsg != NULL)
    free(rmsg);
}
END_TEST

START_TEST(test_ppack_multictx)
{
  int filedes[2];
  CtxMsg cmsg;
  LocMsg lmsg;
  RcvMsg *rmsg;

  cmsg.ctx = CK_CTX_SETUP;
  lmsg.line = 5;
  lmsg.file = "abc123.c";
  pipe(filedes);
  ppack(filedes[1],CK_MSG_CTX, &cmsg);
  ppack(filedes[1],CK_MSG_LOC, &lmsg);
  cmsg.ctx = CK_CTX_TEST;
  ppack(filedes[1],CK_MSG_CTX, &cmsg);
  ppack(filedes[1],CK_MSG_LOC, &lmsg);
  cmsg.ctx = CK_CTX_TEARDOWN;
  ppack(filedes[1],CK_MSG_CTX, &cmsg);
  close(filedes[1]);
  rmsg = punpack(filedes[0]);

  fail_unless (rmsg->test_line == 5,
	       "Test loc not being preserved on CTX change");

  fail_unless (rmsg->fixture_line == -1,
	       "Fixture not reset on CTX change");
  if (rmsg != NULL)
    free(rmsg);
}
END_TEST

START_TEST(test_ppack_nofail)
{
  int filedes[2];
  CtxMsg cmsg;
  LocMsg lmsg;
  RcvMsg *rmsg;

  lmsg.file = "abc123.c";
  lmsg.line = 10;
  cmsg.ctx = CK_CTX_SETUP;
  pipe(filedes);
  ppack(filedes[1],CK_MSG_CTX, &cmsg);
  ppack(filedes[1],CK_MSG_LOC, &lmsg);
  close(filedes[1]);
  rmsg = punpack(filedes[0]);

  fail_unless (rmsg->msg == NULL,
	       "Failure result should be NULL with no failure message");
  if (rmsg != NULL)
    free(rmsg);
}
END_TEST

START_TEST(test_ppack_big)
{
  int filedes[2];
  CtxMsg cmsg;
  LocMsg lmsg;
  FailMsg fmsg;
  RcvMsg *rmsg;

  cmsg.ctx = CK_CTX_TEST;
  lmsg.file = emalloc(CK_MAXMSG);
  memset(lmsg.file,'a',CK_MAXMSG - 1);
  lmsg.file[CK_MAXMSG - 1] = '\0';
  lmsg.line = 10;
  fmsg.msg = emalloc(CK_MAXMSG);
  memset(fmsg.msg,'a',CK_MAXMSG - 1);
  fmsg.msg[CK_MAXMSG - 1] = '\0';
  pipe(filedes);
  ppack(filedes[1],CK_MSG_CTX, &cmsg);
  ppack(filedes[1],CK_MSG_LOC, &lmsg);
  ppack(filedes[1],CK_MSG_FAIL, &fmsg);
  close(filedes[1]);
  rmsg = punpack(filedes[0]);

  fail_unless (rmsg != NULL,
	       "Return value from ppack should always be malloc'ed");
  fail_unless (rmsg->lastctx == CK_CTX_TEST,
	       "CTX not set correctly in ppack");
  fail_unless (rmsg->test_line == 10,
	       "Test line not received correctly");
  fail_unless (strcmp(rmsg->test_file,lmsg.file) == 0,
	       "Test file not received correctly");
  fail_unless (strcmp(rmsg->msg, fmsg.msg) == 0,
	       "Failure message not received correctly");
  
  free(rmsg);
}
END_TEST

Suite *make_pack_suite(void)
{

  Suite *s;
  TCase *tc_core;
  TCase *tc_limit;

  s = suite_create("Pack");
  tc_core = tcase_create("Core");
  tc_limit = tcase_create("Limit");

  suite_add_tcase(s, tc_core);
  tcase_add_test(tc_core, test_pack_fmsg);
  tcase_add_test(tc_core, test_pack_loc);
  tcase_add_test(tc_core, test_pack_ctx);
  tcase_add_test(tc_core, test_pack_len);
  tcase_add_test(tc_core, test_ppack);
  tcase_add_test(tc_core, test_ppack_noctx);
  tcase_add_test(tc_core, test_ppack_onlyctx);
  tcase_add_test(tc_core, test_ppack_multictx);
  tcase_add_test(tc_core, test_ppack_nofail);
  suite_add_tcase(s, tc_limit);
  tcase_add_test(tc_limit, test_pack_ctx_limit);
  tcase_add_test(tc_limit, test_pack_fail_limit);
  tcase_add_test(tc_limit, test_pack_loc_limit);
  tcase_add_test(tc_limit, test_ppack_big);

  return s;
}
