#ifndef BODY_H
#define BODY_H

#include "shape.h"
#include <glm/glm.hpp>

class Body {

public:
    // Constructors
    Body();
    Body(Shape *);

    // Member functions
    Shape * getShape() { return shape; };
    //glm::vec3 getPosition() { return position; };

private:
    Shape * shape;

    //glm::vec3 position;
};

#endif // BODY_H