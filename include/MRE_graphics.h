#ifndef MRE_LINE_H
#define MRE_LINE_H

#include <stdlib.h>
#include <stdio.h>

#include <MRE_system.h>
#include <MRE_math.h>
#include <MRE_color.h>


extern
void
MRE_DrawLine
(
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_Pixel  pixel
);

extern
void
MRE_DrawWireframeTriangle
(
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_I16    x2,
    MRE_I16    y2,
    MRE_Pixel  pixel
);

extern 
void 
MRE_DrawFilledTriangle
(
    MRE_I16    x0,
    MRE_I16    y0,
    MRE_I16    x1,
    MRE_I16    y1,
    MRE_I16    x2,
    MRE_I16    y2,
    MRE_Pixel  pixel
);

extern 
void 
MRE_DrawShadedTriangle
(
    MRE_I16           x0,
    MRE_I16           y0,
    MRE_F64           h0,
    MRE_I16           x1,
    MRE_I16           y1,
    MRE_F64           h1,
    MRE_I16           x2,
    MRE_I16           y2,
    MRE_F64           h2,
    struct MRE_Color  color
);


extern
void
MRE_RenderModel
(
  const void    * vert_p,
  MRE_I32         vert_count,
  const void    * tr_p,
  MRE_I32         tr_size,
  const MRE_Mat4  proj_mat4,
  MRE_Pixel       pixel
);

extern
void
MRE_RenderTriangle
(
  const MRE_Vec2  v0,
  const MRE_Vec2  v1,
  const MRE_Vec2  v2,
  MRE_Pixel       pixel
);


extern MRE_UI32   * MRE_buff;
extern MRE_I16      MRE_buff_w;
extern MRE_I16      MRE_buff_h;


#endif
