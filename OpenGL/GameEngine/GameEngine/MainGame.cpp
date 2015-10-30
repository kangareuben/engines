#include "MainGame.h"
#include "Errors.h"
#include "iostream"
#include <random>

//global pointer point to mainGame
MainGame* g_pMainGame = NULL;

float t = 0.0f;
using namespace std;
double accumulator = 0.0;
float dt = 0.0f;
float ti;
// 0 - ground
// 1 - box 
// 2 - sphere

//Physics Thread
void g_fnPhysicsThread()
{
	while (g_pMainGame->_gameState != GameState::EXIT)
	{
		const float DESIRED_FPS = 60.0f;
		const float MS_PER_SECOND = 1000.0f;
		const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
		const int MAX_PHYSICS_STEPS = 6;
		const float MAX_DELTA_TIME = 1.0f;
		float previousTicks = SDL_GetTicks();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		dt = frameTime / DESIRED_FRAMETIME;

		int i = 0;

		while (dt>0.0f && i <MAX_PHYSICS_STEPS)
		{
			float deltaTime;


			if (dt < MAX_DELTA_TIME)
				deltaTime = dt;
			else
				deltaTime = MAX_DELTA_TIME;

			for (int j = 0; j < g_pMainGame->mapIndex; j++)
			{
				if (g_pMainGame->objMap.at(j)->physics->colliderType != 5)
				{
					for (int k = j + 1; k < g_pMainGame->mapIndex; k++)
					{
						if (g_pMainGame->objMap.at(k)->physics->colliderType != 5)
						{
							g_pMainGame->checkCollision(g_pMainGame->objMap.at(j)->physics, g_pMainGame->objMap.at(k)->physics);

							//This was an attempt to use multiple threads per frame for collision detection
							//It didn't work because too many threads were created and the performance went down
							/*boost::thread CheckCollisionThread(	&MainGame::checkCollision,	
																g_pMainGame, 
																g_pMainGame->objMap.at(j)->physics, 
																g_pMainGame->objMap.at(k)->physics);*/
						}
					}
				}
			}

			for (int j = 0; j < g_pMainGame->mapIndex; j++)
			{
				g_pMainGame->objMap.at(j)->physics->update(deltaTime);
			}

			i++;
			dt -= deltaTime;
		}
	}
}

void g_fnCreateObject(ObjParams* objParams)
{
	g_pMainGame->mtx2.lock();
	g_pMainGame->nLoadThreadCount++;
	g_pMainGame->mtx2.unlock();

	g_pMainGame->createObject(objParams);
	delete objParams;

	g_pMainGame->mtx3.lock();
	g_pMainGame->nLoadThreadCount--;
	g_pMainGame->mtx3.unlock();
	//cout << "hi" << endl;
}

MainGame::MainGame()
{
	currentTime = 0;
	ptr_window = nullptr;
	_windowWidth = 640;
	_windowHeight = 480;
	_xDist = 0;
	_yDist = 0;
	_zDist = 0;
	_yRot = 0;
	_zRot = 0;
	_xRot = 0;
	_eyeX = 0;
	_maxFPS = 60;
	_mouseVel = 0.2f;
	_moveVel = 0.2f;
	mapIndex = 0;
	g_pMainGame = this;
	bIsLoaded = false;
	nLoadThreadCount = 0;
	gravityEnabled = true;
}


MainGame::~MainGame()
{
}

