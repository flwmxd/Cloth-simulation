#ifndef MESH_H
#define MESH_H

#define MESH_SHAPE 0

#include <iostream>
#include <vector>
#include "shape.h"
#include "knot.h"

/*
 * Mesh class for the cloth
 *  This class contains/handles all the knots(vertecies) needed to create a mesh.
 */

class Mesh : public Shape {

public:
    // Constructors
    Mesh() { };
    Mesh(unsigned int, float, glm::vec3);

    // Member functions
    void addKnot(Knot *);
    void createKnots();
    void draw(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&);
    void init();
    unsigned int getType() { return MESH_SHAPE; };
    glm::vec3 getPosition() { return position; };

private:

    std::vector<Knot *> knots;
    unsigned int numKnots;
    float knotSpacing;
    glm::vec3 position;

};

#endif // MESH_H