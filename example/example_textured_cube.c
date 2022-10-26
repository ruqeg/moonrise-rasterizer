#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  600
#define WINDOW_TITLE   "Moonrise example"
#define NO_CURSOR

#include "sdl_helper.c"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <MRE.h>


__MRE_DEF_VERT_SHADER
(
    CustomVertexShader
)
{
#include "vertex.shader"
}

__MRE_DEF_FRAG_SHADER
(
    CustomFragmentShader
)
{
#include "fragment.shader"
}


int 
main
(
    int, char **
)
{
  {
    SDLH_InitSDL();
    
    MRE_F64 fowy  = MRE_PI / 5.0;
    MRE_F64 ratio = WINDOW_WIDTH / ( MRE_F64 )WINDOW_HEIGHT;
    MRE_F64 minz  = 0.5;
    MRE_F64 maxz  = 100;
    
    MRE_SetPrespsectiveView(
      fowy,
      ratio,
      minz,
      maxz
    );
    
    MRE_InitZBuffer( WINDOW_WIDTH, WINDOW_HEIGHT );
  }

  MRE_I32  vertex_count = 36;
  MRE_F64  vertex[  ] =
  {
     1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0,
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0,  0.0,
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  0.0,  1.0,
     1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0,
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  0.0,  1.0,
     1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  1.0,  1.0,

     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  1.0,  0.0,
     1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0,  0.0,
     1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  0.0,  1.0,
     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  1.0,  0.0,
     1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  0.0,  1.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  1.0,  1.0,

    -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  0.0,  1.0,
     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  1.0,  1.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  1.0,  0.0,
    -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  0.0,  1.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  1.0,  0.0,
    -1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  0.0,  0.0,
                                                    
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0,
    -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  0.0,  0.0,
    -1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  0.0,  1.0,
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0,
    -1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  0.0,  1.0,
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  1.0,  1.0,

     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  1.0,  0.0,
    -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  0.0,  0.0,
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0,  1.0,
     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,  1.0,  0.0,
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.0,  1.0,
     1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
                                                    
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  1.0,  0.0,
    -1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  0.0,  0.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  0.0,  1.0,
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  1.0,  0.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,  0.0,  1.0,
     1.0, -1.0,  1.0,  1.0,  0.0,  0.0,  1.0,  1.0
  };


  //Texture settings
  {
    MRE_I32    w;
    MRE_I32    h;
    MRE_I32    nrc; 
    MRE_UI8  * data;

    data = stbi_load( "../example/res/textureC.jpg", &w, &h, &nrc, 0 );

    MRE_InitTextures( 1 );
    MRE_BindTexture( 0 );
    MRE_TextureImage( MRE_RGB, data, w, h );
    MRE_GenerateMipmap( );

    //MRE_TextureParameter( MRE_TEXTURE_MIN_FILTER, MRE_NEAREST );
    //MRE_TextureParameter( MRE_TEXTURE_MIN_FILTER, MRE_NEAREST_MIPMAP_NEAREST );
    //MRE_TextureParameter( MRE_TEXTURE_MIN_FILTER, MRE_LINEAR_MIPMAP_NEAREST );
    MRE_TextureParameter( MRE_TEXTURE_MIN_FILTER, MRE_NEAREST_MIPMAP_LINEAR );
    //MRE_TextureParameter( MRE_TEXTURE_MIN_FILTER, MRE_LINEAR_MIPMAP_LINEAR );
    MRE_TextureParameter( MRE_TEXTURE_MAG_FILTER, MRE_LINEAR );
  
    stbi_image_free(data);
  }

  MRE_I32  quit = 0;

  while ( quit == 0 )
  {
    SDL_Event event;
    while ( SDL_PollEvent( &event ) )
    {
      switch ( event . type )  case SDL_QUIT:  quit = 1;
    }
    
    SDLH_StartRenderSDL();
    SDLH_CleerBufferSDL( 0x00000000u );


    MRE_Vec4 bounding_sphere;
    MRE_SET_VEC4( 0.0, 0.0, 7 + SDL_GetTicks() * 0.001, sqrt( 3 ), bounding_sphere );

    MRE_SetBuffer( SDLH_pixels, WINDOW_WIDTH, WINDOW_HEIGHT );
    MRE_SetVertexAttribSize( 8 );
    MRE_BindVertexShader( CustomVertexShader );
    MRE_BindFragmentShader( CustomFragmentShader );
    MRE_DrawArrays(
      MRE_TRIANGLES,
      vertex, vertex_count,
      bounding_sphere
    );
    MRE_ClearZBuffer();
    
    SDLH_EndRenderSDL();
  }

  MRE_DestroyZBuffer();
  MRE_DestroyTextures();
  SDLH_DestroySDL();

  return 0;
}
