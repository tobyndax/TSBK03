/// @file program.cpp
/// @brief Implementations of functions in program.h

#include "program.h"

#include "GL_utilities.h"

#include "gtc/type_ptr.hpp"
#include <iostream>

Program::Program() {
	screenW = 800;
	screenH = 800;

	isRunning = true;
	mouseHidden = true;

	heightAtClickData = 0.0f;
	heighAtClickProj = 0.0f;

	// Time init
	currentTime = (GLfloat)SDL_GetTicks();
	deltaTime = 0;
}

Program::~Program() {}

int Program::exec() {
	if (!init()) return -1;

	SDL_Event Event;

	while (isRunning) {
		timeUpdate();
		while (SDL_PollEvent(&Event)) handleEvent(&Event);
		checkKeys();
		update();
		display();
	}

	clean();
	return 0;
}

bool Program::init() {
	// SDL, glew and OpenGL init

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Failed to initialise SDL: %s", SDL_GetError());
		return false;
	}

	#ifdef __APPLE__
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	#endif


	screen = SDL_CreateWindow("Voronoi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenW, screenH, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (screen == 0) {
		fprintf(stderr, "Failed to set Video Mode: %s", SDL_GetError());
		return false;
	}


	glcontext = SDL_GL_CreateContext(screen);


	SDL_SetRelativeMouseMode(SDL_FALSE);
	glClearColor(0.1f, 0.7f, 0.1f, 1.0f);
	printError("After SDL init: ");


	#ifdef _WINDOWS
	glewInit();
	#endif

	dumpInfo();

	// Initial placement of camera.
	cam = new Camera(glm::vec3(0.0f,500.0f,0.0f), &screenW, &screenH);

	// Load and compile shaders.
	skyshader = loadShaders("src/shaders/skyshader.vert", "src/shaders/skyshader.frag");
	terrainshader = loadShaders("src/shaders/terrainshader.vert", "src/shaders/terrainshader.frag");
	// Create drawables
	skycube = new SkyCube(skyshader);
	//
	//Box* boxground = new Box(terrainshader,10.0f);
	//boxground->scale(500,0.5,500);
	//boxes.push_back(boxground);




	q3BodyDef bodyDef;
	q3Body* body = scene->CreateBody( bodyDef );
	q3BoxDef boxDef;
	boxDef.SetRestitution( 0 );
	q3Transform tx;
	q3Identity( tx );
	boxDef.Set( tx, q3Vec3( 500.0f, 1.0f, 500.0f ) );
	body->AddBox( boxDef );

	bodyDef.bodyType = eDynamicBody;
	bodyDef.position.Set( 0, 250.0f, 0 );
	q3Body* body1 = scene->CreateBody( bodyDef );


	tx.position.Set(0,-5.0f,0); // this is in body coordinates. (i.e one box at 5.0f and one box at 6.0f)
	boxDef.Set( tx, q3Vec3( 10.0f, 10.0f, 10.0f )); //the latter vector here is the exapansion of the box.
	body1->AddBox(boxDef);

	tx.position.Set(0,40.0f,0); // this is in body coordinates. (i.e one box at 5.0f and one box at 6.0f)
	boxDef.Set( tx, q3Vec3( 10.0f, 10.0f, 10.0f )); //the latter vector here is the exapansion of the box.
	body1->AddBox(boxDef);

	tx.position.Set(0,40.0f,40.0f); // this is in body coordinates. (i.e one box at 5.0f and one box at 6.0f)
	boxDef.Set( tx, q3Vec3( 10.0f, 10.0f, 10.0f )); //the latter vector here is the exapansion of the box.
	body1->AddBox(boxDef);

	Box* box = new Box(terrainshader,1.0f);
	Box* box1 = new Box(terrainshader,1.0f);

	Box* box2 = new Box(terrainshader,1.0f);
	Box* box3 = new Box(terrainshader,1.0f);

	box->addBody(body);
	box->scale(250,1,250);
	box->updateState();

	box1->addBody(body1);
	box1->scale(10,10,10);
	box1->updateState();

	box2->addBody(body1);
	box2->scale(10,10,10);
	box2->translateLocal(0,40.0f,0);
	box2->updateState();


	box3->addBody(body1);
	box3->scale(10,10,10);
	box3->translateLocal(0,40.0f,40.0f);
	box3->updateState();




	boxes.push_back(box);
	boxes.push_back(box1);
	boxes.push_back(box2);
	boxes.push_back(box3);





	printError("After total init: ");

	return true;
}

void Program::timeUpdate() {
	GLfloat t = (GLfloat)SDL_GetTicks();
	deltaTime = t - currentTime;
	currentTime = t;
	FPS = 1000.0f / deltaTime;
}

void Program::update() {
	scene->Step();
	for(std::vector<Box*>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
		(*it)->updateState();
	}

}

void Program::display() {
	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ====================== Draw skybox ===========================
	glUseProgram(skyshader);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	// ---Camera shader data---
	cam->uploadCamData(skyshader);
	skycube->draw();

	// ====================== Draw Terrain ==========================
	glUseProgram(terrainshader);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	for(std::vector<Box*>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
		(*it)->draw();
	}
	cam->uploadCamData(terrainshader);

	printError("After display: ");

	SDL_GL_SwapWindow(screen);
}

void Program::clean() {
	SDL_GL_DeleteContext(glcontext);
	SDL_Quit();
}


// Handle events.
void Program::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_QUIT:
		isRunning = false;
		break;
		case SDL_KEYDOWN:
		handleKeypress(event);
		break;
		case SDL_WINDOWEVENT:
		switch (event->window.event) {
			case SDL_WINDOWEVENT_RESIZED:
			SDL_SetWindowSize(screen, event->window.data1, event->window.data2);
			SDL_GetWindowSize(screen, &screenW, &screenH);
			glViewport(0, 0, screenW, screenH);
			cam->updateVTP();
			break;
		}
		break;
		case SDL_MOUSEMOTION:
		handleMouseMove(event);
		break;
		case SDL_MOUSEBUTTONDOWN:
		handleMouseButton(event);
		break;
		default:
		break;
	}
}

// Handle keys
void Program::handleKeypress(SDL_Event* event) {
	switch (event->key.keysym.sym) {
		case SDLK_ESCAPE:
		isRunning = false;
		break;
		case SDLK_h:
		cam->toggleFrozen();
		mouseHidden = !mouseHidden;
		if (!mouseHidden) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		} else {
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		break;
		default:
		break;
	}
}

void Program::handleMouseMove(SDL_Event* event) {
	cam->changeLookAtPos(event->motion.xrel, event->motion.yrel);
}

void Program::handleMouseButton(SDL_Event* event) {
}

void Program::checkKeys() {
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_W]) {
		cam->forward(deltaTime);
	} else if (keystate[SDL_SCANCODE_S]) {
		cam->forward(-deltaTime);
	}
	if (keystate[SDL_SCANCODE_A]) {
		cam->strafe(deltaTime);
	} else if (keystate[SDL_SCANCODE_D]) {
		cam->strafe(-deltaTime);
	}
	if (keystate[SDL_SCANCODE_Q]) {
		cam->jump(deltaTime);
	} else if (keystate[SDL_SCANCODE_E]) {
		cam->jump(-deltaTime);
	}
}
