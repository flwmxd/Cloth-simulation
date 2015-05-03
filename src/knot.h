#ifndef KNOT_H
#define KNOT_H

#include <glm/glm.hpp>

class Knot {

public:
    // Constructors
    Knot(glm::vec3 p)
        : position(p) {};

    // Member functions
    glm::vec3 getPosition() { return this->position; }
    glm::vec3 getVelocity() { return this->velocity; }

private:
    glm::vec3 position;
    glm::vec3 velocity;
};

#endif // KNOT_H