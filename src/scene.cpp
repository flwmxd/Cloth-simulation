#include "scene.h"

Scene::Scene() {
    lightPosition = glm::vec3(0.0f, 25.0f, 5.0f);
}

void Scene::addBody(Body * b) {
    bodies.push_back(b);
}


void Scene::init() {

    lightSourceColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    //Set up backface culling
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); //our polygon winding is counter clockwise
    glDepthFunc(GL_LESS);

    //std::cout << "init the scene" << std::endl;
    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        (*it)->getShape()->init(this->lightPosition);

    initLightSource();
}


void Scene::initLightSource() {

    std::cout << "Initializing light source drawing" << std::endl;

    lightSource = new sgct_utils::SGCTSphere(0.2f, 5);

    sgct::ShaderManager::instance()->bindShaderProgram("knots");

    MVPLightLoc   = sgct::ShaderManager::instance()->getShaderProgram("knots").getUniformLocation("MVP");
    lightColorLoc = sgct::ShaderManager::instance()->getShaderProgram("knots").getUniformLocation( "in_color" );

    std::cout << "shaders/knot.vert loaded" << std::endl;
    std::cout << "shaders/knot.frag loaded" << std::endl;

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Scene::draw(glm::mat4 activeMVPMatrix, glm::mat4 activeMVMatrix, glm::mat4 cameraRotation, unsigned int drawType) {

    // Set up backface culling, depth test and some blending if alpha channel is used
    glEnable( GL_CULL_FACE );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // Set background color of the scene, just some boring grey is enough for now
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

    // Clear the color buffer from the grey we used for the background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Translate the scene
    glm::mat4 scene_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, -50.0f));
    scene_mat = glm::rotate( scene_mat, 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    // Create some matrices needed for our shapes draw functions
    glm::mat4 MVP       = activeMVPMatrix * scene_mat * cameraRotation;
    glm::mat4 MV        = activeMVMatrix * scene_mat * cameraRotation;
    glm::mat4 MV_light  = activeMVMatrix;
    glm::mat3 NM        = glm::inverseTranspose(glm::mat3(MV));

    // Draw all the shapes
    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        (*it)->getShape()->draw(MVP, MV, MV_light, NM, drawType);

    drawLightSource(MVP);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}


void Scene::drawLightSource(glm::mat4 MVP) {

    glm::mat4 scene_mat = glm::translate(glm::mat4(1.0f), this->lightPosition);
    glm::mat4 MVP_l = MVP * scene_mat;

    sgct::ShaderManager::instance()->bindShaderProgram("knots");

    glUniformMatrix4fv(MVPLightLoc, 1, GL_FALSE, &MVP_l[0][0]);
    glUniform4f(lightColorLoc, lightSourceColor.r, lightSourceColor.g, lightSourceColor.b, lightSourceColor.a);

    lightSource->draw();

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Scene::reset() {

    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it)
        (*it)->getShape()->reset();
}


void Scene::checkCollisions() {

    std::vector<Knot *>mesh_knots = bodies.front()->getShape()->getKnots();

    for(std::vector<Body *>::iterator body_it = bodies.begin(); body_it != bodies.end(); ++body_it) {
        for(std::vector<Knot *>::iterator knot_it = mesh_knots.begin(); knot_it != mesh_knots.end(); ++knot_it) {
            (*body_it)->getShape()->resolveCollision((*knot_it));
        }
    }
}


void Scene::step() {
    
    checkCollisions();
    //applyG();
    applySpringForce();
    integrateVelocities();
    //enforceMaximumStretch();
}


void Scene::applySpringForce() {

    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
        (*it)->getShape()->applySpringForce(t, dt, acceleration);
    }
}


void Scene::integrateVelocities() {

    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
        (*it)->getShape()->integrateVelocity(acceleration, dt);
    }
}


void Scene::enforceMaximumStretch() {
    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
        (*it)->getShape()->enforceMaximumStretch();
    }
}


void Scene::applyG() {

    for(std::vector<Body *>::iterator it = bodies.begin(); it != bodies.end(); ++it) {
        (*it)->getShape()->applyG(acceleration, dt);
    }
}

