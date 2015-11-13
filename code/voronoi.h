#ifndef VORONOI_H
#define VORONOI_H

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "globals.h"

struct Fragment* mainVoronoi(int numPoints);
void testFragments(int k);
GLfloat shatterObj(mat4 viewMatrix,GLfloat timeScale);


struct Fragment{
  GLint numFragments;
  GLint numVertices;
  GLfloat *vertices;
  GLfloat *texCoord;
  GLuint *indicies;
};

extern struct Fragment* fragments;

#endif
