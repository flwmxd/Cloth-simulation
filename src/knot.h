#ifndef KNOT_H
#define KNOT_H

#define _MASS 1.0f
#define SPRING_CONSTANT 0.00001f
#define DAMPING 1.0f

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Knot {

public:
    // Constructors
    Knot(glm::vec3, bool is = false);

    // Member functions
    void reset();

    void integrateVelocity(const glm::vec3, float);

    void applyG(const glm::vec3, float);

    void applySpringForce(float);

    void integrateForce(const float dt) {
        this->velocity += (this->force / (float)_MASS) * dt;
        this->force *= 0.95f;
    }

    void addAdjNeighbor(Knot *k) {
        adjNeighbors.push_back(k);
    }

    void addDiagNeighbor(Knot *k) {
        diagNeighbors.push_back(k);
    }

    void addFlexNeighbor(Knot *k) {
        flexNeighbors.push_back(k);
    }


    // Getters
    glm::vec3 getPosition() { return this->position; }
    glm::vec3 getInitialPosition() { return this->initial_position; }
    glm::vec3 getVelocity() { return this->velocity; }
    glm::vec3 getForce() { return this->force; }
    int getMass() { return _MASS; }    // MAKE THIS MORE PHYSICALY ACCURATE BY USING POLYGON AREA DIVIDED BY 3
    std::vector<Knot *> getAdjNeighbors() { return this->adjNeighbors;  }
    std::vector<Knot *> getDiagNeighbors() { return this->diagNeighbors;  }
    std::vector<Knot *> getFlexNeighbors() { return this->flexNeighbors;  }
    bool isStatic() { return _isStatic; }

    // Setters
    void setStatic() { _isStatic = true; };
    void setPosition(glm::vec3 p) { this->position = p; };
    void setForce(glm::vec3 f) { this->force = f; };
    void setWindForce(glm::vec3 w_f) { this->wind = w_f; };
    void addForce(glm::vec3 f) { this->force += f; };

    void debugKnotPosition() {
        std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    }

private:
    glm::vec3 position;
    glm::vec3 initial_position;
    glm::vec3 velocity;
    glm::vec3 force;
    glm::vec3 wind;
    bool _isStatic;
    std::vector<Knot *> adjNeighbors;
    std::vector<Knot *> diagNeighbors;
    std::vector<Knot *> flexNeighbors;
};


/*
 * Some helpers for Runge-Kutta 4
 */

struct Derivative {
    glm::vec3 dx;
    glm::vec3 dv;
};

struct State {
    glm::vec3 x;
    glm::vec3 v;
};

Derivative evaluate(Knot *knot, float dt, glm::vec3 a, const Derivative &d);
Derivative evaluate(Knot *knot, glm::vec3 a);

/*
Derivative evaluate(Knot *knot, float t, float dt, glm::vec3 a, const Derivative &d) {

    State state;
    //state.x = knot->getPosition() + d.dx * dt;
    //state.v = knot->getVelocity() + d.dv * dt;

    Derivative output;
    //output.dx = state.v;
    //output.dv = a;

    return output;
}
*/
/*

Derivative evaluate(Knot *knot, float t, glm::vec3 a) {

    Derivative output;
    //output.dx = knot->getVelocity();
    //output.dv = a;

    return output;
}
*/
#endif // KNOT_H