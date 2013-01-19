#include <glm/glm.hpp>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <>
#include "GLee/GLee.h"	//GL header file, including extensions
#include "Maths/Maths.h"
#include "main.h"
#include "ModelObject.h"
#include "paserOBJ.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

std::vector<ModelObject> mashe_VectorList;

float rotateThing = 0;
float lightRotateY = 0;

float cameraTranslateX = 0;
float cameraTranslateY = -3;
float cameraTranslateZ = 0;

float cameraRotateY = 0;
float cameraRotateX = 0;

int mausX = 0;
int mausY = 0;

float fPi180 = 0.0174532925f;

bool keys[256];

GLfloat rotateRobotfirstY = 0.0;

//Camera & light positions
VECTOR3D cameraPosition(-2.5f, 3.5f, -2.5f);
VECTOR3D lightPosition(3.0f, 8.0f,-3.0f);



//Size of shadow map
const int shadowMapSize = 768;

//Textures
GLuint shadowMapTexture;

//window size
int windowWidth, windowHeight;

//Matrices
MATRIX4X4 lightProjectionMatrix, lightViewMatrix;
MATRIX4X4 cameraProjectionMatrix, cameraViewMatrix;

void idle(void) {
	if (keys['w']) {
		cameraTranslateZ -= cos(cameraRotateY * fPi180) * 0.06;
		cameraTranslateX -= sin(cameraRotateY * fPi180) * 0.06;
		matrices_calc();
		glutPostRedisplay();
	}
	if (keys['s']) {
		cameraTranslateZ += cos(cameraRotateY * fPi180) * 0.06;
		cameraTranslateX += sin(cameraRotateY * fPi180) * 0.06;
		matrices_calc();
		glutPostRedisplay();
	}
	if (keys['a']) {
		cameraTranslateX -= cos(cameraRotateY * fPi180) * 0.06;
		cameraTranslateZ += sin(cameraRotateY * fPi180) * 0.06;
		matrices_calc();
		glutPostRedisplay();
	}
	if (keys['d']) {
		cameraTranslateX += cos(cameraRotateY * fPi180) * 0.06;
		cameraTranslateZ -= sin(cameraRotateY * fPi180) * 0.06;
		matrices_calc();
		glutPostRedisplay();
	}
	if (rotateThing > 359)
		rotateThing = 0.0f;
	rotateThing += 0.2f;
	if(lightRotateY > 359)
		lightRotateY = 0.0f;
	lightRotateY += 0.8f;
	matrices_calc();
	glutPostRedisplay();
}

//Called for initiation
bool Init(void) {
	//Check for necessary extensions
	if (!GLEE_ARB_depth_texture || !GLEE_ARB_shadow) {
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
		return false;
	}

    ilInit();
    iluInit();
    ilutInit();

	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	//We use glScale when drawing the scene
	glEnable(GL_NORMALIZE);

	//Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize,
			shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//Use the color as the ambient and diffuse material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//White specular material color, shininess 16
	glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*) &white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	//Calculate & save matrices
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, (float) windowWidth / windowHeight, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	glLoadIdentity();
//	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
//				0.0f, 0.0f, 0.0f,
//				0.0f, 1.0f, 0.0f);
	glRotated(360 - cameraRotateX, 1.0, 0.0, 0.0);
	glRotated(360 - cameraRotateY, 0.0, 1.0, 0.0);
	glTranslatef(-cameraTranslateX, cameraTranslateY, -cameraTranslateZ);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 2.0f, 50.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(lightPosition.x, lightPosition.y, lightPosition.z,
				0.0f, 0.0f,	0.0f,
				0.0f, 1.0f, 0.0f);
//	glTranslatef(2.0f, 6.0f,-2.0f);
	glRotatef(lightRotateY, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();

	return true;
}

