#ifndef TERRAIN_H
#define TERRAIN_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "mac/MicroGlut.h"
#include <ApplicationServices/ApplicationServices.h>
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include <math.h>


Model* GenerateTerrain(TextureData *tex);
GLfloat getHeight(GLfloat x,GLfloat z,TextureData *tex);


#endif
