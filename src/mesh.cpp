#include "mesh.h"
#include "body.h"

Mesh::Mesh(unsigned int n, float k, glm::vec3 p) 
    : numKnots(n), knotSpacing(k), position(p) {

    size = std::floor(static_cast<float>(n) / 2.0f) * k;
    createKnots();
    createKnotNeighbors();
    createKnotPoints();
    createVertices();
    createFaceNormals();
    createVertexNormals();
    createColorVector(glm::vec3(1.0f, 0.0f, 0.0f));
    createUVs();
}


Mesh::Mesh(unsigned int n, float k, glm::vec3 p, std::string t, std::string nM) 
    : numKnots(n), knotSpacing(k), position(p), textureName(t), normalMapName(nM) {

    size = std::floor(static_cast<float>(n) / 2.0f) * k;
    createKnots();
    createKnotNeighbors();
    createKnotPoints();
    createVertices();
    createFaceNormals();
    createVertexNormals();
    createColorVector(glm::vec3(1.0f, 0.0f, 0.0f));
    createUVs();
    computeTangentBasis(mVertices, mUvs, mVertexNormals, mTangents, mBitangents);

    // Material properties
    ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    specularity = 2.0f;
    bumpyness = 0.2;
}


Mesh::~Mesh() {

    for(unsigned int i = 0; i < knots.size(); i++) {
        delete knots[i];
    }
    knots.clear();
}


void Mesh::addKnot(Knot * k, unsigned int i) {
    knots.push_back(k);

    k->setIndex(i);
}


void Mesh::createKnots() {

    unsigned int index = 0;

    for(int y = -floor(numKnots / 2); y < floor(numKnots / 2) + 1; y++) {
        for(int x = -floor(numKnots / 2); x < floor(numKnots / 2) + 1; x++) {
            addKnot(new Knot(glm::vec3(static_cast<float>(x) * knotSpacing + position.x,
                                              static_cast<float>(y) * knotSpacing + position.y, 
                                              position.z), knotSpacing),
                                              index);
            index++;
        }
    }
}


void Mesh::createKnotNeighbors() {

    unsigned int index = 0;

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        
        // Add knot to the right
        if((index + 1)%numKnots != 0 || index == 0)
            (*it)->addAdjNeighbor(*(it + 1));

        // Add knot to the left
        if((index)%numKnots != 0)
            (*it)->addAdjNeighbor(*(it - 1));

        // Add knot above
        if((index < knots.size() - numKnots))
            (*it)->addAdjNeighbor(*(it + numKnots));

        // Add knot bellow
        if((index > numKnots - 1))
            (*it)->addAdjNeighbor(*(it - numKnots));

        // Add knot upper right
        if((index < knots.size() - numKnots - 1) && (index + 1)%numKnots != 0)
            (*it)->addDiagNeighbor(*(it + numKnots + 1));

        // Add knot lower left
        if((index > numKnots) && (index)%numKnots != 0)
            (*it)->addDiagNeighbor(*(it - numKnots - 1));

        // Add knot lower right
        if((index > numKnots - 1) && (index + 1)%numKnots != 0)
            (*it)->addDiagNeighbor(*(it - numKnots + 1));

        // Add knot upper left
        if((index < knots.size() - numKnots) && (index)%numKnots != 0)
            (*it)->addDiagNeighbor(*(it + numKnots - 1));

        // Add knot 2 steps right
        if(((index + 2)%numKnots != 0 &&(index + 1)%numKnots != 0) || index == 0)
            (*it)->addFlexNeighbor(*(it + 2));

        // Add knot 2 steps left
        if((index - 1)%numKnots != 0 && index%numKnots != 0)
            (*it)->addFlexNeighbor(*(it - 2));

        // Add knot 2 steps above
        if((index < knots.size() - numKnots*2))
            (*it)->addFlexNeighbor(*(it + numKnots*2));

        // Add knot 2 steps bellow
        if((index > numKnots*2 - 1))
            (*it)->addFlexNeighbor(*(it - numKnots*2));

        index++;
    }
}


