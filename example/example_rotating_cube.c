//macro for sdl
#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  600
#define WINDOW_TITLE   "Moonrise example"
#define NO_CURSOR

#include "sdl_helper.c"


#include <MRE.h>


__MRE_DEF_VERT_SHADER( CustomVertexShader )
{
#include "vertex.shader"
}


__MRE_DEF_FRAG_SHADER( CustomFragmentShader )
{
#include "fragment.shader"
}


int 
main
(
    int, char **
)
{
  // Init
  {
    SDLH_InitSDL();
    
    MRE_F64 fovy  = MRE_PI / 5.0;
    MRE_F64 ratio = WINDOW_WIDTH / ( MRE_F64 )WINDOW_HEIGHT;
    MRE_F64 minz  = 0.5;
    MRE_F64 maxz  = 100;
    
    MRE_SetPrespsectiveView(
      fovy,
      ratio,
      minz,
      maxz
    );
    
    MRE_InitZBuffer( WINDOW_WIDTH, WINDOW_HEIGHT );
  }

  // Create cube model
  // ( format of vertex ) 
  //   => { x, y, z, r, g, b }
  MRE_I32  vertex_count = 36;
  MRE_F64  vertex[  ] =
  {
     1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
     1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
     1.0, -1.0,  1.0,  1.0,  0.0,  0.0,

     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
     1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
     1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
     1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,

    -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,
    -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,
    -1.0, -1.0, -1.0,  0.0,  0.0,  1.0,

    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
    -1.0, -1.0, -1.0,  0.0,  0.0,  1.0,
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
    -1.0, -1.0, -1.0,  0.0,  0.0,  1.0,
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,

     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
    -1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
     1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
     1.0,  1.0,  1.0,  1.0,  1.0,  1.0,

    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
    -1.0, -1.0, -1.0,  0.0,  0.0,  1.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,
    -1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
     1.0, -1.0, -1.0,  0.0,  0.0,  1.0,
     1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
  };


  int quit = 0;
  while ( quit == 0 )
  {
    SDL_Event  event;

    while ( SDL_PollEvent( &event ) )
    {
      switch ( event . type )  case SDL_QUIT:  quit = 1;
    }
  
    
    SDLH_StartRenderSDL();
    SDLH_CleerBufferSDL( 0x00000000u );

    //Init bounding sphere of cube by hand to speed up rendering
    //   Also you can use MRE_SmallestBoundingSphere in MRE_bounding_sphere.h
    MRE_Vec4 bounding_sphere;
    MRE_SET_VEC4( 0.0, 0.0, 7 + SDL_GetTicks() * 0.001, sqrt( 3 ), bounding_sphere );

    MRE_SetBuffer( SDLH_pixels, WINDOW_WIDTH, WINDOW_HEIGHT );
    MRE_SetVertexAttribSize( 6 );
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
  SDLH_DestroySDL();

  return 0;
}
