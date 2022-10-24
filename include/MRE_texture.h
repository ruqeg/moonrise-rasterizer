#ifndef MRE_TEXTURE_H
#define MRE_TEXTURE_H

#include <MRE_types.h>
#include <MRE_math.h>

#include <stdlib.h>
#include <stddef.h>

#define MRE_NONE_TEXTURE  -1

struct MRE_Texture
{
  MRE_I32     size;
  MRE_Vec3  * data;
  MRE_Vec3  * mipmap;
};

extern
void
MRE_GenerateMipmap
(

);

extern
void
MRE_GetMipmapTexture
(
    MRE_Vec3  * * const dtexture,
    MRE_I32   * const   dtexture_size
);

extern
void
MRE_GetCurrentTexture
(
    MRE_Vec3  * * const dtexture,
    MRE_I32   * const   dtexture_size
);

extern
void
MRE_GetNearestTexel
(
    const MRE_Vec3  * texture,
    MRE_I32           texture_size,
    MRE_F64           tx,
    MRE_F64           ty,
    MRE_F64         * dc
);

extern
void
MRE_GetLinearTexel
(
    const MRE_Vec3  * texture,
    MRE_I32           texture_size,
    MRE_F64           tx,
    MRE_F64           ty,
    MRE_F64         * dc
);

extern
void
MRE_GetNearestMipmapTexel
(
    const MRE_Vec3  * texture,
    MRE_I32           texture_size,
    MRE_F64           tx,
    MRE_F64           ty,
    MRE_F64         * dc
);

extern
void
MRE_GetLinearMipmapTexel
(
    const MRE_Vec3  * texture,
    MRE_I32           texture_size,
    MRE_F64           tx,
    MRE_F64           ty,
    MRE_F64         * dc
);

#endif /* MRE_TEXTURE_H */
