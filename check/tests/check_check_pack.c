#include <stdlib.h>
#include <stdio.h>

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
  type = upack (buf, fmsg);

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
  type = upack (buf, lmsg);

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
  CtxMsg *cmsg;
  char *buf;
  enum ck_msg_type type;

  cmsg = emalloc (sizeof(CtxMsg));
  buf = emalloc (CK_MAXMSGBUF);
  cmsg->ctx = CK_CTX_SETUP;

  pack (CK_MSG_CTX, buf, cmsg);
  cmsg->ctx = CK_CTX_TEARDOWN;
  type = upack (buf, cmsg);

  fail_unless (type == CK_MSG_CTX,
	       "Bad type unpacked for CtxMsg");

  if (cmsg->ctx != CK_CTX_SETUP) {
    char *errm = emalloc (CK_MAXMSG);
    snprintf(errm, CK_MAXMSG,
	     "CtxMsg ctx got %d, expected %d",
	     cmsg->ctx, CK_CTX_SETUP);
    fail (errm);
  }

  free (cmsg);
  free (buf);
    
}
END_TEST

START_TEST(test_pack_len)
{
  CtxMsg cmsg;
  char *buf;
  int n = 0;

  buf = emalloc (CK_MAXMSGBUF);
  cmsg.ctx = CK_CTX_TEST;
  n = pack(CK_MSG_CTX,buf,&cmsg);
  fail_unless (n > 0, "Return val from pack not set correctly");

  /* Value below may change with different implementations of pack */
  fail_unless (n == 4, "Return val from pack not correct");

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

  buf = emalloc (CK_MAXMSGBUF);
  fmsg.msg = "";
  pack(CK_MSG_FAIL,buf,&fmsg);
  (void) upack(buf,&fmsg);
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

  buf = emalloc (CK_MAXMSGBUF);
  lmsg.file = "";
  lmsg.line = 0;
  pack(CK_MSG_LOC,buf,&lmsg);
  (void) upack(buf,&lmsg);
  fail_unless (strcmp(lmsg.file, "") == 0,
	       "Empty string not handled properly");
  free(lmsg.file);
  lmsg.file = NULL;
  pack(CK_MSG_LOC,buf,&lmsg);
  pack(CK_MSG_LOC,buf,lmsgp);
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
  suite_add_tcase(s, tc_limit);
  tcase_add_test(tc_limit, test_pack_ctx_limit);
  tcase_add_test(tc_limit, test_pack_fail_limit);
  tcase_add_test(tc_limit, test_pack_loc_limit);

  return s;
}
