#include <string>
//#include "sgct.h"
#include "scene.h"
#include "mesh.h"
#include "floor.h"

void init();
void draw();
void preSync();
void encode();
void decode();
void cleanUp();
void mouseButtonCallback(int button, int action);

// Declare an engine object for sgct
sgct::Engine * gEngine;
// Declare a scene for our simulation
Scene * scene;

// Bool to check if the left mouse button is pressed
bool mouseLeftButton = false;

double mouseDx = 0.0;

double mouseXPos[] = { 0.0, 0.0 };

glm::vec3 view(0.0f, 0.0f, 1.0f);

sgct::SharedObject<glm::mat4> cameraRot;

float rotationSpeed = 0.1f;

// If time is required
sgct::SharedDouble curr_time(0.0);


int main(int argc, char* argv[]) {

    gEngine = new sgct::Engine(argc, argv);
    scene = new Scene();
    std::cout << "lightpos: (" << scene->getLightPosition().x << ", " << scene->getLightPosition().y << ", " << scene->getLightPosition().z << ")" << std::endl;
    gEngine->setInitOGLFunction(init);
    gEngine->setDrawFunction(draw);
    gEngine->setPreSyncFunction(preSync);
    gEngine->setCleanUpFunction(cleanUp);
    gEngine->setMouseButtonCallbackFunction( mouseButtonCallback );
    sgct::SharedData::instance()->setEncodeFunction(encode);
    sgct::SharedData::instance()->setDecodeFunction(decode);

    if(!gEngine->init(sgct::Engine::OpenGL_3_3_Core_Profile)) {
        
        delete gEngine;
        return EXIT_FAILURE;
    }

    gEngine->render();

    delete gEngine;

    exit(EXIT_SUCCESS);
}


void init() {

    // Create our cloth mesh
    Body * cloth = new Body(new Mesh(5, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f)));
    scene->addBody(cloth);

    // Create a floor for some orientation help
    Body * floor = new Body(new Floor(glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, "checker"));
    scene->addBody(floor);
    std::cout << "lightpos: (" << scene->getLightPosition().x << ", " << scene->getLightPosition().y << ", " << scene->getLightPosition().z << ")" << std::endl;
    scene->init();
    std::cout << "lightpos: (" << scene->getLightPosition().x << ", " << scene->getLightPosition().y << ", " << scene->getLightPosition().z << ")" << std::endl;
}


void draw() {

    scene->draw(gEngine->getActiveModelViewProjectionMatrix(), gEngine->getActiveModelViewMatrix(), cameraRot.getVal());
}


void preSync() {
    if(gEngine->isMaster()) {
        curr_time.setVal(sgct::Engine::getTime());

        // Camera movement
        if( mouseLeftButton ) {
            double tmpYPos;
            //get the mouse pos from first window
            sgct::Engine::getMousePos( gEngine->getFocusedWindowIndex(), &mouseXPos[0], &tmpYPos );
            mouseDx = mouseXPos[0] - mouseXPos[1];
        }
        else {
            mouseDx = 0.0;
        }

        static float panRot = 0.0f;
        panRot += (static_cast<float>(mouseDx) * rotationSpeed * static_cast<float>(gEngine->getDt()));

        glm::mat4 ViewRotateX = glm::rotate(
            glm::mat4(1.0f),
            panRot,
            glm::vec3(0.0f, 1.0f, 0.0f)); //rotation around the y-axis

        // Some camera interaction matrices, make the camera
        // rotate around the scene
        glm::mat4 result = ViewRotateX;
        result *= glm::translate( glm::mat4(1.0f), sgct::Engine::getUserPtr()->getPos() );
        result *= glm::translate( glm::mat4(1.0f), -sgct::Engine::getUserPtr()->getPos() );
        
        cameraRot.setVal(result);
    }
}


void encode() {
    sgct::SharedData::instance()->writeDouble(&curr_time);
    sgct::SharedData::instance()->writeObj(&cameraRot);
}


void decode() {
    sgct::SharedData::instance()->readDouble(&curr_time);
    sgct::SharedData::instance()->readObj(&cameraRot);
}


void mouseButtonCallback(int button, int action) {

    if(gEngine->isMaster()) {
        switch(button) {
            
        case SGCT_MOUSE_BUTTON_LEFT:
            mouseLeftButton = (action == SGCT_PRESS ? true : false);
            double tmpYPos;
            sgct::Engine::getMousePos( gEngine->getFocusedWindowIndex(), &mouseXPos[1], &tmpYPos );
            break;
        }
    }
}


void cleanUp() {
    delete scene;
}