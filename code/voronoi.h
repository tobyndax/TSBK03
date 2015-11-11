#ifndef VORONOI_H
#define VORONOI_H
#include "GL_utilities.h"

struct Fragment** mainVoronoi();
void testFragments(struct Fragment* fragments[],int k);

struct Fragment{
  int numVertices;
  GLfloat (*vertices)[];
  GLfloat (*texCoord)[];
  GLuint (*indicies)[];
};

#endif
