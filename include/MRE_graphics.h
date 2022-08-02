#ifndef MRE_LINE_H
#define MRE_LINE_H

#include <stdlib.h>
#include <stdint.h>

#include <MRE_system.h>

extern void MRE_DrawLine
(
    int16_t   x0,
    int16_t   y0,
    int16_t   x1,
    int16_t   y1,
    uint32_t  color
);

extern void MRE_DrawWireframeTriangle
(
    int16_t   x0,
    int16_t   y0,
    int16_t   x1,
    int16_t   y1,
    int16_t   x2,
    int16_t   y2,
    uint32_t  color
);

extern void MRE_DrawFilledTriangle
(
    int16_t   x0,
    int16_t   y0,
    int16_t   x1,
    int16_t   y1,
    int16_t   x2,
    int16_t   y2,
    uint32_t  color
);


extern uint32_t  * MRE_buff;
extern int16_t     MRE_buff_w;


#endif
