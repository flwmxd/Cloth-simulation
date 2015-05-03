#include "scene.h"

void Scene::addBody(Body * b) {
    bodies.push_back(b);
    lightPosition = glm::vec3(-2.0f, 5.0f, 5.0f);
}


void Scene::init() {

    //Set up backface culling
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); //our polygon winding is counter clockwise
    glDepthFunc(GL_LESS);

    //std::cout << "init the scene" << std::endl;
    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        (*it)->getShape()->init();
}


void Scene::draw(glm::mat4 activeMVPMatrix, glm::mat4 activeMVMatrix, glm::mat4 cameraRotation) {

    // Set up backface culling, depth test and some blending if alpha channel is used
    glEnable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Set background color of the scene, just some boring grey is enough
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    // Clear the color buffer from the grey we used for the bg
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Translate the scene
    glm::mat4 scene_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -15.0f));
    scene_mat = glm::rotate( scene_mat, 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 MVP       = activeMVPMatrix * scene_mat * cameraRotation;
    glm::mat4 MV        = activeMVMatrix * scene_mat;
    glm::mat4 MV_light  = activeMVMatrix;
    glm::mat3 NM        = glm::inverseTranspose( glm::mat3( MV ) );

    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        (*it)->getShape()->draw(MVP, MV, MV_light, NM);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}


void Scene::checkCollisions() {
    std::cout << "check scene collisions" << std::endl;
}
