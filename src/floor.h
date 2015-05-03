#ifndef FLOOR_H
#define FLOOR_H

#define FLOOR_SHAPE 1

#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include "shape.h"
#include "sgct.h"


class Floor : public Shape {

public:
    // Constructors
    Floor(glm::vec3, float, const std::string&);

    // Member functions
    void draw(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat3&);
    void init();
    unsigned int getType() { return FLOOR_SHAPE; };

private:
    glm::vec3 position;
    float size;

    std::string textureName;
    GLuint vertexArray;
    GLuint vertexPositionBuffer;
    GLuint texCoordBuffer;

    // Shader data
    GLint MVPLoc;
    GLint MVP_Loc;
    GLint MV_Loc;
    GLint MVLight_Loc;
    GLint NM_Loc;
    GLint lightPos_Loc;
    GLint lightAmb_Loc;
    GLint lightDif_Loc;
    GLint lightSpe_Loc;

    // Material data
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    size_t texHandle;

    unsigned int mNumberOfVerts;

};

#endif // FLOOR_H