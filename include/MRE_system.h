#ifndef MRE_SYSTEM_H
#define MRE_SYSTEM_H

#include <stdint.h>

#define MRE_INTEROPLATE_TEMPLATE_DECLARATION\
( FUNCTION_NAME, TYPE_A, TYPE_B )           \
extern void                                 \
FUNCTION_NAME                               \
(                                           \
  TYPE_B  * ds,                             \
  TYPE_A    i0,                             \
  TYPE_B    d0,                             \
  TYPE_A    i1,                             \
  TYPE_B    d1                              \
);                                          \

MRE_INTEROPLATE_TEMPLATE_DECLARATION
(
  MRE_Interpolate_Int16Int16,
  int16_t,
  int16_t
)

MRE_INTEROPLATE_TEMPLATE_DECLARATION
(
  MRE_Interpolate_Int16Double,
  int16_t,
  double
)

#undef MRE_INTEROPLATE_TEMPLATE_DECLARATION

#include <MRE_system.ini>

#endif
