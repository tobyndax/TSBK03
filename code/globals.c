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

mat4 projectionMatrix;

GLfloat square[] = {
							-1,-1,0,
							-1,1, 0,
							1,1, 0,
							1,-1, 0};
GLfloat squareTexCoord[] = {
							 0, 0,
							 0, 1,
							 1, 1,
							 1, 0};

bool hasMoved = false;
GLuint squareIndices[] = {0, 1, 2, 0, 2, 3};

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
