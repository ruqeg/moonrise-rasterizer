#include <MRE_bounding_sphere.h>

MRE_INLINE_
void
CalcCircumscribedSphere2Points
(
    const MRE_Vec3  p1,
    const MRE_Vec3  p2,
    MRE_Vec4        d
)
{
  d[0] = ( p1[0] + p2[0] ) / 2.0;
  d[1] = ( p1[1] + p2[1] ) / 2.0;
  d[2] = ( p1[2] + p2[2] ) / 2.0;
  d[3] = MRE_POINTS_DIST2( d, p1 );
}

MRE_INLINE_
void
CalcCircumscribedSphere3Points
(
    const MRE_Vec3  p1,
    const MRE_Vec3  p2,
    const MRE_Vec3  p3,
    MRE_Vec4        d
)
{
  //MabX _|_ AB
  const MRE_F64 a1 = p1[0]-p2[0];
  const MRE_F64 b1 = p1[1]-p2[1];
  const MRE_F64 c1 = p1[2]-p2[2];
  const MRE_F64 d1 = (
    MRE_POW2(p1[0]) - MRE_POW2(p2[0]) +
    MRE_POW2(p1[1]) - MRE_POW2(p2[1]) +
    MRE_POW2(p1[2]) - MRE_POW2(p2[2])
  ) / 2.0;
  //MacX _|_ AC
  const MRE_F64 a2 = p1[0]-p3[0];
  const MRE_F64 b2 = p1[1]-p3[1];
  const MRE_F64 c2 = p1[2]-p3[2];
  const MRE_F64 d2 = (
    MRE_POW2(p1[0]) - MRE_POW2(p3[0]) +
    MRE_POW2(p1[1]) - MRE_POW2(p3[1]) +
    MRE_POW2(p1[2]) - MRE_POW2(p3[2])
  ) / 2.0;
  //AX & BX & CX are coplanar
  const MRE_F64 a3 = b1*c2-c1*b2;
  const MRE_F64 b3 = a2*c1-c2*a1;
  const MRE_F64 c3 = a1*b2-a2*b1;
  const MRE_F64 d3 = p1[0]*a3+p1[1]*b3+p1[2]*c3;

  if
  (
    MRE_SolveLinerSystem3
    (
      a1, b1, c1, d1,
      a2, b2, c2, d2,
      a3, b3, c3, d3,
      &d[0], &d[1], &d[2]
    )
  )
  {
    d[3] = MRE_F64_MAX;
  }
  else
  {
    d[3] = MRE_POINTS_DIST2( d, p1 );
  }
}

MRE_INLINE_
void
CalcCircumscribedSphere4Points
(
    const MRE_Vec3  p1,
    const MRE_Vec3  p2,
    const MRE_Vec3  p3,
    const MRE_Vec3  p4,
    MRE_Vec4        d
)
{
  //MabX _|_ AB
  const MRE_F64 a1 = p1[0]-p2[0];
  const MRE_F64 b1 = p1[1]-p2[1];
  const MRE_F64 c1 = p1[2]-p2[2];
  const MRE_F64 d1 = (
    MRE_POW2(p1[0]) - MRE_POW2(p2[0]) +
    MRE_POW2(p1[1]) - MRE_POW2(p2[1]) +
    MRE_POW2(p1[2]) - MRE_POW2(p2[2])
  ) / 2.0;
  //MacX _|_ AC
  const MRE_F64 a2 = p1[0]-p3[0];
  const MRE_F64 b2 = p1[1]-p3[1];
  const MRE_F64 c2 = p1[2]-p3[2];
  const MRE_F64 d2 = (
    MRE_POW2(p1[0]) - MRE_POW2(p3[0]) +
    MRE_POW2(p1[1]) - MRE_POW2(p3[1]) +
    MRE_POW2(p1[2]) - MRE_POW2(p3[2])
  ) / 2.0;
  //|AX| = |DX|
  const MRE_F64 a3 = 2*(p4[0]-p1[0]);
  const MRE_F64 b3 = 2*(p4[1]-p1[1]);
  const MRE_F64 c3 = 2*(p4[2]-p1[2]);
  const MRE_F64 d3 = (
    MRE_POW2(p4[0]) - MRE_POW2(p1[0]) +
    MRE_POW2(p4[1]) - MRE_POW2(p1[1]) +
    MRE_POW2(p4[2]) - MRE_POW2(p1[2])
  );

  if
  (
    MRE_SolveLinerSystem3
    (
      a1, b1, c1, d1,
      a2, b2, c2, d2,
      a3, b3, c3, d3,
      &d[0], &d[1], &d[2]
    )
  )
  {
    d[3] = MRE_F64_MAX;
  }
  else
  {
    d[3] = MRE_POINTS_DIST2( d, p1 );
  }
}

