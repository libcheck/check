/* This file gets included if AC_REPLACE_FUNCS([sleep]) cannot find the function. */
unsigned int
sleep (unsigned int seconds)
{
  return 0;
}