void MainGame::initSystems()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Open an SDL window
	ptr_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (ptr_window == nullptr)
	{
		fatalError("SDL Window could not be created!");
	}

	//Set up our OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(ptr_window);
	if (glContext == nullptr)
	{
		fatalError("SDL_GL context could not be created!");
	}

	//Set up glew 
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fatalError("Could not initialize glew!");
	}

	//Tell SDL that we want a double buffered window so we dont get
	//any flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, _windowWidth, _windowHeight);
	GLfloat aspect = (GLfloat)_windowWidth / _windowHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, aspect, 1, 500);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void MainGame::run()
{
	
	initSystems();
	//file path,initial x, initial y, initial z, gravity true or false, colliderType, width,height,depth,mass
	
	//start loading obj
	bIsLoaded = false;
	ObjParams* op = new ObjParams();
	op->setObjParams("Models/box.obj", 2, 20, 20, 1, 1, 1, 1, 0, 0, 0, true, 1);
	boost::thread t1(&g_fnCreateObject, op);

	/*op = new ObjParams();
	op->setObjParams("Models/sphere.obj", -2, 20, 20, 1, 1, 1, 2, 0, 0, 0, true, 2);
	boost::thread t2(&g_fnCreateObject, op);*/

	op = new ObjParams();
	op->setObjParams("Models/Terrain.obj", 0, 0, 0, 200, 0.001f, 200, 1000, 0, 0, 0, false, 0);
	boost::thread t3(&g_fnCreateObject, op);

	op = new ObjParams();
	op->setObjParams("Models/box.obj", 2, 10, 20, 1, 1, 1, 2, 0, 0, 0, false, 1);
	boost::thread t4(&g_fnCreateObject, op);

	op = new ObjParams();
	op->setObjParams("Models/box.obj", 10, 0, 20, 1, 1, 1, 3, -0.1f, 0, 0, true, 1);
	boost::thread t5(&g_fnCreateObject, op);

	op = new ObjParams();
	op->setObjParams("Models/box.obj", -10, 0, 20, 1, 1, 1, 4, 0.1f, 0, 0, true, 1);
	boost::thread t6(&g_fnCreateObject, op);
	
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(-0.1f, 0.1f);
	std::uniform_real_distribution<double> dist2(0.001f, 0.005f);

	

	for (int i = 0; i < 50; i++)
	{
		float x, y, z;
		x = dist(mt);
		y = dist2(mt);
		z = dist(mt);
		
		op = new ObjParams();
		op->setObjParams("Models/Particle.obj", 0, 0, 0, 0.001f, 0.001f, 0.001f, 0.1f, x, y, z, false, 5);
		boost::thread t6(&g_fnCreateObject, op);

		//createObject("Models/Particle.obj", 0, 0, 0, 0.001f, 0.001f, 0.001f, 0.1f, false, 5);

		//objMap.at(mapIndex - 1)->physics->setVelocity(x, y, z);
	}
	bIsLoaded = true;
	while (nLoadThreadCount != 0)
	{
		cout << "Still waiting for " << nLoadThreadCount << " threads" << endl;
	}
	//start the physics thread first
	boost::thread PhysicsThread(&g_fnPhysicsThread);
	//start rendering
	gameLoop();
}

void MainGame::gameLoop()
{
	//Will loop until we set _gameState to EXIT
	while (_gameState != GameState::EXIT)
	{
		processInput();
		draw();
	}
	
	//before quit the game, clean the objmap
	cleanObject();
}



void MainGame::processInput()
{
	SDL_Event evnt;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_WINDOWEVENT:
			switch (evnt.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				//any flickering
				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

				//Set the background color to blue
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

				//Set the base depth to 1.0
				glClearDepth(1.0);
				//Clear the color and depth buffer
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				break;
			}
			break;

		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//	std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseIn = true;
			SDL_ShowCursor(SDL_DISABLE);
			break;



		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
					//Press F1 to go full screen
				case SDLK_F1:
					SDL_SetWindowFullscreen(ptr_window, SDL_WINDOW_FULLSCREEN);
					break;
					//Press F2 to come back from fullscreen
					//Use function SDL_SetWindowSize() for custom size window
				case SDLK_F2:
					SDL_SetWindowFullscreen(ptr_window, 0);
					break;
				case SDLK_ESCAPE:
					_gameState = GameState::EXIT;
					break;
				case SDLK_w:
					if (mainCam.camPitch != 90 && mainCam.camPitch != -90)
					{
						mainCam.moveCamera(_moveVel, 0.0f);
					}
					mainCam.moveCameraUp(_moveVel, 0.0f);
					break;

				case SDLK_s:
					if (mainCam.camPitch != 90 && mainCam.camPitch != -90)
					{
						mainCam.moveCamera(_moveVel, 180.0f);
					}
					mainCam.moveCameraUp(_moveVel, 180.0f);
					break;
				case SDLK_p:
					mouseIn = false;
					SDL_ShowCursor(SDL_ENABLE);
					break;
				case SDLK_a:
					mainCam.moveCamera(_moveVel, 90.0f);
					break;

				case SDLK_d:
					mainCam.moveCamera(_moveVel, 270);
					break;

				//Simple user interaction
				//Presss g and gravity is toggled
				case SDLK_g:
					if (gravityEnabled)
					{
						gravityEnabled = false;
						for (int i = 0; i < mapIndex; i++)
						{
							objMap.at(i)->physics->gravity = false;
							objMap.at(i)->physics->gravityAcceleration = 0.0f;
						}
					}
					else
					{
						gravityEnabled = true;
						for (int i = 0; i < mapIndex; i++)
						{
							objMap.at(i)->physics->gravity = true;
							objMap.at(i)->physics->gravityAcceleration = 0.01f;
						}
					}
					break;
			}
		}
	}
}



