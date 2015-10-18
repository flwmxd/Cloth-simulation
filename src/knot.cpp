#include "knot.h"

Knot::Knot(glm::vec3 p, float l, bool is)
    : position(p), springLength(l), initial_position(p), _isStatic(is) {

    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    force = glm::vec3(0.0f, 0.0f, 0.0f);
    wind = glm::vec3(0.0f, 0.0f, 0.0f);
    force_damping = 0.75f;
    mass = 1.0f;
    collision_radius = l / 2.0f;
};


void Knot::reset() {
    position = initial_position;
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    force = glm::vec3(0.0f, 0.0f, 0.0f);
}


void Knot::integrateVelocity(const glm::vec3 G, float dt) {

    // Runge-Kutta 4
    Derivative k1 = evaluate(this, G);
    Derivative k2 = evaluate(this, dt*0.5f, G, k1);
    Derivative k3 = evaluate(this, dt*0.5f, G, k2);
    Derivative k4 = evaluate(this, dt, G, k3);

    glm::vec3 dxdt = 1.0f/6.0f * (k1.dx + 2.0f*(k2.dx + k3.dx) + k4.dx);
    glm::vec3 dvdt = 1.0f/6.0f * (k1.dv + 2.0f*(k2.dv + k3.dv) + k4.dv);

    this->position += dxdt * dt;
    this->velocity += dvdt * dt;
};


void Knot::applyG(const glm::vec3 G, float dt) {
    if(!_isStatic)
        velocity += G * dt;
}


void Knot::applySpringForce(float t, glm::vec3 a) {

    // No need to calculate forces on the static knots
    if(!_isStatic) {

        glm::vec3 delta_p;
        glm::vec3 delta_p_hat;
        glm::vec3 delta_v;
        glm::vec3 f;

        float k = 7000.0f;
        float b = 80.0f;
        float l = springLength;
        float l_diag = sqrt(l*l + l*l);
        float l_double = l * 2.0f;
        float spring_elongation;

        for(std::vector<Knot *>::iterator it = adjNeighbors.begin(); it != adjNeighbors.end(); ++it) {

            delta_v = this->velocity - (*it)->getVelocity();
            delta_p = this->position - (*it)->getPosition();
            delta_p_hat = glm::normalize(delta_p);

            spring_elongation = glm::length(delta_p) - l;

            f = (-k * spring_elongation - b * glm::dot(delta_v, delta_p)) * delta_p_hat;
            (*it)->addForce(-f + this->wind);
            this->force += f + this->wind;
        }

        k = 7000.0f;
        b = 80.0f;
        for(std::vector<Knot *>::iterator it = diagNeighbors.begin(); it != diagNeighbors.end(); ++it) {
            
            delta_v = this->velocity - (*it)->getVelocity();
            delta_p = this->position - (*it)->getPosition();
            delta_p_hat = glm::normalize(delta_p);

            spring_elongation = glm::length(delta_p) - l_diag;

            f = (-k * spring_elongation - b * glm::dot(delta_v, delta_p)) * delta_p_hat;

            (*it)->addForce(-f);
            this->force += f + this->wind;
        }

        k = 200.0f;
        b = 30.0f;
        for(std::vector<Knot *>::iterator it = flexNeighbors.begin(); it != flexNeighbors.end(); ++it) {
            
            delta_v = this->velocity - (*it)->getVelocity();
            delta_p = this->position - (*it)->getPosition();
            delta_p_hat = glm::normalize(delta_p);

            spring_elongation = glm::length(delta_p) - l_double;

            f = (-k * spring_elongation - b * glm::dot(delta_v, delta_p)) * delta_p_hat;

            (*it)->addForce(-f);
            this->force += f + this->wind;
        }
    }
}


void Knot::enforceMaximumStretch() {

}


bool Knot::isNeighbor(Knot *k) {

    for(std::vector<Knot *>::iterator it = adjNeighbors.begin(); it != adjNeighbors.end(); ++it) {
        if(k->getIndex() == (*it)->getIndex())
            return true;
    }
    
    for(std::vector<Knot *>::iterator it = diagNeighbors.begin(); it != diagNeighbors.end(); ++it) {
        if(k->getIndex() == (*it)->getIndex())
            return true;
    }

    return false;
}


Derivative evaluate(Knot *knot, float dt, glm::vec3 a, const Derivative &d) {

    State state;
    state.x = knot->getPosition() + d.dx * dt;
    state.v = knot->getVelocity() + d.dv * dt;

    Derivative output;
    output.dx = state.v;
    output.dv = a;

    return output;
}


Derivative evaluate(Knot *knot, glm::vec3 a) {

    Derivative output;
    output.dx = knot->getVelocity();
    output.dv = a;

    return output;
}

