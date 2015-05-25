#include "sphere.h"

Sphere::Sphere(float r, glm::vec3 p, glm::vec3 v)
    : radius(r), position(p), initial_position(p), velocity(v) {

    _isStatic = false;
    obj_mesh = new sgct_utils::SGCTSphere(r, 20);

}


void Sphere::draw(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM, unsigned int drawType) {

    glm::mat4 _MVP = MVP;

    glm::mat4 scene_mat = glm::translate(glm::mat4(1.0f), position);
    _MVP = MVP * scene_mat;

    sgct::ShaderManager::instance()->bindShaderProgram("sphere");

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &_MVP[0][0]);

    obj_mesh->draw();

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Sphere::init(glm::vec3 lightPos) {

    std::cout << "Initializing Sphere" << std::endl;

    sgct::ShaderManager::instance()->addShaderProgram(
        "sphere", 
        "shaders/sphere.vert",
        "shaders/sphere.frag");

    sgct::ShaderManager::instance()->bindShaderProgram("sphere");

    MVPLoc = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation("MVP");

    std::cout << "shaders/sphere.vert loaded" << std::endl;
    std::cout << "shaders/sphere.frag loaded" << std::endl;

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Sphere::reset() {
    position = initial_position;
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}


void Sphere::resolveCollision(Knot *k) {

    // Do we have a collision between a sphere and a mesh knot?
    if(glm::length(k->getPosition() - position) <= radius) {

        std::cout << "Collision between sphere and knot: " << k->getIndex() << std::endl;

        glm::vec3 intersection_normal = glm::normalize(k->getPosition() - position);
        float dist = radius - glm::length(-k->getPosition() + position);

        glm::vec3 pos = k->getPosition();

        pos += dist * intersection_normal * 1.0f;

        k->setPosition(pos);
    }
}

