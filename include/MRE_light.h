#ifndef MRE_LIGHT_H
#define MRE_LIGHT_H

//TODO Fong Shading

#include <MRE_types.h>
#include <MRE_math.h>

#define MRE_LIGHT_POINT 0
#define MRE_LIGHT_RAY   1

struct LightSource
{
  MRE_I32   type;
  MRE_F64   illuminace;
  MRE_Vec3  v;
};

extern MRE_F64               MRE_global_illuminace;
extern struct LightSource  * MRE_light_sources;
extern MRE_I32               MRE_light_sources_count;

extern
MRE_F64
MRE_GetVertIlluminaceLevel
(
    const MRE_Vec3  v,
    const MRE_Vec3  n
);

#endif
