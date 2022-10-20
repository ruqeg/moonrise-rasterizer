#include <MRE_geometry.h>

MRE_I32
MRE_ClipModel
(
    const MRE_F64   * const   v,
    MRE_I32         * const   vc,
    const MRE_I32   * const   t,
    MRE_I32         * const   tc,
    const MRE_Vec4            bsphere,
    MRE_F64         * * const dv,
    MRE_I32         * * const dt
)
{
  MRE_I32      i;
  MRE_I32      status;
  MRE_I32      nvc;
  MRE_I32      ntc;
  MRE_F64      bsphere_dist;
  MRE_F64    * buff1_v;
  MRE_F64    * buff2_v;
  MRE_I32    * buff1_t;
  MRE_I32    * buff2_t;

  


  nvc = *vc;
  ntc = *tc;
  
  buff1_v = malloc( nvc * _MRE_vs * sizeof( MRE_F64 ) );
  buff1_t = malloc( ntc * 3 * sizeof( MRE_I32 ) );

  memcpy( buff1_v, v, *vc * _MRE_vs * sizeof( MRE_F64 ) );
  memcpy( buff1_t, t, *tc * 3 * sizeof( MRE_I32 ));

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
      *tc = 0;
      return MRE_MODEL_OUPL;
    }
     
    status = MRE_MODEL_CLIPED;

    buff2_v = malloc( 3 * nvc * _MRE_vs * sizeof( MRE_F64 ) );
    buff2_t = malloc( 3 * ntc * 3 * sizeof( MRE_I32 ) );

    MRE_ClipTrianglesAgainstPlane(
      _MRE_view_planes[ i ],
      buff1_v, &nvc,
      buff1_t, &ntc,
      buff2_v, buff2_t
    );

    MRE_SWAP( MRE_F64 *, buff1_v, buff2_v );
    MRE_SWAP( MRE_I32 *, buff1_t, buff2_t );

    free( buff2_v );
    free( buff2_t );
  }
  
  
  *vc = nvc;
  *tc = ntc;
  
  if ( buff1_v == NULL )
  {
    MRE_SWAP( MRE_F64 *, buff2_v, *dv );
    MRE_SWAP( MRE_I32 *, buff2_t, *dt );
  }
  else
  {
    MRE_SWAP( MRE_F64 *, buff1_v, *dv );
    MRE_SWAP( MRE_I32 *, buff1_t, *dt );
  }
  
  return status;
}


#define __ADD_VERT_TO_NEW_VERTARR( ind )\
({                                                       \
  if ( vninds[ st[ ind ] ] == -1 )                       \
  {                                                      \
    vninds[ st[ ind ] ] = nvc;                           \
    for ( MRE_I32 kc = 0; kc < _MRE_vs; ++kc )           \
    {                                                    \
      dv[ nvc * _MRE_vs + _MRE_vap[ VAP_P_I ] + kc ] = v[\
        st[ ind ] * _MRE_vs + _MRE_vap[ VAP_P_I ] + kc   \
      ];                                                 \
    }                                                    \
    ++nvc;                                               \
  }                                                      \
})

#define __CALC_INTERSECTION_VERTEX( inv, otv )\
({                                            \
    MRE_F64 a = MRE_IntersectionPlane(        \
      inv + _MRE_vap[ VAP_P_I ],              \
      otv + _MRE_vap[ VAP_P_I ],              \
      plane,                                  \
      dv + _MRE_vs * nvc + _MRE_vap[ VAP_P_I ]\
    );                                        \
    MRE_VEC3_COEFF(                           \
      inv + _MRE_vap[ VAP_C_I ],              \
      otv + _MRE_vap[ VAP_C_I ],              \
      a,                                      \
      dv + _MRE_vs * nvc + _MRE_vap[ VAP_C_I ]\
    );                                        \
})

#define __ADD_TRIANGLE_TO_NEW_TRIANGLEARR()\
({                                             \
  dt[ 3 * ntc ]     = vninds[ t[ 3 * j ] ];    \
  dt[ 3 * ntc + 1 ] = vninds[ t[ 3 * j + 1 ] ];\
  dt[ 3 * ntc + 2 ] = vninds[ t[ 3 * j + 2 ] ];\
  ++ntc;\
})