void MainGame::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	mainCam.control(_moveVel, _mouseVel, mouseIn, ptr_window);
	mainCam.updateCamera();
	gluLookAt(0, 1, 25, 0, 0, 0, 0, 1, 0);
	glTranslatef(mainCam.camX*-1, mainCam.camY*-1, mainCam.camZ*-1);

	for (int i = 0; i < mapIndex; i++)
	{
		glPushMatrix();
		objMap.at(i)->Draw(objMap.at(i)->physics);
		glPopMatrix();
	}

	SDL_GL_SwapWindow(ptr_window);

}

void MainGame::calculateFPS()
{
	static const int _numSamples = 1000;
	static float frameTimes[_numSamples];
	static int _currentFrame = 0;

	static float _prevTicks = SDL_GetTicks();

	float _currentTicks;
	_currentTicks = SDL_GetTicks();

	_frameTime = _currentTicks - _prevTicks;
	frameTimes[_currentFrame%_numSamples] = _frameTime;

	_prevTicks = _currentTicks;

	int _count;

	_currentFrame++;


	if (_currentFrame < _numSamples)
	{
		_count = _currentFrame;
	}

	else
	{
		_count = _numSamples;
	}

	float _frameTimeAverage = 0.0f;
	for (int i = 0; i < _count; i++)
	{
		_frameTimeAverage += frameTimes[i];
	}

	_frameTimeAverage /= _count;

	if (_frameTimeAverage > 0)
	{
		_fps = 1000 / _frameTimeAverage;
	}

	else
	{
		_fps = 60;
	}
}

