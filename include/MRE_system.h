#ifndef MRE_SYSTEM_H
#define MRE_SYSTEM_H

#include <stdint.h>

extern void
MRE_Interpolate_Int16
(
  int16_t    i0,
  int16_t    d0,
  int16_t    i1,
  int16_t    d1,
  int16_t  * d_arr
);


#define MRE_SWAP_I16( x1, x2 )\
(               \
  {             \
  int16_t tmp;  \
                \
  tmp = x1;     \
  x1 = x2;      \
  x2 = tmp;     \
  }             \
)

#define MRE_SWAP_T_I16( x1, x2, y1, y2 )\
(               \
  {             \
  int16_t tmp;  \
                \
  tmp = x1;     \
  x1 = x2;      \
  x2 = tmp;     \
  tmp = y1;     \
  y1 = y2;      \
  y2 = tmp;     \
  }             \
)

#endif
