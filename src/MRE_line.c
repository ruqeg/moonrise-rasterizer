#include <MRE_line.h>

void
MRE_DrawLine
(
    uint32_t  * buff,
    int16_t     buff_w,
    int16_t     x0,
    int16_t     y0,
    int16_t     x1,
    int16_t     y1,
    uint32_t    color
)
{
  if ( abs( x1 - x0 ) > abs( y1 - y0 ) )
  {
    if ( x0 > x1 )  MRE_SWAP_T_I16( x0, x1,  y0, y1);

    int16_t  x;
    int16_t  y_arr[ x1 - x0 ];
    
    
    MRE_Interpolate_Int16( x0, y0, x1, y1, y_arr );
    for ( x = x0; x <= x1; ++x )
    {
      buff[ buff_w * y_arr[ x - x0 ] + x ] = color;
    }
  }
  else
  {
    if ( y0 > y1 )  MRE_SWAP_T_I16( x0, x1,  y0, y1);
    
    int16_t  y;
    int16_t  x_arr[ y1 - y0 ];
    
    
    MRE_Interpolate_Int16( y0, x0, y1, x1, x_arr );
    for ( y = y0; y <= y1; ++y )
    {
      buff[ buff_w * y + x_arr[ y - y0 ] ] = color;
    }
  }
}
