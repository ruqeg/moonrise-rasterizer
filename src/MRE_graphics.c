#include <MRE_graphics.h>

void
MRE_DrawLine
(
    int16_t   x0,
    int16_t   y0,
    int16_t   x1,
    int16_t   y1,
    uint32_t  color
)
{
  int16_t    i;
  int16_t  * ds;


  
  if ( abs( x1 - x0 ) > abs( y1 - y0 ) )
  {
    if ( x0 > x1 )  MRE_SWAP_T_I16( x0, x1,  y0, y1);

    ds = alloca( ( x1 - x0 ) * 16 );
    
    MRE_Interpolate_Int16( ds, x0, y0, x1, y1 );
    for ( i = x0; i <= x1; ++i )
    {
      MRE_buff[ MRE_buff_w * ds[ i - x0 ] + i ] = color;
    }
  }
  else
  {
    if ( y0 > y1 )  MRE_SWAP_T_I16( x0, x1,  y0, y1);
    
    ds = alloca( ( y1 - y0 ) * 16 );
    
    
    MRE_Interpolate_Int16( ds, y0, x0, y1, x1 );
    for ( i = y0; i <= y1; ++i )
    {
      MRE_buff[ MRE_buff_w * i + ds[ i - y0 ] ] = color;
    }
  }
}


void
MRE_DrawWireframeTriangle
(
    int16_t   x0,
    int16_t   y0,
    int16_t   x1,
    int16_t   y1,
    int16_t   x2,
    int16_t   y2,
    uint32_t  color
)
{
  MRE_DrawLine( x0, y0, x1, y1, color );
  MRE_DrawLine( x1, y1, x2, y2, color );
  MRE_DrawLine( x2, y2, x0, y0, color );
}


void
MRE_DrawFilledTriangle
(
    int16_t   x0,
    int16_t   y0,
    int16_t   x1,
    int16_t   y1,
    int16_t   x2,
    int16_t   y2,
    uint32_t  color
)
{
  int16_t    m;
  int16_t    y;
  int16_t    x;
  int16_t  * xsl;
  int16_t  * xsr;
  int16_t  * xs012;
  int16_t  * xs02;

  

  if ( y1 < y0 )  MRE_SWAP_T_I16( x0, x1,  y0, y1);
  if ( y2 < y0 )  MRE_SWAP_T_I16( x0, x2,  y0, y2);
  if ( y2 < y1 )  MRE_SWAP_T_I16( x1, x2,  y1, y2);

  xs012 = alloca( ( y2 - y0 ) * 16 );
  xs02 = alloca( ( y2 - y0 ) * 16 );

  MRE_Interpolate_Int16( xs012, y0, x0, y1, x1 );
  MRE_Interpolate_Int16( xs012 + ( y1 - y0 ), y1, x1, y2, x2 );
  MRE_Interpolate_Int16( xs02, y0, x0, y2, x2 );

  m = ( y2 - y0 ) / 2;

  if ( xs02[ m ] < xs012[ m ] )
  {
    xsl = xs02;
    xsr = xs012;
  }
  else
  {
    xsl = xs012;
    xsr = xs02;
  }

  for ( y = y0; y <= y2; ++y )
  {
    for ( x = xsl[ y - y0 ]; x <= xsr[ y - y0 ]; ++x )
    {
      MRE_buff[ MRE_buff_w * y + x ] = color;
    }
  }
}
