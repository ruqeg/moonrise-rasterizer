#ifndef MRE_TEXTURE_H
#define MRE_TEXTURE_H

#include <MRE_types.h>
#include <MRE_math.h>

#define MRE_NONE_TEXTURE  -1

struct MRE_Texture
{
  MRE_I32     w;
  MRE_I32     h;
  MRE_Vec3  * data;
};

extern
void
MRE_GenerateMipmap
(

);

extern
void
MRE_GetNearestTexel
(
  MRE_F64    tx,
  MRE_F64    ty,
  MRE_F64  * dc
);

extern
void
MRE_GetLinerTexel
(
  MRE_F64    tx,
  MRE_F64    ty,
  MRE_F64  * dc
);

#endif /* MRE_TEXTURE_H */
