/* 
 * File:   paserOBJ.h
 * Author: jakob
 *
 * Created on October 21, 2012, 4:28 PM
 */

#ifndef PASEROBJ_H
#define	PASEROBJ_H
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "ModelObject.h"

using namespace std;
using namespace glm;

int load_obj(const char* filename, vector<ModelObject>& mashe_VectorList);
int load_mtl(const char* mtl_filename, string mtl_Name, char* img_filename);
struct PackedVertex{
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
        bool operator<(const PackedVertex that) const{
                return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
        };
};
bool getSimilarVertexIndex(
        PackedVertex & packed,
        std::map<PackedVertex,unsigned short> & VertexToOutIndex,
        unsigned short & result
);
void indexVBO(
        std::vector<glm::vec3> & in_vertices,
        std::vector<glm::vec2> & in_uvs,
        std::vector<glm::vec3> & in_normals,

        std::vector<unsigned short> & out_indices,
        std::vector<glm::vec3> & out_vertices,
        std::vector<glm::vec2> & out_uvs,
        std::vector<glm::vec3> & out_normals
);
#endif	/* PASEROBJ_H */