void Mesh::createKnotPoints() {

    for(unsigned int i = 0; i < knots.size(); i++) {
        points.push_back(new sgct_utils::SGCTSphere(0.1f, 5));
    }
}


void Mesh::createVertices() {

    unsigned int index = 0;

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end() - (numKnots + 1); ++it) {
        
        // Check if we're on the border
        if((index+1)%numKnots != 0 || index == 0) {

            // Face 1
            mVertices.push_back((*it)->getPosition());
            mVertices.push_back((*(it + numKnots + 1))->getPosition());
            mVertices.push_back((*(it + numKnots))->getPosition());
            // Face 2
            mVertices.push_back((*it)->getPosition());
            mVertices.push_back((*(it + 1))->getPosition());
            mVertices.push_back((*(it + numKnots + 1))->getPosition());
        }

        index++;
    }
}


void Mesh::createColorVector(glm::vec3 color) {

    for(int i = 0; i < mVertices.size(); i++)
        mColors.push_back(color);
}


void Mesh::createFaceNormals() {

    // TODO
    for(std::vector<glm::vec3>::iterator it = mVertices.begin(); it != mVertices.end(); std::advance(it, 3)) {
        
        glm::vec3 v0 = (*(it + 1)) - (*it);
        glm::vec3 v1 = (*(it + 2)) - (*it);

        mFaceNormals.push_back(glm::normalize(glm::cross(v0, v1)));
    }
}

void Mesh::createVertexNormals() {
   
    std::vector<unsigned int> faceNormalIndices;
    unsigned int row = 0;

    for(unsigned int i = 0; i < knots.size(); i++) {
        if(i < numKnots) {
            if(i == 0) {
                faceNormalIndices.push_back(i + 1);
            } else if(i == numKnots - 1) {
                faceNormalIndices.push_back(((i - row) * 2) - 1);
            } else {
                faceNormalIndices.push_back(((i - row) * 2) - 1);
                faceNormalIndices.push_back(((i - row) * 2));
                faceNormalIndices.push_back(((i - row) * 2) + 1);
            }
        } else if(i > (numKnots*numKnots) - (numKnots + 1)) {
            if(i == numKnots*numKnots - 1) {
                faceNormalIndices.push_back((i - (row + 1)) * 2 - (numKnots) * 2);
                faceNormalIndices.push_back((i - (row + 1)) * 2 - (numKnots) * 2 + 1);
            } else if(i == (numKnots*numKnots) - numKnots) {
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2);
            } else {
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2 - 1);
            }
        } else {
            if(i%numKnots == 0) {
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2 + 1);
            } else if((i+1)%numKnots == 0) {
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots) * 2 + 1);
                faceNormalIndices.push_back(((i - row - 1) * 2) - 1);
            } else {
                faceNormalIndices.push_back((i - row - 1) * 2);
                faceNormalIndices.push_back(((i - row - 1) * 2) - 1);
                faceNormalIndices.push_back(((i - row - 1) * 2) + 1);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2 - 2);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2 - 1);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2);
            }
        }

        if((i+1)%(numKnots) == 0 && i > numKnots) {
            row++;
        }

        mUniqueVertexNormals.push_back(computeVertexNormal(faceNormalIndices));
        faceNormalIndices.clear();
    }

    createVertexNormalsList();
}


glm::vec3 Mesh::computeVertexNormal(std::vector<unsigned int> indices) {

    glm::vec3 vertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

    for(std::vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); ++it) {
        vertexNormal += mFaceNormals[*it];
    }
    vertexNormal = glm::normalize(vertexNormal);

    return vertexNormal;
}


void Mesh::createVertexNormalsList() {

    for(int i = 0; i < knots.size(); i++) {
        
        // Check if we're on the border
        if((i+1)%numKnots != 0 || i == 0) {

            // Face 1
            mVertexNormals.push_back(mUniqueVertexNormals[i]);
            mVertexNormals.push_back(mUniqueVertexNormals[i + numKnots + 1]);
            mVertexNormals.push_back(mUniqueVertexNormals[i + numKnots]);
            // Face 2
            mVertexNormals.push_back(mUniqueVertexNormals[i]);
            mVertexNormals.push_back(mUniqueVertexNormals[i + 1]);
            mVertexNormals.push_back(mUniqueVertexNormals[i + numKnots + 1]);
        }
    }
}


