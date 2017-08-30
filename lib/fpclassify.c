/* Replacement function for isnan/isinf/isfinite for processors using
 * IEEE 754 floating point format.
 * Copyright (C) 2017, bel2125
 * License: LGPL v2 or MIT (as you like).
 */


#include "libcompat.h"

double DOUBLE_ZERO = 0.0;

#if defined(NEED_fpclassify)

#if defined(HAVE_STDINT_H)
#include <stdint.h>
typedef uint64_t bitfield64;
#elif defined(_MSC_VER)
typedef unsigned __int64 bitfield64;
#else
typedef unsigned long long bitfield64;
#endif

static bitfield64 ms = 0x8000000000000000;
static bitfield64 me = 0x7FF0000000000000;
static bitfield64 mf = 0x000FFFFFFFFFFFFF;

int fpclassify(double d) 
{
  bitfield64 *p = (bitfield64 *)&d;
  if ((*p & me) != me) {
    /* finite */
    if (*p & mf) {
      /* finite and not null */
      if (*p & me) {
        return FP_NORMAL;
      }
      return FP_SUBNORMAL;
    }	
    return FP_ZERO;
  }
  if (*p & mf) {
    return FP_NAN;
  }
  return FP_INFINITE;
}

#endif

