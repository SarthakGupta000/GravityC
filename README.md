# GravityC

A small Earth–Sun gravity simulation written in C, rendered as two spinning ASCII cubes in the terminal by a hand-written 3D software rasterizer.

## How it works

### Physics (`physics/`)

- Each body has a mass, a 2D position, and a 2D velocity, stored as `long double`.
- Every step, the net force on each body is computed with Newton's law of gravitation, `F = G·m₁·m₂ / r²`, using a softening term (`SOFT`) so the force never blows up at small distances.
- Acceleration comes from `a = F / m`. Velocity is updated first (`v += a·dt`), then position (`p += v·dt`), which is semi-implicit Euler integration.
- Real units are used: the Sun is at the origin, and the Earth starts 1 AU away moving at 29.78 km/s.
- One physics step covers one simulated hour (`SIM_DT`), and 24 steps make up one frame, so each frame is one simulated day. Velocities are scaled by `TIME_SCALE` and masses by `TIME_SCALE²`, so a short real-time step moves the bodies as far as a full simulated hour would.

### Rendering (`rendering/`)

- Objects are meshes of triangles. Each cube has 12 triangles, and each face has its own brightness so you can see it spin.
- Triangles are projected with a perspective camera (`FOCAL_LENGTH`), clipped, and rasterized into a 75×30 character buffer with a depth buffer.
- Brightness is mapped to ASCII characters: the Sun is drawn with `@ % #` and the Earth with `+ = -`.
- Simulated positions in meters are mapped onto the plane `z = PLANE_Z`. 1 AU corresponds to `ORBIT_RADIUS` world units.

## Project layout

```
main.c              sets up the Earth–Sun system and runs the main loop
physics/util.c/.h   gravity, acceleration, and integration
rendering/util.c/.h cubes, camera, rasterizer, and terminal output
Makefile
```

## Key constants (`main.c`)

| Constant          | Value  | Meaning                                    |
|-------------------|--------|--------------------------------------------|
| `SOFT`            | 1000   | Gravitational softening distance (m)       |
| `AU`              | 1.5e11 | Earth–Sun distance (m)                     |
| `ORBIT_RADIUS`    | 4.5    | Earth–Sun distance on screen (world units) |
| `PLANE_Z`         | 3.0    | Depth of the plane the bodies move in      |
| `SIM_DT`          | 3600   | Simulated seconds per physics step         |
| `STEPS_PER_FRAME` | 24     | Physics steps per rendered frame           |
| `FRAME_TIME`      | 33000  | Microseconds per frame (about 30 fps)      |
| `SPIN`            | 0.05   | Cube rotation per frame (radians)          |

## Build and run

You need a C compiler (the Makefile uses `clang`) and the math library.

```bash
make        # build ./gravityc
make run    # build and run
make clean  # remove the binary
```

The simulation runs until you stop it with `Ctrl-C`.
