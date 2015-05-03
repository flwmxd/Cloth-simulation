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
    virtual void draw(glm::mat4&, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM) = 0;
    virtual unsigned int getType() = 0;
    virtual void init() = 0;

    Body * body;

};

#endif // SHAPE_H