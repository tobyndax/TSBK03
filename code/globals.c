#include "globals.h"



Model *m, *m2, *tm, *sphere, *skybox, *bunny,
*tree1,*tree2,*tree3,*tree4,*tree5,*leaves,*leaves2,*cross,*Godzilla,*Hokmuto,*Femuto,
*GodzillaFight;
// Reference to shader program
GLuint program, skyProgram, objectProgram, leafProgram,crossProgram,godzillaProgram;
GLuint tex1, skyTex, waterTex, treeTex, snowTex, dirtTex,leafTex,crossTex;
GLuint GodzillaTex,GlowGodzillaTex,FemutoTex,HokmutoTex;
TextureData ttex; // terrain

vec3 viewPoint;
vec3 camPosition;
vec3 upVector;

GLfloat xSphere =30.0;
GLfloat zSphere =30.0;

GLfloat xGodzilla = 70.0;
GLfloat zGodzilla = 90.0;
GLfloat yGodzilla = -100.0;

int deltax = 0;
int deltay = 0;

mat4 projectionMatrix;

GLfloat square[] = {
                            -0.5,0,0,
                            -0.5,1, 0,
                            0.5,1, 0,
                            0.5,0, 0};
GLfloat squareTexCoord[] = {
                             1, 0,
                             1, 1,
                             0, 1,
                             0, 0}; 



bool firstBillboard = true;
bool hasMoved = false;
bool drawGodzilla = false;
bool glowGodzilla = false;
GLuint squareIndices[] = {0, 1, 2, 0, 2, 3};

Model* squareModel;

FBOstruct *fbo1;

// vertex array object
GLuint billboardProgram,plaintextureshader;

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