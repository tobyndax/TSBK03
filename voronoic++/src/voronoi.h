#ifndef VORONOI_H
#define VORONOI_H


#include "GL_utilities.h"
#include "glm.hpp"


struct Fragment* mainVoronoi(int numPoints);
void testFragments(int k);

struct Fragment{
  GLint numFragments;
  GLint numIndices;
  GLint numVertices;
  GLfloat *vertices;
  GLfloat *pointsOnHull;
  GLfloat *texCoord;
  GLuint *indicies;
  GLfloat *normals;
  glm::vec3 center;
  GLfloat radius;
  GLint numOnHull;
};

extern struct Fragment* fragments;
#endif
