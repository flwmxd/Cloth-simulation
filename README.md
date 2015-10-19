# Cloth-simulation
Simulation of cloth using a mass-spring model. The implementation is written in C++ and OpenGL. The simulation uses a Runge-Kutta 4 integrator and runs 15 simulations per frame.

Demo: https://vimeo.com/141994566

## User controlls

``1`` Run setup 1

``2`` Run setup 2

``3`` Run setup 3

``4`` Run setup 4

``C`` Play/pause simulation

``K`` Toggle draw mode (knot-point debugging or surface drawing)

``N`` Release all static knots

``Arrow up`` Increase height of normalmap

``Arrow down`` Decrease height of normalmap

``Mouse`` Rotate scene

``Z`` Add wind force

## Compiling and running

The included makefile only supports Mac OSX.

``make all && make run`` in terminal to compile and run the simulation

## Dependencies

SGCT 2.1.3(https://c-student.itn.liu.se/wiki/develop:sgct:sgct)

GLM (http://glm.g-truc.net/0.9.7/index.html)