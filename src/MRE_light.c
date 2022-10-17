#include <MRE_light.h>

MRE_F64
MRE_GetVertIlluminaceLevel
(
    const MRE_Vec3  v,
    const MRE_Vec3  n
)
{
  MRE_I32   i;
  MRE_F64   rilluminace;
  MRE_F64   scm;
  MRE_Vec3  light_ray;


  rilluminace = MRE_global_illuminace;
  
  for ( i = 0; i < MRE_light_sources_count; ++i )
  {
    if ( MRE_light_sources[ i ] . type == MRE_LIGHT_POINT )
    {
      MRE_SUB_VEC3( v, MRE_light_sources[ i ] . v, light_ray );
    }
    else //if ( MRE_light_sources[ i ] . type == MRE_LIGHT_RAY )
    {
      MRE_COPY_VEC3( MRE_light_sources[ i ] . v, light_ray );
    }
    
    scm = MRE_SCMUL_VEC3( n, light_ray );
    if ( scm > 0 )
    {
      rilluminace += MRE_light_sources[ i ] . illuminace * (
        scm / ( MRE_VEC3_LENGTH( n ) * MRE_VEC3_LENGTH( light_ray ) )
      );
    }
  }

  return rilluminace;
}
