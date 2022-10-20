#ifndef MRE_DECL_H
#define MRE_DECL_H

#include <MRE_types.h>

#define VAP_COUNT      3
#define VAP_P_I        0
#define VAP_C_I        1
#define VAP_T_I        2

extern  MRE_I16     MRE_buff_w;
extern  MRE_I16     MRE_buff_h;
extern  MRE_UI32  * MRE_buff;


extern  MRE_Mat4    _MRE_projection_mat;
extern  MRE_Vec4    _MRE_view_planes[ 6 ];
extern  MRE_F64   * _MRE_z_buff;

extern  MRE_I32     _MRE_vs;
extern  MRE_I32     _MRE_vap[ VAP_COUNT ];

#endif /* MRE_DECL_H */
