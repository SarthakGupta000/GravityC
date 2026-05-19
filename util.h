#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define G (6.6743 * pow(10, -11))

struct Vec2 {
    double x;
    double y;
};

struct Vec2 *newVector2D(double x, double y) {
    struct Vec2 *newVec = (struct Vec2 *) malloc(sizeof(struct Vec2));
    newVec->x = x;
    newVec->y = y;
    return newVec;
}

void deleteVector2D(struct Vec2 *vec) {
    free(vec);
}

struct Object {
    double mass;
    struct Vec2 *position;
    struct Vec2 *velocity;
};

struct Vec2 *totalForceOnObj(struct Object objList[], int numOfObj, int idOfCurrentObj) {
    struct Vec2 *totalForce = newVector2D(0, 0);
    for (int i = 0; i < numOfObj; i++) {
        if (i == idOfCurrentObj) {
            continue;
        }
        double disty = objList[i].position->y - objList[idOfCurrentObj].position->y;
        double distx = objList[i].position->x - objList[idOfCurrentObj].position->x;
        double totalDist = sqrt((disty * disty) + (distx * distx));
        double magnitude = ((G * objList[i].mass * objList[idOfCurrentObj].mass) / (totalDist * totalDist)); // Newton's Formula for Gravity
        struct Vec2 force;
        force.x = ((distx / totalDist) * magnitude);
        force.y = ((disty / totalDist) * magnitude);
        totalForce->x += force.x;
        totalForce->y += force.y;
    }
    return totalForce;
}

struct Vec2 *totalAccOnObj(struct Vec2 *force, double mass) {
    struct Vec2 *newAcc = newVector2D(0, 0);
    newAcc->x = (force->x / mass);
    newAcc->y = (force->y / mass);
    return newAcc;
}