#include <MRE_texture.h>

#include <MRE_decl.h>

void
_MRE_BuildLameMipmap
(
    MRE_Vec3  * source,
    MRE_I32     size,
    MRE_Vec3  * d
)
{
  // TODO kaiser filter
  MRE_I32  u;
  MRE_I32  v;
  MRE_I32  dsize;

  
  dsize = size / 2;

  for ( v = 0; v < dsize; ++v )
  {
    for ( u = 0; u < dsize; ++u )
    {
      _MRE_CFOR( 3, {
        d[ v * dsize + u ][ _k ] = sqrt(
          MRE_POW2( source[ ( 2 * v + 0 ) * size + 2 * u + 0 ][ _k ] )
          +
          MRE_POW2( source[ ( 2 * v + 0 ) * size + 2 * u + 1 ][ _k ] )
          +
          MRE_POW2( source[ ( 2 * v + 1 ) * size + 2 * u + 0 ][ _k ] )
          +
          MRE_POW2( source[ ( 2 * v + 1 ) * size + 2 * u + 1 ][ _k ] )
        ) / 2.0;
      } );
    }
  }
}

void
MRE_GenerateMipmap
(

)
{
  MRE_I32     size;
  MRE_I32     mipmap_step;
  MRE_Vec3  * rmipmap;



  if ( _MRE_btexture -> mipmap != NULL )
  {
    free( _MRE_btexture -> mipmap );
  }

  size = _MRE_btexture -> size;
  _MRE_btexture -> mipmap = malloc( 
    size * size * sizeof( MRE_Vec3 ) / 3
  );

  

  mipmap_step = 0;
  
  _MRE_BuildLameMipmap(
    _MRE_btexture -> data, size, _MRE_btexture -> mipmap
  );
 
  while ( size > 0 )
  {
    size /= 2;
    
    _MRE_BuildLameMipmap(
      _MRE_btexture -> mipmap + mipmap_step,
      size,
      _MRE_btexture -> mipmap + mipmap_step + size * size
    );
    
    mipmap_step += size * size;
  }
}

void
MRE_GetMipmapTexture
(
    MRE_Vec3  * * const dtexture,
    MRE_I32   * const   dtexture_size
)
{
  MRE_I32  mipmap_level = _MRE_btexture -> size / _MRE_triangle_size;
  if ( mipmap_level & ( mipmap_level - 1 ) )
  {
    --mipmap_level;
    mipmap_level |= mipmap_level >> 1;
    mipmap_level |= mipmap_level >> 2;
    mipmap_level |= mipmap_level >> 4;
    mipmap_level |= mipmap_level >> 8;
    mipmap_level |= mipmap_level >> 16;
    ++mipmap_level;
    mipmap_level >>= 1;
  }

  *dtexture_size = _MRE_btexture -> size / ( 2 * mipmap_level );
  *dtexture = _MRE_btexture -> mipmap + (
      MRE_POW2( _MRE_btexture -> size )
        - MRE_POW2( _MRE_btexture -> size / mipmap_level )
  ) / 3;
}

void
MRE_GetCurrentTexture
(
    MRE_Vec3  * * const dtexture,
    MRE_I32   * const   dtexture_size
)
{
  *dtexture      = _MRE_btexture -> data;
  *dtexture_size = _MRE_btexture -> size;
}

void
MRE_GetNearestTexel
(
    const MRE_Vec3  * texture,
    MRE_I32           texture_size,
    MRE_F64           tx,
    MRE_F64           ty,
    MRE_F64         * dc
)
{
  MRE_I32    rd_tx;
  MRE_I32    rd_ty;

  rd_tx = texture_size * tx;
  rd_ty = texture_size * ty;

  if ( rd_tx == texture_size ) rd_tx = texture_size - 1;
  if ( rd_ty == texture_size ) rd_ty = texture_size - 1;

  MRE_COPY_VEC3(
    texture[ rd_ty * texture_size + rd_tx ],
    dc
  );
}

void
MRE_GetLinearTexel
(
    const MRE_Vec3  * texture,
    MRE_I32           texture_size,
    MRE_F64           tx,
    MRE_F64           ty,
    MRE_F64         * dc
)
{
  MRE_I32          fl_tx;
  MRE_I32          fl_ty;
  MRE_I32          ce_tx;
  MRE_I32          ce_ty;
  MRE_F64          fr_tx;
  MRE_F64          fr_ty;
  const MRE_F64  * tl; 
  const MRE_F64  * tr; 
  const MRE_F64  * bl; 
  const MRE_F64  * br; 
  MRE_Vec3         ct;
  MRE_Vec3         cb;



  tx *= texture_size;
  ty *= texture_size;
  fl_tx = ( MRE_I32 )( tx );
  fl_ty = ( MRE_I32 )( ty );
  ce_tx = ceil( tx );
  ce_ty = ceil( ty );
  
  if ( ce_tx == texture_size )  ce_tx = texture_size - 1;
  if ( ce_ty == texture_size )  ce_ty = texture_size - 1;
  if ( fl_tx == texture_size )  fl_tx = texture_size - 1;
  if ( fl_ty == texture_size )  fl_ty = texture_size - 1;
  
  fr_tx = tx - fl_tx;
  fr_ty = ty - fl_ty;

  tl = texture[ fl_ty * texture_size + fl_tx ];
  tr = texture[ fl_ty * texture_size + ce_tx ];
  bl = texture[ ce_ty * texture_size + fl_tx ];
  br = texture[ ce_ty * texture_size + ce_tx ];

  MRE_VEC3_COEFF_2( tl, 1 - fr_tx, tr, fr_tx, ct );
  MRE_VEC3_COEFF_2( bl, 1 - fr_tx, br, fr_tx, cb );
  MRE_VEC3_COEFF_2( ct, 1 - fr_ty, cb, fr_ty, dc );
}

void
MRE_GetNearestMipmapTexel
(
    const MRE_Vec3  * texture,
    MRE_I32           texture_size,
    MRE_F64           tx,
    MRE_F64           ty,
    MRE_F64         * dc
)
{
  MRE_Vec3  c1;
  MRE_Vec3  c2;
  MRE_F64   t;


  MRE_GetNearestTexel(
    texture,
    texture_size,
    tx, ty,
    c1
  );
  MRE_GetNearestTexel(
    texture + texture_size * texture_size,
    texture_size / 2, 
    tx, ty,
    c2
  );

  t = ( _MRE_triangle_size - texture_size ) / ( MRE_F64 )texture_size;

  MRE_VEC3_COEFF_2( c1, t, c2, 1 - t, dc );
}

void
MRE_GetLinearMipmapTexel
(
    const MRE_Vec3  * texture,
    MRE_I32           texture_size,
    MRE_F64           tx,
    MRE_F64           ty,
    MRE_F64         * dc
)
{
  MRE_Vec3  c1;
  MRE_Vec3  c2;
  MRE_F64   t;


  MRE_GetLinearTexel( texture, texture_size, tx, ty, c1 );
  MRE_GetLinearTexel(
    texture + texture_size * texture_size,
    texture_size / 2, tx, ty, c2
  );
  
  t = ( _MRE_triangle_size - texture_size ) / ( MRE_F64 )texture_size;

  MRE_VEC3_COEFF_2( c1, t, c2, 1.0 - t, dc );
}