MRE_I32
MRE_ClipTrianglesAgainstPlane
(
    const MRE_Vec4           plane,
    const MRE_F64    * const v,
    MRE_I32          * const vc,
    const MRE_I32    * const t,
    MRE_I32          * const tc,
    MRE_F64          * const dv,
    MRE_I32          * const dt
)
{
  MRE_I32    j;
  MRE_I32    tmp;
  MRE_I32    nvc;
  MRE_I32    ntc;
  MRE_IVec3  st;
  MRE_Vec3   vdist;
  MRE_I32    vninds[ *vc ];

 

  memset( vninds, -1, *vc * sizeof( MRE_I32 ) );
  nvc = 0;
  ntc = 0;

  for ( j = 0; j < *tc; ++j )
  {
    MRE_COPY_VEC3( t + j * 3, st );

    _MRE_CFOR( 3, {
      vdist[ _k ] = MRE_PLANE_DIST(
        v + _MRE_vs * st[ _k ] + _MRE_vap[ VAP_P_I ], plane
      );
    } );
    
    
    MRE_SORT3(
      MRE_F64,
      vdist[0], vdist[1], vdist[2],
      MRE_SWAP( MRE_I32, st[0], st[1] ),
      MRE_SWAP( MRE_I32, st[1], st[2] )
    );
    
    if ( vdist[2] < 0 )  continue;

    if ( vdist[1] < 0 )
    {
      __ADD_VERT_TO_NEW_VERTARR( 2 );

      _MRE_CFOR( 2, {
        __CALC_INTERSECTION_VERTEX( 
          dv + _MRE_vs * vninds[ st[2] ],
          v + _MRE_vs * st[ _k ]
        );
        vninds[ st[ _k ] ] = nvc++;
      } );
 
      __ADD_TRIANGLE_TO_NEW_TRIANGLEARR();
    }
    else if ( vdist[0] < 0 )
    {
      __ADD_VERT_TO_NEW_VERTARR( 1 );
      __ADD_VERT_TO_NEW_VERTARR( 2 );

      __CALC_INTERSECTION_VERTEX(
        dv + _MRE_vs * vninds[ st[2] ],
        v + _MRE_vs * st[0]
      );
      vninds[ st[0] ] = nvc++;

      __ADD_TRIANGLE_TO_NEW_TRIANGLEARR();
      
     
      tmp = vninds[ st[2] ];
      vninds[ st[2] ] = vninds[ st[0] ];

      
      __CALC_INTERSECTION_VERTEX(
        dv + _MRE_vs * vninds[ st[1] ],
        v + _MRE_vs * st[0]
      );
      vninds[ st[0] ] = nvc++;
    
      __ADD_TRIANGLE_TO_NEW_TRIANGLEARR();


      vninds[ st[2] ] = tmp;
    }
    else
    {
      __ADD_VERT_TO_NEW_VERTARR( 0 );
      __ADD_VERT_TO_NEW_VERTARR( 1 );
      __ADD_VERT_TO_NEW_VERTARR( 2 );
    
      __ADD_TRIANGLE_TO_NEW_TRIANGLEARR();
    }
  }
  *vc = nvc;
  *tc = ntc;
}

void
MRE_ClipBackFaces
(
    const MRE_F64  * const v,
    MRE_I32        * const vc,
    const MRE_I32  * const t,
    MRE_I32        * const tc,
    MRE_F64        * const dv,
    MRE_I32        * const dt
)
{
  MRE_UI32         j;
  MRE_I32          ntc;
  MRE_I32          nvc;
  MRE_IVec3        st;
  MRE_Vec3         n;
  const MRE_F64  * v1;
  const MRE_F64  * v2;
  const MRE_F64  * v3;
  MRE_I32          vninds[ *vc ];

  
  
  memset( vninds, -1, *vc * sizeof( MRE_I32 ) );

  ntc = 0;
  nvc = 0;
 
  
  for ( j = 0; j < *tc; ++j )
  {
    MRE_COPY_VEC3( t + j * 3, st );
    v1 = v + t[ j * 3 + 0 ] * _MRE_vs + _MRE_vap[ VAP_P_I ];
    v2 = v + t[ j * 3 + 1 ] * _MRE_vs + _MRE_vap[ VAP_P_I ];
    v3 = v + t[ j * 3 + 2 ] * _MRE_vs + _MRE_vap[ VAP_P_I ];

    MRE_TriangleNormal( v1, v2, v3, n );

    if
    (
      MRE_SCMUL_VEC3( n, v1 ) <= 0
      ||
      MRE_SCMUL_VEC3( n, v2 ) <= 0
      ||
      MRE_SCMUL_VEC3( n, v3 ) <= 0
    )
    {
      __ADD_VERT_TO_NEW_VERTARR( 0 );
      __ADD_VERT_TO_NEW_VERTARR( 1 );
      __ADD_VERT_TO_NEW_VERTARR( 2 );
      
      dt[ ntc * 3 + 0 ] = vninds[ st[0] ];
      dt[ ntc * 3 + 1 ] = vninds[ st[1] ];
      dt[ ntc * 3 + 2 ] = vninds[ st[2] ];
      
      ++ntc;
    }
  }

  *tc = ntc;
  *vc = nvc;
}


