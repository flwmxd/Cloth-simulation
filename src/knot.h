#ifndef KNOT_H
#define KNOT_H

#include <glm/glm.hpp>

class Knot {

public:
    // Constructors
    Knot(glm::vec3 p, bool is = false)
        : position(p), _isStatic(is) {};

    // Member functions
    void integrateVelocity(float dt) {
        if(!_isStatic)
            position += velocity * dt;
    };


    void applyG(const float G, float dt) {
        if(!_isStatic)
            velocity += glm::vec3(0.0f, G, 0.0f) * dt;
    };


    void applySpringForce() {

    }

    void addNeighbor(Knot *k) {
        neighbors.push_back(k);
    }

    // Getters
    glm::vec3 getPosition() { return this->position; }
    glm::vec3 getVelocity() { return this->velocity; }

    // Setters
    void setStatic() { _isStatic = true; };

    void debugKnotPosition() {
        std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ")";
    }

private:
    glm::vec3 position;
    glm::vec3 velocity;
    bool _isStatic;
    std::vector<Knot *> neighbors;
};

#endif // KNOT_H