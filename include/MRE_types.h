#ifndef MRE_TYPES_H
#define MRE_TYPES_H

#include <float.h>

typedef unsigned char       MRE_UI8;
typedef signed char         MRE_I8;

typedef unsigned int short  MRE_UI16;
typedef signed int short    MRE_I16;

typedef unsigned int        MRE_UI32;
typedef signed int          MRE_I32;

typedef unsigned int long   MRE_UI64;
typedef signed int long     MRE_I64;


typedef float               MRE_F32;
typedef double              MRE_F64;


typedef MRE_I32   MRE_IVec2[ 2 ];
typedef MRE_I32   MRE_IVec3[ 3 ];
typedef MRE_I32   MRE_IVec4[ 4 ];


typedef MRE_F64   MRE_Vec2[ 2 ];
typedef MRE_F64   MRE_Vec3[ 3 ];
typedef MRE_F64   MRE_Vec4[ 4 ];

typedef MRE_Vec2  MRE_Mat2[ 2 ];
typedef MRE_Vec3  MRE_Mat3[ 3 ];
typedef MRE_Vec4  MRE_Mat4[ 4 ];


#define MRE_F64_MAX DBL_MAX
#define MRE_F64_MIN 0.00000000001


#define MRE_PI    3.14159265358979323846264338327950288
#define MRE_PI_2  1.57079632679489661923132169163975144


typedef MRE_UI32 MRE_Pixel;


#define MRE_INLINE_  inline __attribute__((always_inline)) 


#endif
