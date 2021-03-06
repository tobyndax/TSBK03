/// @file myDrawable.cpp
/// @brief Implementations of functions in myDrawable.h

#include "myDrawable.h"


#include "gtx/transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>


myDrawable::myDrawable(GLuint program)
: program(program) {}

SkyCube::SkyCube(GLuint program)
: myDrawable(program) {
	/* Initialize skycube */
	model = generateCube(10.0f);

	// Creating cubemap texture
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	TextureData texture1;
	memset(&texture1, 0, sizeof(texture1));
	TextureData texture2;
	memset(&texture2, 0, sizeof(texture2));
	TextureData texture3;
	memset(&texture3, 0, sizeof(texture3));
	TextureData texture4;
	memset(&texture4, 0, sizeof(texture4));
	TextureData texture5;
	memset(&texture5, 0, sizeof(texture5));
	TextureData texture6;
	memset(&texture6, 0, sizeof(texture6));

	LoadTGATextureData("resources/Skycube/Xn.tga", &texture1);
	LoadTGATextureData("resources/Skycube/Xp.tga", &texture2);
	LoadTGATextureData("resources/Skycube/Yn.tga", &texture3);
	LoadTGATextureData("resources/Skycube/Yp.tga", &texture4);
	LoadTGATextureData("resources/Skycube/Zn.tga", &texture5);
	LoadTGATextureData("resources/Skycube/Zp.tga", &texture6);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, texture1.width, texture1.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, texture2.width, texture2.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, texture3.width, texture3.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture3.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, texture4.width, texture4.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture4.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, texture5.width, texture5.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture5.imageData);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, texture6.width, texture6.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture6.imageData);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void SkyCube::draw() {
	glUniform1i(glGetUniformLocation(program, "cube_texture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	DrawModel(model, program, "in_Position", NULL, NULL);
}

Terrain::Terrain(GLuint program, std::vector<Model*>* initModel, glm::vec3 scale)
: myDrawable(program) {
	model = initModel;
	MTWMatrix = glm::scale(scale);
	inverseNormalMatrixTrans = glm::transpose(glm::inverse(glm::mat3(MTWMatrix)));

	// Initial one-time shader uploads.
	// Light information:
	glm::vec3 sunPos = { 0.58f, 0.58f, 0.58f }; // Since the sun is a directional source, this is the negative direction, not the position.
	bool sunIsDirectional = 1;
	float sunSpecularExponent = 50.0;
	glm::vec3 sunColor = { 1.0f, 1.0f, 1.0f };
	GLfloat sun_GLf[3] = { sunPos.x, sunPos.y, sunPos.z };

	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "lightSourcePos"), 1, sun_GLf);
	glUniform1i(glGetUniformLocation(program, "isDirectional"), sunIsDirectional);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 1, &sunSpecularExponent);
	GLfloat sunColor_GLf[3] = { sunColor.x, sunColor.y, sunColor.z };
	glUniform3fv(glGetUniformLocation(program, "lightSourceColor"), 1, sunColor_GLf);

	glUniformMatrix4fv(glGetUniformLocation(program, "MTWMatrix"), 1, GL_FALSE, glm::value_ptr(MTWMatrix));
	glUniformMatrix3fv(glGetUniformLocation(program, "iNormalMatrixTrans"), 1, GL_FALSE, glm::value_ptr(inverseNormalMatrixTrans));
}

void Terrain::draw() {
	for (GLuint i = 0; i < model->size(); i++) {
		DrawModel(model->at(i), program, "in_Position", "in_Normal", "in_TexCoord");
	}
}

Box::Box(GLuint program) : myDrawable(program){

	rotation = glm::mat4();
	scaling = glm::mat4();
	MTWMatrix = glm::scale(glm::vec3(1,1,1));
	inverseNormalMatrixTrans = glm::transpose(glm::inverse(glm::mat3(MTWMatrix)));


	model = loadCube("resources/cubeexp.obj");
	// Initial one-time shader uploads.
	// Light information:
	glm::vec3 sunPos = { 50.28f, 50.58f, 50.28f }; // Since the sun is a directional source, this is the negative direction, not the position.
	bool sunIsDirectional = 1;
	float sunSpecularExponent = 50.0;
	glm::vec3 sunColor = { 65.0f, 105.0f, 225.0f};
	sunColor /= 250.0f;
	GLfloat sun_GLf[3] = { sunPos.x, sunPos.y, sunPos.z };

	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "lightSourcePos"), 1, sun_GLf);
	glUniform1i(glGetUniformLocation(program, "isDirectional"), sunIsDirectional);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 1, &sunSpecularExponent);
	GLfloat sunColor_GLf[3] = { sunColor.x, sunColor.y, sunColor.z };
	glUniform3fv(glGetUniformLocation(program, "lightSourceColor"), 1, sunColor_GLf);

	glUniformMatrix4fv(glGetUniformLocation(program, "MTWMatrix"), 1, GL_FALSE, glm::value_ptr(MTWMatrix));
	glUniformMatrix3fv(glGetUniformLocation(program, "iNormalMatrixTrans"), 1, GL_FALSE, glm::value_ptr(inverseNormalMatrixTrans));


}

