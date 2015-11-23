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
  GLint numIndices;
  GLint numVertices;
  GLfloat *vertices;
  GLfloat *texCoord;
  GLuint *indicies;
  GLfloat *normals;
};

typedef struct
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

} PhysicsObj;

extern struct Fragment* fragments;

#endif
