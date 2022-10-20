#include <MRE_math.h>

void
MRE_PerspectiveMat4
(
    MRE_F64   fovy,
    MRE_F64   aspect,
    MRE_F64   nz,
    MRE_F64   fz,
    MRE_Mat4  dest
)
{
  const MRE_F64  ctan_y = 1.0 / tan( fovy * 0.5 );

  dest[0][0] = ctan_y / aspect;
  dest[0][1] = 0.0;
  dest[0][2] = 0.0;
  dest[0][3] = 0.0;
  
  dest[1][0] = 0,0;
  dest[1][1] = ctan_y;
  dest[1][2] = 0.0;
  dest[1][3] = 0.0;
  
  dest[2][0] = 0,0;
  dest[2][1] = 0.0;
  dest[2][2] = ( fz + nz )/( fz - nz );
  dest[2][3] = -2.0*fz*nz/( fz - nz );

  dest[3][0] = 0,0;
  dest[3][1] = 0.0;
  dest[3][2] = 1.0;
  dest[3][3] = 0.0;
}

void
MRE_TanslateMat4
(
    const MRE_Vec3  v,
    MRE_Mat4        dest
)
{
  dest[0][0]=1.0; dest[0][1]=0.0; dest[0][2]=0.0; dest[0][3]=v[0];
  dest[1][0]=0.0; dest[1][1]=1.0; dest[1][2]=0.0; dest[1][3]=v[1];
  dest[2][0]=0.0; dest[2][1]=0.0; dest[2][2]=1.0; dest[2][3]=v[2];
  dest[3][0]=0.0; dest[3][1]=0.0; dest[3][2]=0.0; dest[3][3]=1.0;
}

void
MRE_ScaleMat4
(
    const MRE_Vec3  v,
    MRE_Mat4        dest
)
{
  dest[0][0]=v[0]; dest[0][1]=0.0;  dest[0][2]=0.0;  dest[0][3]=0.0;
  dest[1][0]=0.0;  dest[1][1]=v[1]; dest[1][2]=0.0;  dest[1][3]=0.0;
  dest[2][0]=0.0;  dest[2][1]=0.0;  dest[2][2]=v[2]; dest[2][3]=0.0;
  dest[3][0]=0.0;  dest[3][1]=0.0;  dest[3][2]=0.0;  dest[3][3]=1.0;
}

void
MRE_RotateMat4
(
    const MRE_Vec3  v,
    MRE_Mat4        dest
)
{
  MRE_F64  cos_x  =  cos( v[0] );
  MRE_F64  cos_y  =  cos( v[1] );
  MRE_F64  cos_z  =  cos( v[2] );
  MRE_F64  sin_x  =  sin( v[0] );
  MRE_F64  sin_y  =  sin( v[1] );
  MRE_F64  sin_z  =  sin( v[2] );


  dest[0][0] = cos_x*cos_y;
  dest[0][1] = cos_x*sin_y*sin_z - sin_x*cos_z;
  dest[0][2] = cos_x*sin_y*cos_z + sin_x*sin_z;
  dest[0][3] = 0.0;
  
  dest[1][0] = sin_x*cos_y;
  dest[1][1] = sin_x*sin_y*sin_z + cos_x*cos_z;
  dest[1][2] = sin_x*sin_y*cos_z - cos_x*sin_z;
  dest[1][3] = 0.0;
  
  dest[2][0] = -sin_y;
  dest[2][1] = cos_y*sin_z;
  dest[2][2] = cos_y*cos_z;
  dest[2][3] = 0.0;
  
  dest[3][0] = 0.0;
  dest[3][1] = 0.0;
  dest[3][2] = 0.0;
  dest[3][3] = 1.0;
}

MRE_INLINE_
void
MRE_InverseMat4
(
    const MRE_Mat4  mat4,
    MRE_Mat4        dest
)
{
  //TODO
}


MRE_INLINE_
void
MRE_MulMat4Vec4Vec4
(
    const MRE_Mat4          m,
    const MRE_F64   * const v,
    MRE_F64         * const dest
)
{
  dest[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2] + m[0][3]*v[3];
  dest[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2] + m[1][3]*v[3];
  dest[2] = m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2] + m[2][3]*v[3];
  dest[3] = m[3][0]*v[0] + m[3][1]*v[1] + m[3][2]*v[2] + m[3][3]*v[3];
}

MRE_INLINE_
void
MRE_MulMat4Vec3Vec4
(
    const MRE_Mat4          m,
    const MRE_F64   * const v,
    const MRE_F64           s,
    MRE_F64         * const dest
)
{
  dest[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2] + m[0][3]*s;
  dest[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2] + m[1][3]*s;
  dest[2] = m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2] + m[2][3]*s;
  dest[3] = m[3][0]*v[0] + m[3][1]*v[1] + m[3][2]*v[2] + m[3][3]*s;
}

MRE_INLINE_
void
MRE_MulMat4Vec3Vec3
(
    const MRE_Mat4          m,
    const MRE_F64   * const v,
    const MRE_F64           s,
    MRE_F64         * const dest
)
{
  dest[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2] + m[0][3]*s;
  dest[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2] + m[1][3]*s;
  dest[2] = m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2] + m[2][3]*s;
}

