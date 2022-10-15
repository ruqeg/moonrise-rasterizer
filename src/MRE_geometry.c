#include <MRE_geometry.h>

void
MRE_CreateCube
(
    MRE_Vec3   * const dv,
    MRE_IVec4  * const dt
)
{
  MRE_SET_VEC3(  1,  1,  1, dv[0] );
  MRE_SET_VEC3( -1,  1,  1, dv[1] );
  MRE_SET_VEC3( -1, -1,  1, dv[2] );
  MRE_SET_VEC3(  1, -1,  1, dv[3] );
  MRE_SET_VEC3(  1,  1, -1, dv[4] );
  MRE_SET_VEC3( -1,  1, -1, dv[5] );
  MRE_SET_VEC3( -1, -1, -1, dv[6] );
  MRE_SET_VEC3(  1, -1, -1, dv[7] );


  //TODO
  MRE_SET_VEC4( 0, 1, 2, 0x000000FFu, dt[0] );
  MRE_SET_VEC4( 0, 2, 3, 0x000000FFu, dt[1] );
  MRE_SET_VEC4( 4, 0, 3, 0xFFFF00FFu, dt[2] );
  MRE_SET_VEC4( 4, 3, 7, 0xFFFF00FFu, dt[3] );
  MRE_SET_VEC4( 5, 4, 7, 0xFF0000FFu, dt[4] );
  MRE_SET_VEC4( 5, 7, 6, 0xFF0000FFu, dt[5] );
  MRE_SET_VEC4( 1, 5, 6, 0x00FF00FFu, dt[6] );
  MRE_SET_VEC4( 1, 6, 2, 0x00FF00FFu, dt[7] );
  MRE_SET_VEC4( 4, 5, 1, 0x00F0F0FFu, dt[8] );
  MRE_SET_VEC4( 4, 1, 0, 0x00F0F0FFu, dt[9] );
  MRE_SET_VEC4( 2, 6, 7, 0xFFF0F0FFu, dt[10] );
  MRE_SET_VEC4( 2, 7, 3, 0xFFF0F0FFu, dt[11] );
}

void
MRE_CreateSphere
(
    MRE_I32            vpc,
    MRE_I32            hpc,
    MRE_Vec3   * const dv,
    MRE_IVec4  * const dt
)
{
  MRE_I32  vp;
  MRE_I32  hp;
  MRE_I32  v_ind;
  MRE_I32  t_ind;
  MRE_F64  xang_step;
  MRE_F64  yang_step;
  MRE_F64  sin_yang;
  MRE_F64  cos_yang;
  MRE_F64  sin_xangs[ hpc ];
  MRE_F64  cos_xangs[ hpc ];


  

  xang_step = 2.0 * MRE_PI / hpc;
  yang_step = MRE_PI / vpc;
    
  v_ind = 0;  
  t_ind = 0;
  
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
      dv[ v_ind ][0] = cos_yang * cos_xangs[ hp ];
      dv[ v_ind ][1] = sin_yang;
      dv[ v_ind ][2] = cos_yang * sin_xangs[ hp ];
      ++v_ind;
    }
  }

  MRE_SET_VEC3( 0.0,  1.0, 0.0, dv[ v_ind ] );
  MRE_SET_VEC3( 0.0, -1.0, 0.0, dv[ v_ind + 1 ] );
 

  srand( time(NULL));
  
  for ( vp = 0; vp < vpc - 2; ++vp )
  {
    for ( hp = 0; hp < hpc; ++hp )
    {
      dt[ t_ind ][0] = ( vp + 1 ) * hpc + hp;
      dt[ t_ind ][1] = vp * hpc + hp;
      dt[ t_ind ][2] = ( vp + 1 ) * hpc + ( hp + 1 ) % hpc;
      dt[ t_ind ][3] = MRE_RGBA_TO_PIXEL(
        rand(), rand(), rand(), 255
      );
      ++t_ind;
      

      dt[ t_ind ][0] = vp * hpc + hp;
      dt[ t_ind ][1] = vp * hpc + ( hp + 1 ) % hpc;
      dt[ t_ind ][2] = ( vp + 1 ) * hpc + ( hp + 1 ) % hpc;
      dt[ t_ind ][3] = MRE_RGBA_TO_PIXEL(
        rand(), rand(), rand(), 255
      );
      ++t_ind;
    }
  }


  
  for ( hp = 0; hp < hpc; ++hp )
  {
    dt[ t_ind ][0] = v_ind;
    dt[ t_ind ][1] = ( hp + 1 ) % hpc;
    dt[ t_ind ][2] = hp;
    dt[ t_ind ][3] = MRE_RGBA_TO_PIXEL(
      rand(), rand(), rand(), 255
    );
    ++t_ind;
  }

 
  for ( hp = 0; hp < hpc; ++hp )
  {
    dt[ t_ind ][0] = (vpc - 2) * hpc + hp;
    dt[ t_ind ][1] = (vpc - 2) * hpc + ( hp + 1 ) % hpc;
    dt[ t_ind ][2] = v_ind + 1;
    dt[ t_ind ][3] = MRE_RGBA_TO_PIXEL(
      rand(), rand(), rand(), 255
    );
    ++t_ind;
  }
}
