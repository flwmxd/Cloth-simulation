#include "floor.h"

Floor::Floor(glm::vec3 p, float s, const std::string& t) 
    : position(p), size(s), textureName(t) {

    ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


void Floor::draw(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM) {
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByHandle(texHandle));

    sgct::ShaderManager::instance()->bindShaderProgram( "floor" );

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &MVP[0][0]);

    glBindVertexArray(vertexArray);
    
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, mNumberOfVerts);

    //unbind
    glBindVertexArray(0);

    sgct::ShaderManager::instance()->unBindShaderProgram();    
}

void Floor::init() {

    std::cout << "Initializing floor..." << std::endl;

    // Load texture
    sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
    sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
    sgct::TextureManager::instance()->loadTexure(texHandle, textureName,  "./textures/" + textureName + ".png", true);

    // Setup shaders
    sgct::ShaderManager::instance()->addShaderProgram( "floor",
            "shaders/barebone.vert",
            "shaders/barebone.frag" );

    // Bind shaders
    sgct::ShaderManager::instance()->bindShaderProgram( "floor" );

    MVPLoc = sgct::ShaderManager::instance()->getShaderProgram( "floor").getUniformLocation( "MVP" );
    GLint TexLoc = sgct::ShaderManager::instance()->getShaderProgram( "floor").getUniformLocation( "Tex" );

    std::cout << "shaders/barebone.vert loaded" << std::endl;
    std::cout << "shaders/barebone.frag loaded" << std::endl;

    // Setup uniforms for shaders
    glUniform1i(TexLoc, 0);

    // Unbind the shaders
    sgct::ShaderManager::instance()->unBindShaderProgram();

    // Generate vertecies for floor
    const GLfloat vertecies[] = {
        -1.0f * size, 0.0f, -1.0f * size,
        -1.0f * size, 0.0f,  1.0f * size,
         1.0f * size, 0.0f,  1.0f * size,
        -1.0f * size, 0.0f, -1.0f * size,
         1.0f * size, 0.0f,  1.0f * size,
         1.0f * size, 0.0f, -1.0f * size
    };

    mNumberOfVerts = sizeof(vertecies);

    const GLfloat uvs[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    // Generate VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Generate VBO for vertex positions
    glGenBuffers(1, &vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);

    // Upload vertex data to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    // Upload uv data to GPU
    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
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