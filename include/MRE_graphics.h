#ifndef MRE_GRAPHICS_H
#define MRE_GRAPHICS_H

#include <MRE_decl.h>
#include <MRE_math.h>
#include <MRE_system.h>
#include <MRE_color.h>
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
MRE_DrawColoredTriangle
(
    const MRE_F64   * p0,
    const MRE_F64   * v0,
    const MRE_F64   * p1,
    const MRE_F64   * v1,
    const MRE_F64   * p2,
    const MRE_F64   * v2
);

extern
void
MRE_RenderTrianglesModel
(
    const MRE_F64  * const v,
    MRE_I32                vc
);

#endif /* MRE_GRAPHICS_H */
