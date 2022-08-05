#include <MRE_graphics.h>

void
MRE_DrawLine
(
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_Pixel  pixel
)
{
  MRE_I16    i;
  MRE_I16  * ds;


  
  if ( abs( x1 - x0 ) > abs( y1 - y0 ) )
  {
    if ( x0 > x1 )  MRE_SWAP_T( MRE_I16, x0, x1,  y0, y1);

    ds = alloca( ( x1 - x0 + 1 ) * sizeof( MRE_I16 ) );
    
    MRE_Interpolate_I16_I16( ds, x0, y0, x1, y1 );
    for ( i = x0; i <= x1; ++i )
    {
      MRE_buff[ MRE_buff_w * ds[ i - x0 ] + i ] = pixel;
    }
  }
  else
  {
    if ( y0 > y1 )  MRE_SWAP_T( MRE_I16, x0, x1,  y0, y1);
    
    ds = alloca( ( y1 - y0 + 1 ) * sizeof( MRE_I16 ) );
    
    
    MRE_Interpolate_I16_I16( ds, y0, x0, y1, x1 );
    for ( i = y0; i <= y1; ++i )
    {
      MRE_buff[ MRE_buff_w * i + ds[ i - y0 ] ] = pixel;
    }
  }
}

MRE_INLINE_
void
MRE_DrawWireframeTriangle
(
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_I16    x2,
    MRE_I16    y2,
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
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_I16    x2,
    MRE_I16    y2,
    MRE_Pixel  pixel
)
{
  MRE_I16    m;
  MRE_I16    y;
  MRE_I16    x;
  MRE_I16  * xsl;
  MRE_I16  * xsr;
  MRE_I16  * xs012;
  MRE_I16  * xs02;

  

  if ( y1 < y0 )  MRE_SWAP_T( MRE_I16, x0, x1,  y0, y1);
  if ( y2 < y0 )  MRE_SWAP_T( MRE_I16, x0, x2,  y0, y2);
  if ( y2 < y1 )  MRE_SWAP_T( MRE_I16, x1, x2,  y1, y2);

  xs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  xs02 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );

  MRE_Interpolate_I16_I16( xs012, y0, x0, y1, x1 );
  MRE_Interpolate_I16_I16( xs012 + ( y1 - y0 ), y1, x1, y2, x2 );
  MRE_Interpolate_I16_I16( xs02, y0, x0, y2, x2 );

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
    MRE_I16           x0,
    MRE_I16           y0,
    MRE_F64           h0,
    MRE_I16           x1,
    MRE_I16           y1,
    MRE_F64           h1,
    MRE_I16           x2,
    MRE_I16           y2,
    MRE_F64           h2,
    struct MRE_Color  color
)
{
  MRE_I16    m;
  MRE_I16    y;
  MRE_I16    x;
  MRE_I16    x_l;
  MRE_I16    x_r;
  MRE_I16  * xsl;
  MRE_I16  * xsr;
  MRE_I16  * xs012;
  MRE_I16  * xs02;
  MRE_F64  * hs012;
  MRE_F64  * hs02;
  MRE_F64  * hxs;
  MRE_F64  * hsl;
  MRE_F64  * hsr;

  

  if ( y1 < y0 )  MRE_SWAP_T( MRE_I16, x0, x1,  y0, y1 );
  if ( y2 < y0 )  MRE_SWAP_T( MRE_I16, x0, x2,  y0, y2 );
  if ( y2 < y1 )  MRE_SWAP_T( MRE_I16, x1, x2,  y1, y2 );

  xs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  xs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  hs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  hs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );

  MRE_Interpolate_I16_I16( xs012, y0, x0, y1, x1 );
  MRE_Interpolate_I16_I16( xs012 + ( y1 - y0 ), y1, x1, y2, x2 );
  MRE_Interpolate_I16_I16( xs02, y0, x0, y2, x2 );
  MRE_Interpolate_I16_F64( hs012, y0, h0, y1, h1 );
  MRE_Interpolate_I16_F64( hs012 + ( y1 - y0 ), y1, h1, y2, h2 );
  MRE_Interpolate_I16_F64( hs02, y0, h0, y2, h2 );

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

    hxs = alloca ( ( x_r - x_l + 1 ) * sizeof(  MRE_F64 ) );
    MRE_Interpolate_I16_F64(
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


void MRE_RenderModel
(
  const void    * vert_p,
  MRE_I32         vert_count,
  const void    * tr_p,
  MRE_I32         tr_count,
  const MRE_Mat4  proj_mat4,
  MRE_Pixel       pixel
)
{
  MRE_UI32           i;
  const MRE_Vec3   * c_vert_vec;
  const MRE_IVec3  * tr_vec;
  MRE_Vec4           un_vert_vec;
  MRE_Vec3           gl_vert_p[ vert_count ];

 
  c_vert_vec = vert_p;
  for ( i = 0; i < vert_count; ++i )
  {
    MRE_Set_Vec4_3( c_vert_vec[i], 1.0, un_vert_vec );
    MRE_Mul_Mat4_V( proj_mat4, un_vert_vec, un_vert_vec );
    MRE_Div_Vec4_S( un_vert_vec, un_vert_vec[3], un_vert_vec );
    MRE_Copy_Vec3( un_vert_vec, gl_vert_p[i] );
  }

  tr_vec = tr_p;
  for ( i = 0; i < tr_count; ++i )
  {
    MRE_RenderTriangle(
      gl_vert_p[tr_vec[i][0]],
      gl_vert_p[tr_vec[i][1]],
      gl_vert_p[tr_vec[i][2]],
      pixel
    );
  }
}

MRE_INLINE_
void
MRE_RenderTriangle
(
  const MRE_Vec2  v0,
  const MRE_Vec2  v1,
  const MRE_Vec2  v2,
  MRE_Pixel       pixel
)
{
  MRE_DrawWireframeTriangle(
    ( v0[0] + 0.5 ) * MRE_buff_w, ( v0[1] + 0.5 ) * MRE_buff_h,
    ( v1[0] + 0.5 ) * MRE_buff_w, ( v1[1] + 0.5 ) * MRE_buff_h,
    ( v2[0] + 0.5 ) * MRE_buff_w, ( v2[1] + 0.5 ) * MRE_buff_h,
    pixel
  );
}
