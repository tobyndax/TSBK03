#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "mac/MicroGlut.h"
#include <ApplicationServices/ApplicationServices.h>
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#define W 1024
#define H 1024

// vertex array object
extern Model *m, *m2, *tm, *sphere, *skybox, *bunny, *tree1,*tree2,*tree3,*tree4,*tree5,
*leaves,*leaves2,*cross,*Godzilla,*Hokmuto,*Femuto,*GodzillaFight;
// Reference to shader program
extern GLuint program, skyProgram, objectProgram, leafProgram,crossProgram,godzillaProgram;
extern GLuint tex1, skyTex, waterTex, treeTex, snowTex, dirtTex,leafTex,
crossTex,GodzillaTex,GlowGodzillaTex,FemutoTex,HokmutoTex;
extern TextureData ttex; // terrain


extern vec3 viewPoint;
extern vec3 camPosition;
extern vec3 upVector;

extern GLfloat xSphere;
extern GLfloat zSphere;

extern GLfloat xGodzilla;
extern GLfloat zGodzilla;
extern GLfloat yGodzilla;

extern int deltax;
extern int deltay;

extern mat4 projectionMatrix;

extern GLfloat square[];
extern GLfloat squareTexCoord[];

extern bool firstBillboard;
extern bool hasMoved;
extern bool drawGodzilla;
extern bool glowGodzilla;
extern GLuint squareIndices[];

extern Model* squareModel;

extern FBOstruct *fbo1;

// vertex array object
extern GLuint billboardProgram,plaintextureshader;

extern Point3D lightSourcesColorsArr[];

extern GLfloat specularExponent[];
extern GLint isDirectional[];

extern Point3D lightSourcesDirectionsPositions[];
