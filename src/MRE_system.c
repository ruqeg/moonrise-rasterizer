#include <MRE_system.h>

void
MRE_SetBuffer
(
    MRE_UI32  * const buff,
    MRE_I16           w,
    MRE_I16           h
)
{
  _MRE_buff = buff;
  _MRE_buff_w = w;
  _MRE_buff_h = h;
}

void
MRE_SetVertexAttribSize
(
    MRE_I32  vs
)
{
  _MRE_vs = vs;
}

void
MRE_BindTexture
(
    const struct MRE_Texture  * const t
)
{
  _MRE_texture = t;
}

void
MRE_BindVertexShader
(
    MRE_VertShader  shader
)
{
  _MRE_vertex_shader = shader;
}

void
MRE_BindFragmentShader
(
    MRE_FragShader  shader
)
{
  _MRE_fragment_shader = shader;
}

void
MRE_DrawArrays
(
    MRE_I32                 type,
    const MRE_F64   * const v,
    MRE_I32                 vc,
    const MRE_Vec4          bsphere
)
{
  MRE_I32    j;
  MRE_I32    k;
  MRE_I32    status;
  MRE_I32    nvc;
  MRE_F64  * vworld;
  MRE_F64  * vcliped;
  MRE_F64  * vres;

  vworld = malloc( vc * _MRE_vs * sizeof( MRE_F64 ) );

  for ( j = 0; j < vc; ++j )
  {
    for ( k = MRE_COF; k < _MRE_vs; ++k )
    {
      vworld[ j * _MRE_vs + k ] = v[ j * _MRE_vs + k  ];
    }
    _MRE_vertex_shader(
      v + j * _MRE_vs + MRE_POF,
      vworld + j * _MRE_vs + MRE_POF,
      v + j * _MRE_vs
    );
  }
      
  nvc = vc;

  status = MRE_ClipModel( vworld, &nvc, bsphere, &vcliped );

  if ( status == MRE_MODEL_INPL || status == MRE_MODEL_CLIPED )
  {

    vres = malloc( nvc * _MRE_vs * sizeof( MRE_F64 ) );
    MRE_ClipBackFaces( vcliped, &nvc, vres );

    if ( type == MRE_TRIANGLES )
    {
      MRE_RenderTrianglesModel( vres, nvc );
    }
    
    free( vcliped );
    free( vres );
  }

  free( vworld );
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
    MRE_I32  w,
    MRE_I32  h
)
{
  MRE_DestroyZBuffer();
  _MRE_z_buff = malloc( w * h * sizeof( MRE_F64 ) );
  MRE_ClearZBuffer();
}

void
MRE_ClearZBuffer
(

)
{
  for ( MRE_I32 i = 0; i < _MRE_buff_w * _MRE_buff_h; ++i )
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
