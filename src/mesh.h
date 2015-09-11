#ifndef MESH_H
#define MESH_H

#define MESH_SHAPE 0

#define DRAW_POINTS 0
#define DRAW_SURFACE 1

#include <iostream>
#include <vector>
#include "sgct.h"
#include "shape.h"
#include "knot.h"

/*
 * Mesh class for the cloth
 *  This class contains/handles all the knots(vertecies) needed to create a mesh.
 */

class Mesh : public Shape {

public:
    // Constructors
    Mesh() { };
    Mesh(unsigned int, float, glm::vec3);
    Mesh(unsigned int, float, glm::vec3, std::string);

    // Member functions
    void addKnot(Knot *, unsigned int);

    void createKnots();
    void createKnotNeighbors();
    void createKnotPoints();
    void createVertices();
    void createColorVector(glm::vec3);
    void createFaceNormals();
    void createVertexNormals();
    glm::vec3 computeVertexNormal(std::vector<unsigned int>);
    void createVertexNormalsList();
    void createUVs();

    // Functions that updates data for OpenGL every frame
    void updateVertices();
    void updateFaceNormals();
    void updateVertexNormals();
    void updateVertexNormalsList();

    // Some initial setups for the mesh
    void setup1();
    void setup2();

    void draw(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&, unsigned int);
    void drawSurface(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&);
    void drawKnots(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&);

    void init(glm::vec3);
    void initSurface(glm::vec3);
    void initKnotDrawing(glm::vec3);

    void reset();

    void applySpringForce(float, float, glm::vec3);
    void integrateVelocity(const glm::vec3, float);
    void applyG(const glm::vec3, float);
    void resolveCollision(Knot *);

    // Getters
    unsigned int getType() { return MESH_SHAPE; };
    glm::vec3 getPosition() { return position; };
    std::vector<Knot *> getKnots() { return this->knots; };
    
    // Setters
    void setBodyStatic(int);
    void setWindForce(glm::vec3);
    void setPosition(glm::vec3 p) { position = p; };
    void setTexture(std::string t) { 
        textureName = t;
        sgct::TextureManager::instance()->loadTexure(
            texHandle, 
            textureName, 
            "./textures/" + textureName + ".png",
            true);
    };

    // Debug functions
    void debugMesh();
    void debugColor();

private:

    std::vector<Knot *> knots;
    unsigned int numKnots;
    float knotSpacing;
    glm::vec3 position;
    float size;
    std::string textureName;

    // Data for OpenGl
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mFaceNormals;
    std::vector<glm::vec3> mUniqueVertexNormals;
    std::vector<glm::vec3> mVertexNormals;
    std::vector<glm::vec2> mUvs;
    std::vector<glm::vec3> mColors;
    std::vector<sgct_utils::SGCTSphere *> points;

    GLuint vertexArray;
    GLuint vertexPositionBuffer;
    GLuint normalCoordBuffer;
    GLuint vertexColorBuffer;
    GLuint texCoordBuffer;

    // Shader data
    GLint MVPLoc;           // MVP matrix
    GLint MVPLocKnots;      // Positions of knots 
    GLint MVLoc;            // MV matrix
    GLint MVLightLoc;       // MV light matrix
    GLint NMLoc;            // Normal matrix
    GLint lightPosLoc;      // Light position
    GLint lightAmbLoc;      // Ambient light
    GLint lightDifLoc;      // Diffuse light
    GLint lightSpeLoc;      // Specular light
    GLint specularityLoc;   // Specular constant

    // Material data
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float specularity;

    size_t texHandle;
};

#endif // MESH_H