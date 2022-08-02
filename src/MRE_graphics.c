#include <MRE_graphics.h>

void
MRE_DrawLine
(
    int16_t    x0,
    int16_t    y0,
    int16_t    x1,
    int16_t    y1,
    MRE_Pixel  pixel
)
{
  int16_t    i;
  int16_t  * ds;


  
  if ( abs( x1 - x0 ) > abs( y1 - y0 ) )
  {
    if ( x0 > x1 )  MRE_SWAP_T( int16_t, x0, x1,  y0, y1);

    ds = alloca( ( x1 - x0 + 1 ) * sizeof( int16_t ) );
    
    MRE_Interpolate_Int16Int16( ds, x0, y0, x1, y1 );
    for ( i = x0; i <= x1; ++i )
    {
      MRE_buff[ MRE_buff_w * ds[ i - x0 ] + i ] = pixel;
    }
  }
  else
  {
    if ( y0 > y1 )  MRE_SWAP_T( int16_t, x0, x1,  y0, y1);
    
    ds = alloca( ( y1 - y0 + 1 ) * sizeof( int16_t ) );
    
    
    MRE_Interpolate_Int16Int16( ds, y0, x0, y1, x1 );
    for ( i = y0; i <= y1; ++i )
    {
      MRE_buff[ MRE_buff_w * i + ds[ i - y0 ] ] = pixel;
    }
  }
}

void
MRE_DrawWireframeTriangle
(
    int16_t    x0,
    int16_t    y0,
    int16_t    x1,
    int16_t    y1,
    int16_t    x2,
    int16_t    y2,
    MRE_Pixel  pixel
)
{
  MRE_DrawLine( x0, y0, x1, y1, pixel );
  MRE_DrawLine( x1, y1, x2, y2, pixel );
  MRE_DrawLine( x2, y2, x0, y0, pixel );
}

void
MRE_DrawFilledTriangle
(
    int16_t    x0,
    int16_t    y0,
    int16_t    x1,
    int16_t    y1,
    int16_t    x2,
    int16_t    y2,
    MRE_Pixel  pixel
)
{
  int16_t    m;
  int16_t    y;
  int16_t    x;
  int16_t  * xsl;
  int16_t  * xsr;
  int16_t  * xs012;
  int16_t  * xs02;

  

  if ( y1 < y0 )  MRE_SWAP_T( int16_t, x0, x1,  y0, y1);
  if ( y2 < y0 )  MRE_SWAP_T( int16_t, x0, x2,  y0, y2);
  if ( y2 < y1 )  MRE_SWAP_T( int16_t, x1, x2,  y1, y2);

  xs012 = alloca( ( y2 - y0 + 1 ) * sizeof( int16_t ) );
  xs02 = alloca( ( y2 - y0 + 1 ) * sizeof( int16_t ) );

  MRE_Interpolate_Int16Int16( xs012, y0, x0, y1, x1 );
  MRE_Interpolate_Int16Int16( xs012 + ( y1 - y0 ), y1, x1, y2, x2 );
  MRE_Interpolate_Int16Int16( xs02, y0, x0, y2, x2 );

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
      MRE_buff[ MRE_buff_w * y + x ] = pixel;
    }
  }
}

void
MRE_DrawShadedTriangle
(
    int16_t           x0,
    int16_t           y0,
    double            h0,
    int16_t           x1,
    int16_t           y1,
    double            h1,
    int16_t           x2,
    int16_t           y2,
    double            h2,
    struct MRE_Color  color
)
{
  //TODO init size + 1
  int16_t    m;
  int16_t    y;
  int16_t    x;
  int16_t    x_l;
  int16_t    x_r;
  int16_t  * xsl;
  int16_t  * xsr;
  int16_t  * xs012;
  int16_t  * xs02;
  double   * hs012;
  double   * hs02;
  double   * hxs;
  double   * hsl;
  double   * hsr;

  

  if ( y1 < y0 )  MRE_SWAP_T( int16_t, x0, x1,  y0, y1 );
  if ( y2 < y0 )  MRE_SWAP_T( int16_t, x0, x2,  y0, y2 );
  if ( y2 < y1 )  MRE_SWAP_T( int16_t, x1, x2,  y1, y2 );

  xs012 = alloca( ( y2 - y0 + 1 ) * sizeof( int16_t ) );
  xs02  = alloca( ( y2 - y0 + 1 ) * sizeof( int16_t ) );
  hs012 = alloca( ( y2 - y0 + 1 ) * sizeof( double ) );
  hs02  = alloca( ( y2 - y0 + 1 ) * sizeof( double ) );

  MRE_Interpolate_Int16Int16( xs012, y0, x0, y1, x1 );
  MRE_Interpolate_Int16Int16( xs012 + ( y1 - y0 ), y1, x1, y2, x2 );
  MRE_Interpolate_Int16Int16( xs02, y0, x0, y2, x2 );
  MRE_Interpolate_Int16Double( hs012, y0, h0, y1, h1 );
  MRE_Interpolate_Int16Double( hs012 + ( y1 - y0 ), y1, h1, y2, h2 );
  MRE_Interpolate_Int16Double( hs02, y0, h0, y2, h2 );

  m = ( y2 - y0 ) / 2;

  if ( xs02[ m ] < xs012[ m ] )
  {
    xsl = xs02;
    xsr = xs012;

    hsl = hs02;
    hsr = hs012;
  }
  else
  {
    xsl = xs012;
    xsr = xs02;
    
    hsl = hs012;
    hsr = hs02;
  }

  for ( y = y0; y <= y2; ++y )
  {
    x_l = xsl[ y - y0 ];
    x_r = xsr[ y - y0 ];

    hxs = alloca ( ( x_r - x_l + 1 ) * sizeof( double ) );
    MRE_Interpolate_Int16Double(
      hxs, 
      x_l, hsl[ y - y0 ], 
      x_r, hsr[ y - y0 ]
    );

    for ( x = x_l; x <= x_r; ++x )
    {
      MRE_buff[ MRE_buff_w * y + x ] = MRE_RGBA_TO_PIXEL(
        color . r * hxs[ x - x_l ],
        color . g * hxs[ x - x_l ],
        color . b * hxs[ x - x_l ],
        color . a
      );
    }
  }
}
