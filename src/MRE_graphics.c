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
    MRE_F64    z0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_F64    z1,
    MRE_I16    x2,
    MRE_I16    y2,
    MRE_F64    z2,
    MRE_Pixel  pixel
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
  MRE_F64  * zsl;
  MRE_F64  * zsr;
  MRE_F64  * zs012;
  MRE_F64  * zs02;
  MRE_F64  * zxs;

  
  if ( y1 < y0 )
  {
    MRE_SWAP( MRE_I16, y0, y1 );
    MRE_SWAP( MRE_I16, x0, x1 );
    MRE_SWAP( MRE_F64, z0, z1 );
  }
  if ( y2 < y0 )
  {
    MRE_SWAP( MRE_I16, y0, y2 );
    MRE_SWAP( MRE_I16, x0, x2 );
    MRE_SWAP( MRE_F64, z0, z2 );
  }
  if ( y2 < y1 )
  {
    MRE_SWAP( MRE_I16, y1, y2 );
    MRE_SWAP( MRE_I16, x1, x2 );
    MRE_SWAP( MRE_F64, z1, z2 );
  }

  xs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  zs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  xs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  zs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  
  MRE_Interpolate_I16_I16( xs012, y0, x0, y1, x1 );
  MRE_Interpolate_I16_F64( zs012, y0, z0, y1, z1 );
  MRE_Interpolate_I16_I16( xs012 + ( y1 - y0 ), y1, x1, y2, x2 );
  MRE_Interpolate_I16_F64( zs012 + ( y1 - y0 ), y1, z1, y2, z2 );
  MRE_Interpolate_I16_I16( xs02, y0, x0, y2, x2 );
  MRE_Interpolate_I16_F64( zs02, y0, z0, y2, z2 );
  
  m = ( y2 - y0 ) / 2;

  if ( xs02[ m ] < xs012[ m ] )
  {
    xsl = xs02;
    xsr = xs012;
    
    zsl = zs02;
    zsr = zs012;
  }
  else
  {
    xsl = xs012;
    xsr = xs02;
    
    zsl = zs012;
    zsr = zs02;
  }
    
  for ( y = y0; y < y2; ++y )
  {
    x_l = xsl[ y - y0 ];
    x_r = xsr[ y - y0 ];

    zxs = alloca ( ( x_r - x_l + 1 ) * sizeof(  MRE_F64 ) );
    MRE_Interpolate_I16_F64(
      zxs, 
      x_l, zsl[ y - y0 ], 
      x_r, zsr[ y - y0 ]
    );

    for ( x = x_l; x <= x_r; ++x )
    {
      if ( zxs[ x - x_l ] > _MRE_z_buff[ MRE_buff_w * y + x ] + MRE_F64_MIN )
      {
        MRE_buff[ MRE_buff_w * y + x ] = pixel;
        _MRE_z_buff[ MRE_buff_w * y + x ] = zxs[ x - x_l ];
      }
    }
  }
}

