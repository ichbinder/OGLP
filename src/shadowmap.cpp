/* 
 * File:   shadowmap.cpp
 * Author: fk
 * 
 * Created on 15. Dezember 2012, 16:20
 */

#include "shadowmap.h"
#include "Gl/glew.h"
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include "vertexArray.h"

using namespace glm;



class SHADOWMAP 
{
    public:
        void init(mat4 *lichtMatrix, mat4 sichtDesLichtMatrix, mat4 kameraMatrix, 
                        mat4 sichtDerKameraMatrix , int breite, int hoehe);
        void erzeugeShadowMap(int size);
        void zeichneKamera();
        void zeichneLicht();
        void ende();
        
    private:
        mat4 *lm;       //Lichtmatrix
        mat4 *slm;      //Sicht des LichtsMatrix
        mat4 *km;       //Kamera Matrix
        mat4 *skm;      //Sicht der Kamera
        GLuint shadowtex;
        int breite;
        int hoehe;
};

void SHADOWMAP::init(mat4 *lichtMatrix, mat4 sichtDesLichtMatrix, mat4 kameraMatrix, 
                        mat4 sichtDerKameraMatrix , int breite, int hoehe)
{
    float schattenumgebung{0.4f, 0.4f, 0.4f, 1.0f};
    
    glGenTextures( 1, &shadowtex);
    glBindTexture( GL_TEXTURE_2D, shadowtex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, schattenumgebung);
    
    lm=lichtMatrix;
    slm=sichtDesLichtMatrix;
    km=kameraMatrix;
    skm=sichtDerKameraMatrix;
    this->breite=breite;
    this->hoehe=hoehe;
}
void SHADOWMAP::erzeugeShadowMap(int size)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(*lm);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(*slm);
    
    glViewport( 0, 0, size, size);
    glDisable(GL_LIGHTING);
    glColorMask(0, 0, 0, 0);
    glEnable(GL_POLYGON_OFFSET_FILL);
    //drawScene();// noch einfügen
    
    glBindTexture(GL_TEXTURE_2D, this->shadowtex);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, size, size, 0);
    glEnable(GL_LIGHTING);
    glColorMask(1, 1, 1, 1);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glViewport(0, 0, *breite, *hoehe);
}
void SHADOWMAP::zeichneKamera()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(*km);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(*skm);
    
    static mat4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.5f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.5f, 0.0f,
                           0.5f, 0.5f, 0.5f, 1.0f);
    
    mat4 textureMatrix = biasMatrix(*lm)*(*slm);
    
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.getRow(0));
    glEnable(GL_TEXTURE_GEN_S);
    
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.getRow(1));
    glEnable(GL_TEXTURE_GEN_T);
    
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(2));
    glEnable(GL_TEXTURE_GEN_R);
    
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(3));
    glEnable(GL_TEXTURE_GEN_Q);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
    
    glEnable(GL_FOG);
    //drawScene(); - Einbauen
    glDisable(GL_FOG);
}

void SHADOWMAP::zeichneLicht()
{
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(*lm);
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(*slm);
   //drawScene(); -einbauen   
}
void SHADOWMAP::ende()
{
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
}
float* mat4::GetRow(int i)
{
    float* temp[4];
    
    for(int j = 0; j < 4; j++)
      temp[j] = &m[i][j];
    
    return temp;
}