MRE_INLINE_
void
MRE_MulMat4
(
    const MRE_Mat4  m1,
    const MRE_Mat4  m2,
    MRE_Mat4        dest
)
{
  MRE_F64 a00=m1[0][0], a01=m1[0][1], a02=m1[0][2], a03=m1[0][3];
  MRE_F64 a10=m1[1][0], a11=m1[1][1], a12=m1[1][2], a13=m1[1][3];
  MRE_F64 a20=m1[2][0], a21=m1[2][1], a22=m1[2][2], a23=m1[2][3];
  MRE_F64 a30=m1[3][0], a31=m1[3][1], a32=m1[3][2], a33=m1[3][3];

  MRE_F64 b00=m2[0][0], b01=m2[0][1], b02=m2[0][2], b03=m2[0][3];
  MRE_F64 b10=m2[1][0], b11=m2[1][1], b12=m2[1][2], b13=m2[1][3];
  MRE_F64 b20=m2[2][0], b21=m2[2][1], b22=m2[2][2], b23=m2[2][3];
  MRE_F64 b30=m2[3][0], b31=m2[3][1], b32=m2[3][2], b33=m2[3][3];


  dest[0][0] = a00 * b00 + a01 * b10 + a02 * b20 + a03 * b30;
  dest[0][1] = a00 * b01 + a01 * b11 + a02 * b21 + a03 * b31;
  dest[0][2] = a00 * b02 + a01 * b12 + a02 * b22 + a03 * b32;
  dest[0][3] = a00 * b03 + a01 * b13 + a02 * b23 + a03 * b33;
  dest[1][0] = a10 * b00 + a11 * b10 + a12 * b20 + a13 * b30;
  dest[1][1] = a10 * b01 + a11 * b11 + a12 * b21 + a13 * b31;
  dest[1][2] = a10 * b02 + a11 * b12 + a12 * b22 + a13 * b32;
  dest[1][3] = a10 * b03 + a11 * b13 + a12 * b23 + a13 * b33;
  dest[2][0] = a20 * b00 + a21 * b10 + a22 * b20 + a23 * b30;
  dest[2][1] = a20 * b01 + a21 * b11 + a22 * b21 + a23 * b31;
  dest[2][2] = a20 * b02 + a21 * b12 + a22 * b22 + a23 * b32;
  dest[2][3] = a20 * b03 + a21 * b13 + a22 * b23 + a23 * b33;
  dest[3][0] = a30 * b00 + a31 * b10 + a32 * b20 + a33 * b30;
  dest[3][1] = a30 * b01 + a31 * b11 + a32 * b21 + a33 * b31;
  dest[3][2] = a30 * b02 + a31 * b12 + a32 * b22 + a33 * b32;
  dest[3][3] = a30 * b03 + a31 * b13 + a32 * b23 + a33 * b33;
}

MRE_INLINE_
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
)
{
  MRE_F64 D = MRE_DET3_S(
    a1, b1, c1,
    a2, b2, c2,
    a3, b3, c3
  );
  MRE_F64 Dx = MRE_DET3_S(
    d1, b1, c1,
    d2, b2, c2,
    d3, b3, c3
  );
  MRE_F64 Dy = MRE_DET3_S(
    a1, d1, c1,
    a2, d2, c2,
    a3, d3, c3
  );
  MRE_F64 Dz = MRE_DET3_S(
    a1, b1, d1,
    a2, b2, d2,
    a3, b3, d3
  );

  if ( D == 0.0 )
  {
    //TODO
    if ( Dx*Dx + Dy*Dy + Dz*Dz == 0 )  return 1;
    else                               return 1;
  }

  *dx = Dx / D;
  *dy = Dy / D;
  *dz = Dz / D;

  return 0;
}

MRE_INLINE_
MRE_F64
MRE_IntersectionPlane
(
    const MRE_F64   * const p1,
    const MRE_F64   * const p2,
    const MRE_Vec4          plane,
    MRE_F64         * const d
)
{
  MRE_F64   t; 

  MRE_SUB_VEC3( p2, p1, d );
  t = -MRE_PLANE_DIST( p1, plane ) / MRE_SCMUL_VEC3( d, plane );

  d[0] = p1[0] + t * d[0];
  d[1] = p1[1] + t * d[1];
  d[2] = p1[2] + t * d[2];

  return t;
}

MRE_INLINE_
void
MRE_TriangleNormal
(
    const MRE_F64  * const p1, 
    const MRE_F64  * const p2, 
    const MRE_F64  * const p3, 
    MRE_F64        * const d
)
{
  d[0] = MRE_DET2_S(
    p2[1] - p1[1],  p2[2] - p1[2],
    p3[1] - p1[1],  p3[2] - p1[2] 
  );
  d[1] = -MRE_DET2_S(
    p2[0] - p1[0],  p2[2] - p1[2],
    p3[0] - p1[0],  p3[2] - p1[2] 
  );
  d[2] = MRE_DET2_S(
    p2[0] - p1[0],  p2[1] - p1[1],
    p3[0] - p1[0],  p3[1] - p1[1] 
  );
}
