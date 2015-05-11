#include "floor.h"

Floor::Floor(glm::vec3 p, float s, const std::string& t, bool is) 
    : position(p), size(s), textureName(t), _isStatic(is) {

    velocity = glm::vec3(0.0f);

    Floor::createVertices();
    Floor::createFaceNormals();
    Floor::createVertexNormals();
    Floor::createUVs();

    // Set some standard material properties
    ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


void Floor::draw(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM, unsigned int drawType) {
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByHandle(texHandle));

    sgct::ShaderManager::instance()->bindShaderProgram( "floor" );

    glUniformMatrix4fv(MVPLoc,      1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(MVLoc,       1, GL_FALSE, &MV[0][0]);
    glUniformMatrix4fv(MVLightLoc,  1, GL_FALSE, &MV_light[0][0]);
    glUniformMatrix3fv(NMLoc,       1, GL_FALSE, &NM[0][0]);

    glBindVertexArray(vertexArray);
    
    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    // Unbind
    glBindVertexArray(0);

    sgct::ShaderManager::instance()->unBindShaderProgram();
}

void Floor::init(glm::vec3 lightPos) {

    std::cout << "Initializing floor..." << std::endl;
    //std::cout << "lightpos: (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ")" << std::endl;

    // Load texture
    sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
    sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
    sgct::TextureManager::instance()->loadTexure(texHandle, textureName,  "./textures/" + textureName + ".png", true);

    // Setup shaders
    sgct::ShaderManager::instance()->addShaderProgram( "floor",
            "shaders/floorphong.vert",
            "shaders/floorphong.frag" );

    // Bind shaders
    sgct::ShaderManager::instance()->bindShaderProgram( "floor" );

    MVPLoc          = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "MVP" );
    GLint TexLoc    = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "Tex" );
    MVLoc           = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "MV" );
    MVLightLoc      = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "MVLight" );
    NMLoc           = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "normalMatrix" );
    lightPosLoc     = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "lightPos" );
    lightAmbLoc     = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "lightAmbient" );
    lightDifLoc     = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "lightDiffuse" );
    lightSpeLoc     = sgct::ShaderManager::instance()->getShaderProgram("floor").getUniformLocation( "lightSpecular" );

    std::cout << "shaders/floorphong.vert loaded" << std::endl;
    std::cout << "shaders/floorphong.frag loaded" << std::endl;

    // Setup uniforms for shaders
    glUniform1i(TexLoc, 0);
    glUniform4f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z, 1.0f);
    glUniform4f(lightAmbLoc, ambient.r, ambient.g, ambient.b, ambient.a);
    glUniform4f(lightDifLoc, diffuse.r, diffuse.g, diffuse.b, diffuse.a);
    glUniform4f(lightSpeLoc, specular.r, specular.g, specular.b, specular.a);

    // Unbind the shaders
    sgct::ShaderManager::instance()->unBindShaderProgram();

    // Generate VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Generate VBO for vertex positions
    glGenBuffers(1, &vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);

    // Upload vertex data to GPU
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );


    glGenBuffers(1, &normalCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalCoordBuffer);
    // Upload vertex data to GPU
    glBufferData(GL_ARRAY_BUFFER, mVertexNormals.size() * sizeof(glm::vec3), &mVertexNormals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    // Upload uv data to GPU
    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, mUvs.size() * sizeof(glm::vec2), &mUvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::cout << "Floor initialized" << std::endl;
}


void Floor::createVertices() {
    mVertices.push_back(glm::vec3(-1.0f * size + position.x, 0.0f + position.y, -1.0f * size + position.z));
    mVertices.push_back(glm::vec3(-1.0f * size + position.x, 0.0f + position.y,  1.0f * size + position.z));
    mVertices.push_back(glm::vec3( 1.0f * size + position.x, 0.0f + position.y,  1.0f * size + position.z));
    mVertices.push_back(glm::vec3(-1.0f * size + position.x, 0.0f + position.y, -1.0f * size + position.z));
    mVertices.push_back(glm::vec3( 1.0f * size + position.x, 0.0f + position.y,  1.0f * size + position.z));
    mVertices.push_back(glm::vec3( 1.0f * size + position.x, 0.0f + position.y, -1.0f * size + position.z));
}

void Floor::createFaceNormals() {

    for(std::vector<glm::vec3>::iterator it = mVertices.begin(); it != mVertices.end(); std::advance(it, 3)){
        glm::vec3 v0 = (*(it + 1)) - (*it);
        glm::vec3 v1 = (*(it + 2)) - (*it);
        mFaceNormals.push_back(glm::normalize(glm::cross(v0, v1)));
    }
}


void Floor::createVertexNormals() {

    unsigned int i = 0;

    for(std::vector<glm::vec3>::iterator it = mFaceNormals.begin(); it != mFaceNormals.end(); ++it) {
        mVertexNormals.push_back(mFaceNormals.at(i));
        mVertexNormals.push_back(mFaceNormals.at(i));
        mVertexNormals.push_back(mFaceNormals.at(i));
        i++;
    }
}


void Floor::createUVs() {
    mUvs.push_back(glm::vec2(0.0f, 0.0f));
    mUvs.push_back(glm::vec2(0.0f, 1.0f));
    mUvs.push_back(glm::vec2(1.0f, 1.0f));
    mUvs.push_back(glm::vec2(0.0f, 0.0f));
    mUvs.push_back(glm::vec2(1.0f, 1.0f));
    mUvs.push_back(glm::vec2(1.0f, 0.0f));
}

void Floor::integrateVelocity(float dt) {

    position += velocity * dt;
}

void Floor::applyG(const float G, float dt) {
    velocity += glm::vec3(0.0f, G, 0.0f) * dt;
}