Box::Box(GLuint program, float s) : myDrawable(program) {

	rotation = glm::mat4();
	scaling = glm::mat4();
	MTWMatrix = glm::scale(glm::vec3(1,1,1));
	inverseNormalMatrixTrans = glm::transpose(glm::inverse(glm::mat3(MTWMatrix)));


	model = loadCube("resources/cubeexp.obj");
	// Initial one-time shader uploads.
	// Light information:
	glm::vec3 sunPos = { 0.58f, 0.58f, 0.58f }; // Since the sun is a directional source, this is the negative direction, not the position.
	bool sunIsDirectional = 1;
	float sunSpecularExponent = 50.0;
	glm::vec3 sunColor = { 1.0f, 1.0f, 1.0f };
	GLfloat sun_GLf[3] = { sunPos.x, sunPos.y, sunPos.z };

	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "lightSourcePos"), 1, sun_GLf);
	glUniform1i(glGetUniformLocation(program, "isDirectional"), sunIsDirectional);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 1, &sunSpecularExponent);
	GLfloat sunColor_GLf[3] = { sunColor.x, sunColor.y, sunColor.z };
	glUniform3fv(glGetUniformLocation(program, "lightSourceColor"), 1, sunColor_GLf);

	glUniformMatrix4fv(glGetUniformLocation(program, "MTWMatrix"), 1, GL_FALSE, glm::value_ptr(MTWMatrix));
	glUniformMatrix3fv(glGetUniformLocation(program, "iNormalMatrixTrans"), 1, GL_FALSE, glm::value_ptr(inverseNormalMatrixTrans));


}

Box::Box(GLuint program, glm::vec3 trans,glm::vec3 ex, q3Body* body) : Box(program,1.0f){
	//Handle the actual box added to the body.

	addBody(body);

	q3Transform tx;
	q3Identity( tx );

	q3BoxDef boxDef;

	tx.position.x = trans.x;
	tx.position.y = trans.y;
	tx.position.z = trans.z;

	boxDef.Set(tx, q3Vec3(ex.x,ex.y,ex.z));

	body->AddBox(boxDef);

	//Set the local transformations.
	Box::translateLocal(trans.x,trans.y,trans.z);
	rotation = glm::mat4();
	scaling = glm::scale(ex/2.0f);
	updateState();

}

Box::Box(GLuint program, glm::vec3 trans,glm::vec3 ex, q3Body* body, bool ground) : Box(program){
	//Handle the actual box added to the body.

	addBody(body);

	q3Transform tx;
	q3Identity( tx );

	q3BoxDef boxDef;

	tx.position.x = trans.x;
	tx.position.y = trans.y;
	tx.position.z = trans.z;

	boxDef.Set(tx, q3Vec3(ex.x,ex.y,ex.z));

	body->AddBox(boxDef);

	//Set the local transformations.
	Box::translateLocal(trans.x,trans.y,trans.z);
	rotation = glm::mat4();
	scaling = glm::scale(ex/2.0f);
	updateState();

}

void Box::scale(float x, float y, float z){
	scaling = glm::scale(glm::vec3(x,y,z));
}

void Box::rotate(float deg, float x, float y, float z){

	rotation = glm::rotate(glm::mat4(), deg, glm::vec3(x, y, z));
}

