#ifndef VORONOI_H
#define VORONOI_H

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include "globals.h"

struct Fragment* mainVoronoi(int numPoints);
void testFragments(int k);
GLfloat shatterObj(mat4 viewMatrix,GLfloat timeScale);
void updateWorld(double dT);

struct Fragment{
  GLint numFragments;
  GLint numIndices;
  GLint numVertices;
  GLfloat *vertices;
  GLfloat *pointsOnHull;
  GLfloat *texCoord;
  GLuint *indicies;
  GLfloat *normals;
  vec3 center;
  GLfloat radius;
  GLint numOnHull;
};

struct PhysicsObj
{
	GLuint tex;
	GLfloat mass;

	vec3 Pos, LinMom, AngMom; // position, linear momentum, angular momentum
	mat4 Rot; // Rotation

	vec3 F, T; // accumulated force and torque

	//  mat4 J, Ji; We could have these but we can live without them for spheres.
	vec3 omega; // Angular velocity
	vec3 v; // Change in velocity
	mat3 I;  // Inertia
  mat3 InvI; //Inverse inertia

  GLfloat radius;
  GLfloat gravity;

  vec3 massCenter;
};

extern struct Fragment* fragments;

#endif
