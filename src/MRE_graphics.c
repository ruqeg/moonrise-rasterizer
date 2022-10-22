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
      _MRE_buff[ _MRE_buff_w * ds[ i - x0 ] + i ] = pixel;
    }
  }
  else
  {
    if ( y0 > y1 )  MRE_SWAP_T( MRE_I16, x0, x1,  y0, y1);
    
    ds = alloca( ( y1 - y0 + 1 ) * sizeof( MRE_I16 ) );
    
    
    MRE_Interpolate_I16_I16( ds, y0, x0, y1, x1 );
    for ( i = y0; i <= y1; ++i )
    {
      _MRE_buff[ _MRE_buff_w * i + ds[ i - y0 ] ] = pixel;
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
      if ( zxs[ x - x_l ] > _MRE_z_buff[ _MRE_buff_w * y + x ] + MRE_F64_MIN )
      {
        _MRE_buff[ _MRE_buff_w * y + x ] = pixel;
        _MRE_z_buff[ _MRE_buff_w * y + x ] = zxs[ x - x_l ];
      }
    }
  }
}

void
MRE_DrawColoredTriangle
(
    const MRE_F64   * p0,
    const MRE_F64   * v0,
    const MRE_F64   * p1,
    const MRE_F64   * v1,
    const MRE_F64   * p2,
    const MRE_F64   * v2
)
{
  MRE_I16     m;
  MRE_I16     x;
  MRE_I16     y;
  MRE_I16     ymx;
  MRE_I16     ymd;
  MRE_I16     ymn;
  MRE_I16     xl;
  MRE_I16     xr;
  MRE_F64     iz;
  MRE_Vec3    vcolor;
  MRE_Vec3    vpos;
  MRE_Vec3    vcord;
  MRE_Vec3    rvcolor;
  MRE_F64   * xsl;
  MRE_F64   * xsr;
  MRE_F64   * xs012;
  MRE_F64   * xs02;
  MRE_F64   * zsl;
  MRE_F64   * zsr;
  MRE_F64   * zs012;
  MRE_F64   * zs02;
  MRE_F64   * zxs;
  MRE_F64   * vs012[6];
  MRE_F64   * vs02[6];
  MRE_F64   * vxs[6];
  MRE_F64   * vsl[6];
  MRE_F64   * vsr[6];
  

  



  if ( p1[1] < p0[1] )
  {
    MRE_SWAP( const MRE_F64 *, p0, p1 );
    MRE_SWAP( const MRE_F64 *, v0, v1 );
  }
  if ( p2[1] < p0[1] )
  {
    MRE_SWAP( const MRE_F64 *, p0, p2 );
    MRE_SWAP( const MRE_F64 *, v0, v2 );
  }
  if ( p2[1] < p1[1] )
  {
    MRE_SWAP( const MRE_F64 *, p1, p2 );
    MRE_SWAP( const MRE_F64 *, v1, v2 );
  }

  ymn = floor( ( p0[1] * 0.5 + 0.5 ) * ( _MRE_buff_h - 1 ) );
  ymd = floor( ( p1[1] * 0.5 + 0.5 ) * ( _MRE_buff_h - 1 ) );
  ymx = ceil(  ( p2[1] * 0.5 + 0.5 ) * ( _MRE_buff_h - 1 ) );

  xs012 = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  zs012 = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  _MRE_CFOR( 6, {
    vs012[ _k ] = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  } );
  xs02  = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  zs02  = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  _MRE_CFOR( 6, {
    vs02[ _k ] = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  } );
    


  
  MRE_Interpolate_I16_F64( xs012, ymn, p0[0], ymd, p1[0] );
  MRE_Interpolate_I16_F64(
    zs012, 
    ymn, p0[2] / v0[ MRE_POF + 2 ],
    ymd, p1[2] / v1[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_POF + 2, {
    MRE_Interpolate_I16_F64(
      vs012[ _k ],
      ymn, v0[ _k ] / v0[ MRE_POF + 2 ],
      ymd, v1[ _k ] / v1[ MRE_POF + 2 ]
    );
  } );
  MRE_Interpolate_I16_F64(
    vs012[ MRE_POF + 2 ],
    ymn, 1.0 / v0[ MRE_POF + 2 ],
    ymd, 1.0 / v1[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_COF, {
    MRE_Interpolate_I16_F64(
      vs012[ MRE_COF + _k ],
      ymn, v0[ MRE_COF + _k ] / v0[ MRE_POF + 2 ],
      ymd, v1[ MRE_COF + _k ] / v1[ MRE_POF + 2 ]
    );
  } );

  MRE_Interpolate_I16_F64( xs012 + ( ymd - ymn ), ymd, p1[0], ymx, p2[0] );
  MRE_Interpolate_I16_F64(
    zs012 + ( ymd - ymn ), 
    ymd, p1[2] / v1[ MRE_POF + 2 ],
    ymx, p2[2] / v2[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_POF + 2, {
    MRE_Interpolate_I16_F64(
      vs012[ _k ] + ( ymd - ymn ),
      ymd, v1[ _k ] / v1[ MRE_POF + 2 ],
      ymx, v2[ _k ] / v2[ MRE_POF + 2 ]
    );
  } );
  MRE_Interpolate_I16_F64(
    vs012[ MRE_POF + 2 ] + ( ymd - ymn ),
    ymd, 1.0 / v1[ MRE_POF + 2 ],
    ymx, 1.0 / v2[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_COF, {
    MRE_Interpolate_I16_F64(
      vs012[ MRE_COF + _k ] + ( ymd - ymn ),
      ymd, v1[ MRE_COF + _k ] / v1[ MRE_POF + 2 ],
      ymx, v2[ MRE_COF + _k ] / v2[ MRE_POF + 2 ]
    );
  } );

  MRE_Interpolate_I16_F64( xs02, ymn, p0[0], ymx, p2[0] );
  MRE_Interpolate_I16_F64(
    zs02, 
    ymn, p0[2] / v0[ MRE_POF + 2 ],
    ymx, p2[2] / v2[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_POF + 2, {
    MRE_Interpolate_I16_F64(
      vs02[ _k ],
      ymn, v0[ _k ] / v0[ MRE_POF + 2 ],
      ymx, v2[ _k ] / v2[ MRE_POF + 2 ]
    );
  } );
  MRE_Interpolate_I16_F64(
    vs02[ MRE_POF + 2 ],
    ymn, 1.0 / v0[ MRE_POF + 2 ],
    ymx, 1.0 / v2[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_COF, {
    MRE_Interpolate_I16_F64(
      vs02[ MRE_COF + _k ],
      ymn, v0[ MRE_COF + _k ] / v0[ MRE_POF + 2 ],
      ymx, v2[ MRE_COF + _k ] / v2[ MRE_POF + 2 ]
    );
  } );




  m = ( ymx - ymn ) / 2;

  if ( xs02[ m ] < xs012[ m ] )
  {
    xsl = xs02;
    xsr = xs012;
    
    zsl = zs02;
    zsr = zs012;

    _MRE_CFOR( 6, {
      vsl[ _k ] = vs02[ _k ];
      vsr[ _k ] = vs012[ _k ];
    } );
  }
  else
  {
    xsl = xs012;
    xsr = xs02;
    
    zsl = zs012;
    zsr = zs02;
    
    _MRE_CFOR( 6, {
      vsl[ _k ] = vs012[ _k ];
      vsr[ _k ] = vs02[ _k ];
    } );
  }

  

  for ( y = ymn; y < ymx; ++y )
  {
    xl = floor( ( xsl[ y - ymn ] * 0.5 + 0.5 ) * ( _MRE_buff_w - 1 ) );
    xr = ceil(  ( xsr[ y - ymn ] * 0.5 + 0.5 ) * ( _MRE_buff_w - 1 ) );

    zxs = alloca ( ( xr - xl + 1 ) * sizeof(  MRE_F64 ) );
    MRE_Interpolate_I16_F64(
      zxs, 
      xl, zsl[ y - ymn ], 
      xr, zsr[ y - ymn ]
    );
    
    _MRE_CFOR( 6, {
      vxs[ _k ] = alloca ( ( xr - xl + 1 ) * sizeof(  MRE_F64 ) );
      MRE_Interpolate_I16_F64(
        vxs[ _k ], 
        xl, vsl[ _k ][ y - ymn ], 
        xr, vsr[ _k ][ y - ymn ]
      );
    } );

    for ( x = xl; x <= xr; ++x )
    {
      iz = vxs[ MRE_POF + 2 ][ x - xl ];
      if ( iz > _MRE_z_buff[ _MRE_buff_w * y + x ] + MRE_F64_MIN )
      {
        MRE_SET_VEC3(
          vxs[ MRE_COF + 0 ][ x - xl ] / iz,
          vxs[ MRE_COF + 1 ][ x - xl ] / iz,
          vxs[ MRE_COF + 2 ][ x - xl ] / iz,
          vcolor
        );
        MRE_SET_VEC3(
          vxs[ MRE_POF + 0 ][ x - xl ] / iz,
          vxs[ MRE_POF + 1 ][ x - xl ] / iz,
          1.0 / iz,
          vpos
        );
        MRE_SET_VEC3( x, y, zxs[ x - xl ] / iz, vcord );

        _MRE_fragment_shader( vcolor, rvcolor, vpos, vcord );

        _MRE_buff[ _MRE_buff_w * y + x ] = MRE_RGBA_TO_PIXEL(
          255 * rvcolor[0],
          255 * rvcolor[1],
          255 * rvcolor[2],
          255
        );
        _MRE_z_buff[ _MRE_buff_w * y + x ] = iz;
      }
    }
  }
}

extern 
void 
MRE_DrawTexturedTriangle
(
    const MRE_F64   * p0,
    const MRE_F64   * v0,
    const MRE_F64   * p1,
    const MRE_F64   * v1,
    const MRE_F64   * p2,
    const MRE_F64   * v2
)
{
  MRE_I16     m;
  MRE_I16     x;
  MRE_I16     y;
  MRE_I16     ymx;
  MRE_I16     ymd;
  MRE_I16     ymn;
  MRE_I16     xl;
  MRE_I16     xr;
  MRE_I32     u;
  MRE_I32     v;
  MRE_F64     iz;
  MRE_Vec3    vcolor;
  MRE_Vec3    vpos;
  MRE_Vec3    vcord;
  MRE_Vec3    rvcolor;
  MRE_F64   * xsl;
  MRE_F64   * xsr;
  MRE_F64   * xs012;
  MRE_F64   * xs02;
  MRE_F64   * zsl;
  MRE_F64   * zsr;
  MRE_F64   * zs012;
  MRE_F64   * zs02;
  MRE_F64   * zxs;
  MRE_F64   * vs012[8];
  MRE_F64   * vs02[8];
  MRE_F64   * vxs[8];
  MRE_F64   * vsl[8];
  MRE_F64   * vsr[8];
  






  if ( p1[1] < p0[1] )
  {
    MRE_SWAP( const MRE_F64 *, p0, p1 );
    MRE_SWAP( const MRE_F64 *, v0, v1 );
  }
  if ( p2[1] < p0[1] )
  {
    MRE_SWAP( const MRE_F64 *, p0, p2 );
    MRE_SWAP( const MRE_F64 *, v0, v2 );
  }
  if ( p2[1] < p1[1] )
  {
    MRE_SWAP( const MRE_F64 *, p1, p2 );
    MRE_SWAP( const MRE_F64 *, v1, v2 );
  }

  ymn = floor( ( p0[1] * 0.5 + 0.5 ) * ( _MRE_buff_h - 1 ) );
  ymd = floor( ( p1[1] * 0.5 + 0.5 ) * ( _MRE_buff_h - 1 ) );
  ymx = ceil(  ( p2[1] * 0.5 + 0.5 ) * ( _MRE_buff_h - 1 ) );

  xs012 = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  zs012 = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );



  _MRE_CFOR( 8, {
    vs012[ _k ] = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  } );
  xs02  = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  zs02  = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  _MRE_CFOR( 8, {
    vs02[ _k ] = alloca( ( ymx - ymn + 1 ) * sizeof( MRE_F64 ) );
  } );
    




  MRE_Interpolate_I16_F64( xs012, ymn, p0[0], ymd, p1[0] );
  MRE_Interpolate_I16_F64(
    zs012, 
    ymn, p0[2] / v0[ MRE_POF + 2 ],
    ymd, p1[2] / v1[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_POF + 2, {
    MRE_Interpolate_I16_F64(
      vs012[ _k ],
      ymn, v0[ _k ] / v0[ MRE_POF + 2 ],
      ymd, v1[ _k ] / v1[ MRE_POF + 2 ]
    );
  } );
  MRE_Interpolate_I16_F64(
    vs012[ MRE_POF + 2 ],
    ymn, 1.0 / v0[ MRE_POF + 2 ],
    ymd, 1.0 / v1[ MRE_POF + 2 ]
  );
  _MRE_CFOR( 5, {
    MRE_Interpolate_I16_F64(
      vs012[ MRE_COF + _k ],
      ymn, v0[ MRE_COF + _k ] / v0[ MRE_POF + 2 ],
      ymd, v1[ MRE_COF + _k ] / v1[ MRE_POF + 2 ]
    );
  } );

  MRE_Interpolate_I16_F64( xs012 + ( ymd - ymn ), ymd, p1[0], ymx, p2[0] );
  MRE_Interpolate_I16_F64(
    zs012 + ( ymd - ymn ), 
    ymd, p1[2] / v1[ MRE_POF + 2 ],
    ymx, p2[2] / v2[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_POF + 2, {
    MRE_Interpolate_I16_F64(
      vs012[ _k ] + ( ymd - ymn ),
      ymd, v1[ _k ] / v1[ MRE_POF + 2 ],
      ymx, v2[ _k ] / v2[ MRE_POF + 2 ]
    );
  } );
  MRE_Interpolate_I16_F64(
    vs012[ MRE_POF + 2 ] + ( ymd - ymn ),
    ymd, 1.0 / v1[ MRE_POF + 2 ],
    ymx, 1.0 / v2[ MRE_POF + 2 ]
  );
  _MRE_CFOR( 5, {
    MRE_Interpolate_I16_F64(
      vs012[ MRE_COF + _k ] + ( ymd - ymn ),
      ymd, v1[ MRE_COF + _k ] / v1[ MRE_POF + 2 ],
      ymx, v2[ MRE_COF + _k ] / v2[ MRE_POF + 2 ]
    );
  } );

  MRE_Interpolate_I16_F64( xs02, ymn, p0[0], ymx, p2[0] );
  MRE_Interpolate_I16_F64(
    zs02, 
    ymn, p0[2] / v0[ MRE_POF + 2 ],
    ymx, p2[2] / v2[ MRE_POF + 2 ]
  );
  _MRE_CFOR( MRE_POF + 2, {
    MRE_Interpolate_I16_F64(
      vs02[ _k ],
      ymn, v0[ _k ] / v0[ MRE_POF + 2 ],
      ymx, v2[ _k ] / v2[ MRE_POF + 2 ]
    );
  } );
  MRE_Interpolate_I16_F64(
    vs02[ MRE_POF + 2 ],
    ymn, 1.0 / v0[ MRE_POF + 2 ],
    ymx, 1.0 / v2[ MRE_POF + 2 ]
  );
  _MRE_CFOR( 5, {
    MRE_Interpolate_I16_F64(
      vs02[ MRE_COF + _k ],
      ymn, v0[ MRE_COF + _k ] / v0[ MRE_POF + 2 ],
      ymx, v2[ MRE_COF + _k ] / v2[ MRE_POF + 2 ]
    );
  } );



  m = ( ymx - ymn ) / 2;

  if ( xs02[ m ] < xs012[ m ] )
  {
    xsl = xs02;
    xsr = xs012;
    
    zsl = zs02;
    zsr = zs012;

    _MRE_CFOR( 8, {
      vsl[ _k ] = vs02[ _k ];
      vsr[ _k ] = vs012[ _k ];
    } );
  }
  else
  {
    xsl = xs012;
    xsr = xs02;
    
    zsl = zs012;
    zsr = zs02;
    
    _MRE_CFOR( 8, {
      vsl[ _k ] = vs012[ _k ];
      vsr[ _k ] = vs02[ _k ];
    } );
  }

  for ( y = ymn; y < ymx; ++y )
  {
    xl = floor( ( xsl[ y - ymn ] * 0.5 + 0.5 ) * ( _MRE_buff_w - 1 ) );
    xr = ceil(  ( xsr[ y - ymn ] * 0.5 + 0.5 ) * ( _MRE_buff_w - 1 ) );

    zxs = alloca ( ( xr - xl + 1 ) * sizeof(  MRE_F64 ) );
    MRE_Interpolate_I16_F64(
      zxs, 
      xl, zsl[ y - ymn ], 
      xr, zsr[ y - ymn ]
    );
    
    _MRE_CFOR( 8, {
      vxs[ _k ] = alloca ( ( xr - xl + 1 ) * sizeof(  MRE_F64 ) );
      MRE_Interpolate_I16_F64(
        vxs[ _k ], 
        xl, vsl[ _k ][ y - ymn ], 
        xr, vsr[ _k ][ y - ymn ]
      );
    } );

    for ( x = xl; x <= xr; ++x )
    {
      iz = vxs[ MRE_POF + 2 ][ x - xl ];
      if ( iz > _MRE_z_buff[ _MRE_buff_w * y + x ] + MRE_F64_MIN )
      {
        u = ( _MRE_texture -> w - 1 ) * vxs[ MRE_TOF + 0 ][ x - xl ] / iz;
        v = ( _MRE_texture -> h - 1 ) * vxs[ MRE_TOF + 1 ][ x - xl ] / iz;
        MRE_PIXEL_TO_COLOR(
          _MRE_texture -> pixels[ v * _MRE_texture -> w + u ],
          vcolor
        );
        MRE_DIV_VEC3_S( vcolor, 255.0, vcolor );

        MRE_SET_VEC3(
          vcolor[0] * vxs[ MRE_COF + 0 ][ x - xl ] / iz,
          vcolor[1] * vxs[ MRE_COF + 1 ][ x - xl ] / iz,
          vcolor[2] * vxs[ MRE_COF + 2 ][ x - xl ] / iz,
          vcolor
        );
        MRE_SET_VEC3(
          vxs[ MRE_POF + 0 ][ x - xl ] / iz,
          vxs[ MRE_POF + 1 ][ x - xl ] / iz,
          1.0 / iz,
          vpos
        );
        MRE_SET_VEC3( x, y, zxs[ x - xl ] / iz, vcord );

        _MRE_fragment_shader( vcolor, rvcolor, vpos, vcord );

        _MRE_buff[ _MRE_buff_w * y + x ] = MRE_RGBA_TO_PIXEL(
          255 * rvcolor[0],
          255 * rvcolor[1],
          255 * rvcolor[2],
          255
        );

        _MRE_z_buff[ _MRE_buff_w * y + x ] = iz;
      }
    }
  }
}


void
MRE_RenderTrianglesModel
(
    const MRE_F64  * const v,
    MRE_I32                vc
)
{
  MRE_UI32          j;
  MRE_Vec3          n;
  MRE_Vec4        * vproj;
  void            (*drawfun)
  (
      const MRE_F64 *, const MRE_F64 *,
      const MRE_F64 *, const MRE_F64 *,
      const MRE_F64 *, const MRE_F64 *
  );

  

  if ( _MRE_texture == NULL )  drawfun = MRE_DrawColoredTriangle;
  else                         drawfun = MRE_DrawTexturedTriangle;

  vproj = malloc( vc * sizeof( MRE_Vec4 ) );
 
  for ( j = 0; j < vc; j += 3 )
  {
    _MRE_CFOR( 3, {
      MRE_MulMat4Vec3Vec4( 
        _MRE_projection_mat, 
        v + ( j + _k ) * _MRE_vs + MRE_POF, 1.0,
        vproj[ j + _k ]
      );
      MRE_DIV_VEC3_S(
        vproj[ j + _k ], vproj[ j + _k ][3], vproj[ j + _k ]
      );
    } );
    
    drawfun(
      vproj[ j + 0 ],
      v + ( j + 0 ) * _MRE_vs,
      vproj[ j + 1 ],
      v + ( j + 1 ) * _MRE_vs,
      vproj[ j + 2 ],
      v + ( j + 2 ) * _MRE_vs
    );
  }
    
  free( vproj );
}
