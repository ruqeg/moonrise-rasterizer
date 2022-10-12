#ifndef MRE_COLOR_H
#define MRE_COLOR_H

struct MRE_Color
{
  MRE_UI8  r;
  MRE_UI8  g;
  MRE_UI8  b;
  MRE_UI8  a;
};



#define MRE_PIXEL_TO_COLOR( pixel, color )\
({                                          \
  color . r = ( pixel >> 24 ) & 0x000000FFu;\
  color . g = ( pixel >> 16 ) & 0x000000FFu;\
  color . b = ( pixel >> 8  ) & 0x000000FFu;\
  color . a =  pixel          & 0x000000FFu;\
})

#define MRE_COLOR_TO_PIXEL( color )\
(                                  \
    color . r << 24                \
  | color . g << 16                \
  | color . b << 8                 \
  | color . a                      \
)

#define MRE_RGBA_TO_PIXEL( r, g, b, a )\
(                                      \
    ( MRE_UI8 )(r) << 24               \
  | ( MRE_UI8 )(g) << 16               \
  | ( MRE_UI8 )(b) << 8                \
  | ( MRE_UI8 )(a)                     \
)




#endif
