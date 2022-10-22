#include <MRE.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  600

MRE_I32
LoadTextureFromFile_SDL2
(
    const char          * p,
    struct MRE_Texture  * t
)
{

  SDL_PixelFormat  * format;
  SDL_Surface      * sdl_surface;

  format      = SDL_AllocFormat( SDL_PIXELFORMAT_RGBA8888 );
  sdl_surface = SDL_ConvertSurface( IMG_Load( p ), format, 0 );

  if ( sdl_surface == NULL )  return -1;

  t -> w = sdl_surface -> w;
  t -> h = sdl_surface -> h;

  t -> pixels = malloc( t -> w * t -> h * sizeof( MRE_UI32 ) );

  memcpy(
    t -> pixels,
    sdl_surface -> pixels,
    sdl_surface -> h * sdl_surface -> pitch
  );

  SDL_FreeSurface( sdl_surface );

  return 1;
}

__MRE_DEF_VERT_SHADER
(
    CustomVertexShader
)
{
  MRE_Mat4  transform_mat;
  MRE_Mat4  rotate_mat;
  MRE_Mat4  world_mat;
  MRE_Vec3  vpos;
  MRE_Vec3  vrot;
  
  MRE_SET_VEC3( 0.0, 0.0, 9.0, vpos );
  MRE_SET_VEC3(
    MRE_PI_2 * 0.5 * SDL_GetTicks() * 0.001,
    MRE_PI_2 * 0.5 * SDL_GetTicks() * 0.001, 
    MRE_PI_2 * 0.5 * SDL_GetTicks() * 0.001,
    vrot
  );

  MRE_TanslateMat4( vpos, transform_mat );
  MRE_RotateMat4( vrot, rotate_mat );
  
  MRE_MulMat4(
    transform_mat,
    rotate_mat,
    world_mat
  );
  
  MRE_MulMat4Vec3Vec3(
    world_mat,
    in, 1.0,
    out
  );
}

__MRE_DEF_FRAG_SHADER
(
    CustomFragmentShader
)
{
  MRE_COPY_VEC3( in, out );
}

int 
main
(
    int, char **
)
{
  int                 quit;

  int                 pixels_pitch;
  void              * pixels;

  SDL_Event           event;
  
  SDL_Window        * window;
  SDL_Renderer      * sdl_renderer;
  SDL_Texture       * sdl_texture;

  MRE_I32             vertex_count;
  MRE_Vec4            bounding_sphere;

  struct MRE_Texture  texture;  




  SDL_Init(SDL_INIT_VIDEO);
  SDL_SetRelativeMouseMode(SDL_TRUE);

  window = SDL_CreateWindow(
    "Moonrise Engine | Example",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  
  sdl_renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED
  );
  
  sdl_texture = SDL_CreateTexture(
    sdl_renderer, 
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING, 
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );

  
  MRE_InitZBuffer( WINDOW_WIDTH, WINDOW_HEIGHT );
  MRE_SetPrespsectiveView(
    MRE_PI / 5.0,
    WINDOW_WIDTH / ( MRE_F64 )WINDOW_HEIGHT,
    0.5,
    100.0
  );

  vertex_count = 36;
  
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

  if ( LoadTextureFromFile_SDL2( "../example/res/texture.jpg", &texture ) == -1 )
  {
    printf("Can't load texture.png\n");
  }

  quit = 0;

  while ( quit == 0 )
  {
    while ( SDL_PollEvent( &event ) )
    {
      switch ( event . type )
      {
        case SDL_QUIT:
          quit = 1;
          break;
      }
    }
    
    SDL_LockTexture( sdl_texture, NULL, &pixels, &pixels_pitch );

    {   
      memset( pixels, 0, sizeof(MRE_UI32) * WINDOW_WIDTH * WINDOW_HEIGHT);

      MRE_SET_VEC4( 0.0, 0.0, 8.0, sqrt( 3 ), bounding_sphere );

      MRE_SetBuffer( pixels, WINDOW_WIDTH, WINDOW_HEIGHT );
      MRE_SetVertexAttribSize( 8 );
      MRE_BindVertexShader( CustomVertexShader );
      MRE_BindFragmentShader( CustomFragmentShader );
      MRE_BindTexture( &texture );
      MRE_DrawArrays(
        MRE_TRIANGLES,
        vertex, vertex_count,
        bounding_sphere
      );
      MRE_ClearZBuffer();
    }

    for ( int y = 0; y < texture . h; ++y )
    {
      for ( int x = 0; x < texture . w; ++x )
      {
        *((MRE_UI32*)pixels + y * WINDOW_WIDTH + x ) = texture . pixels [ y * texture . w + x ];
      }
    }

    SDL_UnlockTexture( sdl_texture );
    
    SDL_Rect r = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy( sdl_renderer, sdl_texture, NULL, &r );

    SDL_RenderPresent( sdl_renderer );
  }

  free( texture . pixels );

  MRE_DestroyZBuffer();

  SDL_DestroyTexture( sdl_texture );
  SDL_DestroyRenderer( sdl_renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();

  return 0;
}