void Mesh::createUVs() {

    float d_uv = 1.0f / static_cast<float>(numKnots-1);
    unsigned int row = 0, col = 0;

    for(unsigned int i = 0; i < knots.size(); i++) {
        
        if(((i+1)%numKnots != 0 && i < (numKnots*numKnots) - numKnots) || (i == 0 && i < (numKnots*numKnots) - numKnots)) {
            mUvs.push_back(glm::vec2(static_cast<float>(col) * d_uv, static_cast<float>(row) * d_uv));
            mUvs.push_back(glm::vec2(static_cast<float>(col+1) * d_uv, static_cast<float>(row+1) * d_uv));
            mUvs.push_back(glm::vec2(static_cast<float>(col) * d_uv, static_cast<float>(row+1) * d_uv));

            mUvs.push_back(glm::vec2(static_cast<float>(col) * d_uv, static_cast<float>(row) * d_uv));
            mUvs.push_back(glm::vec2(static_cast<float>(col+1) * d_uv, static_cast<float>(row) * d_uv));
            mUvs.push_back(glm::vec2(static_cast<float>(col+1) * d_uv, static_cast<float>(row+1) * d_uv));
        }

        col++;

        if((i+1)%(numKnots) == 0 ) {
            row++;
            col = 0;
        }
    }
}


void Mesh::computeTangentBasis(std::vector<glm::vec3> &vertices,
                               std::vector<glm::vec2> &uvs,
                               std::vector<glm::vec3> &normals,
                               std::vector<glm::vec3> &tangents,
                               std::vector<glm::vec3> &bitangents) {

    for(unsigned int i = 0; i < vertices.size(); i += 3) {

        // Shortcuts for vertices
        glm::vec3 &v0 = vertices[i + 0];
        glm::vec3 &v1 = vertices[i + 1];
        glm::vec3 &v2 = vertices[i + 2];

        // Shortcut for UVs
        glm::vec2 &uv0 = uvs[i + 0];
        glm::vec2 &uv1 = uvs[i + 1];
        glm::vec2 &uv2 = uvs[i + 2];

        // Edges of the triangle
        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
}


void Mesh::updateVertices() {

    unsigned int indx = 0;

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end() - (numKnots + 1); ++it) {
        
        if(((*it)->getIndex() + 1)%numKnots != 0 ||(*it)->getIndex() == 0) {

            mVertices[indx] = (*it)->getPosition();
            mVertices[indx + 1] = (*(it + numKnots + 1))->getPosition();
            mVertices[indx + 2] = (*(it + numKnots))->getPosition();
            
            mVertices[indx + 3] = (*it)->getPosition();
            mVertices[indx + 4] = (*(it + 1))->getPosition();
            mVertices[indx + 5] = (*(it + numKnots + 1))->getPosition();

            indx += 6;
        }
    }
}


void Mesh::updateFaceNormals() {

    unsigned int indx = 0;

    for(std::vector<glm::vec3>::iterator it = mVertices.begin(); it != mVertices.end(); std::advance(it, 3)) {
        
        glm::vec3 v0 = (*(it + 1)) - (*it);
        glm::vec3 v1 = (*(it + 2)) - (*it);

        mFaceNormals[indx] = glm::normalize(glm::cross(v0, v1));

        indx++;
    }
}


