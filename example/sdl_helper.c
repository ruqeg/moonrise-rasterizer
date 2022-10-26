#include <SDL2/SDL.h>
  

SDL_Window    * SDLH_window;
SDL_Renderer  * SDLH_renderer;
SDL_Texture   * SDLH_texture;

int             SDLH_pixels_pitch;
void          * SDLH_pixels;


void
SDLH_InitSDL
(

)
{
  SDL_Init(SDL_INIT_VIDEO);
#ifdef NO_CURSOR
  SDL_SetRelativeMouseMode(SDL_TRUE);
#endif
  SDLH_window = SDL_CreateWindow(
    WINDOW_TITLE,
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  
  SDLH_renderer = SDL_CreateRenderer(
    SDLH_window, -1, SDL_RENDERER_ACCELERATED
  );
  
  SDLH_texture = SDL_CreateTexture(
    SDLH_renderer, 
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STREAMING, 
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );
}

void
SDLH_StartRenderSDL
(
   
)
{
  SDL_LockTexture( 
    SDLH_texture, NULL, &SDLH_pixels, &SDLH_pixels_pitch
  );
}

void
SDLH_CleerBufferSDL
(
    unsigned int  color
)
{
  memset(
    SDLH_pixels, color, SDLH_pixels_pitch * WINDOW_HEIGHT 
  );
}

void
SDLH_EndRenderSDL
(
   
)
{
  SDL_UnlockTexture( SDLH_texture );
  
  SDL_Rect r = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
  SDL_RenderCopy( SDLH_renderer, SDLH_texture, NULL, &r );

  SDL_RenderPresent( SDLH_renderer );
}

void
SDLH_DestroySDL
(

)
{
  SDL_DestroyTexture( SDLH_texture );
  SDL_DestroyRenderer( SDLH_renderer );
  SDL_DestroyWindow( SDLH_window );
  SDL_Quit();
}
