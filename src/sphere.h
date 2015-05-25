#ifndef SPHERE_H
#define SPHERE_H

#define SPHERE_SHAPE 2

#include <iostream>
#include <vector>
#include "sgct.h"
#include "shape.h"

class Sphere : public Shape {

public:

    // Constructors
    Sphere(float, glm::vec3, glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f));
    //~Sphere();

    // Member functions
    void draw(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&, unsigned int);
    void init(glm::vec3);
    void reset();

    void resolveCollision(Knot *);

    // Setters
    void setPosition(glm::vec3 p) { this->position = p; };
    void setVelocity(glm::vec3 v) { this->velocity = v; };
    void setBodyStatic(int indx) { this->_isStatic = true; };

    // Getters
    unsigned int getType() { return SPHERE_SHAPE; };
    glm::vec3 getPosition() { return this->position; };

private:
    float radius;
    glm::vec3 position;
    glm::vec3 initial_position;
    glm::vec3 velocity;
    bool _isStatic;

    sgct_utils::SGCTSphere * obj_mesh;

    GLint MVPLoc;
};


#endif // SPHERE_H