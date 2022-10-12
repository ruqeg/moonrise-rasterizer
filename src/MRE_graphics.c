#include <MRE_graphics.h>



MRE_F64   * _MRE_z_buff;
MRE_Mat4    _MRE_projection_mat;
MRE_Vec4    _MRE_view_planes[ 6 ];



void
MRE_InitZBuffer
(

)
{
  MRE_DestroyZBuffer();
  _MRE_z_buff = malloc( MRE_buff_w * MRE_buff_h * sizeof( MRE_F64 ) );
  MRE_ClearZBuffer();
}

void
MRE_ClearZBuffer
(

)
{
  for ( MRE_I32 i = 0; i < MRE_buff_w * MRE_buff_h; ++i )
  {
    _MRE_z_buff[ i ] = 0;
  }
}

void
MRE_DestroyZBuffer
(

)
{
  if ( _MRE_z_buff != NULL )  free( _MRE_z_buff );
}

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
    
  for ( y = y0; y <= y2; ++y )
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

  

  if ( y1 < y0 )
  {
    MRE_SWAP( MRE_I16, y0, y1 );
    MRE_SWAP( MRE_I16, x0, x1 );
    MRE_SWAP( MRE_F64, h0, h1 );
  }
  if ( y2 < y0 )
  {
    MRE_SWAP( MRE_I16, y0, y2 );
    MRE_SWAP( MRE_I16, x0, x2 );
    MRE_SWAP( MRE_F64, h0, h2 );
  }
  if ( y2 < y1 )
  {
    MRE_SWAP( MRE_I16, y1, y2 );
    MRE_SWAP( MRE_I16, x1, x2 );
    MRE_SWAP( MRE_F64, h1, h2 );
  }

  xs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  hs012 = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );
  xs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_I16 ) );
  hs02  = alloca( ( y2 - y0 + 1 ) * sizeof( MRE_F64 ) );

  MRE_Interpolate_I16_I16( xs012, y0, x0, y1, x1 );
  MRE_Interpolate_I16_F64( hs012, y0, h0, y1, h1 );
  MRE_Interpolate_I16_I16( xs012 + ( y1 - y0 ), y1, x1, y2, x2 );
  MRE_Interpolate_I16_F64( hs012 + ( y1 - y0 ), y1, h1, y2, h2 );
  MRE_Interpolate_I16_I16( xs02, y0, x0, y2, x2 );
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


MRE_INLINE_
void
MRE_RenderTriangle
(
    const MRE_Vec3  v0,
    const MRE_Vec3  v1,
    const MRE_Vec3  v2,
    MRE_Pixel       pixel
)
{
  MRE_DrawFilledTriangle(
    ( v0[0] * 0.5 + 0.5 ) * ( MRE_buff_w - 1 ),
    ( v0[1] * 0.5 + 0.5 ) * ( MRE_buff_h - 1 ),
    1.0 / v0[2],
    ( v1[0] * 0.5 + 0.5 ) * ( MRE_buff_w - 1 ),
    ( v1[1] * 0.5 + 0.5 ) * ( MRE_buff_h - 1 ),
    1.0 / v1[2],
    ( v2[0] * 0.5 + 0.5 ) * ( MRE_buff_w - 1 ),
    ( v2[1] * 0.5 + 0.5 ) * ( MRE_buff_h - 1 ),
    1.0 / v2[2],
    pixel
  );
}

void MRE_RenderModel
(
    const MRE_Vec3   * const vert,
    MRE_I32                  vert_count,
    const MRE_IVec4  * const triangles,
    MRE_I32                  triangles_count
)
{
  MRE_UI32  i;
  MRE_Vec4  proj_vert[ vert_count ];
 

  for ( i = 0; i < vert_count; ++i )
  {
    MRE_MulMat4Vec3Vec4( _MRE_projection_mat, vert[ i ], 1.0, proj_vert[ i ] );
    MRE_DIV_VEC3_S( proj_vert[ i ], proj_vert[ i ][3], proj_vert[ i ] );
  }

  for ( i = 0; i < triangles_count; ++i )
  {
    MRE_RenderTriangle(
      proj_vert[ triangles[ i ][0] ],
      proj_vert[ triangles[ i ][1] ],
      proj_vert[ triangles[ i ][2] ],
      triangles[ i ][3]
    );
  }
}

