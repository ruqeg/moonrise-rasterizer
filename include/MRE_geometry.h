#ifndef MRE_GEOMETRY_H
#define MRE_GEOMETRY_H

#include <MRE_types.h>
#include <MRE_math.h>
#include <MRE_color.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

extern
void
MRE_CreateCube
(
    MRE_Vec3   * const dv,
    MRE_IVec4  * const dt
);

extern
void
MRE_CreateSphere
(
    MRE_I32            vpc,
    MRE_I32            hpc,
    MRE_Vec3   * const dv,
    MRE_IVec4  * const dt
);

#endif /* MRE_GEOMETRY_H */