bool MainGame :: checkCollision(ObjPhysics *objA, ObjPhysics *objB)
{
	int flag = 0;

	if (objA->colliderType == 1 && objB->colliderType == 0)
	{
		if (fabs(objA->position[0] - objB->position[0]) > (objA->width / 2 + objB->width / 2)) flag = 0;
		else if (fabs(objA->position[1] - objB->position[1]) > (objA->height / 2 + objB->height / 2)) flag = 0;
		else if (fabs(objA->position[2] - objB->position[2]) > (objA->depth / 2 + objB->depth / 2)) flag = 0;
		else flag = 1;
		if (flag == 1)
		{
			objA->velocity[0] *= -1; objA->velocity[1] *= -1; objA->velocity[2] *= -1;
	
			return true;
		}

		else
			return false;
	}

	if (objA->colliderType == 1 && objB->colliderType == 1)
	{
		if (fabs(objA->position[0] - objB->position[0]) > (objA->width / 2 + objB->width / 2)) flag = 0;
		else if (fabs(objA->position[1] - objB->position[1]) > (objA->height / 2 + objB->height / 2)) flag = 0;
		else if (fabs(objA->position[2] - objB->position[2]) > (objA->depth / 2 + objB->depth / 2)) flag = 0;
		else flag = 1;
		if (flag == 1)
		{
		//	float m1 = objA->mass / (objA->mass + objB->mass);
		//	float m1 = objA->mass / (objA->mass + objB->mass);

			float temp0 = objA->velocity[0];
			float temp1 = objA->velocity[1];
			float temp2 = objA->velocity[2];
			objA->velocity[0] = -1 * objB->velocity[0]; objA->velocity[1] = -1 * objB->velocity[1]; objA->velocity[2] = -1 * objB->velocity[2];
			objB->velocity[0] = -1 * temp0; objB->velocity[1] = -1 * temp1; objB->velocity[2] = -1 * temp2;

			return true;
		}

		else
			return false;
	}

	else if (objA->colliderType == 2 && objB->colliderType == 0)
	{
		double squaredDistance;
		auto check = [&](
			const double pn,
			const double bmin,
			const double bmax) -> double
		{
			double out = 0;
			double v = pn;

			if (v < bmin)
			{
				double val = (bmin - v);
				out += val * val;
			}

			if (v > bmax)
			{
				double val = (v - bmax);
				out += val * val;
			}

			return out;
		};

		// Squared distance
		double sq = 0.0;

		sq += check(objA->position[0], objB->point1[0], objB->point8[0]);
		sq += check(objA->position[1], objB->point1[1], objB->point8[1]);
		sq += check(objA->position[2], objB->point1[2], objB->point8[2]);

		squaredDistance = sq;
		if (squaredDistance <= (objA->width * objA->width) == true)
		{
			//objA->reverseSpeed();
			objA->velocity[0] *= -1; objA->velocity[1] *= -1; objA->velocity[2] *= -1;

			objB->velocity[0] *= -1; objB->velocity[1] *= -1; objB->velocity[2] *= -1;

		//	objB->reverseSpeed();
			
		}
		//cout << "squaredDistance = " << squaredDistance << "    width squared = " << objA->width * objA->width << endl;
		return squaredDistance <= (objA->width * objA->width);
	}

	else if (objB->colliderType == 2 && objA->colliderType == 0)
	{
		double squaredDistance;
		auto check = [&](
			const double pn,
			const double bmin,
			const double bmax) -> double
		{
			double out = 0;
			double v = pn;

			if (v < bmin)
			{
				double val = (bmin - v);
				out += val * val;
			}

			if (v > bmax)
			{
				double val = (v - bmax);
				out += val * val;
			}

			return out;
		};

		// Squared distance
		double sq = 0.0;

		sq += check(objB->position[0], objA->point1[0], objA->point8[0]);
		sq += check(objB->position[1], objA->point1[1], objA->point8[1]);
		sq += check(objB->position[2], objA->point1[2], objA->point8[2]);

		squaredDistance = sq;
		if (squaredDistance <= (objB->width * objB->width) == true)
		{ 
		
			objA->velocity[0] *= -1; objA->velocity[1] *= -1; objA->velocity[2] *= -1;

			objB->velocity[0] *= -1; objB->velocity[1] *= -1; objB->velocity[2] *= -1;
		}
		return squaredDistance <= (objB->width * objB->width);
	}

	else
		return false;
}

void MainGame::createObject(ObjParams* objParams/*char* fileName, float x, float y, float z, float width,
							float height, float depth, float mass, bool hasGravity, int colliderType*/)
{
	Object* obj = new Object();
	
	ObjLoader* model = new ObjLoader();
	model->load(objParams->fileName);
	obj->setModel(model);

	ObjPhysics* physics = new ObjPhysics(objParams->x, objParams->y, objParams->z, objParams->width, objParams->height,
										 objParams->depth, objParams->mass, objParams->xVelo, objParams->yVelo, objParams->zVelo, objParams->hasGravity, objParams->colliderType);
	obj->setPhysics(physics);

	mtx.lock();
	obj->setIndex(mapIndex);
	objMap.insert(pair<int, Object*>(mapIndex, obj));
	mapIndex++;
	mtx.unlock();
}


void MainGame::cleanObject()
{
	map<int, Object*>::iterator itr = objMap.begin();
	while (itr != objMap.end())
	{
		delete (itr->second->model);
		delete (itr->second->physics);
		delete (itr->second);
		itr = objMap.erase(itr);
	}
}
