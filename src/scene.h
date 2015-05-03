#ifndef SCENE_H
#define SCENE_H

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
    Scene() { };
    //~Scene();

    void init();
    void draw(glm::mat4, glm::mat4, glm::mat4);
    void checkCollisions();
    void addBody(Body *);


private:
    std::vector<Body *>bodies;

    // Position of the lightsource
    glm::vec3 lightPosition;
};


#endif // SCENE_H