void Mesh::updateVertexNormals() {
    
    std::vector<unsigned int> faceNormalIndices;
    unsigned int row = 0;

    for(unsigned int i = 0; i < knots.size(); i++) {
        if(i < numKnots) {
            if(i == 0) {
                faceNormalIndices.push_back(i + 1);
            } else if(i == numKnots - 1) {
                faceNormalIndices.push_back(((i - row) * 2) - 1);
            } else {
                faceNormalIndices.push_back(((i - row) * 2) - 1);
                faceNormalIndices.push_back(((i - row) * 2));
                faceNormalIndices.push_back(((i - row) * 2) + 1);
            }
        } else if(i > (numKnots*numKnots) - (numKnots + 1)) {
            if(i == numKnots*numKnots - 1) {
                faceNormalIndices.push_back((i - (row + 1)) * 2 - (numKnots) * 2);
                faceNormalIndices.push_back((i - (row + 1)) * 2 - (numKnots) * 2 + 1);
            } else if(i == (numKnots*numKnots) - numKnots) {
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2);
            } else {
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2 - 1);
            }
        } else {
            if(i%numKnots == 0) {
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2 + 1);
            } else if((i+1)%numKnots == 0) {
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots) * 2);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots) * 2 + 1);
                faceNormalIndices.push_back(((i - row - 1) * 2) - 1);
            } else {
                faceNormalIndices.push_back((i - row - 1) * 2);
                faceNormalIndices.push_back(((i - row - 1) * 2) - 1);
                faceNormalIndices.push_back(((i - row - 1) * 2) + 1);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2 - 2);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2 - 1);
                faceNormalIndices.push_back((i - row - 1) * 2 - (numKnots - 1) * 2);
            }
        }

        if((i+1)%(numKnots) == 0 && i > numKnots) {
            row++;
        }

        mUniqueVertexNormals[i] = computeVertexNormal(faceNormalIndices);
        faceNormalIndices.clear();
    }
}


void Mesh::updateVertexNormalsList() {

    unsigned int indx = 0;

    for(int i = 0; i < knots.size(); i++) {
        
        // Check if we're on the border
        if((i+1)%numKnots != 0 || i == 0) {

            // Face 1
            mVertexNormals[indx] = mUniqueVertexNormals[i];
            mVertexNormals[indx + 1] = mUniqueVertexNormals[i + numKnots + 1];
            mVertexNormals[indx + 2] = mUniqueVertexNormals[i + numKnots];
            // Face 2
            mVertexNormals[indx + 3] = mUniqueVertexNormals[i];
            mVertexNormals[indx + 4] = mUniqueVertexNormals[i + 1];
            mVertexNormals[indx + 5] = mUniqueVertexNormals[i + numKnots + 1];

            indx += 6;
        }
    }
}


void Mesh::draw(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM, unsigned int drawType) {

    if(drawType == DRAW_POINTS)
        drawKnots(MVP, MV, MV_light, NM);
    else
        drawSurface(MVP, MV, MV_light, NM);
}


/*
 * Draws a polygon surface of the mesh
 */
void Mesh::drawSurface(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM) {

    // Update vertex positions and normals
    updateVertices();
    updateFaceNormals();
    updateVertexNormals();
    updateVertexNormalsList();

    // Disable back face culling, since we want both sides of the cloth to be visible
    glDisable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByHandle(texHandle));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureByHandle(normalMapHandle));

    sgct::ShaderManager::instance()->bindShaderProgram("cloth_plain");

    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(MVLoc,       1, GL_FALSE, &MV[0][0]);
    glUniformMatrix4fv(MVLightLoc,  1, GL_FALSE, &MV_light[0][0]);
    glUniformMatrix3fv(NMLoc,       1, GL_FALSE, &NM[0][0]);
    glUniform4f(lightAmbLoc, ambient.r, ambient.g, ambient.b, ambient.a);
    glUniform4f(lightDifLoc, diffuse.r, diffuse.g, diffuse.b, diffuse.a);
    glUniform4f(lightSpeLoc, specular.r, specular.g, specular.b, specular.a);
    glUniform1f(specularityLoc, specularity);
    glUniform1f(bumpynessLoc, bumpyness);

    // Rebind the buffer data, since our vertices are now updated
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);

    // Rebind the buffer data, normals are now updated
    glBindBuffer(GL_ARRAY_BUFFER, normalCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertexNormals.size() * sizeof(glm::vec3), &mVertexNormals[0], GL_STATIC_DRAW);

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    // Unbind vertex array
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sgct::ShaderManager::instance()->unBindShaderProgram();

    glEnable(GL_CULL_FACE);
}


