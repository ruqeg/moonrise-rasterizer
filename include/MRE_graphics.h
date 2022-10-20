#ifndef MRE_GRAPHICS_H
#define MRE_GRAPHICS_H

#include <MRE_decl.h>
#include <MRE_math.h>
#include <MRE_system.h>
#include <MRE_color.h>
#include <MRE_light.h>
#include <MRE_alg.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MRE_MODEL_INPL    1
#define MRE_MODEL_OUPL   -1
#define MRE_MODEL_CLIPED  0

extern
void
MRE_InitZBuffer
(

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

extern
void
MRE_DrawLine
(
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_Pixel  pixel
);

extern
void
MRE_DrawWireframeTriangle
(
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_I16    x2,
    MRE_I16    y2,
    MRE_Pixel  pixel
);

extern 
void 
MRE_DrawFilledTriangle
(
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_F64    z0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_F64    z1,
    MRE_I16    x2,
    MRE_I16    y2,
    MRE_F64    z2,
    MRE_Pixel  pixel
);

extern 
void 
MRE_DrawShadedTriangle
(
    MRE_I16          x0,
    MRE_I16          y0,
    MRE_F64          z0,
    MRE_F64          h0,
    const MRE_F64  * c0,
    MRE_I16          x1,
    MRE_I16          y1,
    MRE_F64          z1,
    MRE_F64          h1,
    const MRE_F64  * c1,
    MRE_I16          x2,
    MRE_I16          y2,
    MRE_F64          z2,
    MRE_F64          h2,
    const MRE_F64  * c2
);


extern
void
MRE_RenderTriangle
(
    const MRE_Vec3  v0,
    const MRE_Vec3  c0,
    const MRE_F64   h0,
    const MRE_Vec3  v1,
    const MRE_Vec3  c1,
    const MRE_F64   h1,
    const MRE_Vec3  v2,
    const MRE_Vec3  c2,
    const MRE_F64   h2
);

extern
void
MRE_RenderTrianglesModel
(
    const MRE_F64  * const v,
    MRE_I32                vc,
    const MRE_I32  * const t,
    MRE_I32                tc
);

extern
void
MRE_RenderSphereModel
(
    const MRE_F64  * const v,
    MRE_I32                vc,
    const MRE_I32  * const t,
    MRE_I32                tc,
    MRE_Vec3               center
);

#endif /* MRE_GRAPHICS_H */
