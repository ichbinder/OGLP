/*
 * shadowmap.h
 *
 *  Created on: Jan 8, 2013
 *      Author: jakob
 */

#ifndef SHADOWMAP_H_
#define SHADOWMAP_H_
#include <GL/glu.h>
#include <GL/gl.h>
//#include <GL/glew.h>
//#include <GL/glut.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>
#include "main.h"

using namespace glm;

class Shadowmap
{
    public:
		Shadowmap();
		void init(mat4 lichtMatrix, mat4 sichtDesLichtMatrix, mat4 kameraMatrix,
							mat4 sichtDerKameraMatrix , int breite, int hoehe);
        void erzeugeShadowMap(int size);
        void zeichneKamera();
        void zeichneLicht();
        void ende();

    private:
        mat4 lm;       //Lichtmatrix
        mat4 slm;      //Sicht des LichtsMatrix
        mat4 km;       //Kamera Matrix
        mat4 skm;      //Sicht der Kamera
        GLuint shadowtex;
        int breite;
        int hoehe;

        float* GetRow(int row, mat4x4 matrix);
};



#endif /* SHADOWMAP_H_ */
