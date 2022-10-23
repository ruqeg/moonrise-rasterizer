#ifndef MRE_H
#define MRE_H

#include <MRE_types.h>
#include <MRE_math.h>
#include <MRE_color.h>
#include <MRE_graphics.h> 
#include <MRE_geometry.h>
#include <MRE_texture.h>
#include <MRE_alg.h>
#include <MRE_bounding_sphere.h>

#define MRE_RGB  0

extern
void
MRE_InitZBuffer
(
    MRE_I32  w,
    MRE_I32  h
);

extern
void
MRE_DestroyZBuffer
(

);

extern
void
MRE_InitTextures
(
    MRE_I32 count
);

extern
void
MRE_BindTexture
(
    MRE_I32 ind
);

extern
void
MRE_TextureImage
(
    MRE_I32    format,
    MRE_UI8  * data,
    MRE_I32    w,
    MRE_I32    h
);

extern
void
MRE_TextureParameter
(
    MRE_I32  name,
    MRE_I32  param
);

extern
void
MRE_DestroyTextures
(

);

extern
void
MRE_SetBuffer
(
    MRE_UI32  * const buff,
    MRE_I16           w,
    MRE_I16           h
);

extern
void
MRE_SetPrespsectiveView
(
    MRE_F64  fowy,
    MRE_F64  aspect,
    MRE_F64  z_min,
    MRE_F64  z_max
);

extern
void
MRE_SetVertexAttribSize
(
    MRE_I32  vs
);

extern
void
MRE_TextureImage
(
    MRE_I32    type,
    MRE_UI8  * data,
    MRE_I32    w,
    MRE_I32    h
);

extern
void
MRE_BindVertexShader
(
    MRE_VertShader  shader
);

extern
void
MRE_BindFragmentShader
(
    MRE_FragShader  shader
);

extern
void
MRE_DrawArrays
(
    MRE_I32                 ptype,
    const MRE_F64   * const v,
    MRE_I32                 vc,
    const MRE_Vec4          bsphere
);

extern
void
MRE_ClearZBuffer
(

);

#endif /* MRE_H */
