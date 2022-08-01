#include <MRE_system.h>

void
MRE_Interpolate_Int16
(
  int16_t    i0,
  int16_t    d0,
  int16_t    i1,
  int16_t    d1,
  int16_t  * d_arr
)
{
  double  d;
  double  i;
  double  a;

  a = ( d1 - d0 ) / ( double )( i1 - i0 );
  d = d0;

  for ( i = i0; i <= i1; ++i )
  {
    *( d_arr++ ) = d;
    d = d + a;
  }
}