void matrices_calc() {
	//Calculate & save matrices
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, (float) windowWidth / windowHeight, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	glLoadIdentity();
	glRotated(360 - cameraRotateX, 1.0, 0.0, 0.0);
	glRotated(360 - cameraRotateY, 0.0, 1.0, 0.0);
	glTranslatef(-cameraTranslateX, cameraTranslateY, -cameraTranslateZ);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 2.0f, 50.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(lightPosition.x, lightPosition.y, lightPosition.z,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);
	glRotatef(lightRotateY, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();
}

void DrawScene(void)
{
    glPushMatrix();
//        glTranslatef(5.0, -0.5, 5.0);
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
	//Display lists for objects
    float angle = 5;
//    static GLuint spheresList=0, torusList=0, baseList=0, towerList=0;
//
//    	//Create spheres list if necessary
//    	if(!spheresList)
//    	{
//    		spheresList=glGenLists(1);
//    		glNewList(spheresList, GL_COMPILE);
//    		{
//    			glColor3f(0.0f, 1.0f, 0.0f);
//    			glPushMatrix();
//
//    			glTranslatef(0.45f, 1.0f, 0.45f);
//    			glutSolidSphere(0.2, 24, 24);
//
//    			glTranslatef(-0.9f, 0.0f, 0.0f);
//    			glutSolidSphere(0.2, 24, 24);
//
//    			glTranslatef(0.0f, 0.0f,-0.9f);
//    			glutSolidSphere(0.2, 24, 24);
//
//    			glTranslatef(0.9f, 0.0f, 0.0f);
//    			glutSolidSphere(0.2, 24, 24);
//
//    			glPopMatrix();
//    		}
//    		glEndList();
//    	}
//
//    	//Create torus if necessary
//    	if(!torusList)
//    	{
//    		torusList=glGenLists(1);
//    		glNewList(torusList, GL_COMPILE);
//    		{
//    			glColor3f(1.0f, 0.0f, 0.0f);
//    			glPushMatrix();
//
//    			glTranslatef(0.0f, 0.5f, 0.0f);
//    			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
//    			glutSolidTorus(0.2, 0.5, 24, 48);
//
//    			glPopMatrix();
//    		}
//    		glEndList();
//    	}
//
//    	//Create base if necessary
//    	if(!baseList)
//    	{
//    		baseList=glGenLists(1);
//    		glNewList(baseList, GL_COMPILE);
//    		{
//    			glColor3f(0.0f, 0.0f, 1.0f);
//    			glPushMatrix();
//
//    			glScalef(1.0f, 0.05f, 1.0f);
//    			glutSolidCube(3.0f);
//
//    			glPopMatrix();
//
//    		}
//    		glEndList();
//    	}
//
//    	//Draw objects
//    	glCallList(baseList);
//    	glCallList(torusList);
////    	glCallList(towerList);
//
//    	glPushMatrix();
//    	glRotatef(angle, 0.0f, 1.0f, 0.0f);
//    	glCallList(spheresList);
//    	glPopMatrix();

}

//Called to draw scene
void display(void) {
	glutWarpPointer(mausX, mausY); // setze die Maus in die mitte des Glutfensters

	//First pass - from light's point of view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const float*) &lightProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((const float*) &lightViewMatrix);

	//Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(0, 0, 0, 0);

	//Draw the scene
	DrawScene();

	//Read the depth buffer into the shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize,
			shadowMapSize);

	//restore states
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);

	//2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const float*) &cameraProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((const float*) &cameraViewMatrix);

	glViewport(0, 0, windowWidth, windowHeight);

	//Use dim light to represent shadowed areas
	glLightfv(GL_LIGHT1, GL_POSITION, (float*) &VECTOR4D(lightPosition));
	glLightfv(GL_LIGHT1, GL_AMBIENT, (float*) &(white * 0.2f));
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (float*) &(white * 0.2f));
	glLightfv(GL_LIGHT1, GL_SPECULAR, (float*) &black);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	DrawScene();

	//3rd pass
	//Draw with bright light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat*) &white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, (GLfloat*) &white);

	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	//It is postmultiplied by the inverse of the current view matrix when specifying texgen
	static MATRIX4X4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.5f,	0.0f, 0.0f,
								0.0f, 0.0f, 0.5f, 0.0f,
								0.5f, 0.5f, 0.5f, 1.0f);//bias from [-1, 1] to [0, 1]
	MATRIX4X4 textureMatrix = biasMatrix * lightProjectionMatrix * lightViewMatrix;

	//Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, (float*) &textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, (float*) &textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, (float*) &textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, (float*) &textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	//Bind & enable shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glEnable(GL_TEXTURE_2D);

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
			GL_COMPARE_R_TO_TEXTURE);

	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	//Set alpha test to discard false comparisons
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glEnable(GL_ALPHA_TEST);

	DrawScene();

	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	//Restore other states
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);

	//Set matrices for ortho
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//reset matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glFinish();
	glutSwapBuffers();
	glutPostRedisplay();
}

//Called on window resize
void Reshape(int w, int h) {
	//Save new window size
	windowWidth = w, windowHeight = h;

	//Update the camera's projection matrix
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0f, (float) windowWidth / windowHeight, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
	glPopMatrix();

	mausX = windowWidth / 2; // setze den maus X Wert in die mitte
	mausY = windowHeight / 2; // setze den maus Y Wert in die mitte
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
	cameraRotateY += (mausX - x) * 0.2;
	cameraRotateX += (mausY - y) * 0.2;
	matrices_calc();
	glutPostRedisplay();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(300, 100);
	glutCreateWindow(argv[0]);

	Init();
	load_obj("monkey2.obj", mashe_VectorList);
	glutDisplayFunc(display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mausMove);
    glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