/*
 * Draws small spheres on the knot positions.
 * This helps a lot when debugging stuff
 */
void Mesh::drawKnots(glm::mat4& MVP, glm::mat4& MV, glm::mat4& MV_light, glm::mat3& NM) {
    
    unsigned int indx = 0;

    glm::mat4 tmpMVP;

    for(std::vector<sgct_utils::SGCTSphere *>::iterator it = points.begin(); it != points.end(); ++it) {
        
        glm::mat4 scene_mat = glm::translate(glm::mat4(1.0f), knots[indx]->getPosition());
        tmpMVP = MVP * scene_mat;

        sgct::ShaderManager::instance()->bindShaderProgram("knots");

        glUniformMatrix4fv(MVPLocKnots, 1, GL_FALSE, &tmpMVP[0][0]);
        glUniform4f(knotColorLoc, knotColor.r, knotColor.g, knotColor.b, knotColor.a);

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

    std::cout << "Initializing knot drawing..." << std::endl;

    knotColor = glm::vec4(0.8, 0.3, 0.3, 1.0);

    sgct::ShaderManager::instance()->addShaderProgram(
        "knots", 
        "shaders/knot.vert",
        "shaders/knot.frag");

    sgct::ShaderManager::instance()->bindShaderProgram("knots");

    MVPLocKnots  = sgct::ShaderManager::instance()->getShaderProgram("knots").getUniformLocation("MVP");
    knotColorLoc = sgct::ShaderManager::instance()->getShaderProgram("knots").getUniformLocation( "in_color" );

    std::cout << "shaders/knot.vert loaded" << std::endl;
    std::cout << "shaders/knot.frag loaded" << std::endl;

    sgct::ShaderManager::instance()->unBindShaderProgram();
}


void Mesh::initSurface(glm::vec3 lightPos) {

    std::cout << "Initializing mesh...";

    // Load textures
    sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
    sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
    sgct::TextureManager::instance()->loadTexure(texHandle, textureName,  "./textures/" + textureName + ".png", true);
    sgct::TextureManager::instance()->loadTexure(normalMapHandle, normalMapName,  "./textures/normalmaps/" + normalMapName + ".png", true);

    sgct::ShaderManager::instance()->addShaderProgram(
        "cloth_plain",
        "shaders/cloth_plain.vert",
        "shaders/cloth_plain.frag");

    sgct::ShaderManager::instance()->bindShaderProgram("cloth_plain");

    MVPLoc              = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "MVP" );
    GLint TexLoc        = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "Tex" );
    GLint NormalMapLoc  = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "NormalMap" );
    MVLoc               = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "MV" );
    MVLightLoc          = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "MVLight" );
    NMLoc               = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "normalMatrix" );
    lightPosLoc         = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "lightPos" );
    lightAmbLoc         = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "lightAmbient" );
    lightDifLoc         = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "lightDiffuse" );
    lightSpeLoc         = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "lightSpecular" );
    specularityLoc      = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "specularity" );
    bumpynessLoc        = sgct::ShaderManager::instance()->getShaderProgram("cloth_plain").getUniformLocation( "bumpyness" );

    std::cout << "shaders/cloth_plain.vert loaded" << std::endl;
    std::cout << "shaders/cloth_plain.frag loaded" << std::endl;

    // Setup uniforms for shaders
    glUniform1i(TexLoc, 0);
    glUniform1i(NormalMapLoc, 1);
    glUniform4f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z, 1.0f);

    // Unbind the shaders
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
        2,                  // attribute 2. No particular reason for 1, but must match the layout in the shader.
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    glGenBuffers(1, &tangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, mTangents.size() * sizeof(glm::vec3), &mTangents[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3,                  // attribute 3. No particular reason for 1, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    glGenBuffers(1, &bitangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, mBitangents.size() * sizeof(glm::vec3), &mBitangents[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4,                  // attribute 3. No particular reason for 1, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::reset() {

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        if(!(*it)->isStatic())
            (*it)->reset();
    }
}


