/* 
 * File:   ModelObject.h
 * Author: jakob
 *
 * Created on October 19, 2012, 6:12 PM
 */
#ifndef MODELOBJECT_H
#define	MODELOBJECT_H
#include <vector>
#include <iostream>
#include <cstdlib>
#include <glm/glm.hpp>
#include <GL/glu.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
class ModelObject {
public:
    ModelObject(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCorUV, std::vector<GLushort> elements, char* texImg_filePath);
    virtual ~ModelObject();
    void DrawModel();
private:
    std::vector<glm::vec3>      vertices;
    std::vector<glm::vec3>      normals;
    std::vector<glm::vec2>      texCorUV;
    std::vector<GLushort>       elements;
    char*                       texImg_filePath;
    GLuint                      textureID;                                      // Create a texture ID as a GLuint
    GLuint loadImageTex(const char* theFileName);
};
#endif	/* MODELOBJECT_H */