void
MRE_DrawShadedTriangle
(
    MRE_I16          x0,
    MRE_I16          y0,
    MRE_F64          z0,
    MRE_F64          h0,
    const MRE_F64  * c0,
    MRE_I16          x1,
    MRE_I16          y1,
    MRE_F64          z1,
    MRE_F64          h1,
    const MRE_F64  * c1,
    MRE_I16          x2,
    MRE_I16          y2,
    MRE_F64          z2,
    MRE_F64          h2,
    const MRE_F64  * c2
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
  MRE_F64  * zs012;
  MRE_F64  * zs02;
  MRE_F64  * zxs;
  MRE_F64  * zsl;
  MRE_F64  * zsr;
  
  MRE_F64  * cs012[3];
  MRE_F64  * cs02[3];
  MRE_F64  * cxs[3];
  MRE_F64  * csl[3];
  MRE_F64  * csr[3];
  

  if ( y1 < y0 )
  {
    MRE_SWAP( MRE_I16, y0, y1 );
    MRE_SWAP( MRE_I16, x0, x1 );
    MRE_SWAP( MRE_F64, h0, h1 );
    MRE_SWAP( MRE_F64, z0, z1 );
    MRE_SWAP( const MRE_F64 *, c0, c1 );
  }
  if ( y2 < y0 )
  {
    MRE_SWAP( MRE_I16, y0, y2 );
    MRE_SWAP( MRE_I16, x0, x2 );
    MRE_SWAP( MRE_F64, h0, h2 );
    MRE_SWAP( MRE_F64, z0, z2 );
    MRE_SWAP( const MRE_F64 *, c0, c2 );
  }
  if ( y2 < y1 )
  {
    MRE_SWAP( MRE_I16, y1, y2 );
    MRE_SWAP( MRE_I16, x1, x2 );
    MRE_SWAP( MRE_F64, h1, h2 );
    MRE_SWAP( MRE_F64, z1, z2 );
    MRE_SWAP( const MRE_F64 *, c1, c2 );
  }

  xs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  hs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  zs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  _MRE_CFOR( 3, {
    cs012[ _k ] = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  } );
  xs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  hs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  zs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  _MRE_CFOR( 3, {
    cs02[ _k ] = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  } );
    
  MRE_Interpolate_I16_I16( xs012, y0, x0, y1, x1 );
  MRE_Interpolate_I16_F64( hs012, y0, h0, y1, h1 );
  MRE_Interpolate_I16_F64( zs012, y0, z0, y1, z1 );
  _MRE_CFOR( 3, {
    MRE_Interpolate_I16_F64( cs012[ _k ], y0, c0[ _k ], y1, c1[ _k ] ); 
  } );
  MRE_Interpolate_I16_I16( xs012 + ( y1 - y0 ), y1, x1, y2, x2 );
  MRE_Interpolate_I16_F64( hs012 + ( y1 - y0 ), y1, h1, y2, h2 );
  MRE_Interpolate_I16_F64( zs012 + ( y1 - y0 ), y1, z1, y2, z2 );
  _MRE_CFOR( 3, {
    MRE_Interpolate_I16_F64( 
      cs012[ _k ] + ( y1 - y0 ), y1, c1[ _k ], y2, c2[ _k ]
    ); 
  } );
  MRE_Interpolate_I16_I16( xs02, y0, x0, y2, x2 );
  MRE_Interpolate_I16_F64( hs02, y0, h0, y2, h2 );
  MRE_Interpolate_I16_F64( zs02, y0, z0, y2, z2 );
  _MRE_CFOR( 3, {
    MRE_Interpolate_I16_F64( cs02[ _k ], y0, c0[ _k ], y2, c2[ _k ] ) ;
  } );

  m = ( y2 - y0 ) / 2;

  if ( xs02[ m ] < xs012[ m ] )
  {
    xsl = xs02;
    xsr = xs012;

    hsl = hs02;
    hsr = hs012;
    
    zsl = zs02;
    zsr = zs012;
    
    _MRE_CFOR( 3, {
      csl[ _k ] = cs02[ _k ];
      csr[ _k ] = cs012[ _k ];
    } );
  }
  else
  {
    xsl = xs012;
    xsr = xs02;
    
    hsl = hs012;
    hsr = hs02;
    
    zsl = zs012;
    zsr = zs02;
    
    _MRE_CFOR( 3, {
      csl[ _k ] = cs012[ _k ];
      csr[ _k ] = cs02[ _k ];
    } );
  }

  for ( y = y0; y < y2; ++y )
  {
    x_l = xsl[ y - y0 ];
    x_r = xsr[ y - y0 ];

    hxs = alloca ( ( x_r - x_l + 1 ) * sizeof(  MRE_F64 ) );
    MRE_Interpolate_I16_F64(
      hxs, 
      x_l, hsl[ y - y0 ], 
      x_r, hsr[ y - y0 ]
    );
    
    zxs = alloca ( ( x_r - x_l + 1 ) * sizeof(  MRE_F64 ) );
    MRE_Interpolate_I16_F64(
      zxs, 
      x_l, zsl[ y - y0 ], 
      x_r, zsr[ y - y0 ]
    );
    
    _MRE_CFOR( 3, {
      cxs[ _k ] = alloca ( ( x_r - x_l + 1 ) * sizeof(  MRE_F64 ) );
      MRE_Interpolate_I16_F64(
        cxs[ _k ], 
        x_l, csl[ _k ][ y - y0 ], 
        x_r, csr[ _k ][ y - y0 ]
      );
    } );

    for ( x = x_l; x <= x_r; ++x )
    {
      if ( zxs[ x - x_l ] > _MRE_z_buff[ MRE_buff_w * y + x ] + MRE_F64_MIN )
      {
        MRE_buff[ MRE_buff_w * y + x ] = MRE_RGBA_TO_PIXEL(
          255 * cxs[0][ x - x_l ] * hxs[ x - x_l ],
          255 * cxs[1][ x - x_l ] * hxs[ x - x_l ],
          255 * cxs[2][ x - x_l ] * hxs[ x - x_l ],
          255
        );
        _MRE_z_buff[ MRE_buff_w * y + x ] = zxs[ x - x_l ];
      }
    }
  }
}


