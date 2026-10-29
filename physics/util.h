#include <math.h>
#include <unistd.h>


struct PhysicsVec2 {
    long double x;
    long double y;
};

struct PhysicsObject {
    long double mass;
    struct PhysicsVec2 position;
    struct PhysicsVec2 velocity;
};

struct PhysicsVec2 totalForceOnObj(struct PhysicsObject objList[], int numOfObj, int idOfCurrentObj, double softener);

struct PhysicsVec2 totalAccOnObj(struct PhysicsVec2 force, double mass);

struct PhysicsVec2 totalAccOnObj(struct PhysicsVec2 force, double mass);

struct PhysicsVec2 updateOfVelocity(struct PhysicsVec2 vel, struct PhysicsVec2 acc, double time);

struct PhysicsVec2 updateOfPosition(struct PhysicsVec2 pos, struct PhysicsVec2 vel, double time);

void physicsIteration(struct PhysicsObject objList[], int numOfObj, double softener, double dt);