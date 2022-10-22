#ifndef MRE_GEOMETRY_H
#define MRE_GEOMETRY_H

#include <MRE_math.h>
#include <MRE_color.h>
#include <MRE_alg.h>

#include <stdlib.h>
#include <string.h>

#define MRE_MODEL_INPL    1
#define MRE_MODEL_OUPL   -1
#define MRE_MODEL_CLIPED  0

//! FREE d_vert/d_triangles after func_call
extern
MRE_I32
MRE_ClipModel
(
    const MRE_F64   * const   v,
    MRE_I32         * const   vc,
    const MRE_Vec4            bsphere,
    MRE_F64         * * const dv
);

extern
MRE_I32
MRE_ClipTrianglesAgainstPlane
(
    const MRE_Vec4           plane,
    const MRE_F64    * const v,
    MRE_I32          * const vc,
    MRE_F64          * const dv
);

extern
void
MRE_ClipBackFaces
(
    const MRE_F64  * const v,
    MRE_I32        * const vc,
    MRE_F64        * const dv
);

#endif /* MRE_GEOMETRY_H */
