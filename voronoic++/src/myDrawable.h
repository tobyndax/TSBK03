/// @file drawable.h
/// @brief Contains classes with draw methods.

#ifndef MYDRAWABLE_H
#define MYDRAWABLE_H

#include "loadobj.h"
#include "glm.hpp"
#include <q3.h>

#include "Utilities.h"
#include <vector>

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
	Box(GLuint program, float s);
	Box(GLuint program, glm::vec3 trans,glm::vec3 ex, q3Body* body);
	void rotate(float deg, float x, float y, float z);
	virtual void draw();
	void scale(float x, float y, float z);
	void updateState();

	void translate(float x, float y, float z);
	void translateLocal(float x, float y, float z);

};

#endif // DRAWABLE_H
