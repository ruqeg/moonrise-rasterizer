#include <MRE_texture.h>

#include <MRE_decl.h>

void
MRE_GenerateMipmap
(

)
{

}

void
MRE_GetNearestTexel
(
  MRE_F64    tx,
  MRE_F64    ty,
  MRE_F64  * dc
)
{
  MRE_I32    rd_tx;
  MRE_I32    rd_ty;

  rd_tx = round( tx );
  rd_ty = round( ty );

  if ( rd_tx == _MRE_btexture -> w ) rd_tx = _MRE_btexture -> w - 1;
  if ( rd_ty == _MRE_btexture -> h ) rd_ty = _MRE_btexture -> h - 1;

  MRE_COPY_VEC3(
    _MRE_btexture -> data[ rd_ty * _MRE_btexture -> w + rd_tx ],
    dc
  );
}

void
MRE_GetLinerTexel
(
  MRE_F64    tx,
  MRE_F64    ty,
  MRE_F64  * dc
)
{
  MRE_I32    fl_tx;
  MRE_I32    fl_ty;
  MRE_I32    ce_tx;
  MRE_I32    ce_ty;
  MRE_F64    fr_tx;
  MRE_F64    fr_ty;
  MRE_F64  * tl; 
  MRE_F64  * tr; 
  MRE_F64  * bl; 
  MRE_F64  * br; 
  MRE_Vec3   ct;
  MRE_Vec3   cb;



  fl_tx = ( MRE_I32 )( tx );
  fl_ty = ( MRE_I32 )( ty );
  ce_tx = ceil( tx );
  ce_ty = ceil( ty );
  fr_tx = tx - fl_tx;
  fr_ty = ty - fl_ty;

  if ( ce_tx == _MRE_btexture -> w ) ce_tx = _MRE_btexture -> w - 1;
  if ( ce_ty == _MRE_btexture -> h ) ce_ty = _MRE_btexture -> h - 1;

  // bfilt
  tl = _MRE_btexture -> data[ fl_ty * _MRE_btexture -> w + fl_tx ];
  tr = _MRE_btexture -> data[ fl_ty * _MRE_btexture -> w + ce_tx ];
  bl = _MRE_btexture -> data[ ce_ty * _MRE_btexture -> w + fl_tx ];
  br = _MRE_btexture -> data[ ce_ty * _MRE_btexture -> w + ce_tx ];

  MRE_VEC3_COEFF_2( tl, 1 - fr_tx, tr, fr_tx, ct );
  MRE_VEC3_COEFF_2( bl, 1 - fr_tx, br, fr_tx, cb );
  MRE_VEC3_COEFF_2( ct, 1 - fr_ty, cb, fr_ty, dc );
}
