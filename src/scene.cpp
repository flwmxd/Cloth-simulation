#include "scene.h"

Scene::Scene() {
    lightPosition = glm::vec3(0.0f, 10.0f, -25.0f);
}

void Scene::addBody(Body * b) {
    bodies.push_back(b);
}


void Scene::init() {

    //Set up backface culling
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); //our polygon winding is counter clockwise
    glDepthFunc(GL_LESS);

    //std::cout << "init the scene" << std::endl;
    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        (*it)->getShape()->init(this->lightPosition);
}


void Scene::draw(glm::mat4 activeMVPMatrix, glm::mat4 activeMVMatrix, glm::mat4 cameraRotation, unsigned int drawType) {

    // Set up backface culling, depth test and some blending if alpha channel is used
    glEnable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Set background color of the scene, just some boring grey is enough for now
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    // Clear the color buffer from the grey we used for the background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Translate the scene
    glm::mat4 scene_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, -15.0f));
    scene_mat = glm::rotate( scene_mat, 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    // Create some matrices needed for our shapes draw functions
    glm::mat4 MVP       = activeMVPMatrix * scene_mat * cameraRotation;
    glm::mat4 MV        = activeMVMatrix * scene_mat;
    glm::mat4 MV_light  = activeMVMatrix;
    glm::mat3 NM        = glm::inverseTranspose(glm::mat3(MV));

    // Draw all the shapes
    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        (*it)->getShape()->draw(MVP, MV, MV_light, NM, drawType);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}


void Scene::checkCollisions() {
    //std::cout << "check scene collisions" << std::endl;
}


void Scene::step() {
    checkCollisions();
    applySpringForce();
    applyG();
    integrateVelocities();
}


void Scene::applySpringForce() {

    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
        (*it)->getShape()->applySpringForce(GRAVITY, dt);
    }
}


void Scene::integrateVelocities() {

    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
        // Some euler stuff
        (*it)->getShape()->integrateVelocity(dt);
    }
}

void Scene::applyG() {

    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
        (*it)->getShape()->applyG(GRAVITY, dt);
    }
}

