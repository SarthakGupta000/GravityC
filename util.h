#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

long double G = 6.6743e-11;

struct Vec2 {
    long double x;
    long double y;
};

struct Object {
    long double mass;
    struct Vec2 position;
    struct Vec2 velocity;
};

// adds up all force vectors
// give result
struct Vec2 totalForceOnObj(struct Object objList[], int numOfObj, int idOfCurrentObj, double softener) {
    struct Vec2 totalForce;
    totalForce.x = 0;
    totalForce.y = 0;
    for (int i = 0; i < numOfObj; i++) {
        if (i == idOfCurrentObj) {
            continue;
        }
        double disty = objList[i].position.y - objList[idOfCurrentObj].position.y;
        double distx = objList[i].position.x - objList[idOfCurrentObj].position.x;
        double totalDist = sqrt((disty * disty) + (distx * distx) + (softener * softener));
        double magnitude = ((G * objList[i].mass * objList[idOfCurrentObj].mass) / (totalDist * totalDist)); // Newton's Formula for Gravity
        struct Vec2 force;
        force.x = ((distx / totalDist) * magnitude);
        force.y = ((disty / totalDist) * magnitude);
        totalForce.x += force.x;
        totalForce.y += force.y;
    }
    return totalForce;
}

// just fancy division
struct Vec2 totalAccOnObj(struct Vec2 force, double mass) {
    struct Vec2 newAcc;
    newAcc.x = (force.x / mass);
    newAcc.y = (force.y / mass);
    return newAcc;
}

struct Vec2 updateOfVelocity(struct Vec2 vel, struct Vec2 acc, double time) { // really small quantity
    struct Vec2 velclone;
    velclone.x = vel.x;
    velclone.y = vel.y;
    velclone.x += acc.x * time;
    velclone.y += acc.y * time;
    return velclone;
}

struct Vec2 updateOfPosition(struct Vec2 pos, struct Vec2 vel, double time) { // again... really small quantity
    struct Vec2 posclone;
    posclone.x = pos.x;
    posclone.y = pos.y;
    posclone.x += vel.x * time;
    posclone.y += vel.y * time;
    return posclone;
}

// actual manager
void physicsLoop(struct Object objList[], int numOfObj, double softener, double timeReal, double timeSim) {
    double dt = timeReal; // dummy
    long double arr[numOfObj][2][2]; // contains vel and pos respectively, also, this is a vla, so, only for new compilers
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start); // dummy
    struct timespec end;
    double accumulator = 0;
    while (1) {
        // accumulates over time and maintains consistency
        clock_gettime(CLOCK_MONOTONIC, &end);
        accumulator += (end.tv_sec - start.tv_sec) + ((double) (end.tv_nsec - start.tv_nsec)) / pow(10, 9);
        start = end;
        while (accumulator >= dt) {
            for (int i = 0; i < numOfObj; i++) {
                struct Vec2 force = totalForceOnObj(objList, numOfObj, i, softener);
                struct Vec2 acc = totalAccOnObj(force, objList[i].mass);
                struct Vec2 vel = updateOfVelocity(objList[i].velocity, acc, timeSim);
                struct Vec2 pos = updateOfPosition(objList[i].position, vel, timeSim);
                arr[i][0][0] = vel.x;
                arr[i][0][1] = vel.y;
                arr[i][1][0] = pos.x;
                arr[i][1][1] = pos.y;
            }
            for (int j = 0; j < numOfObj; j++) {
                printf("position of obj%d is (%Lf, %Lf)\n", j, arr[j][1][0], arr[j][1][1]);
            }
            for (int i = 0; i < numOfObj; i++) {
                objList[i].velocity.x = arr[i][0][0];
                objList[i].velocity.y = arr[i][0][1];
                objList[i].position.x = arr[i][1][0];
                objList[i].position.y = arr[i][1][1];
            }
            accumulator -= dt;
        }
    }
}