#ifndef MRE_TEXTURE_H
#define MRE_TEXTURE_H

#include <MRE_types.h>

struct MRE_Texture
{
  MRE_I32     w;
  MRE_I32     h;
  MRE_UI32  * pixels;
};

#endif /* MRE_TEXTURE_H */
