#include "util.h"

long double G = 6.6743e-11;

// adds up all force vectors
// give result
struct PhysicsVec2 totalForceOnObj(struct PhysicsObject objList[], int numOfObj, int idOfCurrentObj, double softener) {
    struct PhysicsVec2 totalForce;
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
        struct PhysicsVec2 force;
        force.x = ((distx / totalDist) * magnitude);
        force.y = ((disty / totalDist) * magnitude);
        totalForce.x += force.x;
        totalForce.y += force.y;
    }
    return totalForce;
}

// just fancy division
struct PhysicsVec2 totalAccOnObj(struct PhysicsVec2 force, double mass) {
    struct PhysicsVec2 newAcc;
    newAcc.x = (force.x / mass);
    newAcc.y = (force.y / mass);
    return newAcc;
}

struct PhysicsVec2 updateOfVelocity(struct PhysicsVec2 vel, struct PhysicsVec2 acc, double time) { // really small quantity
    struct PhysicsVec2 velclone;
    velclone.x = vel.x;
    velclone.y = vel.y;
    velclone.x += acc.x * time;
    velclone.y += acc.y * time;
    return velclone;
}

struct PhysicsVec2 updateOfPosition(struct PhysicsVec2 pos, struct PhysicsVec2 vel, double time) { // again... really small quantity
    struct PhysicsVec2 posclone;
    posclone.x = pos.x;
    posclone.y = pos.y;
    posclone.x += vel.x * time;
    posclone.y += vel.y * time;
    return posclone;
}

// actual manager
void physicsIteration(struct PhysicsObject objList[], int numOfObj, double softener, double dt) {
    // accumulates over time and maintains consistency
    for (int i = 0; i < numOfObj; i++) {
        struct PhysicsVec2 force = totalForceOnObj(objList, numOfObj, i, softener);
        struct PhysicsVec2 acc = totalAccOnObj(force, objList[i].mass);
        objList[i].velocity = updateOfVelocity(objList[i].velocity, acc, dt);
        objList[i].position = updateOfPosition(objList[i].position, objList[i].velocity, dt);
    }
    usleep((dt * 1e6));
}