void Box::translate(float x, float y, float z){
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Box::translateLocal(float x, float y, float z){
	localTrans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Box::updateState(){
	q3Transform tx = body->GetTransform();
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(tx.position.x, tx.position.y, tx.position.z));
	glm::vec4 ex = glm::vec4(tx.rotation.ex.x,tx.rotation.ex.y,tx.rotation.ex.z,0.0f);
	glm::vec4 ey = glm::vec4(tx.rotation.ey.x,tx.rotation.ey.y,tx.rotation.ey.z,0.0f);
	glm::vec4 ez = glm::vec4(tx.rotation.ez.x,tx.rotation.ez.y,tx.rotation.ez.z,0.0f);
	glm::vec4 e1 = glm::vec4(0,0,0,1);
	rotation = glm::mat4(ex,ey,ez,e1);
}

void Box::draw() {
	glUseProgram(program);

	glm::mat4 total = translation*rotation*localTrans*scaling;

	inverseNormalMatrixTrans = glm::transpose(glm::inverse(glm::mat3(total)));

	glUniformMatrix3fv(glGetUniformLocation(program, "iNormalMatrixTrans"), 1, GL_FALSE, glm::value_ptr(inverseNormalMatrixTrans));
	glUniformMatrix4fv(glGetUniformLocation(program, "MTWMatrix"), 1, GL_FALSE, glm::value_ptr(total));
	DrawModel(model, program, "in_Position","in_Normal", "in_TexCoord");

}
void calcMassCenterP(struct Fragment F){

	float A = 0;
	float Cx = 0;
	float Cy = 0;

	for (int i = 0; i < F.numOnHull-1; i++) {
		int j = i % (F.numOnHull-1);
		float x1 = F.pointsOnHull[j*3+0];
		float y1 = F.pointsOnHull[j*3+1];
		float x2 = F.pointsOnHull[(j+1)*3+0];
		float y2 = F.pointsOnHull[(j+1)*3+1];

		std::cout <<"x1: " << x1 << "  x2: " << x2  <<"y1: " << y1 << "  y2: " << y2<< std::endl;

		Cx += (x1 + x2)*(x1*y2 - x2*y1);
		Cy += (y1 + y2)*(x1*y2 - x2*y1);

		A  += (x1*y2 - x2*y1);
	}

	Cx /= (6.0f*A/2.0f);
	Cy /= (6.0f*A/2.0f);
	if (isnan(Cx))




	std::cout <<"Cx: " << Cx << "  Cy: " << Cy << std::endl;
}


glm::vec3 Frag::calcMassCenter(struct Fragment F){

	float A = 0;
	float Cx = 0;
	float Cy = 0;

	for (int i = 0; i < F.numOnHull-1; i++) {
		int j = i % (F.numOnHull-1);
		float x1 = F.pointsOnHull[j*3+0];
		float y1 = F.pointsOnHull[j*3+1];
		float x2 = F.pointsOnHull[(j+1)*3+0];
		float y2 = F.pointsOnHull[(j+1)*3+1];


		Cx += (x1 + x2)*(x1*y2 - x2*y1);
		Cy += (y1 + y2)*(x1*y2 - x2*y1);

		A  += (x1*y2 - x2*y1);
	}

	Cx /= (6.0f*A/2.0f);
	Cy /= (6.0f*A/2.0f);
	if (isnan(Cx)){
		//calcMassCenterP(F);
		float avg = 1.0f/(float)F.numOnHull;
		float xm =0.0f;
		float ym =0.0f;
		float zm =0.0f;

		for (int i = 0; i < F.numOnHull; i++) {
			xm +=  avg*(F.pointsOnHull[i*3 +0]);
			ym +=  avg*(F.pointsOnHull[i*3 +1]);
			zm +=  avg*(F.pointsOnHull[i*3 +2]);
		}
		return glm::vec3(xm,ym,zm);
	}
	return glm::vec3(Cx,Cy,0);
}


void Frag::addHullBoxes(struct Fragment F){
		for (int i = 0; i < F.numOnHull-1; i++) {
			glm::vec3 point = glm::vec3(F.pointsOnHull[i*3+0],F.pointsOnHull[i*3+1],F.pointsOnHull[i*3+2]);
			glm::vec3 dir = point - center;
			point -= dir*0.1f;
			Box* b = new Box(boxprogram,point,glm::vec3(0.5f,0.5f,0.25f), body);
			if(DEBUG_HULL){
				hull.push_back(b);
			}
		}
}

q3BodyDef Frag::addDynamics(struct Fragment F,q3BodyDef bodyDef){
	glm::vec3 dir = center - colPoint;
	float distance = glm::length(dir);
	dir = glm::normalize(dir);
	glm::vec3 linMom = dir/distance * 1500.0f;

	bodyDef.linearVelocity.Set(linMom.x/2,linMom.y/2,linMom.z);

	float x = glm::dot(glm::vec3(1,0,0),dir);
	float y = glm::dot(glm::vec3(0,1,0),dir);


	bodyDef.angularVelocity.Set(y*1.5f,-x*1.5f,0);


	return bodyDef;
}


Frag::Frag(GLuint program,GLuint boxprogramIn, struct Fragment frag,q3Scene* scene) : myDrawable(program) {
	rotation = glm::mat4();
	scaling = glm::mat4();
	q3BodyDef bodyDef;
	bodyDef.bodyType = eDynamicBody;
	bodyDef.position.Set(0,5,0);

	boxprogram = boxprogramIn;
	center = calcMassCenter(frag);
	bodyDef = addDynamics(frag,bodyDef);



	body = scene->CreateBody( bodyDef );

	updateState();

	if(DEBUG_SEED){
		Box* b = new Box(boxprogram,glm::vec3(frag.center),glm::vec3(0.5f,0.5f,0.5f), body);
		seed.push_back(b);
	}

	if(center.x == 0 && center.y == 0 ){
		//remove if at origin. (shouldn't be any center there)
	}else{

	Box* b = new Box(boxprogram,center,glm::vec3(0.5f,0.5f,0.5f), body);
	cent.push_back(b);
	addHullBoxes(frag);
	}

	MTWMatrix = glm::scale(glm::vec3(1,1,1));
	inverseNormalMatrixTrans = glm::transpose(glm::inverse(glm::mat3(MTWMatrix)));

	GLint verts = frag.numVertices;
	model = LoadDataToModel(frag.vertices,frag.normals,NULL, NULL,frag.indicies,verts,verts);

	// Initial one-time shader uploads.
	// Light information:
	glm::vec3 sunPos = { 0.58f, 0.58f, 0.58f }; // Since the sun is a directional source, this is the negative direction, not the position.
	bool sunIsDirectional = 1;
	float sunSpecularExponent = 50.0;
	glm::vec3 sunColor = { 1.0f, 1.0f, 1.0f };
	GLfloat sun_GLf[3] = { sunPos.x, sunPos.y, sunPos.z };

	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "lightSourcePos"), 1, sun_GLf);
	glUniform1i(glGetUniformLocation(program, "isDirectional"), sunIsDirectional);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 1, &sunSpecularExponent);
	GLfloat sunColor_GLf[3] = { sunColor.x, sunColor.y, sunColor.z };
	glUniform3fv(glGetUniformLocation(program, "lightSourceColor"), 1, sunColor_GLf);

	glUniformMatrix4fv(glGetUniformLocation(program, "MTWMatrix"), 1, GL_FALSE, glm::value_ptr(MTWMatrix));
	glUniformMatrix3fv(glGetUniformLocation(program, "iNormalMatrixTrans"), 1, GL_FALSE, glm::value_ptr(inverseNormalMatrixTrans));


}

