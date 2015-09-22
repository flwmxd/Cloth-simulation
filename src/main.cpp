#include <string>
#include <math.h>
#include "scene.h"
#include "mesh.h"
#include "floor.h"
#include "sphere.h"

void init();
void draw();
void preSync();
void encode();
void decode();
void cleanUp();
void keyCallback(int key, int action);
void mouseButtonCallback(int button, int action);

// Declare an engine object for sgct
sgct::Engine * gEngine;
// Declare a scene for our simulation
Scene * scene;

// Some bodies for the scene
Body * cloth;
Body * sphere;
Body * floor_;

// Mouse stuff
bool mouseLeftButton = false;
double mouseDx = 0.0;
double mouseXPos[] = { 0.0, 0.0 };

// Toggle draw functions
unsigned int drawType = 0;

// Play or pause animation
bool play_pause = false;

// Wind or not?
bool wind = false;

// How many simualtions per frame
const unsigned int simulations_per_frame = 20;

// Camera rotation
sgct::SharedObject<glm::mat4> cameraRot;

// How fast shall the camera rotation be?
const float rotationSpeed = 0.2f;

// If time is required
sgct::SharedDouble curr_time(0.0);


int main(int argc, char* argv[]) {

    gEngine = new sgct::Engine(argc, argv);
    
    scene = new Scene();

    scene->setAcceleration(glm::vec3(0.0f, -1.0f, 0.0f) * 9.82f);

    gEngine->setInitOGLFunction(init);
    gEngine->setDrawFunction(draw);
    gEngine->setPreSyncFunction(preSync);
    gEngine->setCleanUpFunction(cleanUp);
    gEngine->setKeyboardCallbackFunction(keyCallback);
    gEngine->setMouseButtonCallbackFunction(mouseButtonCallback);
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
    cloth = new Body(new Mesh(33,                           // Number of knots in vertical and horizintal direction
                              0.5f,                         // Spacing between knots points
                              glm::vec3(0.0f, 7.0f, 0.0f),  // Position of mesh center point
                              "kappa",               // Texture
                              "fabric_normal"));            // Normalmap

    // Set some knot points static, we dont want the whole peice to fall
    cloth->getShape()->setBodyStatic(1056);
    cloth->getShape()->setBodyStatic(1064);
    cloth->getShape()->setBodyStatic(1072);
    cloth->getShape()->setBodyStatic(1080);
    cloth->getShape()->setBodyStatic(1088);

    // Add the cloth to our scene
    scene->addBody(cloth);

    // Add a collision sphere, doesn't work correctly
    //sphere = new Body(new Sphere(4.0f, glm::vec3(0.0f, 0.0f, 5.0f)));
    //scene->addBody(sphere);

    // Create a checkered floor for some orientation help
    floor_ = new Body(new Floor(glm::vec3(0.0f, -3.0f, 0.0f), 30.0f, "checker"));
    scene->addBody(floor_);
    
    scene->init();
}


void draw() {
    // Set current time and step size for the simulation
    scene->setTime(static_cast<float>(curr_time.getVal()));
    scene->setDt(gEngine->getDt() / static_cast<float>(simulations_per_frame));
    
    // Step the simulation one time step forward if it is not paused
    if(play_pause) {
        for(unsigned int i = 0; i < simulations_per_frame; i++) {
            scene->step();
        }
    }

    // Draw the scene with the current scene matrices
    scene->draw(gEngine->getActiveModelViewProjectionMatrix(), gEngine->getActiveModelViewMatrix(), cameraRot.getVal(), drawType);
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

        // Some camera interaction matrices, make the camera rotate around the scene
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


void keyCallback(int key, int action)
{
    if( gEngine->isMaster() )
    {
        switch( key )
        {
        // Draw vertices or ploygons?
        case SGCT_KEY_K:
            if(action == SGCT_PRESS)
                drawType = (drawType == 0) ? 1 : 0;
            break;

        // Reset the simulation
        case SGCT_KEY_R:
            if(action == SGCT_PRESS)
                scene->reset();
            break;

        // Controls for collision sphere
        case SGCT_KEY_W:
            sphere->getShape()->setPosition(sphere->getShape()->getPosition() + glm::vec3(0.0f, 0.0f, -0.2f));
            break;

        case SGCT_KEY_S:
            sphere->getShape()->setPosition(sphere->getShape()->getPosition() + glm::vec3(0.0f, 0.0f, 0.2f));
            break;

        case SGCT_KEY_A:
            sphere->getShape()->setPosition(sphere->getShape()->getPosition() + glm::vec3(-0.2f, 0.0f, 0.0f));
            break;

        case SGCT_KEY_D:
            sphere->getShape()->setPosition(sphere->getShape()->getPosition() + glm::vec3(0.2f, 0.0f, 0.0f));
            break;

        case SGCT_KEY_Q:
            sphere->getShape()->setPosition(sphere->getShape()->getPosition() + glm::vec3(0.0f, -0.2f, 0.0f));
            break;

        case SGCT_KEY_E:
            sphere->getShape()->setPosition(sphere->getShape()->getPosition() + glm::vec3(0.0f, 0.2f, 0.0f));
            break;

        // Load setup 1 for the cloth
        case SGCT_KEY_1:
            if(action == SGCT_PRESS)
                cloth->getShape()->setup1();
            break;

        // Load setup 2 for the cloth
        case SGCT_KEY_2:
            if(action == SGCT_PRESS)
                cloth->getShape()->setup2();
            break;

        // Load setup 3 for the cloth
        case SGCT_KEY_3:
            if(action == SGCT_PRESS)
                cloth->getShape()->setup3();
            break;

        // Load setup 4 for the cloth
        case SGCT_KEY_4:
            if(action == SGCT_PRESS)
                cloth->getShape()->setup4();
            break;

        // Play or pause the simulation
        case SGCT_KEY_C:
            if(action == SGCT_PRESS) {
                if(play_pause)
                    play_pause = false;
                else
                    play_pause = true;
            }
            break;

        // Toggle wind force
        case SGCT_KEY_Z:
            if (action == SGCT_PRESS) {
                if(!wind) {
                    cloth->getShape()->setWindForce(glm::vec3(0.0f, 0.0f, (sin(curr_time.getVal()) + 0.0) / 20.0f ));
                    wind = true;
                } else {
                    cloth->getShape()->setWindForce(glm::vec3(0.0f, 0.0f, 0.0f));
                    wind = false;
                }
            }
            break;

        case SGCT_KEY_UP:
            if(action == SGCT_PRESS)
                cloth->getShape()->setBumpyness(0.05f);
            break;

        case SGCT_KEY_DOWN:
            if(action == SGCT_PRESS)
                cloth->getShape()->setBumpyness(-0.05f);
            break;
        }
    }
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
    delete cloth;
}