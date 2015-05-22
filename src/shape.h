#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>

/*
 * Abstract Shape class
 *  This is an abstract class for all shapes that are supported
 */

class Body;

class Shape {

public:
    virtual void draw(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&, unsigned int) = 0;
    virtual unsigned int getType() = 0;
    virtual void init(glm::vec3) = 0;
    virtual void reset() = 0;
    virtual glm::vec3 getPosition() = 0;
    virtual void integrateVelocity(const glm::vec3, float) = 0;
    virtual void applyG(const glm::vec3, float) = 0;
    virtual void applySpringForce(float, float) = 0;
    virtual void setBodyStatic(int) = 0;
    virtual void setWindForce(glm::vec3) = 0;

    Body * body;

};

#endif // SHAPE_H