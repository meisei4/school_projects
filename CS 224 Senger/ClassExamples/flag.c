#include <stdio.h>
#include <math.h>

// ./flag | gm convert -size 950x500 -depth 8 rgb:- flag.png

#define M_2PI (2 * M_PI)
#define HEIGHT 500
#define WIDTH ((int)(HEIGHT * 1.9))
#define UNION_WIDTH ((int)(WIDTH * 2 / 5.0))
#define UNION_HEIGHT ((int)(HEIGHT / 13.0 * 7))

int
main(int argc, char * argv[])
{
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (x < UNION_WIDTH && y < UNION_HEIGHT) {
        double d = hypot(x - (UNION_WIDTH / 2),y - (UNION_HEIGHT / 2));
        double a = atan2(y - (UNION_HEIGHT / 2),x - (UNION_WIDTH / 2)) + 11/10.0 * M_PI;
        a = fmod(a, (M_2PI / 5.0));
        a = fabs(a - M_2PI / 10.0);
        double r = 35 / cos(a - M_2PI / 5.0);
        if (d < r) {
          printf("%c%c%c",255,255,255);
        }
        else {
          printf("%c%c%c",60,59,110); // Old Glory Blue
        }
      }
      else {
        if ( (int)(y / 38.46) % 2 == 0) {
          printf("%c%c%c",178,34,52); // Old Glory Red
        }
        else {
          printf("%c%c%c",255,255,255);
        }
      }
    }
  }
}