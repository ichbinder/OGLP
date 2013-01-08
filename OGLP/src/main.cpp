/* 
 * File:   main.cpp
 * Author: jakob
 *
 * Created on October 4, 2012, 2:42 PM
 */

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <vector>
#include <math.h>
#include "paserOBJ.h"
#include "ModelObject.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

using namespace std;
vector<ModelObject> mashe_VectorList;
float rotateThing = 0;

float cameraTranslateX = 0;
float cameraTranslateY = -3;
float cameraTranslateZ = 0;

float cameraRotateY = 0;
float cameraRotateX = 0;

int mausX = 0;
int mausY = 0;

float fPi180 = 0.0174532925f;

int width = 1024;
int height = 768;

bool keys[256];

GLfloat light_position3[] = {0.0, 0.0, 0.0, 1.0};

void idle(void) {
    if(keys['w']) {
            cameraTranslateZ -= cos(cameraRotateY * fPi180) * 0.06;
            cameraTranslateX -= sin(cameraRotateY * fPi180) * 0.06;
            glutPostRedisplay();
    }
    if(keys['s']) {
            cameraTranslateZ += cos(cameraRotateY * fPi180) * 0.06;
            cameraTranslateX += sin(cameraRotateY * fPi180) * 0.06;
            glutPostRedisplay();
    }
    if(keys['a']) {
            cameraTranslateX -= cos(cameraRotateY * fPi180) * 0.06;
            cameraTranslateZ += sin(cameraRotateY * fPi180) * 0.06;
            glutPostRedisplay();
    }
    if(keys['d']) {
            cameraTranslateX += cos(cameraRotateY * fPi180) * 0.06;
            cameraTranslateZ -= sin(cameraRotateY * fPi180) * 0.06;
            glutPostRedisplay();
    }
    if (rotateThing > 359)
        rotateThing = 0.0f;
    rotateThing += 0.2f;
}

void init(void) {   
    ilInit();
    iluInit();
    ilutInit();
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0}; // wo ist das licht ? letzter wert wie stralt die licht quelle Sonne = 0, Gl�lampe = 1, ... 
    GLfloat light_position2[] = {-1.0, 1.0, 1.0, 0.0};
    GLfloat red[] = {1, 0, 0, 1};
    GLfloat green[] = {0, 1, 0, 1};
    GLfloat white[] = {1, 1, 1, 1};

    glClearColor(0.0, 0.5, 1.0, 0.0); // definirt die bidschirm l�schfabe 
    //glShadeModel (GL_FLAT); // wie sollen die schatierten fl�chen dagestellt werden
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); // matrial optionen die reflektion des lichtes das auf dem matrial 
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // matrial optionen wo soll das licht auflschlagen, wie soll die leicht shininess sein
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // Position des lichtes LIGHT0 mit der Variable light_position... ansich kann man hier die optionen des lichts einstallen
    glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
        
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);

    glEnable(GL_LIGHTING); // beleuchtung einschalten
    glDisable(GL_LIGHT0); // Licht Quelle einschalten -> sonst dunkel 
    glDisable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_DEPTH_TEST); // schaltet deph puffer ein
    glEnable(GL_NORMALIZE); // richtet die normal-Vektoren neu bei gestreckten objekten damit die beleuchtung wieder richtig funktioniert
    glutSetCursor(GLUT_CURSOR_NONE);
}

void zeichneKS() {
    //PusMatrix kopiert die aktuelle Matrix, diese Matrix kann nun verändert werden. mit PopMatrix wird die kopierte Matrix gelöscht. 
    glPushMatrix();
    glScalef(10.0, 0.01, 0.01);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.01, 10.0, 0.01);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glScalef(0.01, 0.01, 10.0);
    glutSolidCube(1.0);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutWarpPointer(mausX, mausY); // setze die Maus in die mitte des Glutfensters 

    glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity();

    glRotated(360 - cameraRotateX, 1.0, 0.0, 0.0);
    glRotated(360 - cameraRotateY, 0.0, 1.0, 0.0);
    glTranslatef(-cameraTranslateX, cameraTranslateY, -cameraTranslateZ);
    
    zeichneKS();
    
    glPushMatrix();
    glLightfv(GL_LIGHT2, GL_POSITION, light_position3);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(10.0, -0.5, 10.0); 
        for (int i = 0; i <= mashe_VectorList.size() - 1; i++) {
            mashe_VectorList[i].DrawModel();
        }
    glPopMatrix();
    
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.4, 0.4);
    glVertex3f(-100.0f, -1.0f, -100.0f);
    glVertex3f(-100.0f, -1.0f, 100.0f);
    glVertex3f(100.0f, -1.0f, 100.0f);
    glVertex3f(100.0f, -1.0f, -100.0f);
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, width / height, 1.0f, 100.0f);
    //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    width = w;
    height = h;
    mausX = width / 2; // setze den maus X Wert in die mitte 
    mausY = height / 2; // setze den maus Y Wert in die mitte
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
            break;
            //Bewegungs steuerung 
        case 'w':
            keys[key] = true;
            break;
        case 's':
            keys[key] = true;
            break;
        case 'a':
            keys[key] = true;
            break;
        case 'd':
            keys[key] = true;
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            keys[key] = false;
            break;
        case 's':
            keys[key] = false;
            break;
        case 'a':
            keys[key] = false;
            break;
        case 'd':
            keys[key] = false;
            break;
    }
}

void mausMove(int x, int y) {
    cameraRotateY += (mausX - x) * 0.002;
    cameraRotateX += (mausY - y) * 0.002;
    glutPostRedisplay();
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(300, 100);
    glutCreateWindow(argv[0]);
    init();
    load_obj("world.obj", mashe_VectorList);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mausMove);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

