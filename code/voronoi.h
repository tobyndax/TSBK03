#ifndef VORONOI_H
#define VORONOI_H

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "globals.h"

struct Fragment** mainVoronoi();
void testFragments(struct Fragment* fragments[],int k);
GLfloat shatterObj(struct Fragment* fragments[],mat4 viewMatrix,GLfloat timeScale);

struct Fragment{
  int numFragments;
  int numVertices;
  GLfloat (*vertices)[];
  GLfloat (*texCoord)[];
  GLuint (*indicies)[];
};

#endif
