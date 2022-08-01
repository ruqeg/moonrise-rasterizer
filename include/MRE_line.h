#ifndef MRE_LINE_H
#define MRE_LINE_H

#include <stdlib.h>
#include <stdint.h>

#include <MRE_system.h>

extern void MRE_DrawLine
(
    uint32_t  * buff,
    int16_t     buff_w,
    int16_t     x0,
    int16_t     y0,
    int16_t     x1,
    int16_t     y1,
    uint32_t    color
);

#endif
