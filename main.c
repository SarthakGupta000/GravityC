#include <stdlib.h>
#include <time.h>
#include "physics/util.h"
#include "rendering/util.h"
#define SOFT 1000
#define PLANE_Z 3.0            // every object lives on the plane z = PLANE_Z
#define ORBIT_RADIUS 4.5       // earth's distance from the sun in world units
#define AU 1.5e11              // earth's distance from the sun in meters
#define SIM_DT 3600.0          // one physics step is one simulated hour
#define STEPS_PER_FRAME 24     // one frame is one simulated day
#define FRAME_TIME 33000       // microseconds per frame (~30 fps)
#define SPIN 0.05              // radians the cubes turn each frame
#define REAL_DT (FRAME_TIME / 1e6 / STEPS_PER_FRAME) // real seconds physicsIteration sleeps per step
#define TIME_SCALE (SIM_DT / REAL_DT)                // simulated seconds per real second

// rotates a point around the y axis and then around the x axis
struct Vec3 rotatePoint(struct Vec3 p, double angle) {
    struct Vec3 r;
    r.x = (p.x * cos(angle)) + (p.z * sin(angle));
    r.z = (-p.x * sin(angle)) + (p.z * cos(angle));
    r.y = p.y;
    struct Vec3 s;
    s.x = r.x;
    s.y = (r.y * cos(angle)) - (r.z * sin(angle));
    s.z = (r.y * sin(angle)) + (r.z * cos(angle));
    return s;
}

// spins the cube in place (its triangles are stored relative to its origin)
void rotateObject(struct Object *obj, double angle) {
    for (int i = 0; i < obj->numOfTriangles; i++) {
        for (int k = 0; k < 3; k++) {
            obj->triangles[i][k] = rotatePoint(obj->triangles[i][k], angle);
        }
    }
}

// unit cube centred on its origin, each face gets its own shade so the spin is visible
struct Object setupUnitCube(struct Vec3 origin, double shades[3]) {
    double h = 0.5;
    struct Vec3 vertices[8] = {
        {-h, -h, -h}, {h, -h, -h}, {h, h, -h}, {-h, h, -h},
        {-h, h, h}, {h, h, h}, {h, -h, h}, {-h, -h, h}
    };
    struct Object cube = setupCuboid(vertices, origin, shades[0]);
    for (int i = 0; i < cube.numOfTriangles; i++) {
        cube.triangles[i][3].x = shades[(i / 2) % 3]; // two triangles per face
    }
    return cube;
}

int main() {
    struct PhysicsObject sun, earth;
    struct PhysicsVec2 sunpos, sunvel, earthpos, earthvel;
    sunpos.x = 0;
    sunpos.y = 0;
    sunvel.x = 0;
    sunvel.y = 0;
    earthpos.x = AU;
    earthpos.y = 0;
    earthvel.x = 0;
    earthvel.y = 29780 * TIME_SCALE;
    sun.position = sunpos;
    sun.velocity = sunvel;
    earth.position = earthpos;
    earth.velocity = earthvel;
    // velocities scale by TIME_SCALE and masses by TIME_SCALE^2 so that a step of
    // REAL_DT moves things exactly as far as a step of SIM_DT would in real units
    sun.mass = 2e30 * TIME_SCALE * TIME_SCALE;
    earth.mass = 6e24 * TIME_SCALE * TIME_SCALE;
    struct PhysicsObject objs[2];
    objs[0] = sun;
    objs[1] = earth;

    double sunShades[3] = {250, 225, 200};  // @ % #
    double earthShades[3] = {140, 115, 90}; // + = -
    struct Object cubes[2];
    cubes[0] = setupUnitCube((struct Vec3) {0, 0, PLANE_Z}, sunShades);
    cubes[1] = setupUnitCube((struct Vec3) {ORBIT_RADIUS, 0, PLANE_Z}, earthShades);
    struct World *world = setupWorld(2, cubes);
    struct Camera *camera = setupCamera();

    while (1) {
        // physics (each call sleeps REAL_DT, so this also paces the frame)
        for (int step = 0; step < STEPS_PER_FRAME; step++) {
            physicsIteration(objs, 2, SOFT, REAL_DT);
        }

        // map meters onto the z = PLANE_Z plane and spin the cubes
        for (int i = 0; i < 2; i++) {
            world->objs[i].origin.x = (objs[i].position.x / AU) * ORBIT_RADIUS;
            world->objs[i].origin.y = (objs[i].position.y / AU) * ORBIT_RADIUS;
            world->objs[i].origin.z = PLANE_Z;
            rotateObject(&world->objs[i], SPIN);
        }

        clearScreen(camera);
        render(camera, world);
    }

    deleteCamera(camera);
    deleteWorld(world);
}