void Mesh::applySpringForce(float t, float dt, glm::vec3 a) {

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        if((*it)->isStatic()) continue;
            (*it)->applySpringForce(t, a);
    }

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        if((*it)->isStatic()) continue;
            (*it)->integrateForce(dt);
    }
}


void Mesh::integrateVelocity(const glm::vec3 G, float dt) {

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        if((*it)->isStatic()) continue;
            (*it)->integrateVelocity(G, dt);
    }
}


void Mesh::applyG(const glm::vec3 G, float dt) {

    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        if((*it)->isStatic()) continue;
            (*it)->applyG(G, dt);
    }
}


void Mesh::resolveCollision(Knot *k) {
    // Add cloth self-collision if time is given
}


void Mesh::debugMesh() {
    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        std::cout << "knot position: ";
        (*it)->debugKnotPosition();
        std::cout << std::endl;
    }
}


void Mesh::setAllBodiesNonStatic() {
    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it)
        (*it)->setNonStatic();
}


void Mesh::setWindForce(glm::vec3 w_f) {
    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {
        (*it)->setWindForce(w_f);
    }
}


void Mesh::setup1() {
    
    std::cout << "Loading setup 1 ...";

    setAllBodiesNonStatic();

    setBodyStatic(1056);
    //setBodyStatic(1064);
    //setBodyStatic(1072);
    //setBodyStatic(1080);
    setBodyStatic(1088);

    // Reset the mesh, i.e. positions, velocities and forces.
    reset();

    unsigned int indx = 0;

    glm::vec3 init_pos = knots.front()->getInitialPosition();
    float x = init_pos.x;
    float y = init_pos.y;
    float z = init_pos.z;

    // Give all knots new positions
    for(std::vector<Knot* >::iterator it = knots.begin(); it != knots.end(); ++it) {
        
        (*it)->setPosition(glm::vec3(x, y, z));
        (*it)->setForceDamping(0.75f);
        (*it)->setMass(1.0f);
        x += knotSpacing;

        if((indx + 1)%numKnots == 0 && indx > 0) {
            y += knotSpacing;
            x = init_pos.x;
        }
        indx++;
    }

    knots[1072]->addForce(glm::vec3(0.0, 0.0, 0.5));

    std::cout << "\tDone!" << std::endl << std::endl;
}


void Mesh::setup2() {

    std::cout << "Loading setup 2 ...";

    setAllBodiesNonStatic();

    setBodyStatic(1056);
    setBodyStatic(1064);
    setBodyStatic(1072);
    setBodyStatic(1080);
    setBodyStatic(1088);

    // Reset the mesh, i.e. positions, velocities and forces.
    reset();

    unsigned int indx = 0;

    glm::vec3 init_pos = knots.back()->getInitialPosition();
    float x = -init_pos.x;
    float y = init_pos.y;
    float z = -((numKnots - 1.0f) * knotSpacing);

    // Give all knots new positions
    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {

        (*it)->setPosition(glm::vec3(x, y, z));
        glm::vec3 init_force = (*it)->getForce();
        //(*it)->setForce(glm::vec3(init_force.x*0.5, init_force.y*0.5, init_force.z*0.5));
        (*it)->setForceDamping(0.75f);
        (*it)->setMass(1.0f);
        x += knotSpacing;

        if((indx + 1)%numKnots == 0 && indx > 0) {
            z += knotSpacing;
            x = -init_pos.x;
        }
        indx++;
    }
    std::cout << "\tDone!" << std::endl << std::endl;
}


