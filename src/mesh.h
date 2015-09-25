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
 *  This class contains/handles all the knots(vertecies) needed to create a cloth-like mesh.
 *  A Mesh is a shape
 */

class Mesh : public Shape {

public:
    // Constructors
    Mesh() { };
    Mesh(unsigned int, float, glm::vec3);
    Mesh(unsigned int, float, glm::vec3, std::string, std::string);

    // Destructor
    ~Mesh();

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
    void computeTangentBasis(std::vector<glm::vec3>&,
                             std::vector<glm::vec2>&,
                             std::vector<glm::vec3>&,
                             std::vector<glm::vec3>&,
                             std::vector<glm::vec3>&);

    // Functions that updates data for OpenGL every frame
    void updateVertices();
    void updateFaceNormals();
    void updateVertexNormals();
    void updateVertexNormalsList();

    // Some initial setups for the mesh
    void setup1();
    void setup2();
    void setup3();
    void setup4();
    void setup5();

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
    void setBodyStatic(int index) { knots[index]->setStatic(); };
    void setBodyNonStatic(int index) { knots[index]->setNonStatic(); }
    void setAllBodiesNonStatic();
    void setWindForce(glm::vec3);
    void setPosition(glm::vec3 p) { position = p; };
    void setTexture(unsigned int);
    void setBumpyness(float b) { bumpyness += b; };

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
    std::string normalMapName;

    // Data for OpenGl
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mFaceNormals;
    std::vector<glm::vec3> mUniqueVertexNormals;
    std::vector<glm::vec3> mVertexNormals;
    std::vector<glm::vec2> mUvs;
    std::vector<glm::vec3> mColors;
    std::vector<glm::vec3> mTangents;
    std::vector<glm::vec3> mBitangents;
    std::vector<sgct_utils::SGCTSphere *> points;

    // VAOs and VBOs
    GLuint vertexArray;
    GLuint vertexPositionBuffer;
    GLuint normalCoordBuffer;
    GLuint vertexColorBuffer;
    GLuint texCoordBuffer;
    GLuint tangentBuffer;
    GLuint bitangentBuffer;

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
    GLint bumpynessLoc;     // Bumpyness
    GLuint knotColorLoc;    // Color on knots, for debuging

    // Material data
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float specularity;
    float bumpyness;

    glm::vec4 knotColor;

    size_t texHandle;
    size_t normalMapHandle;
};

#endif // MESH_H