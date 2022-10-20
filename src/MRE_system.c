#include <MRE_system.h>

void
MRE_VertexAttribPointer
(
    MRE_I32  vs,
    MRE_I32  vap_offset_v,
    MRE_I32  vap_offset_c,
    MRE_I32  vap_offset_t
)
{
  _MRE_vs = vs;
  _MRE_vap[ VAP_P_I ] = vap_offset_v;
  _MRE_vap[ VAP_C_I ] = vap_offset_c;
  _MRE_vap[ VAP_T_I ] = vap_offset_t;
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
