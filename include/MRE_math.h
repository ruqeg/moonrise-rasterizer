#ifndef MRE_MATH_H
#define MRE_MATH_H

#include <math.h>
#include <complex.h>
#include <stdio.h>

#include <MRE_types.h>

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
  const MRE_Mat4  m,
  const MRE_Vec4  v,
  MRE_Vec4        dest
);

extern
void
MRE_MulMat4Vec3Vec4
(
  const MRE_Mat4  m,
  const MRE_Vec3  v,
  const MRE_F64   s,
  MRE_Vec4        dest
);

extern
void
MRE_MulMat4Vec3Vec3
(
  const MRE_Mat4  m,
  const MRE_Vec3  v,
  const MRE_F64   s,
  MRE_Vec3        dest
);

extern
void
MRE_MulMat4
(
  const MRE_Mat4  m1,
  const MRE_Mat4  m2,
  MRE_Mat4        dest
);

#include <MRE_math.ini>

#endif