void
CalcCircumscribedSphere
(
    const MRE_Vec3  * r,
    MRE_I32           rs,
    MRE_Vec4          d
)
{
  if ( rs == 0 )
  {
    MRE_SET_VEC4( 0.0, 0.0, 0.0, 0.0, d );
  }
  else if ( rs == 1 )
  {
    MRE_SET_VEC4_VEC3S( r[0], 0.0, d );
  }
  else if ( rs == 2 )
  {
    CalcCircumscribedSphere2Points( r[0], r[1], d );
  }
  else if ( rs == 3 )
  {
    MRE_Vec4 new_d;
#define SML_3P_SPHERE_2P( p1, p2, op )\
({                                                \
  CalcCircumscribedSphere2Points( p1, p2, new_d );\
  if ( new_d[3] < d[3] )                          \
    if ( MRE_IS_POINT_IN_SPHERE( op, new_d ) )    \
      MRE_COPY_VEC4( new_d, d);                   \
})

    CalcCircumscribedSphere3Points( r[0], r[1], r[2], d );
    SML_3P_SPHERE_2P( r[1], r[2], r[0] );
    SML_3P_SPHERE_2P( r[0], r[2], r[1] );
    SML_3P_SPHERE_2P( r[0], r[1], r[2] );
  }
  else if ( rs == 4 )
  {
    MRE_Vec4 new_d;
#define SML_4P_SPHERE_2P( p1, p2, op1, op2 )\
({                                                \
  CalcCircumscribedSphere2Points( p1, p2, new_d );\
  if ( new_d[3] < d[3] )                          \
    if ( MRE_IS_POINT_IN_SPHERE( op1, new_d ) )   \
      if ( MRE_IS_POINT_IN_SPHERE( op2, new_d ) ) \
        MRE_COPY_VEC4( new_d, d );                \
})
#define SML_4P_SPHERE_3P( p1, p2, p3, op )\
({                                                    \
  CalcCircumscribedSphere3Points( p1, p2, p3, new_d );\
  if ( new_d[3] < d[3] )                              \
    if ( MRE_IS_POINT_IN_SPHERE( op, new_d ) ){       \
      MRE_COPY_VEC4( new_d, d );           }          \
})

    CalcCircumscribedSphere4Points( r[0], r[1], r[2], r[3], d );

    SML_4P_SPHERE_3P( r[0], r[1], r[2], r[3]);
    SML_4P_SPHERE_3P( r[0], r[1], r[3], r[2]);
    SML_4P_SPHERE_3P( r[0], r[3], r[2], r[1]);
    SML_4P_SPHERE_3P( r[3], r[1], r[2], r[0]);

    SML_4P_SPHERE_2P( r[0], r[1], r[2], r[3]);
    SML_4P_SPHERE_2P( r[0], r[2], r[1], r[3]);
    SML_4P_SPHERE_2P( r[0], r[3], r[2], r[1]);
    SML_4P_SPHERE_2P( r[1], r[2], r[0], r[3]);
    SML_4P_SPHERE_2P( r[1], r[3], r[2], r[0]);
    SML_4P_SPHERE_2P( r[2], r[3], r[0], r[1]);
  }
}


void
WelzlAlgSmallestBoundingSphere_Recursive
(
    const MRE_Vec3  * p,
    MRE_I32           ps,
    MRE_Vec3        * r,
    MRE_I32           rs,
    MRE_Vec4          d
)
{
  if ( ps == 0 || rs == 4 )
  {
    CalcCircumscribedSphere( r, rs, d );
  }
  else
  {
    WelzlAlgSmallestBoundingSphere_Recursive( p, ps - 1, r, rs, d );
    if ( ! MRE_IS_POINT_IN_SPHERE( p[ ps - 1 ], d ) )
    {
      MRE_COPY_VEC3( p[ ps - 1], r[ rs ] );
      WelzlAlgSmallestBoundingSphere_Recursive( p, ps - 1, r, rs + 1, d );
    }
  }
}

void
WelzlAlgSmallestBoundingSphere_Iterative
(
    const MRE_Vec3  * p,
    MRE_I32           ps,
    MRE_Vec4          d
)
{
  MRE_I32    * task;
  MRE_I32    * new_task;
  MRE_IVec3  * stack;
  MRE_I32      stack_size;
  MRE_Vec3     r[ 4 ];


  stack = malloc( ( ps + 1 ) * sizeof(MRE_IVec3));

  stack[0][0] = 0;
  stack[0][1] = ps;
  stack[0][2] = 0;
  
  stack_size = 1;

  while ( stack_size > 0 )
  {
    task = stack[ stack_size - 1 ];

    if ( task[0] == 0 )
    {
      if ( task[1] == 0 || task[2] == 4 )
      {
        CalcCircumscribedSphere( r, task[2], d );
      }
      else
      {
        new_task = stack[ stack_size ];
        
        task[0] = 1;
        
        new_task[0] = 0;
        new_task[1] = task[1] - 1;
        new_task[2] = task[2];
        
        ++stack_size;
        continue;
      }
    }
    else if ( task[0] == 1 )
    {
      if ( ! MRE_IS_POINT_IN_SPHERE( p[ task[1] - 1 ], d ) )
      {
        MRE_COPY_VEC3( p[ task[1] - 1 ], r[ task[2] ] );
        
        new_task = stack[ stack_size ];
        
        task[0] = -1;
        
        new_task[0] = 0;
        new_task[1] = task[1] - 1;
        new_task[2] = task[2] + 1;

        ++stack_size;
        continue;
      }
    }

    --stack_size;
  }

  free( stack );
}


void
MRE_SmallestBoundingSphere
(
    const MRE_Vec3  * p,
    MRE_I32           ps,
    MRE_I32           type,
    MRE_Vec4          d
)
{
  if ( type == 0 )
  {
    MRE_Vec3  r[ 4 ];
    WelzlAlgSmallestBoundingSphere_Recursive( p, ps, r, 0, d );
  }
  else
  {
    WelzlAlgSmallestBoundingSphere_Iterative( p, ps, d );
  }
  d[3] = sqrt( d[3] );
}
