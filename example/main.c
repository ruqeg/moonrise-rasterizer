#include <MRE_graphics.h>
#include <MRE_bounding_sphere.h>
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



  const MRE_F64  aspect =  MRE_buff_w / (MRE_F64)MRE_buff_h;
  const MRE_F64  z_min  =  0.5;
  const MRE_F64  z_max  =  100.0;
  const MRE_F64  fowy   =  MRE_PI / 5.0;




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

  MRE_SetPrespsectiveView(
    fowy,
    aspect,
    z_min,
    z_max
  );

  MRE_Vec3   veretices[] =
  {
    { 1,  1,  1},
    {-1,  1,  1},
    {-1, -1,  1},
    { 1, -1,  1},
    { 1,  1, -1},
    {-1,  1, -1},
    {-1, -1, -1},
    { 1, -1, -1}
  };
  

  MRE_IVec3   traingles[] =
  {
   {0, 1, 2},
   {0, 2, 3},
   {4, 0, 3},
   {4, 3, 7},
   {5, 4, 7},
   {5, 7, 6},
   {1, 5, 6},
   {1, 6, 2},
   {4, 5, 1},
   {4, 1, 0},
   {2, 6, 7},
   {2, 7, 3}
  };

  MRE_TanslateMat4(
    ( MRE_Vec3 ){2, 0, 12},
    transform_mat
  );
  MRE_ScaleMat4(
    ( MRE_Vec3 ){1, 1, 1},
    scale_mat
  );
  MRE_RotateMat4(
    ( MRE_Vec3 ){0, 0, 0},
    rotate_mat
  );

  MRE_MulMat4(
    rotate_mat,
    scale_mat,
    model_mat
  );
  MRE_MulMat4(
    transform_mat,
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

  MRE_MulMat4(
    camera_rot_mat,
    camera_pos_mat,
    camera_mat
  );
  MRE_MulMat4(
    camera_mat,
    model_mat,
    world_mat
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
        case SDL_KEYDOWN:
          switch ( event.key.keysym.sym )
          {
            case SDLK_w:  camera_pos_z += 0.1;   break;
            case SDLK_s:  camera_pos_z -= 0.1;   break;
            case SDLK_a:  camera_pos_x -= 0.1;   break;
            case SDLK_d:  camera_pos_x += 0.1;   break;
            case SDLK_x:  camera_pos_y += 0.1;   break;
            case SDLK_c:  camera_pos_y -= 0.1;   break;
            
            case SDLK_y:  camera_rot_z += 0.1;   break;
            case SDLK_u:  camera_rot_z -= 0.1;   break;
            case SDLK_h:  camera_rot_x += 0.1;   break;
            case SDLK_j:  camera_rot_x -= 0.1;   break;
            case SDLK_n:  camera_rot_y += 0.1;   break;
            case SDLK_m:  camera_rot_y -= 0.1;   break;
          }
          MRE_RotateMat4(
            ( MRE_Vec3 ){-camera_rot_x, -camera_rot_y, -camera_rot_z},
            camera_rot_mat
          );
          MRE_TanslateMat4(
            ( MRE_Vec3 ){-camera_pos_x, -camera_pos_y, -camera_pos_z},
            camera_pos_mat
          );

          MRE_MulMat4(
            camera_rot_mat,
            camera_pos_mat,
            camera_mat
          );
          MRE_MulMat4(
            camera_mat,
            model_mat,
            world_mat
          );
          break;
      }
    }
    
    SDL_LockTexture( sdl_texture, NULL, &pixels, &pixels_pitch );
   
    memset( pixels, 0, sizeof(MRE_UI32) * WINDOW_WIDTH * WINDOW_HEIGHT);

    MRE_buff = pixels;
   
    MRE_Vec3  world_vert[ 8 ];
    for ( int k = 0; k < 8; ++k )
    {
      MRE_MulMat4Vec3Vec3( world_mat, veretices[ k ], 1.0, world_vert[ k ] );
    }

    MRE_Vec4 sphere;

    MRE_SmallestBoundingSphere( world_vert, 8, MRE_RECR_SBS_FLAG, sphere );

    MRE_I32      new_vert_count = 8;
    MRE_I32      new_trig_count = 12;
    MRE_Vec3   * vert_res;
    MRE_IVec3  * triangles_res;
    MRE_I32 status = MRE_ClipModel(
      world_vert, &new_vert_count,
      traingles, &new_trig_count, 
      sphere,
      &vert_res, &triangles_res
    );
    
    if ( status == MRE_MODEL_INPL || status == MRE_MODEL_CLIPED )
    {
      MRE_RenderModel(
        vert_res, new_vert_count,
        triangles_res, new_trig_count,
        0xFFFFFFFFu
      );
    }
    free( vert_res );
    free( triangles_res );

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

