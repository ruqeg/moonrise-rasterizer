#include <MRE_geometry.h>

MRE_I32
MRE_ClipModel
(
    const MRE_F64   * const   v,
    MRE_I32         * const   vc,
    const MRE_Vec4            bsphere,
    MRE_F64         * * const dv
)
{
  MRE_I32      i;
  MRE_I32      status;
  MRE_I32      nvc;
  MRE_F64      bsphere_dist;
  MRE_F64    * buff1_v;
  MRE_F64    * buff2_v;

  


  nvc = *vc;
  
  buff1_v = malloc( nvc * _MRE_vs * sizeof( MRE_F64 ) );

  memcpy( buff1_v, v, *vc * _MRE_vs * sizeof( MRE_F64 ) );

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
      *vc = 0;
      return MRE_MODEL_OUPL;
    }
     
    status = MRE_MODEL_CLIPED;

    buff2_v = malloc( 3 * nvc * _MRE_vs * sizeof( MRE_F64 ) );

    MRE_ClipTrianglesAgainstPlane(
      _MRE_view_planes[ i ],
      buff1_v, &nvc,
      buff2_v
    );

    MRE_SWAP( MRE_F64 *, buff1_v, buff2_v );

    free( buff2_v );
  }
  
  
  *vc = nvc;
  
  if ( buff1_v == NULL )  MRE_SWAP( MRE_F64 *, buff2_v, *dv );
  else                    MRE_SWAP( MRE_F64 *, buff1_v, *dv );
  
  return status;
}


#define __ADD_VERT_TO_NEW_VERTARR( ind )\
({                                           \
  if ( vninds[ st[ ind ] ] == -1 )           \
  {                                          \
    vninds[ st[ ind ] ] = nvc;               \
    _MRE_CFOR( _MRE_vs, {                    \
      dv[ nvc * _MRE_vs + MRE_POF + _k ] = v[\
        st[ ind ] * _MRE_vs + MRE_POF + _k   \
      ];                                     \
    } );                                     \
    ++nvc;                                   \
  }                                          \
})

#define __ADD_INTERSECTION_VERTEX( _v1, _v2, _dv )\
({                                  \
  MRE_F64 a = MRE_IntersectionPlane(\
    _v1 + MRE_POF,                  \
    _v2 + MRE_POF,                  \
    plane,                          \
    _dv + MRE_POF                   \
  );                                \
  MRE_VEC3_COEFF(                   \
    _v1 + MRE_COF,                  \
    _v2 + MRE_COF,                  \
    a,                              \
    _dv + MRE_COF                   \
  );                                \
                                    \
  if ( _MRE_texture != NULL )       \
  {                                 \
    MRE_VEC3_COEFF(                 \
      _v1 + MRE_TOF,                \
      _v2 + MRE_TOF,                \
      a,                            \
      _dv + MRE_TOF                 \
    );                              \
  }                                 \
})
#define __COPY_VERTEX( v, dv )\
({                                                      \
  for ( i = 0; i < _MRE_vs; ++i )  (dv)[ i ] = (v)[ i ];\
})


MRE_I32
MRE_ClipTrianglesAgainstPlane
(
    const MRE_Vec4           plane,
    const MRE_F64    * const v,
    MRE_I32          * const vc,
    MRE_F64          * const dv
)
{
  MRE_I32          i;
  MRE_I32          j;
  MRE_I32          nvc;
  const MRE_F64  * v0;
  const MRE_F64  * v1;
  const MRE_F64  * v2;
  MRE_F64        * dv0;
  MRE_F64        * dv1;
  MRE_F64        * dv2;
  MRE_IVec3        svj;
  MRE_Vec3         vdist;

 

  nvc = 0;

  for ( j = 0; j < *vc; j += 3 )
  {
    MRE_SET_VEC3( 0, 1, 2, svj );

    _MRE_CFOR( 3, {
      vdist[ _k ] = MRE_PLANE_DIST(
        v + ( j + _k ) * _MRE_vs + MRE_POF, plane
      );
    } );
    
    MRE_SORT3(
      MRE_F64,
      vdist[0], vdist[1], vdist[2],
      MRE_SWAP( MRE_I32, svj[0], svj[1] ),
      MRE_SWAP( MRE_I32, svj[1], svj[2] )
    );
    
    
    if ( vdist[2] < 0 )  continue;

    //should i make _cfor???
    v0 = v + ( j + svj[0] ) * _MRE_vs;
    v1 = v + ( j + svj[1] ) * _MRE_vs;
    v2 = v + ( j + svj[2] ) * _MRE_vs;
    
    dv0 = dv + ( nvc + svj[0] ) * _MRE_vs;
    dv1 = dv + ( nvc + svj[1] ) * _MRE_vs;
    dv2 = dv + ( nvc + svj[2] ) * _MRE_vs;

    if ( vdist[1] < 0 )
    {
      __ADD_INTERSECTION_VERTEX( v2, v0, dv0 );
      __ADD_INTERSECTION_VERTEX( v2, v1, dv1 );
      __COPY_VERTEX( v2, dv2 );

      nvc += 3;
    }
    else if ( vdist[0] < 0 )
    {
      __ADD_INTERSECTION_VERTEX( v1, v0, dv0 );
      __COPY_VERTEX( v1, dv1 );
      __COPY_VERTEX( v2, dv2 );


      __ADD_INTERSECTION_VERTEX( v2, v0, dv0 + 3 * _MRE_vs );
      __COPY_VERTEX( dv0, dv1 + 3 * _MRE_vs );
      __COPY_VERTEX( v2, dv2 + 3 * _MRE_vs );
      
      nvc += 6; 
    }
    else
    {
      __COPY_VERTEX( v0, dv0 );
      __COPY_VERTEX( v1, dv1 );
      __COPY_VERTEX( v2, dv2 );
     
      nvc += 3;
    }
  }
  *vc = nvc;
}

void
MRE_ClipBackFaces
(
    const MRE_F64  * const v,
    MRE_I32        * const vc,
    MRE_F64        * const dv
)
{
  MRE_I32          j;
  MRE_I32          i;
  MRE_I32          nvc;
  MRE_Vec3         n;
  const MRE_F64  * v0;
  const MRE_F64  * v1;
  const MRE_F64  * v2;

  
  
  nvc = 0;
  
  for ( j = 0; j < *vc; j += 3 )
  {
    v0 = v + ( j + 0 ) * _MRE_vs;
    v1 = v + ( j + 1 ) * _MRE_vs;
    v2 = v + ( j + 2 ) * _MRE_vs;

    MRE_TriangleNormal(
      v0 + MRE_POF,
      v1 + MRE_POF,
      v2 + MRE_POF,
      n
    );

    if
    (
      MRE_SCMUL_VEC3( n, v0 + MRE_POF ) <= 0
      ||
      MRE_SCMUL_VEC3( n, v1 + MRE_POF ) <= 0
      ||
      MRE_SCMUL_VEC3( n, v2 + MRE_POF ) <= 0
    )
    {
      for ( i = 0; i < _MRE_vs; ++i )
      {
        dv[ ( nvc + 0 ) * _MRE_vs + i ] = v0[ i ];
        dv[ ( nvc + 1 ) * _MRE_vs + i ] = v1[ i ];
        dv[ ( nvc + 2 ) * _MRE_vs + i ] = v2[ i ];
      }
      
      nvc += 3;
    }
  }

  *vc = nvc;
}