void
MRE_CreateSphere
(
    MRE_I32            vpc,
    MRE_I32            hpc,
    MRE_F64    * const dv,
    MRE_I32    * const dt
)
{
  MRE_I32  vp;
  MRE_I32  hp;
  MRE_I32  vind;
  MRE_I32  tind;
  MRE_F64  xang_step;
  MRE_F64  yang_step;
  MRE_F64  sin_yang;
  MRE_F64  cos_yang;
  MRE_F64  sin_xangs[ hpc ];
  MRE_F64  cos_xangs[ hpc ];


  

  xang_step = 2.0 * MRE_PI / hpc;
  yang_step = MRE_PI / vpc;
    
  vind = 0;  
  tind = 0;
  
  for ( hp = 0; hp < hpc; ++hp )
  {
    sin_xangs[ hp ] = sin( hp * xang_step );
    cos_xangs[ hp ] = cos( hp * xang_step );
  }

  for ( vp = 1; vp < vpc; ++vp )
  {
    cos_yang = cos( MRE_PI_2 + yang_step * vp );
    sin_yang = sin( MRE_PI_2 + yang_step * vp );

    for ( hp = 0; hp < hpc; ++hp )
    {
      MRE_SET_VEC3(
        cos_yang * cos_xangs[ hp ],
        sin_yang,
        cos_yang * sin_xangs[ hp ],
        dv + vind * _MRE_vs + _MRE_vap[ VAP_P_I ]
      );
      MRE_SET_VEC3(
        1.0, 1.0, 1.0,
        dv + vind * _MRE_vs + _MRE_vap[ VAP_C_I ]
      );
      MRE_SET_VEC2(
        1.0, 1.0,
        dv + vind * _MRE_vs + _MRE_vap[ VAP_T_I ]
      );
      ++vind;
    }
  }

  MRE_SET_VEC3( 0.0,  1.0, 0.0, dv + vind * _MRE_vs + _MRE_vap[ VAP_P_I] + 0 );
  MRE_SET_VEC3( 0.0, -1.0, 0.0, dv + vind * _MRE_vs + _MRE_vap[ VAP_P_I] + 1 );
 

  for ( vp = 0; vp < vpc - 2; ++vp )
  {
    for ( hp = 0; hp < hpc; ++hp )
    {
      dt[ 3 * tind + 0 ] = ( vp + 1 ) * hpc + hp;
      dt[ 3 * tind + 1 ] = vp * hpc + hp;
      dt[ 3 * tind + 2 ] = ( vp + 1 ) * hpc + ( hp + 1 ) % hpc;
      ++tind;
      

      dt[ 3 * tind + 0 ] = vp * hpc + hp;
      dt[ 3 * tind + 1 ] = vp * hpc + ( hp + 1 ) % hpc;
      dt[ 3 * tind + 2 ] = ( vp + 1 ) * hpc + ( hp + 1 ) % hpc;
      ++tind;
    }
  }


  
  for ( hp = 0; hp < hpc; ++hp )
  {
    dt[ 3 * tind + 0 ] = vind;
    dt[ 3 * tind + 1 ] = ( hp + 1 ) % hpc;
    dt[ 3 * tind + 2 ] = hp;
    ++tind;
  }

 
  for ( hp = 0; hp < hpc; ++hp )
  {
    dt[ 3 * tind + 0 ] = (vpc - 2) * hpc + hp;
    dt[ 3 * tind + 1 ] = (vpc - 2) * hpc + ( hp + 1 ) % hpc;
    dt[ 3 * tind + 2 ] = vind + 1;
    ++tind;
  }
}
