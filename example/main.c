#include <MRE_graphics.h>
#include <MRE_math.h>

#include <SDL2/SDL.h>


#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  600

int16_t     MRE_buff_w = WINDOW_WIDTH;
int16_t     MRE_buff_h = WINDOW_HEIGHT;
uint32_t  * MRE_buff   = NULL;

int 
main
(
    int, char **
)
{
  int                quit;

  int                pixels_pitch;
  void             * pixels;

  SDL_Event          event;
  
  SDL_Window       * window;
  SDL_Renderer     * sdl_renderer;
  SDL_Texture      * sdl_texture;

  struct MRE_Color   triangle_color;

  MRE_Mat4           projection_mat;
  MRE_Mat4           transform_mat;
  MRE_Mat4           scale_mat;
  MRE_Mat4           rotate_mat;
  MRE_Mat4           mat;



  SDL_Init(SDL_INIT_VIDEO);

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


 
  triangle_color.r = 255;
  triangle_color.g = 0;
  triangle_color.b = 255;
  triangle_color.a = 255;

  quit = 0;

  MRE_PerspectiveMat4(
    MRE_PI / 3.0,
    WINDOW_WIDTH / ( double )( WINDOW_HEIGHT ),
    1,
    100,
    projection_mat
  );

  MRE_F64   veretices[] =
  {
     1,  1,  1,
    -1,  1,  1,
    -1, -1,  1,
     1, -1,  1,
     1,  1, -1,
    -1,  1, -1,
    -1, -1, -1,
     1, -1, -1
  };
  

  MRE_UI32  traingles[] =
  {
    0, 1, 2,
    0, 2, 3,
    4, 0, 3,
    4, 3, 7,
    5, 4, 7,
    5, 7, 6,
    1, 5, 6,
    1, 6, 2,
    4, 5, 1,
    4, 1, 0,
    2, 6, 7,
    2, 7, 3
  };

  MRE_TanslateMat4(
    ( MRE_Vec3 ){2, 0, 6},
    transform_mat
  );
  MRE_ScaleMat4(
    ( MRE_Vec3 ){1, 1.5, 1},
    scale_mat
  );
  MRE_RotateMat4(
    ( MRE_Vec3 ){MRE_PI_2, 0, 0},
    rotate_mat
  );

  MRE_Mul_Mat4(
    rotate_mat,
    scale_mat,
    mat
  );
  MRE_Mul_Mat4(
    transform_mat,
    mat,
    mat
  );
  MRE_Mul_Mat4(
    projection_mat,
    mat,
    mat
  );

  while ( quit == 0 )
  {
    while ( SDL_PollEvent( &event ) )
    {
      switch ( event.type )
      {
        case SDL_QUIT:
          quit = 1;
          break;
      }
    }
    
    SDL_LockTexture( sdl_texture, NULL, &pixels, &pixels_pitch );
   
    memset( pixels, 0, sizeof(MRE_UI32) * WINDOW_WIDTH * WINDOW_HEIGHT);

    MRE_buff = pixels;
    
    MRE_RenderModel(
      veretices, 8,
      traingles, 12,
      mat,
      0xFFFFFFFFu
    );
    
    SDL_UnlockTexture( sdl_texture );
    

    SDL_Rect r = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy( sdl_renderer, sdl_texture, NULL, &r );
    
    SDL_RenderPresent( sdl_renderer );
  }


  SDL_DestroyTexture( sdl_texture );
  SDL_DestroyRenderer( sdl_renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();

  return 0;
}
