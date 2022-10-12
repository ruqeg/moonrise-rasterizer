#ifndef MRE_BOUNDING_SPHERE
#define MRE_BOUNDING_SPHERE

#include <MRE_types.h>
#include <MRE_math.h>

#include <math.h>

#include <stdlib.h>
#include <float.h>

#define MRE_RECR_SBS_FLAG  0
#define MRE_ITER_SBS_FLAG  !0

extern
void
MRE_SmallestBoundingSphere
(
    const MRE_Vec3  * p,
    MRE_I32           ps,
    MRE_I32           type,
    MRE_Vec4          d
);

#endif
