#ifndef MRE_SYSTEM_H
#define MRE_SYSTEM_H

#include <MRE_math.h>
#include <MRE_types.h>
#include <MRE_decl.h>

#include <stdlib.h>

extern
void
MRE_VertexAttribPointer
(
    MRE_I32  vs,
    MRE_I32  vap_offset_v,
    MRE_I32  vap_offset_c,
    MRE_I32  vap_offset_t
);

extern
void
MRE_SetPrespsectiveView
(
    MRE_F64  fowy,
    MRE_F64  aspect,
    MRE_F64  z_min,
    MRE_F64  z_max
);

extern
void
MRE_InitZBuffer
(

);

extern
void
MRE_ClearZBuffer
(

);

extern
void
MRE_DestroyZBuffer
(

);

#endif /* MRE_SYSTEM_H */
