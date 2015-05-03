#include "mesh.h"
#include "body.h"

Mesh::Mesh(unsigned int n, float k, glm::vec3 p) 
    : numKnots(n), knotSpacing(k), position(p) {
    
    createKnots();
}

void Mesh::addKnot(Knot * k) {
    knots.push_back(k);
}


void Mesh::createKnots() {

    for(int y = -floor(numKnots / 2); y < floor(numKnots / 2) + 1; y++) {
        for(int x = -floor(numKnots / 2); x < floor(numKnots / 2) + 1; x++) {
            addKnot(new Knot(glm::vec3(static_cast<float>(x) * knotSpacing,
                                       static_cast<float>(y) * knotSpacing, 
                                       position.z)));
        }
    }
}


void Mesh::draw(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM) {
    // TODO
}

void Mesh::init(glm::vec3 lightPos) {
    // TODO
    std::cout << "INIT MESH!!!" << std::endl;
}
