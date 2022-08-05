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
  MRE_F64   ratio,
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
MRE_Set_Vec4
(
  MRE_F64   x,
  MRE_F64   y,
  MRE_F64   z,
  MRE_F64   w,
  MRE_Vec4  dest
);

extern
void
MRE_Set_Vec4
(
  MRE_F64   x,
  MRE_F64   y,
  MRE_F64   z,
  MRE_F64   w,
  MRE_Vec4  dest
);


void
MRE_Set_Vec4_3
(
  const MRE_Vec3  v,
  MRE_F64         last,
  MRE_Vec4        dest
);

extern
void
MRE_Add_Vec4
(
  const MRE_Vec4  v1,
  const MRE_Vec4  v2,
  MRE_Vec4        dest
);

extern
void
MRE_Div_Vec4_S
(
  const MRE_Vec4  v,
  MRE_F64         s,
  MRE_Vec4        dest
);

extern
void
MRE_Copy_Vec4
(
  const MRE_Vec4  v,
  MRE_Vec4        dest
);

extern
void
MRE_Copy_Vec3
(
  const MRE_Vec3  v,
  MRE_Vec3        dest
);

extern
void
MRE_Mul_Mat4_V
(
  const MRE_Mat4  m,
  const MRE_Vec4  v,
  MRE_Vec4        dest
);

extern
void
MRE_Mul_Mat4
(
  const MRE_Mat4  m1,
  const MRE_Mat4  m2,
  MRE_Mat4        dest
);

#endif