MRE_I32
MRE_ClipModel
(
    const MRE_Vec3   * const   vert,
    MRE_I32          * const   vert_count,
    const MRE_IVec4  * const   triangles,
    MRE_I32          * const   triangles_count,
    const MRE_Vec4             bsphere,
    MRE_Vec3         * * const d_vert,
    MRE_IVec4        * * const d_triangles
)
{
  MRE_I32      i;
  MRE_I32      status;
  MRE_I32      new_vert_count;
  MRE_I32      new_triangles_count;
  MRE_F64      bsphere_dist;
  MRE_Vec3   * buff1_vert;
  MRE_Vec3   * buff2_vert;
  MRE_IVec4  * buff1_triangles;
  MRE_IVec4  * buff2_triangles;

  

  
  new_vert_count      = *vert_count;
  new_triangles_count = *triangles_count;
  
  buff1_vert      = malloc( new_vert_count * sizeof( MRE_Vec3 ) );
  buff1_triangles = malloc( new_triangles_count * sizeof( MRE_IVec4 ) );

  memcpy( buff1_vert, vert, *vert_count * sizeof( MRE_Vec3 ));
  memcpy( buff1_triangles, triangles, *triangles_count * sizeof( MRE_IVec4 ));

  status = -2;

  
  for ( i = 0; i < 6; ++i )
  {
    bsphere_dist = MRE_PLANE_DIST( bsphere, _MRE_view_planes[ i ] );
  
    if ( bsphere_dist > bsphere[3] )
    {
      if ( status != MRE_MODEL_CLIPED )
      {
        status = MRE_MODEL_INPL;
      }
      continue;
    }
    if ( bsphere_dist < -bsphere[3] )
    {
      *vert_count = 0;
      *triangles_count = 0;
      return MRE_MODEL_OUPL;
    }
   
    status = MRE_MODEL_CLIPED;

    buff2_vert      = malloc( 3 * new_vert_count * sizeof( MRE_Vec3 ) );
    buff2_triangles = malloc( 3 * new_triangles_count * sizeof( MRE_IVec4 ) );

    //TODO input vert 
    MRE_ClipTrianglesAgainstPlane(
      _MRE_view_planes[ i ],
      buff1_vert, &new_vert_count,
      buff1_triangles, &new_triangles_count,
      buff2_vert, buff2_triangles
    );

    MRE_SWAP( MRE_Vec3 *, buff1_vert, buff2_vert );
    MRE_SWAP( MRE_IVec4 *, buff1_triangles, buff2_triangles );

    free( buff2_vert );
    free( buff2_triangles );
  }
  
  
  *vert_count      = new_vert_count;
  *triangles_count = new_triangles_count;
  
  if ( buff1_vert == NULL )
  {
    MRE_SWAP( MRE_Vec3 *, buff2_vert, *d_vert );
    MRE_SWAP( MRE_IVec4 *, buff2_triangles, *d_triangles );
  }
  else
  {
    MRE_SWAP( MRE_Vec3 *, buff1_vert, *d_vert );
    MRE_SWAP( MRE_IVec4 *, buff1_triangles, *d_triangles );
  }
  
  return status;
}


#define __ADD_VERT_TO_NEW_VERTARR( ind )\
({                                             \
  if ( vert_inds[ v_ind[ ind ] ] == -1 )       \
  {                                            \
    vert_inds[ v_ind[ ind ] ] = new_vert_count;\
    MRE_COPY_VEC3(                             \
      vert[ v_ind[ ind ] ],                    \
      d_vert[ new_vert_count ]                 \
    );                                         \
    ++new_vert_count;                          \
  }                                            \
})

