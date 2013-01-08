#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include "vertexArray.h"
#include "paserOBJ.h"

using namespace std;



void initVertexArray(vector<glm::vec3> vertise, vector<GLushort> vertexIndices, vector<glm::vec3> normals, vector<glm::vec2> uvmap) {

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &(normals[0]));
    glVertexPointer(3, GL_FLOAT, 0, &(vertise[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(uvmap[0]));
    glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_SHORT, &vertexIndices[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
