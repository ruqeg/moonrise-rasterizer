#include <MRE_graphics.h>
#include <MRE_bounding_sphere.h>
#include <MRE_math.h>

#include <SDL2/SDL.h>

#include <time.h>


#define P_COUNT 6

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
  
  MRE_F64            points_x[ P_COUNT ];
  MRE_F64            points_y[ P_COUNT ];

  MRE_Mat4           projection_mat;
  MRE_Mat4           transform_mat;
  MRE_Mat4           scale_mat;
  MRE_Mat4           rotate_mat;
  MRE_Mat4           model_mat;
  MRE_Mat4           world_mat;
  MRE_F64            camera_pos_x;
  MRE_F64            camera_pos_y;
  MRE_F64            camera_pos_z;
  MRE_F64            camera_rot_x;
  MRE_F64            camera_rot_y;
  MRE_F64            camera_rot_z;
  MRE_Mat4           camera_pos_mat;
  MRE_Mat4           camera_rot_mat;
  MRE_Mat4           camera_mat;



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
    ( MRE_Vec3 ){2, 0, 12},
    transform_mat
  );
  MRE_ScaleMat4(
    ( MRE_Vec3 ){3, 1, 1},
    scale_mat
  );
  MRE_RotateMat4(
    ( MRE_Vec3 ){0, 0, 0},
    rotate_mat
  );

  MRE_Mul_Mat4(
    rotate_mat,
    scale_mat,
    model_mat
  );
  MRE_Mul_Mat4(
    transform_mat,
    model_mat,
    model_mat
  );
  MRE_Mul_Mat4(
    projection_mat,
    model_mat,
    model_mat
  );
  MRE_TanslateMat4(
    ( MRE_Vec3 ){0,0,0},
    camera_pos_mat
  );
  MRE_RotateMat4(
    ( MRE_Vec3 ){0,0,0},
    camera_rot_mat
  );

  MRE_Mul_Mat4(
    camera_rot_mat,
    camera_pos_mat,
    camera_mat
  );
  MRE_Mul_Mat4(
    camera_mat,
    model_mat,
    world_mat
  );
  MRE_Mul_Mat4(
    projection_mat,
    world_mat,
    world_mat
  );

  srand(time(NULL));

  for ( int i = 0; i < P_COUNT; ++i )
  {
    points_x[ i ] = WINDOW_WIDTH * 0.25 + rand() % (int)(WINDOW_WIDTH * 0.5 );
    points_y[ i ] = WINDOW_HEIGHT * 0.25 + rand() % (int)(WINDOW_HEIGHT * 0.5 );
  }


  while ( quit == 0 )
  {
    while ( SDL_PollEvent( &event ) )
    {
      switch ( event.type )
      {
        case SDL_QUIT:
          quit = 1;
          break;
        case SDL_KEYDOWN:
          switch ( event.key.keysym.sym )
          {
            case SDLK_w:  camera_pos_x += 0.1;   break;
            case SDLK_s:  camera_pos_x -= 0.1;   break;
            case SDLK_a:  camera_pos_y += 0.1;   break;
            case SDLK_d:  camera_pos_y -= 0.1;   break;
            case SDLK_x:  camera_pos_z += 0.1;   break;
            case SDLK_c:  camera_pos_z -= 0.1;   break;
            
            case SDLK_y:  camera_rot_x += 0.1;   break;
            case SDLK_u:  camera_rot_x -= 0.1;   break;
            case SDLK_h:  camera_rot_y += 0.1;   break;
            case SDLK_j:  camera_rot_y -= 0.1;   break;
            case SDLK_n:  camera_rot_z += 0.1;   break;
            case SDLK_m:  camera_rot_z -= 0.1;   break;
          }
          MRE_TanslateMat4(
            ( MRE_Vec3 ){-camera_pos_x, -camera_pos_y, -camera_pos_z},
            camera_pos_mat
          );
          MRE_RotateMat4(
            ( MRE_Vec3 ){-camera_rot_x, -camera_rot_y, -camera_rot_z},
            camera_rot_mat
          );

          MRE_Mul_Mat4(
            camera_rot_mat,
            camera_pos_mat,
            camera_mat
          );
          MRE_Mul_Mat4(
            camera_mat,
            model_mat,
            world_mat
          );
          MRE_Mul_Mat4(
            projection_mat,
            world_mat,
            world_mat
          );
          break;
      }
    }
    
    SDL_LockTexture( sdl_texture, NULL, &pixels, &pixels_pitch );
   
    memset( pixels, 0, sizeof(MRE_UI32) * WINDOW_WIDTH * WINDOW_HEIGHT);

    MRE_buff = pixels;
    
    MRE_RenderModel(
      veretices, 8,
      traingles, 12,
      world_mat,
      0xFFFFFFFFu
    );
  
    for ( int i = 0; i < P_COUNT; ++i )
    {
      MRE_buff[ (int)(points_x[ i ] + 1 + points_y[ i ] * WINDOW_WIDTH) ] = 0x00FF00FFu;
      MRE_buff[ (int)(points_x[ i ] - 1 + points_y[ i ] * WINDOW_WIDTH) ] = 0x00FF00FFu;
      MRE_buff[ (int)(points_x[ i ] + 1 + (points_y[ i ] + 1) * WINDOW_WIDTH) ] = 0x00FF00FFu;
      MRE_buff[ (int)(points_x[ i ] - 1 + (points_y[ i ] + 1) * WINDOW_WIDTH) ] = 0x00FF00FFu;
      MRE_buff[ (int)(points_x[ i ] + 1 + (points_y[ i ] - 1) * WINDOW_WIDTH) ] = 0x00FF00FFu;
      MRE_buff[ (int)(points_x[ i ] - 1 + (points_y[ i ] - 1) * WINDOW_WIDTH) ] = 0x00FF00FFu;
      MRE_buff[ (int)(points_x[ i ] + (points_y[ i ] + 1) * WINDOW_WIDTH) ] = 0x00FF00FFu;
      MRE_buff[ (int)(points_x[ i ] + (points_y[ i ] - 1) * WINDOW_WIDTH) ] = 0x00FF00FFu;
      MRE_buff[ (int)(points_x[ i ] + points_y[ i ] * WINDOW_WIDTH) ] = 0x00FF00FFu;
    }
    
    SDL_UnlockTexture( sdl_texture );
    

    MRE_Vec3 p[500000];
    MRE_Vec3 ru[3];
    MRE_Vec4 d;

    MRE_SmallestBoundingSphere(p, 7, d);
    printf("%f %f %f %f\n", d[0], d[1], d[2], d[3]);

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