void Mesh::setup3() {

    std::cout << "Loading setup 3 ...";

    setAllBodiesNonStatic();

    // Side 1
    setBodyStatic(0);
    setBodyStatic(8);
    setBodyStatic(16);
    setBodyStatic(24);
    setBodyStatic(32);

    // Sida 2
    setBodyStatic(264);
    setBodyStatic(528);
    setBodyStatic(792);

    // Sida 3
    setBodyStatic(1056);
    setBodyStatic(1064);
    setBodyStatic(1072);
    setBodyStatic(1080);
    setBodyStatic(1088);

    // Sida 4
    setBodyStatic(296);
    setBodyStatic(560);
    setBodyStatic(824);

    // Reset the mesh, i.e. positions, velocities and forces.
    reset();

    unsigned int indx = 0;

    glm::vec3 init_pos = knots.back()->getInitialPosition();
    float x = -init_pos.x;
    float y = init_pos.y / 2.0;
    float z = ((numKnots - 1.0f) * knotSpacing) / 2.0;

    // Give all knots new positions
    for(std::vector<Knot *>::iterator it = knots.begin(); it != knots.end(); ++it) {

        (*it)->setPosition(glm::vec3(x, y, z));
        glm::vec3 init_force = (*it)->getForce();
        //(*it)->setForce(glm::vec3(init_force.x*0.5, init_force.y*0.5, init_force.z*0.5));
        (*it)->setForceDamping(0.75f);
        (*it)->setMass(1.0f);
        x += knotSpacing;

        if((indx + 1)%numKnots == 0 && indx > 0) {
            z -= knotSpacing;
            x = -init_pos.x;
        }
        indx++;
    }

    std::cout << "\tDone!" << std::endl << std::endl;
}


void Mesh::setup4() {

    std::cout << "Loading setup 4 ...";

    setAllBodiesNonStatic();

    setBodyStatic(0);
    setBodyStatic(32);
    setBodyStatic(1056);
    setBodyStatic(1088);

    // Reset the mesh, i.e. positions, velocities and forces.
    reset();

    unsigned int indx = 0;

    glm::vec3 init_pos = knots.front()->getInitialPosition();
    float x = init_pos.x;
    float y = init_pos.y;
    float z = init_pos.z;

    // Give all knots new positions
    for(std::vector<Knot* >::iterator it = knots.begin(); it != knots.end(); ++it) {
        
        (*it)->setPosition(glm::vec3(x, y, z));
        (*it)->setForceDamping(0.75f);
        (*it)->setMass(1.0f);
        x += knotSpacing;

        if((indx + 1)%numKnots == 0 && indx > 0) {
            y += knotSpacing;
            x = init_pos.x;
        }
        indx++;
    }

    float stretch = 0.5f;
    // Stretch the mesh along the diagonals
    knots[0]->setPosition( knots[0]->getPosition() + stretch * glm::vec3(-1.0, -1.0, 0.0) );
    knots[32]->setPosition( knots[32]->getPosition() + stretch * glm::vec3(1.0, -1.0, 0.0) );
    knots[1056]->setPosition( knots[1056]->getPosition() + stretch * glm::vec3(-1.0, 1.0, 0.0) );
    knots[1088]->setPosition( knots[1088]->getPosition() + stretch * glm::vec3(1.0, 1.0, 0.0) );

    std::cout << "\tDone!" << std::endl << std::endl;
}


void Mesh::setup5() {
    
    std::cout << "Loading setup 5 ...";

    setAllBodiesNonStatic();

    for(unsigned int i = 0; i < numKnots * numKnots - 1; i+=33)
        setBodyStatic(i);

    // Reset the mesh, i.e. positions, velocities and forces.
    reset();

    unsigned int indx = 0;

    glm::vec3 init_pos = knots.front()->getInitialPosition();
    float x = init_pos.x;
    float y = init_pos.y;
    float z = init_pos.z;

    // Give all knots new positions
    for(std::vector<Knot* >::iterator it = knots.begin(); it != knots.end(); ++it) {
        
        (*it)->setPosition(glm::vec3(x, y, z));
        (*it)->setForceDamping(0.75f);
        (*it)->setMass(1.0f);
        x += knotSpacing;

        if((indx + 1)%numKnots == 0 && indx > 0) {
            y += knotSpacing;
            x = init_pos.x;
        }
        indx++;
    }

    knots[1072]->addForce(glm::vec3(0.0, 0.0, 0.5));

    std::cout << "\tDone!" << std::endl << std::endl;
}


void Mesh::debugColor() {

}
