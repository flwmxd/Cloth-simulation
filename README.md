# Cloth-simulation
Simulation of cloth using a mass-spring model with basic normal mapping on the surface. The implementation is written in C++ and OpenGL. The simulation uses a Runge-Kutta 4 integrator and runs 20 simulations per frame.

## User controlls

``1`` Run setup1

``2`` Run setup2

``3`` Run setup3

``4`` Run setup4

``C`` Play/pause simulation

``K`` Toggle draw mode (knot-point debugging or surface drawing)

``Arrow up`` Increase height of normalmap

``Arrow down`` Decrease height of normalmap

``Mouse` Rotate scene

## Compiling and running

The included makefile only supports Mac OSX.

Type ``make all && make run`` in terminal to compile and run the simulation