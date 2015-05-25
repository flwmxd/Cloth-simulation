#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include "knot.h"

/*
 * Abstract Shape class
 *  This is an abstract class for all shapes that are supported
 */

 // FIX THE VIRTUAL FUNCTIONS IN SUB-CLASSES

class Body;

class Shape {

public:
    virtual void draw(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&, unsigned int) = 0;
    virtual void init(glm::vec3) = 0;
    virtual void reset() {};

    virtual void integrateVelocity(const glm::vec3, float) {};
    virtual void applyG(const glm::vec3, float) {};
    virtual void applySpringForce(float, float, glm::vec3) {};
    virtual void resolveCollision(Knot *) {};
    
    virtual unsigned int getType() = 0;
    virtual glm::vec3 getPosition() = 0;
    virtual std::vector<Knot *> getKnots() { std::vector<Knot *>temp; return temp; };

    virtual void setBodyStatic(int) = 0;
    virtual void setWindForce(glm::vec3) {};
    virtual void setPosition(glm::vec3) = 0;
    
    virtual void setup1() {};
    virtual void setup2() {};

    Body * body;

};

#endif // SHAPE_H