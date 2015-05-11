#ifndef SCENE_H
#define SCENE_H

#define GRAVITY -9.82

#include <vector>
#include <iostream>
#include "body.h"
#include "sgct.h"
#include "glm/gtc/matrix_inverse.hpp"

/*
 * Scene class to handle the simulation.
 *  This class is the core of the simulation, it contains all objects
 */

class Scene {
public:
    Scene();
    //~Scene();

    void init();
    void draw(glm::mat4, glm::mat4, glm::mat4, unsigned int);
    
    void checkCollisions();

    void step();
    void integrateVelocities();
    void applyG();

    void addBody(Body *);

    // Getters
    glm::vec3 getLightPosition() { return this->lightPosition; };
    glm::vec3 acceleration() { return glm::vec3(0.0f, GRAVITY, 0.0f); };

    // Setters
    void setDt(float _dt) { this->dt = _dt; };
    void setTime(float _t) { this->t = _t; };


private:
    std::vector<Body *>bodies;

    // Position of the lightsource
    glm::vec3 lightPosition;

    // Time and delta time for our simulation
    float t;
    float dt;
};


#endif // SCENE_H