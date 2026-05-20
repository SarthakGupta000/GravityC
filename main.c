#include <stdlib.h>
#include <time.h>
#include "util.h"
#define SOFT 10
#define MAX 10000000
#define OBJS 2

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
        obj.mass = rand() % MAX;
        objs[i] = obj;
    }
    physicsLoop(objs, OBJS, SOFT, 0.05);
}