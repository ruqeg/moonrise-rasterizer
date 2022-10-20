#ifndef MRE_MATH_H
#define MRE_MATH_H

#include <MRE_types.h>

#include <math.h>

extern
void
MRE_PerspectiveMat4
(
    MRE_F64   fovy,
    MRE_F64   aspect,
    MRE_F64   nz,
    MRE_F64   fz,
    MRE_Mat4  dest
);

extern
void
MRE_TanslateMat4
(
    const MRE_Vec3  v,
    MRE_Mat4        dest
);

extern
void
MRE_ScaleMat4
(
    const MRE_Vec3  v,
    MRE_Mat4        dest
);

extern
void
MRE_RotateMat4
(
    const MRE_Vec3  v,
    MRE_Mat4        dest
);

extern
void
MRE_InverseMat4
(
    const MRE_Mat4  mat4,
    MRE_Mat4        dest
);

extern
void
MRE_MulMat4Vec4Vec4
(
    const MRE_Mat4          m,
    const MRE_F64   * const v,
    MRE_F64         * const dest
);

extern
void
MRE_MulMat4Vec3Vec4
(
    const MRE_Mat4          m,
    const MRE_F64   * const v,
    const MRE_F64           s,
    MRE_F64         * const dest
);

extern
void
MRE_MulMat4Vec3Vec3
(
    const MRE_Mat4          m,
    const MRE_F64   * const v,
    const MRE_F64           s,
    MRE_F64         * const dest
);

extern
void
MRE_MulMat4
(
    const MRE_Mat4  m1,
    const MRE_Mat4  m2,
    MRE_Mat4        dest
);

extern
MRE_I32
MRE_SolveLinerSystem3
(
    MRE_F64    a1,
    MRE_F64    b1,
    MRE_F64    c1,
    MRE_F64    d1,
    MRE_F64    a2,
    MRE_F64    b2,
    MRE_F64    c2,
    MRE_F64    d2,
    MRE_F64    a3,
    MRE_F64    b3,
    MRE_F64    c3,
    MRE_F64    d3,
    MRE_F64  * dx,
    MRE_F64  * dy,
    MRE_F64  * dz
);

extern
MRE_F64
MRE_IntersectionPlane
(
    const MRE_F64   * const p1,
    const MRE_F64   * const p2,
    const MRE_Vec4          plane,
    MRE_F64         * const d
);

extern
void
MRE_TriangleNormal
(
    const MRE_F64  * const p1, 
    const MRE_F64  * const p2, 
    const MRE_F64  * const p3, 
    MRE_F64        * const d
);

#include <MRE_math.ini>

#endif /* MRE_MATH_H */
