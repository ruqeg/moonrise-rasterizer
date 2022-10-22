#ifndef MRE_COLOR_H
#define MRE_COLOR_H

#define MRE_PIXEL_TO_COLOR( pixel, color )\
({                                             \
  (color)[0] = ( (pixel) >> 24 ) & 0x000000FFu;\
  (color)[1] = ( (pixel) >> 16 ) & 0x000000FFu;\
  (color)[2] = ( (pixel) >> 8  ) & 0x000000FFu;\
})

#define MRE_COLOR_TO_PIXEL( color )\
(                                  \
    (color)[0] << 24               \
  | (color)[1] << 16               \
  | (color)[2] << 8                \
)

#define MRE_RGBA_TO_PIXEL( r, g, b, a )\
(                                      \
    ( MRE_UI8 )(r) << 24               \
  | ( MRE_UI8 )(g) << 16               \
  | ( MRE_UI8 )(b) << 8                \
  | ( MRE_UI8 )(a)                     \
)

#endif /* MRE_COLOR_H */
