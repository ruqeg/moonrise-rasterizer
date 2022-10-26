#ifndef MRE_DECL_H
#define MRE_DECL_H

#include <MRE_types.h>
#include <MRE_texture.h>

#define MRE_POF               0
#define MRE_COF               3
#define MRE_TOF               6

extern  MRE_VertShader      * _MRE_vertex_shader;
extern  MRE_FragShader      * _MRE_fragment_shader;

extern  MRE_UI32            * _MRE_buff;
extern  MRE_I16               _MRE_buff_w;
extern  MRE_I16               _MRE_buff_h;

extern  MRE_F64             * _MRE_z_buff;

extern  MRE_Mat4              _MRE_projection_mat;
extern  MRE_Vec4              _MRE_view_planes[ 6 ];

extern  MRE_I32               _MRE_vs;

extern  struct MRE_Texture  * _MRE_textures;
extern  MRE_I32               _MRE_textures_count;

extern  MRE_I32               _MRE_binded_texture_index;
extern  struct MRE_Texture  * _MRE_btexture;

extern  MRE_I32               _MRE_triangle_size;

extern  void  (*_MRE_gettexel_fun[2])(
  const MRE_Vec3 *, MRE_I32, MRE_F64, MRE_F64, MRE_F64 *
);
extern  void  (*_MRE_gettexture_fun[2])(
  MRE_Vec3 * * const, MRE_I32 * const
);

#endif /* MRE_DECL_H */
