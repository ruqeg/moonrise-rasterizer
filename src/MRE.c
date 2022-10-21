#include <MRE.h>

MRE_VertShader  * _MRE_vertex_shader   = NULL;
MRE_FragShader  * _MRE_fragment_shader = NULL;

MRE_UI32    * _MRE_buff;
MRE_I16       _MRE_buff_w;
MRE_I16       _MRE_buff_h;

MRE_Mat4      _MRE_projection_mat;
MRE_Vec4      _MRE_view_planes[ 6 ];
MRE_F64     * _MRE_z_buff = NULL;

MRE_I32       _MRE_vs;

const struct MRE_Texture   * _MRE_texture = NULL;