MRE_I32
MRE_ClipTrianglesAgainstPlane
(
    const MRE_Vec4           plane,
    const MRE_Vec3   * const vert,
    MRE_I32          * const vert_count,
    const MRE_IVec4  * const triangles,
    MRE_I32          * const triangles_count,
    MRE_Vec3         * const d_vert,
    MRE_IVec4        * const d_triangles
)
{
  MRE_I32    j;
  MRE_I32    tmp;
  MRE_I32    new_vert_count;
  MRE_I32    new_triangles_count;
  MRE_IVec3  v_ind;
  MRE_Vec3   v_dist;
  MRE_I32    vert_inds[ *vert_count ];

 

  memset( vert_inds, -1, *vert_count * sizeof( MRE_I32 ) );
  new_vert_count      = 0;
  new_triangles_count = 0;

  for ( j = 0; j < *triangles_count; ++j )
  {
    MRE_COPY_VEC3( triangles[ j ], v_ind );
    v_dist[0] = MRE_PLANE_DIST( vert[ v_ind[0] ], plane );
    v_dist[1] = MRE_PLANE_DIST( vert[ v_ind[1] ], plane );
    v_dist[2] = MRE_PLANE_DIST( vert[ v_ind[2] ], plane );
    
    
    MRE_SORT3(
      MRE_F64,
      v_dist[0], v_dist[1], v_dist[2],
      MRE_SWAP( MRE_I32, v_ind[0], v_ind[1] ),
      MRE_SWAP( MRE_I32, v_ind[1], v_ind[2] )
    );
    
    if ( v_dist[2] < 0 )  continue;

    if ( v_dist[1] < 0 )
    {
      __ADD_VERT_TO_NEW_VERTARR( 2 );

      MRE_INTERSECTION_PLANE(
        d_vert[ vert_inds[ v_ind[2] ] ],
        vert[ v_ind[1] ],
        plane,
        d_vert[ new_vert_count ]
      );
      vert_inds[ v_ind[1] ] = new_vert_count++;
      
      MRE_INTERSECTION_PLANE(
        d_vert[ vert_inds[ v_ind[2] ] ], 
        vert[ v_ind[0] ], 
        plane,
        d_vert[ new_vert_count ]
      );
      vert_inds[ v_ind[0] ] = new_vert_count++;
      
      d_triangles[ new_triangles_count ][0] = vert_inds[ triangles[ j ][0] ];
      d_triangles[ new_triangles_count ][1] = vert_inds[ triangles[ j ][1] ];
      d_triangles[ new_triangles_count ][2] = vert_inds[ triangles[ j ][2] ];
      d_triangles[ new_triangles_count ][3] = triangles[ j ][3];
      
      ++new_triangles_count;
    }
    else if ( v_dist[0] < 0 )
    {
      __ADD_VERT_TO_NEW_VERTARR( 1 );
      __ADD_VERT_TO_NEW_VERTARR( 2 );

      MRE_INTERSECTION_PLANE(
        d_vert[ vert_inds[ v_ind[2] ] ],
        vert[ v_ind[0] ],
        plane,
        d_vert[ new_vert_count ]
      );
      vert_inds[ v_ind[0] ] = new_vert_count++;

      d_triangles[ new_triangles_count ][0] = vert_inds[ triangles[ j ][0] ];
      d_triangles[ new_triangles_count ][1] = vert_inds[ triangles[ j ][1] ];
      d_triangles[ new_triangles_count ][2] = vert_inds[ triangles[ j ][2] ];
      d_triangles[ new_triangles_count ][3] = triangles[ j ][3];
      ++new_triangles_count;
      
      tmp = vert_inds[ v_ind[2] ];
      vert_inds[ v_ind[2] ] = vert_inds[ v_ind[0] ];

      MRE_INTERSECTION_PLANE(
        d_vert[ vert_inds[ v_ind[1] ] ], 
        vert[ v_ind[0] ],
        plane,
        d_vert[ new_vert_count ]
      );
      vert_inds[ v_ind[0] ] = new_vert_count++;
      
      
      d_triangles[ new_triangles_count ][0] = vert_inds[ triangles[ j ][0] ];
      d_triangles[ new_triangles_count ][1] = vert_inds[ triangles[ j ][1] ];
      d_triangles[ new_triangles_count ][2] = vert_inds[ triangles[ j ][2] ];
      d_triangles[ new_triangles_count ][3] = triangles[ j ][3];
      
      ++new_triangles_count;
      vert_inds[ v_ind[2] ] = tmp;
    }
    else
    {
      __ADD_VERT_TO_NEW_VERTARR( 0 );
      __ADD_VERT_TO_NEW_VERTARR( 1 );
      __ADD_VERT_TO_NEW_VERTARR( 2 );
    
      d_triangles[ new_triangles_count ][0] = vert_inds[ triangles[ j ][0] ];
      d_triangles[ new_triangles_count ][1] = vert_inds[ triangles[ j ][1] ];
      d_triangles[ new_triangles_count ][2] = vert_inds[ triangles[ j ][2] ];
      d_triangles[ new_triangles_count ][3] = triangles[ j ][3];
      ++new_triangles_count;
    }
  }
  *vert_count = new_vert_count;
  *triangles_count = new_triangles_count;
}


