#include <MRE_bounding_sphere.h>
#include <MRE_math.h>

#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define POINTS_COUNT 10000000

#define WIDTH   800
#define HEIGHT  800
#define DIST    800
#define POS_X   -400
#define POS_Y   -400
#define POS_Z   -400

int16_t     MRE_buff_w = 1;
int16_t     MRE_buff_h = 1;
uint32_t  * MRE_buff   = NULL;

#define IPIS_EPS 0.01
#define IS_POINT_IN_SHPERE( p, s )\
(                                                \
  MRE_POINTS_DIST2( p, s ) <= s[3] + IPIS_EPS    \
)

int 
main
(
    int, char **
)
{
  srand(time(NULL));

  clock_t start, end;
 double cpu_time_used;

  double sum = 0;
  int count = 0;

  MRE_Vec4 d;
  MRE_Vec3 * p = malloc((POINTS_COUNT) * sizeof(MRE_Vec3));

  while ( 1 )
  {
    for ( int i = 0; i < POINTS_COUNT; ++i )
    {
      p[ i ][0] = (POS_X + rand() % (int)( WIDTH )) / 800.0;
      p[ i ][1] = (POS_Y + rand() % (int)( HEIGHT )) / 800.0;
      p[ i ][2] = (POS_Z + rand() % (int)( DIST )) / 800.0;
    }
     
    start = clock();
    MRE_SmallestBoundingSphere(p, POINTS_COUNT, !0, d);
    end = clock();
    /*for ( size_t i = 0; i < POINTS_COUNT; ++i )
    {
      if ( ! IS_POINT_IN_SHPERE( p[i], d ) ) {
        printf("P[%i]: P%i = (%f, %f, %f)\n", i, i, p[i][0], p[i][1], p[i][2]);
        printf("D:(x-%f)*(x-%f) + (y-%f)*(y-%f) + (z-%f)*(z-%f) = %f\n\n", d[0], d[0], d[1], d[1], d[2], d[2], d[3] * d[3]);
        exit(0);
      }
    }*/
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    sum += cpu_time_used;
    ++count;
   
    printf("POINTS_COUNT: %i\ntime: %f\nsum time: %f\nmiddle: %f\ncount: %i\n\n\n",
      POINTS_COUNT, cpu_time_used, sum, sum / count, count
    );
  }

  free(p);
  return 0;
}
