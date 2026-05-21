#include <stdlib.h>
#include <time.h>
#include "util.h"
#define SOFT 1000

int main() {
    struct Object sun, earth;
    struct Vec2 sunpos, sunvel, earthpos, earthvel;
    sunpos.x = 0;
    sunpos.y = 0;
    sunvel.x = 0;
    sunvel.y = 0;
    earthpos.x = 1.5e11;
    earthpos.y = 0;
    earthvel.x = 0;
    earthvel.y = 29780;
    sun.position = sunpos;
    sun.velocity = sunvel;
    earth.position = earthpos;
    earth.velocity = earthvel;
    sun.mass = 2e30;
    earth.mass = 6e24;
    struct Object objs[2];
    objs[0] = sun;
    objs[1] = earth;
    physicsLoop(objs, 2, SOFT, 0.01, 3600);
}