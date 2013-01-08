#ifndef _VERTEXARRAY_H
#define _VERTEXARRAY_H
#include <glm/glm.hpp>
#include <GL/glut.h>
#include <vector>
using namespace std;

void initVertexArray(vector<glm::vec3> vertise, vector<GLushort> vertexIndices, vector<glm::vec3> normals, vector<glm::vec2> uvmap);
 
void drawVertexArray(void);
 
#endif