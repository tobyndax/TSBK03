// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <math.h>
#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"

int numBoids = 0;
float kAlignmentWeight = 1;
float kCohesionWeight = 0.005f;
float kAvoidanceWeight = 1;
float kMaxDistance = 150;


bool myDistance(SpritePtr sp1, SpritePtr sp2, float kMaxDistance){
	float d1 = pow(sp2->position.h - sp1->position.h,2);
	float d2 = pow(sp2->position.v - sp1->position.v,2);
	//printf("%f\n",sqrt(d1+d2) );
	return (sqrt(d1+d2)<kMaxDistance);
}


void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.

SpritePtr sp1 = gSpriteRoot;
SpritePtr sp2;

for (int i = 0; i < numBoids; ++i) {
	int count = 0;
	sp1->averagePosition.h = 0;
	sp1->averagePosition.v = 0;

	sp2 = gSpriteRoot;
	for (int j = 0; j < numBoids; ++j) {
		if (i == j){

		}else if(myDistance(sp1,sp2,kMaxDistance)){
				sp1->averagePosition.h += sp2->position.h;
				sp1->averagePosition.v += sp2->position.v;
				count += 1;

		}
		sp2=sp2->next;
	}

	//update boid i's states
	if (count > 0){
		sp1->averagePosition.h /= count;
		sp1->averagePosition.v /= count;
	}else{
		sp1->averagePosition.h = sp1->position.h;
		sp1->averagePosition.v = sp1->position.v;
	}

	//Go to next main boid
	sp1 = sp1->next;
}
SpritePtr sp = gSpriteRoot;
for (int i = 0; i < numBoids; ++i) {
	printf("%f \n", sp->averagePosition.h);
	//sp->speed.h += sp->speedDiff.h*kAlignmentWeight + (sp->averagePosition.h )*kCohesionWeight + sp->avoidanceVector.h *kAvoidanceWeight;
	//sp->speed.v += sp->speedDiff.v*kAlignmentWeight + (sp->averagePosition.v ) *kCohesionWeight + sp->avoidanceVector.v *kAvoidanceWeight;

	sp->speed.h += sp->speedDiff.h*kAlignmentWeight + (sp->averagePosition.h -sp->position.h)*kCohesionWeight + sp->avoidanceVector.h *kAvoidanceWeight;
	sp->speed.v += sp->speedDiff.v*kAlignmentWeight + (sp->averagePosition.v -sp->position.v) *kCohesionWeight + sp->avoidanceVector.v *kAvoidanceWeight;
	sp = sp->next;
}



}

// Drawing routine
void Display()
{
	SpritePtr sp;

	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawBackground();

	SpriteBehavior(); // Din kod!

// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);

	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat

	NewSprite(sheepFace, 100, 200, 1, 1);
	NewSprite(sheepFace, 200, 100, 1.5, -1);
	NewSprite(sheepFace, 250, 200, -1, 1.5);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");

	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);

	InitSpriteLight();
	Init();

	glutMainLoop();
	return 0;
}
