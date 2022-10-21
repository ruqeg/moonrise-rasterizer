#ifndef MRE_SYSTEM_H
#define MRE_SYSTEM_H

#include <MRE_types.h>
#include <MRE_decl.h>
#include <MRE_alg.h>
#include <MRE_math.h>
#include <MRE_graphics.h>
#include <MRE_geometry.h>
#include <MRE_texture.h>

#include <stdlib.h>

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
MRE_SetVertexAttribSize
(
    MRE_I32  vs
);

extern
void
MRE_BindTexture
(
    const struct MRE_Texture  * const t
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
    MRE_I32                 type,
    const MRE_F64   * const v,
    MRE_I32                 vc,
    const MRE_Vec4          bsphere
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
MRE_InitZBuffer
(
    MRE_I32  w,
    MRE_I32  h
);

extern
void
MRE_ClearZBuffer
(

);

extern
void
MRE_DestroyZBuffer
(

);

#endif /* MRE_SYSTEM_H */
