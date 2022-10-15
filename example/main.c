#include <MRE_graphics.h>
#include <MRE_bounding_sphere.h>
#include <MRE_geometry.h>
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

  MRE_Vec4           bounding_sphere;
  MRE_Vec3         * veretices_cliped;
  MRE_IVec4        * triangles_cliped;
  MRE_Vec3         * veretices_res;
  MRE_IVec4        * triangles_res;
  MRE_I32            new_veretices_count;
  MRE_I32            new_triangles_count;

  MRE_Vec3           camera_pos;
  MRE_Vec3           camera_rot;
  MRE_Mat4           transform_mat;
  MRE_Mat4           rotation_mat;
  MRE_Mat4           world_mat;



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

  
  MRE_InitZBuffer();
  MRE_SetPrespsectiveView(
    MRE_PI / 5.0,
    MRE_buff_w / (MRE_F64)MRE_buff_h,
    0.5,
    100.0
  );


  MRE_I32            vpc = 8;
  MRE_I32            hpc = 8;


  MRE_I32  veretices_count = 2 + ( vpc - 1 ) * hpc;
  MRE_I32  triangles_count = 2 * hpc * ( vpc - 2 ) + 2 * hpc;
  

  MRE_Vec3    veretices[ veretices_count ];
  MRE_IVec4   triangles[ triangles_count ];
  
  MRE_Vec3    world_veretices[ veretices_count ];
  

  MRE_CreateSphere( vpc, hpc, veretices, triangles );


  camera_pos[0] = 0;
  camera_pos[1] = 0;
  camera_pos[2] = 10;
  
  camera_rot[0] = 0;
  camera_rot[1] = 0;
  camera_rot[2] = 0;

  MRE_TanslateMat4( camera_pos, transform_mat );
  MRE_RotateMat4( camera_rot, rotation_mat );

  MRE_MulMat4(
    rotation_mat,
    transform_mat,
    world_mat
  );


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
        case SDL_KEYDOWN:
          switch ( event . key . keysym . sym )
          {
            case SDLK_w:  camera_pos[2] -= 0.1;   break;
            case SDLK_s:  camera_pos[2] += 0.1;   break;
            case SDLK_a:  camera_pos[0] += 0.1;   break;
            case SDLK_d:  camera_pos[0] -= 0.1;   break;
            case SDLK_x:  camera_pos[1] -= 0.1;   break;
            case SDLK_c:  camera_pos[1] += 0.1;   break;

            case SDLK_y:  camera_rot[2] += 0.1;   break;
            case SDLK_u:  camera_rot[2] -= 0.1;   break;
            case SDLK_h:  camera_rot[0] += 0.1;   break;
            case SDLK_j:  camera_rot[0] -= 0.1;   break;
            case SDLK_n:  camera_rot[1] += 0.1;   break;
            case SDLK_m:  camera_rot[1] -= 0.1;   break;
          }
          MRE_RotateMat4( camera_rot, rotation_mat );
          MRE_TanslateMat4( camera_pos, transform_mat );

          MRE_MulMat4(
            rotation_mat,
            transform_mat,
            world_mat
          );
      }
    }

    for ( int k = 0; k < veretices_count; ++k )
    {
      MRE_MulMat4Vec3Vec3(
        world_mat, veretices[ k ], 1.0, world_veretices[ k ]
      );
    }

    MRE_SmallestBoundingSphere(
      world_veretices, veretices_count, MRE_RECR_SBS_FLAG, bounding_sphere
    );

    new_veretices_count = veretices_count;
    new_triangles_count = triangles_count;

    MRE_I32 status = MRE_ClipModel(
      world_veretices, &new_veretices_count,
      triangles, &new_triangles_count, 
      bounding_sphere,
      &veretices_cliped, &triangles_cliped
    );

    veretices_res = alloca( new_veretices_count * sizeof( MRE_Vec3 ) );
    triangles_res = alloca( new_triangles_count * sizeof( MRE_IVec4 ) );

    MRE_ClipBackFaces(
      veretices_cliped, &new_veretices_count, 
      triangles_cliped, &new_triangles_count,
      veretices_res, triangles_res
    );

    
    SDL_LockTexture( sdl_texture, NULL, &pixels, &pixels_pitch );
    
    {   
      memset( pixels, 0, sizeof(MRE_UI32) * WINDOW_WIDTH * WINDOW_HEIGHT);

      MRE_buff = pixels;


      if ( status == MRE_MODEL_INPL || status == MRE_MODEL_CLIPED )
      {
        MRE_RenderModel(
          veretices_res, new_veretices_count,
          triangles_res, new_triangles_count
        );

        free( veretices_cliped );
        free( triangles_cliped );
      }
    }

    SDL_UnlockTexture( sdl_texture );
  
    MRE_ClearZBuffer();
    
    SDL_Rect r = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy( sdl_renderer, sdl_texture, NULL, &r );

    SDL_RenderPresent( sdl_renderer );
  }


  MRE_DestroyZBuffer();

  SDL_DestroyTexture( sdl_texture );
  SDL_DestroyRenderer( sdl_renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();

  return 0;
}
