#include <MRE_system.h>

#define MRE_INTERPOLATE_TEMPLATE_IMPLEMENTATION\
( FUNCTION_NAME, TYPE_A, TYPE_B )                                \
void                                                             \
FUNCTION_NAME                                                    \
(                                                                \
  TYPE_B  * ds,                                                  \
  TYPE_A    i0,                                                  \
  TYPE_B    d0,                                                  \
  TYPE_A    i1,                                                  \
  TYPE_B    d1                                                   \
)                                                                \
{                                                                \
  TYPE_A   i;                                                    \
  double   a;                                                    \
  double   d;                                                    \
                                                                 \
  a = ( d1 - d0 ) / ( double )( i1 - i0 );                       \
  d = d0;                                                        \
                                                                 \
  for ( i = i0; i <= i1; ++i )                                   \
  {                                                              \
    ds[ i - i0 ] = d;                                            \
    d = d + a;                                                   \
  }                                                              \
}

MRE_INTERPOLATE_TEMPLATE_IMPLEMENTATION
(
  MRE_Interpolate_Int16Int16,
  int16_t, int16_t 
)
MRE_INTERPOLATE_TEMPLATE_IMPLEMENTATION
(
  MRE_Interpolate_Int16Double, 
  int16_t, double
)
