#include <MRE.h>

#include <MRE_decl.h>

MRE_VertShader      * _MRE_vertex_shader        = NULL;
MRE_FragShader      * _MRE_fragment_shader      = NULL;

MRE_UI32            * _MRE_buff                 = NULL;
MRE_I16               _MRE_buff_w               = -1;
MRE_I16               _MRE_buff_h               = -1;

MRE_Mat4              _MRE_projection_mat;
MRE_Vec4              _MRE_view_planes[ 6 ];
MRE_F64             * _MRE_z_buff               = NULL;

MRE_I32               _MRE_vs                   = -1;

struct MRE_Texture  * _MRE_textures             = NULL;
MRE_I32               _MRE_textures_count       = -1;
MRE_I32               _MRE_binded_texture_index = MRE_NONE_TEXTURE;
struct MRE_Texture  * _MRE_btexture             = NULL;

void  (*_MRE_gettexel_fun[2])(
  MRE_F64, MRE_F64, MRE_F64 *
);


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
MRE_InitTextures
(
    MRE_I32 count
)
{
  //TODO reinit textures arr
  if ( _MRE_textures != NULL )  free( _MRE_textures );
  _MRE_textures = malloc( count * sizeof( struct MRE_Texture ) );
  
  for ( MRE_I32 j = 0; j < count; ++j )
  {
    _MRE_textures[ j ] . data = NULL;
  }

  _MRE_textures_count = count;
}

void
MRE_BindTexture
(
    MRE_I32 ind
)
{
  _MRE_binded_texture_index = ind;
  if ( ind == MRE_NONE_TEXTURE )
  {
    _MRE_btexture = NULL;
  }
  else
  {
    _MRE_btexture = _MRE_textures + _MRE_binded_texture_index;
  }
}

void
MRE_TextureImage
(
    MRE_I32    format,
    MRE_UI8  * data,
    MRE_I32    w,
    MRE_I32    h
)
{
  if ( _MRE_btexture -> data != NULL ) free( _MRE_btexture -> data );
  _MRE_btexture -> data = malloc( w * h * sizeof( MRE_Vec3 ) );

  for ( MRE_I32 j = 0; j < w * h; ++j )
  {
    _MRE_btexture -> data[ j + format ][0] = data[ j * 3 + 0 ] / 255.0;
    _MRE_btexture -> data[ j + format ][1] = data[ j * 3 + 1 ] / 255.0;
    _MRE_btexture -> data[ j + format ][2] = data[ j * 3 + 2 ] / 255.0;
  }
  _MRE_btexture -> w = w;
  _MRE_btexture -> h = h;
}

void
MRE_DestroyTextures
(

)
{
  free( _MRE_textures );
}

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
MRE_SetVertexAttribSize
(
    MRE_I32  vs
)
{
  _MRE_vs = vs;
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
    MRE_I32                 ptype,
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

  void  (* renderfun )(
    const MRE_F64 * const, MRE_I32
  );



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

  if ( ptype == MRE_TRIANGLES )  renderfun = MRE_RenderTrianglesModel;

  if ( status == MRE_MODEL_INPL || status == MRE_MODEL_CLIPED )
  {
    vres = malloc( nvc * _MRE_vs * sizeof( MRE_F64 ) );
    MRE_ClipBackFaces( vcliped, &nvc, vres );
    
    renderfun( vres, nvc );
    
    free( vcliped );
    free( vres );
  }

  free( vworld );
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
MRE_TextureParameter
(
    MRE_I32  name,
    MRE_I32  param
)
{
  
  switch ( param )
  {
    case MRE_NEAREST:
      _MRE_gettexel_fun[ name ] = MRE_GetNearestTexel;
      break;
    case MRE_LINEAR:
      _MRE_gettexel_fun[ name ] = MRE_GetLinerTexel;
      break;
  }
}

void
MRE_DestroyZBuffer
(

)
{
  if ( _MRE_z_buff != NULL )  free( _MRE_z_buff );
}
