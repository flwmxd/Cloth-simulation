#include "mesh.h"
#include "body.h"

Mesh::Mesh(unsigned int n, float k, glm::vec3 p) 
    : numKnots(n), knotSpacing(k), position(p) {

    size = std::floor(static_cast<float>(n) / 2.0f) * k;
    createKnots();
    createKnotNeighbors();
    createKnotPoints();
    createVertices();
    createColorVector(glm::vec3(1.0f, 0.0f, 0.0f));
}

void Mesh::addKnot(Knot * k) {
    knots.push_back(k);
}


void Mesh::createKnots() {

    for(int y = -floor(numKnots / 2); y < floor(numKnots / 2) + 1; y++) {
        for(int x = -floor(numKnots / 2); x < floor(numKnots / 2) + 1; x++) {
            addKnot(new Knot(size * glm::vec3(static_cast<float>(x) * knotSpacing + position.x,
                                              static_cast<float>(y) * knotSpacing + position.y, 
                                              position.z)));
        }
    }
}


void Mesh::createKnotNeighbors() {

    unsigned int index = 0;

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        
        // Add knot to the right
        if((index + 1)%numKnots != 0 || index == 0)
            (*it)->addNeighbor(*(it + 1));

        // Add knot to the left
        if((index)%numKnots != 0)
            (*it)->addNeighbor(*(it - 1));

        // Add knot above
        if((index < knots.size() - numKnots))
            (*it)->addNeighbor(*(it + numKnots));

        // Add knot bellow
        if((index > numKnots - 1))
            (*it)->addNeighbor(*(it - numKnots));

        // Add knot upper right
        if((index < knots.size() - numKnots - 1) && (index + 1)%numKnots != 0)
            (*it)->addNeighbor(*(it + numKnots + 1));

        // Add knot lower left
        if((index > numKnots) && (index)%numKnots != 0)
            (*it)->addNeighbor(*(it - numKnots - 1));

        // Add knot lower right
        if((index > numKnots - 1) && (index + 1)%numKnots != 0)
            (*it)->addNeighbor(*(it - numKnots + 1));

        // Add knot upper left
        if((index < knots.size() - numKnots) && (index)%numKnots != 0)
            (*it)->addNeighbor(*(it + numKnots - 1));

        index++;
    }
}


void Mesh::createKnotPoints() {

    for(unsigned int i = 0; i < knots.size(); i++) {
        points.push_back(new sgct_utils::SGCTSphere(0.1f, 5));
    }
}


void Mesh::createVertices() {

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end() - 6; ++it) {
        // Face 1
        mVertices.push_back((*it)->getPosition());
        mVertices.push_back((*(it + 6))->getPosition());
        mVertices.push_back((*(it + 5))->getPosition());
        // Face 2
        mVertices.push_back((*it)->getPosition());
        mVertices.push_back((*(it + 1))->getPosition());
        mVertices.push_back((*(it + 6))->getPosition());
    }
}


void Mesh::createColorVector(glm::vec3 color) {
    for(int i = 0; i < mVertices.size(); i++)
        mColors.push_back(color);
}


void Mesh::draw(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM, unsigned int drawType) {
    if(drawType == DRAW_POINTS)
        drawKnots(MVP, MV, MV_light, NM);
    else
        drawSurface(MVP, MV, MV_light, NM);
}


void Mesh::drawSurface(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM) {
    // TODO
    sgct::ShaderManager::instance()->bindShaderProgram("mesh");

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &MVP[0][0]);

    glBindVertexArray(vertexArray);

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    // Unbind vertex array
    glBindVertexArray(0);

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Mesh::drawKnots(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM) {
    unsigned int indx = 0;

    glm::mat4 tmpMVP;

    for(std::vector<sgct_utils::SGCTSphere *>::iterator it = points.begin(); it != points.end(); ++it) {
        
        glm::mat4 scene_mat = glm::translate(glm::mat4(1.0f), knots[indx]->getPosition());
        tmpMVP = MVP * scene_mat;

        sgct::ShaderManager::instance()->bindShaderProgram("knots");

        glUniformMatrix4fv(MVPLocKnots, 1, GL_FALSE, &tmpMVP[0][0]);

        (*it)->draw();

        sgct::ShaderManager::instance()->unBindShaderProgram();

        indx++;
    }
}


void Mesh::init(glm::vec3 lightPos) {
    initKnotDrawing(lightPos);
    initSurface(lightPos);
}


void Mesh::initKnotDrawing(glm::vec3 lightPos) {

    std::cout << "Initializing knot drawing" << std::endl;

    sgct::ShaderManager::instance()->addShaderProgram(
        "knots", 
        "shaders/knot.vert",
        "shaders/knot.frag");

    sgct::ShaderManager::instance()->bindShaderProgram("knots");

    MVPLocKnots = sgct::ShaderManager::instance()->getShaderProgram("knots").getUniformLocation("MVP");

    std::cout << "shaders/knot.vert loaded" << std::endl;
    std::cout << "shaders/knot.frag loaded" << std::endl;

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Mesh::initSurface(glm::vec3 lightPos) {
    // TODO
    std::cout << "Initializing mesh" << std::endl;

    sgct::ShaderManager::instance()->addShaderProgram(
        "mesh",
        "shaders/mesh_bare.vert",
        "shaders/mesh_bare.frag");

    sgct::ShaderManager::instance()->bindShaderProgram("mesh");

    MVPLoc = sgct::ShaderManager::instance()->getShaderProgram("mesh").getUniformLocation( "MVP" );

    std::cout << "shaders/mesh_bare.vert loaded" << std::endl;
    std::cout << "shaders/mesh_bare.frag loaded" << std::endl;

    sgct::ShaderManager::instance()->unBindShaderProgram();


    // Generate the VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Generate VBO for vertex positions
    glGenBuffers(1, &vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    
    // Upload data to GPU
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<void*>(0)
    );


    glGenBuffers(1, &vertexColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);

    glBufferData(GL_ARRAY_BUFFER, mColors.size() * sizeof(glm::vec3), &mColors[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<void*>(0)
    );

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void Mesh::integrateVelocity(float dt) {

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        (*it)->integrateVelocity(dt);
    }
}


void Mesh::applyG(const float G, float dt) {

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        (*it)->applyG(G, dt);
    }
}


void Mesh::debugMesh() {
    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        std::cout << "knot position: ";
        (*it)->debugKnotPosition();
        std::cout << std::endl;
    }
}


void Mesh::setBodyStatic(int index) {
    knots[index]->setStatic();
}


void Mesh::debugColor() {

}