void
MRE_ClipBackFaces
(
    const MRE_Vec3   * const vert,
    MRE_I32          * const vert_count,
    const MRE_IVec4  * const triangles,
    MRE_I32          * const triangles_count,
    MRE_Vec3         * const d_vert,
    MRE_IVec4        * const d_triangles
)
{
  MRE_UI32         j;
  MRE_I32          new_triangles_count;
  MRE_I32          new_vert_count;
  MRE_IVec3        v_ind;
  const MRE_F64  * v1;
  const MRE_F64  * v2;
  const MRE_F64  * v3;
  MRE_I32          vert_inds[ *vert_count ];

  
  
  memset( vert_inds, -1, *vert_count * sizeof( MRE_I32 ) );

  new_triangles_count = 0;
  new_vert_count      = 0;
 
  
  for ( j = 0; j < *triangles_count; ++j )
  {
    MRE_COPY_VEC3( triangles[ j ], v_ind );

    v1 = vert[ triangles[ j ][0] ];
    v2 = vert[ triangles[ j ][1] ];
    v3 = vert[ triangles[ j ][2] ];

    MRE_F64 nx = MRE_DET2_S(
      v2[1] - v1[1],  v2[2] - v1[2],
      v3[1] - v1[1],  v3[2] - v1[2]
    );
    MRE_F64 ny = -MRE_DET2_S(
      v2[0] - v1[0],  v2[2] - v1[2],
      v3[0] - v1[0],  v3[2] - v1[2]
    );
    MRE_F64 nz = MRE_DET2_S(
      v2[0] - v1[0],  v2[1] - v1[1],
      v3[0] - v1[0],  v3[1] - v1[1]
    );

    if
    (
      nx * v1[0] + ny * v1[1] + nz * v1[2] <= 0
      ||
      nx * v2[0] + ny * v2[1] + nz * v2[2] <= 0
      ||
      nx * v3[0] + ny * v3[1] + nz * v3[2] <= 0
    )
    {
      __ADD_VERT_TO_NEW_VERTARR( 0 );
      __ADD_VERT_TO_NEW_VERTARR( 1 );
      __ADD_VERT_TO_NEW_VERTARR( 2 );
      
      d_triangles[ new_triangles_count ][0] = vert_inds[ v_ind[0] ];
      d_triangles[ new_triangles_count ][1] = vert_inds[ v_ind[1] ];
      d_triangles[ new_triangles_count ][2] = vert_inds[ v_ind[2] ];
      d_triangles[ new_triangles_count ][3] = triangles[ j ][3];
      

      ++new_triangles_count;
    }
  }
  *triangles_count = new_triangles_count;
  *vert_count      = new_vert_count;
}

void
MRE_SetPrespsectiveView
(
    MRE_F64  fowy,
    MRE_F64  aspect,
    MRE_F64  z_min,
    MRE_F64  z_max
)
{
  const MRE_F64 fowx_2 = atan( aspect * tan( fowy * 0.5 ) );

  MRE_PerspectiveMat4(
    fowy,
    aspect,
    z_min,
    z_max,
    _MRE_projection_mat
  );

  _MRE_view_planes[0][0] =  0;
  _MRE_view_planes[0][1] =  0;
  _MRE_view_planes[0][2] =  1;
  _MRE_view_planes[0][3] = -z_min;

  _MRE_view_planes[1][0] =  0;
  _MRE_view_planes[1][1] =  0;
  _MRE_view_planes[1][2] = -1;
  _MRE_view_planes[1][3] =  z_max;

  
  _MRE_view_planes[2][0] =  0;
  _MRE_view_planes[2][1] =  cos( fowy * 0.5 );
  _MRE_view_planes[2][2] =  sin( fowy * 0.5 );
  _MRE_view_planes[2][3] =  0;
  
  _MRE_view_planes[3][0] =  0;
  _MRE_view_planes[3][1] = -( _MRE_view_planes[2][1] );
  _MRE_view_planes[3][2] =  _MRE_view_planes[2][2];
  _MRE_view_planes[3][3] =  0;

  _MRE_view_planes[4][0] =  cos( fowx_2 );
  _MRE_view_planes[4][1] =  0;
  _MRE_view_planes[4][2] =  sin( fowx_2 );
  _MRE_view_planes[4][3] =  0;
  
  _MRE_view_planes[5][0] = -( _MRE_view_planes[4][0] );
  _MRE_view_planes[5][1] =  0;
  _MRE_view_planes[5][2] =  _MRE_view_planes[4][2];
  _MRE_view_planes[5][3] =  0;
}
