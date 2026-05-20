#include <stdlib.h>
#include <time.h>
#include "util.h"
#define SOFT 1000
#define MAX 100000
#define OBJS 1000

struct Vec2 getRandomVector(long int max) {
    struct Vec2 vec;
    vec.x = rand() % max;
    vec.y = rand() % max;
    return vec;
}

int main() {
    srand(time(NULL));
    struct Object objs[OBJS]; // vla but fine
    for (int i = 0; i < OBJS; i++) {
        struct Object obj;
        obj.position = getRandomVector(MAX);
        obj.velocity = getRandomVector(MAX);
        obj.mass = (rand() % MAX) + 1;
        objs[i] = obj;
    }
    physicsLoop(objs, OBJS, SOFT, 0.0001);
}