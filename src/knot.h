#ifndef KNOT_H
#define KNOT_H

#define _MASS 10.0
#define INIT_SPRING 0.5
#define DIAG_SPRING 0.71
#define SPRING_CONSTANT 5.0
#define DAMPING 20.0

#include <glm/glm.hpp>

class Knot {

public:
    // Constructors
    Knot(glm::vec3 p, bool is = false)
        : position(p), _isStatic(is) {

            velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            force = glm::vec3(0.0f, 0.0f, 0.0f);
        };

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

        //if(!_isStatic) {

            //glm::vec3 f_int(0.0f, 0.0f, 0.0f);
            glm::vec3 delta_p;
            glm::vec3 delta_p_hat;
            glm::vec3 delta_v;
            glm::vec3 f;

            float k = SPRING_CONSTANT;
            float l = INIT_SPRING;
            float b = DAMPING;
            float spring_elongation;

            for(std::vector<Knot *>::iterator it = adjNeighbors.begin(); it != adjNeighbors.end(); ++it) {

                delta_v = this->velocity - (*it)->getVelocity();
                delta_p = this->position - (*it)->getPosition();
                delta_p_hat = glm::normalize(delta_p);

                spring_elongation = glm::length(delta_p) - l;

                f = (-k * spring_elongation - b * glm::dot(delta_v, delta_p_hat)) * delta_p_hat;

                (*it)->addForce(-f);
                this->force += f;
            }

            l = DIAG_SPRING;

            for(std::vector<Knot *>::iterator it = diagNeighbors.begin(); it != diagNeighbors.end(); ++it) {
                //neighborStretch += SPRING_CONSTANT * (glm::length(position - (*it)->getPosition()) - DIAG_SPRING);
                delta_v = this->velocity - (*it)->getVelocity();
                delta_p = this->position - (*it)->getPosition();
                delta_p_hat = glm::normalize(delta_p);

                spring_elongation = glm::length(delta_p) - l;

                f = (-k * spring_elongation - b * glm::dot(delta_v, delta_p_hat)) * delta_p_hat;

                (*it)->addForce(-f);
                this->force += f;
            }

           // std::cout << std::endl << std::endl;
        //}
    }


    void integrateForce(const float dt) {

        this->velocity += (this->force / (float)_MASS) * dt;
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
    glm::vec3 getVelocity() { return this->velocity; }
    glm::vec3 getForce() { return this->force; }
    int getMass() { return _MASS; }    // MAKE THIS MORE PHYSICALY ACCURATE BY USING POLYGON AREA DIVIDED BY 3
    std::vector<Knot *> getAdjNeighbors() { return this->adjNeighbors;  }
    std::vector<Knot *> getDiagNeighbors() { return this->diagNeighbors;  }
    std::vector<Knot *> getFlexNeighbors() { return this->flexNeighbors;  }

    // Setters
    void setStatic() { _isStatic = true; };
    void setForce(glm::vec3 f) { this->force = f; };
    void addForce(glm::vec3 f) { this->force += f; };

    void debugKnotPosition() {
        std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    }

private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    bool _isStatic;
    std::vector<Knot *> adjNeighbors;
    std::vector<Knot *> diagNeighbors;
    std::vector<Knot *> flexNeighbors;
};

#endif // KNOT_H