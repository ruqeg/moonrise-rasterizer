#include <MRE_alg.h>

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
  MRE_F64  a;                                                    \
  MRE_F64  d;                                                    \
                                                                 \
  a = ( d1 - d0 ) / ( MRE_F64 )( i1 - i0 );                      \
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
    MRE_Interpolate_I16_I16,
    MRE_I16, MRE_I16
)

MRE_INTERPOLATE_TEMPLATE_IMPLEMENTATION
(
    MRE_Interpolate_I16_F64,
    MRE_I16, MRE_F64
)
#undef MRE_INTERPOLATE_TEMPLATE_IMPLEMENTATION
