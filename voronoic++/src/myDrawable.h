/// @file drawable.h
/// @brief Contains classes with draw methods.

#ifndef MYDRAWABLE_H
#define MYDRAWABLE_H

#include "loadobj.h"
#include "glm.hpp"
#include <q3.h>
#include "LoadTGA.h"
#include <cmath>

#include "Utilities.h"
#include <vector>
#include "voronoi.h"

class myDrawable {
protected:
	glm::mat4 MTWMatrix;
	GLuint program;

public:
	myDrawable(GLuint program);
	virtual void draw() = 0;
};

class SkyCube : public myDrawable {
private:
	Model* model;
	GLuint textureID;

public:
	SkyCube(GLuint program);
	virtual void draw();
};

class Terrain : public myDrawable {
private:
	std::vector<Model*>* model;
	glm::mat3 inverseNormalMatrixTrans;

public:
	Terrain(GLuint program, std::vector<Model*>*, glm::vec3 scale);
	virtual void draw();
};

class Box : public myDrawable {
private:

	glm::mat3 inverseNormalMatrixTrans;
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scaling;
	q3Body* body;
	glm::mat4 localTrans;
	Model* model;

public:

	void addBody(q3Body* b){body = b;}
	Box(GLuint program);
	Box(GLuint program, float s);
	Box(GLuint program, glm::vec3 trans,glm::vec3 ex, q3Body* body);
	Box(GLuint program, glm::vec3 trans,glm::vec3 ex, q3Body* body,bool ground);
	void rotate(float deg, float x, float y, float z);
	virtual void draw();
	void scale(float x, float y, float z);
	void updateState();

	void translate(float x, float y, float z);
	void translateLocal(float x, float y, float z);

};

class Frag : public myDrawable {
private:
	glm::mat3 inverseNormalMatrixTrans;
	glm::mat4 translation;
	glm::mat4 rotation;
	glm::mat4 scaling;
	q3Body* body;
	std::vector<Box*> seed;
	std::vector<Box*> cent;
	std::vector<Box*> hull;
	glm::mat4 localTrans;
	Model* model;
	GLuint boxprogram;

	float* vertices;
	float* normals;
	GLuint* indicies;
	glm::vec3 calcMassCenter(struct Fragment);


	glm::vec3 colPoint = glm::vec3{50,50,25};

	glm::vec3 center;

	void addHullBoxes(struct Fragment F);
	q3BodyDef addDynamics(struct Fragment F,q3BodyDef bodyDef);
public:

	static const bool DEBUG_CENTER = !true;
	static const bool DEBUG_SEED = !true;
	static const bool DEBUG_HULL = !true;


	Frag(GLuint program,GLuint boxprogram,struct Fragment frag,q3Scene* scene);
	virtual void draw();

	void rotate(float deg, float x, float y, float z);
	void scale(float x, float y, float z);

	void translate(float x, float y, float z);
	void translateLocal(float x, float y, float z);

	void updateState();
};




#endif // DRAWABLE_H
