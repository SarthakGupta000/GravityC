# GravityC

A 2D N-body gravitational simulation in C using Newton's law of universal gravitation with a fixed-timestep accumulator loop.

## How it works

- **Objects** have mass, position, and velocity (2D vectors using `long double`).
- Each tick, the net gravitational force on every object from all others is computed via `F = G * m₁ * m₂ / r²` (with a softening factor to prevent singularities).
- Acceleration is derived from `F = ma`, then velocity and position are integrated with `v += a·dt` and `p += v·dt`.
- Positions and velocities are updated in a double-buffered fashion to avoid order-dependent bias.
- The main loop uses an accumulator pattern: real wall-clock time drives a fixed simulation timestep (`dt = 0.0001s`), keeping the sim rate consistent regardless of frame time.

## Constants

| Constant | Value | Meaning |
|---|---|---|
| `SOFT`   | 1000  | Softening parameter (avoids division by zero) |
| `MAX`    | 100000 | Range for random initial values |
| `OBJS`   | 1000  | Number of simulated objects |

## Usage

```bash
# Compile (requires C99 or later, link math library)
gcc main.c -lm -o gravityc

# Run
./gravityc
```

The simulation runs indefinitely. Hit `Ctrl-C` to stop.
