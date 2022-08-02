#include <MRE_graphics.h>

#include <SDL2/SDL.h>


#define WINDOW_WIDTH   800
#define WINDOW_HEIGHT  600

int16_t     MRE_buff_w = WINDOW_WIDTH;
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

  struct MRE_Color   f_color;
  struct MRE_Color   s_color;
  


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


 
  f_color.r = 255;
  f_color.g = 0;
  f_color.b = 255;
  f_color.a = 255;
  
  s_color.r = 7;
  s_color.g = 239;
  s_color.b = 177;
  s_color.a = 255;

  quit = 0;



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
    
    MRE_buff = pixels;
    MRE_DrawFilledTriangle(
      150, 150,
      350, 200,
      0,   400,
      0xFFFFFFFFu
    );
    MRE_DrawWireframeTriangle(
      150, 150,
      350, 200,
      0,   400,
      0xFF00FFFFu
    );
    MRE_DrawShadedTriangle
    (
      450, 50, 0.3,
      350, 120, 0,
      700, 400, 0.9,
      f_color
    );
    MRE_DrawShadedTriangle
    (
      400, 300, 0.1,
      400, 500, 0,
      600, 500, 1,
      s_color
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
