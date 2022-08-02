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