void Frag::draw(){

	if(DEBUG_CENTER){
		for(std::vector<Box*>::iterator it = cent.begin(); it !=  cent.end(); ++it) {
			(*it)->updateState();
			(*it)->scale(0.25f,0.25f,1);
			(*it)->draw();
		}
	}

	if(DEBUG_SEED){
		for(std::vector<Box*>::iterator it = seed.begin(); it != seed.end(); ++it) {
			(*it)->updateState();
			(*it)->scale(0.25f,0.25f,1);
			(*it)->draw();
		}
	}
	if(DEBUG_HULL){
		for(std::vector<Box*>::iterator it = hull.begin(); it != hull.end(); ++it) {
			(*it)->updateState();
			(*it)->scale(0.25f,0.25f,1);
			(*it)->draw();
		}
	}

	glUseProgram(program);


	glm::mat4 total = translation*rotation;

	inverseNormalMatrixTrans = glm::transpose(glm::inverse(glm::mat3(total)));

	glUniformMatrix3fv(glGetUniformLocation(program, "iNormalMatrixTrans"), 1, GL_FALSE, glm::value_ptr(inverseNormalMatrixTrans));
	glUniformMatrix4fv(glGetUniformLocation(program, "MTWMatrix"), 1, GL_FALSE, glm::value_ptr(total));
	DrawModel(model, program, "in_Position","in_Normal", "in_TexCoord");

}


void Frag::scale(float x, float y, float z){
	scaling = glm::scale(glm::vec3(x,y,z));
}

void Frag::rotate(float deg, float x, float y, float z){

	rotation = glm::rotate(glm::mat4(), deg, glm::vec3(x, y, z));
}

void Frag::translate(float x, float y, float z){
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Frag::translateLocal(float x, float y, float z){
	localTrans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Frag::updateState(){
	q3Transform tx = body->GetTransform();
	translation = glm::translate(glm::mat4(1.0f), glm::vec3(tx.position.x, tx.position.y, tx.position.z));
	glm::vec4 ex = glm::vec4(tx.rotation.ex.x,tx.rotation.ex.y,tx.rotation.ex.z,0.0f);
	glm::vec4 ey = glm::vec4(tx.rotation.ey.x,tx.rotation.ey.y,tx.rotation.ey.z,0.0f);
	glm::vec4 ez = glm::vec4(tx.rotation.ez.x,tx.rotation.ez.y,tx.rotation.ez.z,0.0f);
	glm::vec4 e1 = glm::vec4(0,0,0,1);
	rotation = glm::mat4(ex,ey,ez,e1);
}
