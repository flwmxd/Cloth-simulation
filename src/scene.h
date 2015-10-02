#ifndef SCENE_H
#define SCENE_H

//#define GRAVITY -0.01

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
    void initLightSource();
    void draw(glm::mat4, glm::mat4, glm::mat4, unsigned int);
    void drawLightSource(glm::mat4);

    void reset();
    
    void checkCollisions();

    void step();
    void applySpringForce();
    void enforceMaximumStretch();
    void integrateVelocities();
    void applyG();

    void addBody(Body *);

    // Getters
    glm::vec3 getLightPosition() { return this->lightPosition; };
    glm::vec3 getAcceleration() { return this->acceleration; };

    // Setters
    void setDt(float _dt) { this->dt = _dt; };
    void setTime(float _t) { this->t = _t; };
    void setAcceleration(glm::vec3 _a) { this->acceleration = _a; };


private:
    std::vector<Body *>bodies;

    // Position of the lightsource
    glm::vec3 lightPosition;
    GLint MVPLightLoc;           // MVP matrix for light source
    GLuint lightColorLoc;
    sgct_utils::SGCTSphere *lightSource;
    glm::vec4 lightSourceColor;

    // Time, delta time and acceleration for our simulation
    float t;
    float dt;
    glm::vec3 acceleration;
};


#endif // SCENE_H