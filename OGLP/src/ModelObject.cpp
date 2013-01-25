/* 
 * File:   ModelObject.cpp
 * Author: jakob
 * 
 * Created on October 19, 2012, 6:12 PM
 */
#include "ModelObject.h"

ModelObject::ModelObject(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCorUV, std::vector<GLushort> elements, char* texImg_filePath) {
    this->vertices =            vertices;
    this->normals =             normals;
    this->texCorUV =            texCorUV;
    this->elements =            elements;
    this->texImg_filePath =     new char[255]; 
    this->texImg_filePath =     texImg_filePath;
    this->textureID =           loadImageTex(this->texImg_filePath);
}

// Draw Model with texture
void ModelObject::DrawModel() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);               // Select Our Texture
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &(normals[0]));
    glVertexPointer(3, GL_FLOAT, 0, &(vertices[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(texCorUV[0]));
    glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, &elements[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

// Draw Model withOut texture
void ModelObject::DrawModel_withOut_tex() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &(normals[0]));
    glVertexPointer(3, GL_FLOAT, 0, &(vertices[0]));
    glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, &elements[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

// Function load a image, turn it into a texture, and return the texture ID as a GLuint for use
GLuint ModelObject::loadImageTex(const char* theFileName) {
    ILuint      imageID;                        // Create an image ID as a ULuint
    ILboolean   success;                        // Create a flag to keep track of success/failure
    ILenum      error;				// Create a flag to keep track of the IL error state
    ilGenImages(1, &imageID);                   // Generate the image ID
    ilBindImage(imageID); 			// Bind the image
    success = ilLoadImage(theFileName);         // Load the image file
    // If we managed to load the image, then we can start to do things with it...
    if (success) {
            // If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
            ILinfo ImageInfo;
            iluGetImageInfo(&ImageInfo);
            if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
                    iluFlipImage();
            }
            // Convert the image into a suitable format to work with
            // NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
            success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
            // Quit out if we failed the conversion
            if (!success) {
                    error = ilGetError();
                    std::cout << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
                    exit(-1);
            }
            // Generate a new texture
            glGenTextures(1, &textureID);
            // Bind the texture to a name
            glBindTexture(GL_TEXTURE_2D, textureID);
            // Set texture clamping method
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            // Set texture interpolation method to use linear interpolation (no MIPMAPS)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // Specify the texture specification
            glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
                                        0,				// Pyramid level (for mip-mapping) - 0 is the top level
                                        ilGetInteger(IL_IMAGE_BPP),	// Image colour depth
                                        ilGetInteger(IL_IMAGE_WIDTH),	// Image width
                                        ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
                                        0,				// Border width in pixels (can either be 1 or 0)
                                        ilGetInteger(IL_IMAGE_FORMAT),	// Image format (i.e. RGB, RGBA, BGR etc.)
                                        GL_UNSIGNED_BYTE,		// Image data type
                                        ilGetData());			// The actual image data itself
            //glGenerateMipmap(GL_TEXTURE_2D); // Note: This requires OpenGL 3.0 or higher
    } else { // If we failed to open the image file in the first place...
            error = ilGetError();
            std::cout << "Image load failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
            exit(-1);
    }

    ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.
    std::cout << "Texture creation successful." << std::endl;
    return textureID; // Return the GLuint to the texture so you can use it!
}

ModelObject::~ModelObject() {
}