MRE_INLINE_
void
MRE_RenderTriangle
(
    const MRE_Vec3  v0,
    const MRE_Vec3  c0,
    const MRE_F64   h0,
    const MRE_Vec3  v1,
    const MRE_Vec3  c1,
    const MRE_F64   h1,
    const MRE_Vec3  v2,
    const MRE_Vec3  c2,
    const MRE_F64   h2
)
{
  //TODO better code???

  MRE_DrawShadedTriangle(
    round( ( v0[0] * 0.5 + 0.5 ) * ( MRE_buff_w - 1 ) ),
    round( ( v0[1] * 0.5 + 0.5 ) * ( MRE_buff_h - 1 ) ),
    1.0 / v0[2],
    h0,
    c0,
    round( ( v1[0] * 0.5 + 0.5 ) * ( MRE_buff_w - 1 ) ),
    round( ( v1[1] * 0.5 + 0.5 ) * ( MRE_buff_h - 1 ) ),
    1.0 / v1[2],
    h1,
    c1,
    round( ( v2[0] * 0.5 + 0.5 ) * ( MRE_buff_w - 1 ) ),
    round( ( v2[1] * 0.5 + 0.5 ) * ( MRE_buff_h - 1 ) ),
    1.0 / v2[2],
    h2,
    c2
  );
}

void
MRE_RenderTrianglesModel
(
    const MRE_F64  * const v,
    MRE_I32                vc,
    const MRE_I32  * const t,
    MRE_I32                tc
)
{
  MRE_UI32          i;
  MRE_Vec3          n;
  MRE_Vec4        * vproj;
  const MRE_F64   * v0;
  const MRE_F64   * v1;
  const MRE_F64   * v2;


  vproj = malloc( vc * sizeof( MRE_Vec4 ) );
 
  for ( i = 0; i < vc; ++i )
  {
    MRE_MulMat4Vec3Vec4( 
      _MRE_projection_mat, 
      v + i * _MRE_vs + _MRE_vap[ VAP_P_I ], 1.0, 
      vproj[ i ]
    );
    MRE_DIV_VEC3_S(
      vproj[ i ], vproj[ i ][3], vproj[ i ]
    );
  }
    
  for ( i = 0; i < tc; ++i )
  {
    v0 = v + t[ 3 * i + 0 ] * _MRE_vs;
    v1 = v + t[ 3 * i + 1 ] * _MRE_vs;
    v2 = v + t[ 3 * i + 2 ] * _MRE_vs;

    MRE_TriangleNormal(
      v0 + _MRE_vap[ VAP_P_I ], 
      v1 + _MRE_vap[ VAP_P_I ], 
      v2 + _MRE_vap[ VAP_P_I ], 
      n
    );

    MRE_RenderTriangle(
      vproj[ t[ 3 * i + 0 ] ],
      v0 + _MRE_vap[ VAP_C_I ],
      MRE_GetVertIlluminaceLevel( v0 + _MRE_vap[ VAP_P_I ], n ),
      vproj[ t[ 3 * i + 1 ] ],
      v1 + _MRE_vap[ VAP_C_I ],
      MRE_GetVertIlluminaceLevel( v1 + _MRE_vap[ VAP_P_I ], n ),
      vproj[ t[ 3 * i + 2 ] ],
      v2 + _MRE_vap[ VAP_C_I ],
      MRE_GetVertIlluminaceLevel( v2 + _MRE_vap[ VAP_P_I ], n )
    );
  }

  free( vproj );
}

void
MRE_RenderSphereModel
(
    const MRE_F64  * const v,
    MRE_I32                vc,
    const MRE_I32  * const t,
    MRE_I32                tc,
    MRE_Vec3               center
)
{
  MRE_UI32          i;
  MRE_Vec3          n;
  const MRE_F64   * v0;
  MRE_F64         * vilmc;
  MRE_Vec4        * vproj;


  vproj = malloc( vc * sizeof( MRE_Vec4 ) );
  vilmc = malloc( vc * sizeof( MRE_F64 ) );
 

  for ( i = 0; i < vc; ++i )
  {
    v0 = v + i * _MRE_vs;
    MRE_MulMat4Vec3Vec4( 
      _MRE_projection_mat, 
      v0 + _MRE_vap[ VAP_P_I ], 1.0, 
      vproj[ i ]
    );
    MRE_DIV_VEC3_S(
      vproj[ i ], vproj[ i ][3], vproj[ i ]
    );

    MRE_SUB_VEC3( v0 + _MRE_vap[ VAP_P_I ], center, n );
    vilmc[ i ] = MRE_GetVertIlluminaceLevel( v0 + _MRE_vap[ VAP_P_I ], n );
  }

  for ( i = 0; i < tc; ++i )
  {
    MRE_RenderTriangle(
      vproj[ t[ 3 * i + 0 ] ],
      v + t[ 3 * i + 0 ] * _MRE_vs + _MRE_vap[ VAP_C_I ],
      vilmc[ t[ 3 * i + 0 ] ],
      vproj[ t[ 3 * i + 1 ] ],
      v + t[ 3 * i + 1 ] * _MRE_vs + _MRE_vap[ VAP_C_I ],
      vilmc[ t[ 3 * i + 1 ] ],
      vproj[ t[ 3 * i + 2 ] ],
      v + t[ 3 * i + 2 ] * _MRE_vs + _MRE_vap[ VAP_C_I ],
      vilmc[ t[ 3 * i + 2 ] ]
    );
  }

  free( vilmc );
  free( vproj );
}
