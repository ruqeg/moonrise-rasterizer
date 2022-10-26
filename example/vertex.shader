MRE_Mat4  transform_mat;
MRE_Mat4  rotate_mat;
MRE_Mat4  world_mat;

MRE_TanslateMat4( ( MRE_Vec3 )
  { 
    0.0, 0.0, 7 + SDL_GetTicks() * 0.001
  },
  transform_mat 
);
MRE_RotateMat4( ( MRE_Vec3 )
  {
    MRE_PI_2 * SDL_GetTicks() * 0.001,
    MRE_PI_2 * SDL_GetTicks() * 0.001, 
    MRE_PI_2 * SDL_GetTicks() * 0.001,
  },
  rotate_mat
);

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
