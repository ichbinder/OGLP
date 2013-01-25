/* 
 * File:   paserOBJ.cpp
 * Author: jakob
 * 
 * Created on October 21, 2012, 4:28 PM
 */

#include "paserOBJ.h"
int load_obj(const char* filename, vector<ModelObject>& mashe_VectorList) {
    ifstream objFileStream(filename);
    string                      dataString;    
    std::vector<unsigned int>   vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3>      temp_vertices;
    std::vector<glm::vec2>      temp_uvs;
    std::vector<glm::vec3>      temp_normals;
    char*                       texImg_Filename = new char[255];
    char*                       mtl_filename = new char[255];

    //Seeking the mtl file in the obj file
    while (!objFileStream.eof()) {
        getline(objFileStream, dataString);
        if (dataString.substr(0, 7) == "mtllib ") {
            istringstream mtllib(dataString.substr(7));
            string tempFile; mtllib >> tempFile;
            strcpy(mtl_filename, tempFile.c_str());
            break;
        }
    }

    //reset the stringstream
    objFileStream.clear();
    objFileStream.seekg(0, ios::beg);

    //pass the obj file
    bool whileOut = false;
    while (whileOut || !objFileStream.eof()) {
    	if(whileOut)
    		break;
    	//looking for a mesh object in obj file
        getline(objFileStream, dataString);
        if (dataString.substr(0, 2) == "o ") {
            while (!objFileStream.eof()) {
            	cout << "Test2" << endl;
                getline(objFileStream, dataString);
                if (dataString.substr(0, 2) == "o ") {
                    break;
                }
                else if (objFileStream.eof()) {
                	cout << "Test1" << endl;
                    whileOut = 1;
                    break;
                }

                //looking for a vertex
                else if (dataString.substr(0, 2) == "v ") {
                    istringstream s(dataString.substr(2));
                    vec3 v; s >> v.x; s >> v.y; s >> v.z;
                    temp_vertices.push_back(v);
                }

                //looking for a texture
                else if (dataString.substr(0, 3) == "vt ") {
                    istringstream s(dataString.substr(2));
                    vec2 vt; s >> vt.x; s >> vt.y;
                    temp_uvs.push_back(vt);
                }

                //looking for a normal
                else if (dataString.substr(0, 3) == "vn ") {
                    istringstream r(dataString.substr(3));
                    vec3 vn; r >> vn.x; r >> vn.y; r >> vn.z;
                    temp_normals.push_back(vn);
                }

                //looking for a texture in mtl file (jpg, png, ...)
                else if (dataString.substr(0, 7) == "usemtl ") {
                    istringstream usemtl(dataString.substr(7));
                    string mtlName; usemtl >> mtlName;
                    load_mtl(mtl_filename, mtlName, texImg_Filename);
                }

                //looking for Elements
                else if (dataString.substr(0, 2) == "f ") {
                    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                    sscanf(dataString.substr(2).c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]); 
                    for (int i = 0; i < 3; i++) {
                        vertexIndices.push_back(vertexIndex[i]);
                        uvIndices    .push_back(uvIndex[i]);
                        normalIndices.push_back(normalIndex[i]);
                    }
                }    
            }

            //redesign Elements
            std::vector<glm::vec3>      in_vertices;
            std::vector<glm::vec2>      in_uvmap;
            std::vector<glm::vec3>      in_normals;   
            std::vector<glm::vec3>      out_vertices;
            std::vector<glm::vec2>      out_uvmap;
            std::vector<glm::vec3>      out_normals;
            std::vector<GLushort>       out_elements;   
            // For each vertex of each triangle
            for( unsigned int i=0; i<vertexIndices.size(); i++ ){
                    // Get the indices of its attributes
                    unsigned int vertexIndex = vertexIndices[i];
                    unsigned int uvIndex = uvIndices[i];
                    unsigned int normalIndex = normalIndices[i];
                    // Get the attributes thanks to the index
                    glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
                    glm::vec2 uv = temp_uvs[ uvIndex-1 ];
                    glm::vec3 normal = temp_normals[ normalIndex-1 ];
                    // Put the attributes in buffers
                    in_vertices.push_back(vertex);
                    in_uvmap     .push_back(uv);
                    in_normals .push_back(normal);
            }
            //redesign Elements old to opengl standard
            indexVBO(in_vertices, in_uvmap, in_normals, out_elements, out_vertices, out_uvmap, out_normals);
            //create a new mash object
            ModelObject newObjekt(out_vertices, out_normals, out_uvmap, out_elements, texImg_Filename);
            mashe_VectorList.push_back(newObjekt);
            objFileStream.seekg(-(dataString.length() + 1), ios::cur);
        }
    }
}

//
int load_mtl(const char* mtl_filename, string mtl_Name, char* img_filename) {
    ifstream mtlFileStream(mtl_filename);
    if (mtlFileStream.fail()) {
        cout << "File: " << mtl_filename << " could not load." << endl; 
        exit(-1);
    } else {
    string dataString;
        while (!mtlFileStream.eof()) {
                getline(mtlFileStream, dataString);
            if (dataString.substr(0, 7) == "newmtl ") {
                istringstream newmtl(dataString.substr(7));
                if (newmtl.str() == mtl_Name) {
                    getline(mtlFileStream, dataString);
                    while (!(dataString.substr(0, 7) == "newmtl ") || !mtlFileStream.eof()) {
                        if (dataString.substr(0, 7) == "map_Kd ") {
                            istringstream map_Kd(dataString.substr(7));
                            string tempFile; map_Kd >> tempFile;
                            strcpy(img_filename, tempFile.c_str());
                            return 0;
                        }
                        getline(mtlFileStream, dataString);
                    }
                }
            }
        }
    }
    return -1;
}

bool getSimilarVertexIndex(PackedVertex & packed, std::map<PackedVertex,unsigned short> & VertexToOutIndex, unsigned short & result) {
        std::map<PackedVertex,unsigned short>::iterator it = VertexToOutIndex.find(packed);
        if ( it == VertexToOutIndex.end() ){
                return false;
        }else{
                result = it->second;
                return true;
        }
}

void indexVBO(std::vector<glm::vec3> & in_vertices, std::vector<glm::vec2> & in_uvs, std::vector<glm::vec3> & in_normals, std::vector<GLushort> & out_indices, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals) {
        std::map<PackedVertex,unsigned short> VertexToOutIndex;
        // For each input vertex
        for ( unsigned int i=0; i<in_vertices.size(); i++ ){
                PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};
                // Try to find a similar vertex in out_XXXX
                unsigned short index;
                bool found = getSimilarVertexIndex( packed, VertexToOutIndex, index);
                if ( found ){ // A similar vertex is already in the VBO, use it instead !
                        out_indices.push_back( index );
                }else{ // If not, it needs to be added in the output data.
                        out_vertices.push_back( in_vertices[i]);
                        out_uvs     .push_back( in_uvs[i]);
                        out_normals .push_back( in_normals[i]);
                        unsigned short newindex = (unsigned short)out_vertices.size() - 1;
                        out_indices .push_back( newindex );
                        VertexToOutIndex[ packed ] = newindex;
                }
        }
}
