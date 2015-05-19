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

    // Member functions
    void addKnot(Knot *);

    void createKnots();
    void createKnotNeighbors();
    void createKnotPoints();
    void createVertices();
    void createColorVector(glm::vec3);

    void draw(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&, unsigned int);
    void drawSurface(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&);
    void drawKnots(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&);

    void init(glm::vec3);
    void initSurface(glm::vec3);
    void initKnotDrawing(glm::vec3);

    void applySpringForce(const float, float);
    void integrateVelocity(float);
    void applyG(const float, float);

    // Getters
    unsigned int getType() { return MESH_SHAPE; };
    glm::vec3 getPosition() { return position; };
    
    // Setters
    void setBodyStatic(int);

    // Debug functions
    void debugMesh();
    void debugColor();

private:

    std::vector<Knot *> knots;
    unsigned int numKnots;
    float knotSpacing;
    glm::vec3 position;
    float size;

    // Data for OpenGl
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mFaceNormals;
    std::vector<glm::vec3> mVertexNormals;
    std::vector<glm::vec3> mUvs;
    std::vector<glm::vec3> mColors;
    std::vector<sgct_utils::SGCTSphere *> points;

    GLuint vertexArray;
    GLuint vertexPositionBuffer;
    GLuint vertexColorBuffer;

    GLint MVPLoc;
    GLint MVPLocKnots;

};

#endif // MESH_H