#include "sphere.h"

Sphere::Sphere(float r, glm::vec3 p, glm::vec3 v)
    : radius(r), position(p), initial_position(p), velocity(v) {

    _isStatic = false;
    obj_mesh = new sgct_utils::SGCTSphere(r * 0.96f, 20);

    ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    specularity = 5.0f;
}


void Sphere::draw(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM, unsigned int drawType) {

    glm::mat4 _MVP = MVP;

    glm::mat4 scene_mat = glm::translate(glm::mat4(1.0f), position);
    _MVP = MVP * scene_mat;

    sgct::ShaderManager::instance()->bindShaderProgram("sphere");

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &_MVP[0][0]);
    glUniformMatrix4fv(MVLoc,       1, GL_FALSE, &MV[0][0]);
    glUniformMatrix4fv(MVLightLoc,  1, GL_FALSE, &MV_light[0][0]);
    glUniformMatrix3fv(NMLoc,       1, GL_FALSE, &NM[0][0]);
    glUniform4f(lightAmbLoc, ambient.r, ambient.g, ambient.b, ambient.a);
    glUniform4f(lightDifLoc, diffuse.r, diffuse.g, diffuse.b, diffuse.a);
    glUniform4f(lightSpeLoc, specular.r, specular.g, specular.b, specular.a);
    glUniform1f(specularityLoc, specularity);

    obj_mesh->draw();

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Sphere::init(glm::vec3 lightPos) {

    std::cout << "Initializing Sphere..." << std::endl;

    sgct::ShaderManager::instance()->addShaderProgram(
        "sphere", 
        "shaders/sphere.vert",
        "shaders/sphere.frag");

    sgct::ShaderManager::instance()->bindShaderProgram("sphere");

    MVPLoc = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation("MVP");
    MVLoc               = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation( "MV" );
    MVLightLoc          = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation( "MVLight" );
    NMLoc               = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation( "normalMatrix" );
    lightPosLoc         = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation( "lightPos" );
    lightAmbLoc         = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation( "lightAmbient" );
    lightDifLoc         = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation( "lightDiffuse" );
    lightSpeLoc         = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation( "lightSpecular" );
    specularityLoc      = sgct::ShaderManager::instance()->getShaderProgram("sphere").getUniformLocation( "specularity" );

    // Set uniforms that won't change
    glUniform4f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z, 1.0f);

    std::cout << "shaders/sphere.vert loaded" << std::endl;
    std::cout << "shaders/sphere.frag loaded" << std::endl;

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Sphere::reset() {
    position = initial_position;
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}


void Sphere::resolveCollision(Knot *k) {

    float dist = glm::length(k->getPosition() - position);

    // Do we have a collision between a sphere and a mesh knot?
    if(dist < radius) {

        glm::vec3 intersection_normal = glm::normalize(k->getPosition() - position);
        float penetration = radius - dist;

        glm::vec3 pos = k->getPosition();

        pos += (1.0f * penetration) * intersection_normal;
        k->setPosition(pos);    
    }
}

