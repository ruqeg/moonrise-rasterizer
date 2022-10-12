#ifndef MRE_LINE_H
#define MRE_LINE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <MRE_system.h>
#include <MRE_math.h>
#include <MRE_color.h>

#define MRE_MODEL_INPL    1
#define MRE_MODEL_OUPL   -1
#define MRE_MODEL_CLIPED  0


extern  MRE_I16     MRE_buff_w;
extern  MRE_I16     MRE_buff_h;
extern  MRE_UI32  * MRE_buff;


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
    MRE_I16           x0,
    MRE_I16           y0,
    MRE_F64           h0,
    MRE_I16           x1,
    MRE_I16           y1,
    MRE_F64           h1,
    MRE_I16           x2,
    MRE_I16           y2,
    MRE_F64           h2,
    struct MRE_Color  color
);


extern
void
MRE_RenderTriangle
(
    const MRE_Vec3  v0,
    const MRE_Vec3  v1,
    const MRE_Vec3  v2,
    MRE_Pixel       pixel
);

extern
void
MRE_RenderModel
(
    const MRE_Vec3   * const vert,
    MRE_I32                  vert_count,
    const MRE_IVec4  * const triangles,
    MRE_I32                  triangles_count
);

//! FREE d_vert/d_triangles after func_call
extern
MRE_I32
MRE_ClipModel
(
    const MRE_Vec3   * const   vert,
    MRE_I32          * const   vert_count,
    const MRE_IVec4  * const   triangles,
    MRE_I32          * const   triangles_count,
    const MRE_Vec4             bsphere,
    MRE_Vec3         * * const d_vert,
    MRE_IVec4        * * const d_triangles
);

extern
MRE_I32
MRE_ClipTrianglesAgainstPlane
(
    const MRE_Vec4           plane,
    const MRE_Vec3   * const vert,
    MRE_I32          * const vert_count,
    const MRE_IVec4  * const triangles,
    MRE_I32          * const triangles_count,
    MRE_Vec3         * const d_vert,
    MRE_IVec4        * const d_triangles
);

extern
void
MRE_ClipBackFaces
(
    const MRE_Vec3   * const vert,
    MRE_I32          * const vert_count,
    const MRE_IVec4  * const triangles,
    MRE_I32          * const triangles_count,
    MRE_Vec3         * const d_vert,
    MRE_IVec4        * const d_triangles
);

extern
void
MRE_SetPrespsectiveView
(
    MRE_F64  fow_y,
    MRE_F64  ratio,
    MRE_F64  z_min,
    MRE_F64  z_max
);

#endif
