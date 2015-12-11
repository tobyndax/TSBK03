#include "globals.h"



Model *tm, *skybox;

// Reference to shader program
GLuint program, skyProgram, objectProgram;
GLuint tex1, skyTex, waterTex, snowTex, dirtTex;
TextureData ttex; // terrain

vec3 viewPoint;
vec3 camPosition;
vec3 upVector;

int deltax = 0;
int deltay = 0;


struct Fragment* fragments = NULL;

mat4 projectionMatrix;
GLfloat square[] = {
	//fram
	-1,1,0,
	-1,-1,0,
	1,-1,0,

	-1,1,0,
	1,-1,0,
	1,1,0,

	//höger
	1,1,0,
	1,-1,0,
	1,-1,0.05f,

	1,1,0,
	1,-1,0.05f,
	1,1,0.05f,

	//upp
	-1,1,0,
	1,1,0,
	1,1,0.05f,

	-1,1,0,
	1,1,0.05f,
	-1,1,0.05f,

	//vänster
	-1,1,0,
	-1,-1,0,
	-1,-1,0.05f,

	-1,1,0,
	-1,-1,0.05f,
	-1,1,0.05f,

	//ner
	-1,-1,0,
	1,-1,0,
	1,-1,0.05f,

	-1,-1,0,
	1,-1,0.05f,
	-1,-1,0.05f,

	//bak
	-1,1,0.05f,
	-1,-1,0.05f,
	1,-1,0.05f,

	-1,1,0.05f,
	1,-1,0.05f,
	1,1,0.05f,
};

GLfloat squareNormal[] = {
	0,0,1,
	0,0,1,
	0,0,1,
	0,0,1,
	0,0,1,
	0,0,1,

	1,0,0,
	1,0,0,
	1,0,0,
	1,0,0,
	1,0,0,
	1,0,0,

	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,

	-1,0,0,
	-1,0,0,
	-1,0,0,
	-1,0,0,
	-1,0,0,
	-1,0,0,

	0,-1,0,
	0,-1,0,
	0,-1,0,
	0,-1,0,
	0,-1,0,
	0,-1,0,

	0,0,-1,
	0,0,-1,
	0,0,-1,
	0,0,-1,
	0,0,-1,
	0,0,-1
};

GLfloat squareTexCoord[] = {
	0, 0,
	0, 1,
	1, 1,
	1, 0,
	0, 0,
	0, 1,
	1, 1,
	1, 0,
	0, 0,
	0, 1,
	1, 1,
	1, 0,
	0, 0,
	0, 1,
	1, 1,
	1, 0};

	bool hasMoved = false;
	GLuint squareIndices[] = {0, 1, 2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,
		30,31,32,
		33,34,35};

		Model* squareModel;


		// vertex array object
		GLuint plaintextureshader;

		Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
		{0.0f, 1.0f, 0.0f}, // Green light
		{0.0f, 0.0f, 1.0f}, // Blue light
		{1.0f, 1.0f, 1.0f} }; // White light

		GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
		GLint isDirectional[] = {0,0,1,1};

		Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
		{0.0f, 5.0f, 10.0f}, // Green light, positional
		{-1.0f, 0.0f, 0.0f}, // Blue light along X
		{0.0f, 0.0f, -1.0f} }; // White light along Z
