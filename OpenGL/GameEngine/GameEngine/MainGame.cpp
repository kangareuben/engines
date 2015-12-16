#include "MainGame.h"
#include "Errors.h"
#include "iostream"
#include "ObjLoader.h"
#include "boundingSphere.h"
#include "aabb.h"
#include "plane.h"
#include "physicsObject.h"
#include "physicsEngine.h"
#include "collision.h"
#include "collisionsphere.h"
#include "collisionplane.h"
#include <time.h>

enum GameState { PLAY, EXIT };
PhysicsEngine physicsEngine;
GameState _gameState;
collision col;

bool k1 = true;
double t = 0.0;
double dt = 0.01;
int iSecret;
double currentTime = 0.0;
double accumulator = 0.0;
int bSize = 100;
int numBalls = 45;
//Position, Radius, Velocity

collisionsphere ob[50];
collisionplane pLower(0, 0, 0, bSize, 0, 0, bSize, 0, bSize, 0, 0, bSize);
collisionplane pUpper(0, bSize, 0, bSize, bSize, 0, bSize, bSize, bSize, 0, bSize, bSize);

collisionplane pLeft(0, 0, 0, 0, 0, bSize, 0, bSize, bSize, 0, bSize, 0);
collisionplane pRight(bSize, 0, 0, bSize, 0, bSize, bSize, bSize, bSize, bSize, bSize, 0);

collisionplane pFront(0, 0, 0, bSize, 0, 0, bSize, bSize, 0, 0, bSize, 0);
collisionplane pBack(0, 0, bSize, bSize, 0, bSize, bSize, bSize, bSize, 0, bSize, bSize);

using namespace std;
char ip[20];

MainGame::MainGame()
{
	ptr_window = NULL;
	_windowWidth = 640;
	_windowHeight = 640;
	_xDist = 0;
	_yDist = 0;
	_zDist = 0;
	_yRot = 0;
	_zRot = 0;
	_xRot = 0;
	_eyeX = 0;
	_maxFPS = 60;
	_mouseVel = 0.5f;
	_moveVel = 0.2f;
	isOnline = true;

	if (isOnline)
	{
		std::cin.getline(ip, 20);
	}
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
	if (ptr_window == NULL)
	{
		fatalError("SDL Window could not be created!");
	}

	//Set up our OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(ptr_window);
	if (glContext == NULL)
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
	//glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, amb_light);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	//Lighting test
	ambient1 = new GLfloat[4];

	//Disco lighting
	/*ambient1[0] = 0.2;
	ambient1[1] = 0.5;
	ambient1[2] = 0.3;
	ambient1[3] = 1.0;*/

	ambient1[0] = 0.0;
	ambient1[1] = 0.0;
	ambient1[2] = 0.0;
	ambient1[3] = 1.0;

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	GLfloat diffuse1[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	GLfloat specular1[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	/*GLfloat position1[] = { -50, 50, 0, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	GLfloat direction1[] = { 0, -1, 0 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
	GLfloat exponent1 = 128;
	glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, &exponent1);
	GLfloat cutoff1 = 5;
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, &cutoff1);*/
	//glEnable(GL_LIGHT1);

	GLfloat ambient2[] = { 1.0, 0.55, 0.0, 1.0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);

	GLfloat ambient3[] = { 1.0, 1.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);

	GLfloat ambient4[] = { 0.133, 0.545, 0.133, 1.0 };
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambient4);

	GLfloat ambient5[] = { 0.0, 0.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambient5);

	GLfloat ambient6[] = { 0.294, 0.0, 0.51, 1.0 };
	glLightfv(GL_LIGHT6, GL_AMBIENT, ambient6);

	GLfloat ambient7[] = { 0.58, 0.0, 0.827, 1.0 };
	glLightfv(GL_LIGHT7, GL_AMBIENT, ambient7);

	//Set up OpenAL
	ALFWInit();
	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
	}

	audioManager = new AudioManager();

//	if (isOnline)
	//	net = new Network(ip);

	glRotatef(0, 1.0, 0.0, 0.0);
	glRotatef(-180, 0.0, 1.0, 0.0);
	glRotatef(-180, 0, 0, 1);

	ImGui_ImplSdl_Init(ptr_window);
	show_test_window = false;
	show_another_window = false;
}

void MainGame::run()
{
	initSystems();
	//_obn1 = obj.load("Models/sphere.obj");
	//_obn2 = obj2.load("Models/sphere.obj");

	for (int i = 0; i<numBalls; i++)
	{
		_obn[i] = objList[i].load("Models/sphere.obj");
		//_obn[i] = objList[i].load("Models/capsule.obj");
	}
	srand(time(NULL));
	float posRandX;
	float posRandY;
	float posRandZ;
	float velRandX;
	float velRandY;
	float velRandZ;
	posRandX = 2;
	posRandY = 2;


	for (int i = 0; i<numBalls; i++)
	{
		//    posRandX = rand() % bSize + 1  ;
		//   posRandY = rand() % bSize  +1 ;
		posRandZ = rand() % bSize + 1;
		velRandX = rand() % 30 + 1;
		velRandY = rand() % 30 + 1;
		velRandZ = rand() % 10 + 1;
		// cout<<posRandZ<<endl;
		ob[i].setValues(vector3d(posRandX, posRandY, 2), 0.9, vector3d(velRandX, velRandY, 0));
		posRandX += 1.5;
		posRandY += 1.5;
		//cout<<"X:"<<posRandX<<endl;
	}

	gameLoop();
}

void MainGame::gameLoop()
{
	//Will loop until we set _gameState to EXIT
	while (_gameState != EXIT)
	{
		processInput();
		draw();

		//if (isOnline)
	//	{
	//		net->Send(&mainCam);
	//	}

		double newTime = SDL_GetTicks();
		double frameTime = newTime - currentTime;
		if (frameTime > 0.25)
			frameTime = 0.25;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			for (int i = 0; i<numBalls; i++)
			{
				ob[i].integrate(t, dt);
				t += dt;
				accumulator -= dt;
			}
		}

		//Disco lighting
		/*for (int i = 0; i < 3; i++)
		{
			//ambient1[i] += (((float)rand()) / RAND_MAX * 2 - 1) / 5;
			ambient1[i] += ((float)rand()) / RAND_MAX * .02;
			if (ambient1[i] >= 1.0)
			{
				ambient1[i] = 0.0;
			}
		}*/
		ambient1[0] = lightR;
		ambient1[1] = lightG;
		ambient1[2] = lightB;
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	}

	//Clean up OpenAL
	audioManager->Cleanup();

	ALFWShutdownOpenAL();
	ALFWShutdown();
	delete audioManager;

	//Clean up lighting
	delete[] ambient1;

	//Clean up imgui
	ImGui_ImplSdl_Shutdown();
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
			_gameState = EXIT;
			break;
		case SDL_MOUSEMOTION:
			//	std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;

		/*case SDL_MOUSEBUTTONDOWN:
			mouseIn = true;
			SDL_ShowCursor(SDL_DISABLE);
			break;*/



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
				_gameState = EXIT;
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

			//Press q to play a sound
			case SDLK_q:
			{
				ALfloat position[3] = { 0, 0, 0 };
				ALfloat velocity[3] = { 0, 0, 0 };
				ALfloat orientation[6] = { 1, 0, 0, 0, 1, 0 };
				audioManager->Play("thud.wav", position, velocity, orientation);
				break;
			}

			case SDLK_1:
				if (glIsEnabled(GL_LIGHT1))
				{
					glDisable(GL_LIGHT1);
				}
				else
				{
					glEnable(GL_LIGHT1);
				}
				break;

			case SDLK_2:
				if (glIsEnabled(GL_LIGHT2))
				{
					glDisable(GL_LIGHT2);
				}
				else
				{
					glEnable(GL_LIGHT2);
				}
				break;

			case SDLK_3:
				if (glIsEnabled(GL_LIGHT3))
				{
					glDisable(GL_LIGHT3);
				}
				else
				{
					glEnable(GL_LIGHT3);
				}
				break;

			case SDLK_4:
				if (glIsEnabled(GL_LIGHT4))
				{
					glDisable(GL_LIGHT4);
				}
				else
				{
					glEnable(GL_LIGHT4);
				}
				break;

			case SDLK_5:
				if (glIsEnabled(GL_LIGHT5))
				{
					glDisable(GL_LIGHT5);
				}
				else
				{
					glEnable(GL_LIGHT5);
				}
				break;

			case SDLK_6:
				if (glIsEnabled(GL_LIGHT6))
				{
					glDisable(GL_LIGHT6);
				}
				else
				{
					glEnable(GL_LIGHT6);
				}
				break;

			case SDLK_7:
				if (glIsEnabled(GL_LIGHT7))
				{
					glDisable(GL_LIGHT7);
				}
				else
				{
					glEnable(GL_LIGHT7);
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
	//gluLookAt(0, 1, 25, 0, 0, 0, 0, 1, 0);
	gluLookAt(50, 50, -150, 50, 50, 0, 0, 1, 0);
	glTranslatef(mainCam.camX*-1, mainCam.camY*-1, mainCam.camZ*-1);

	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 1.0, 0.0);

	glPointSize(5);
	glLineWidth(5);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.4, 0.0, 0.0);
	glVertex3f(100.4, 100.4, 0.0);
	glVertex3f(0.0, 100.4, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	/*glVertex3f(0.0, 0.0, 100.0);
	glVertex3f(100.0, 0.0, 100.0);
	glVertex3f(100.0, 0.0, 0.0);
	glVertex3f(100.0, 100.0, 0.0);
	glVertex3f(100.0, 100.0, 100.0);
	glVertex3f(0.0, 100.0, 100.0);
	glVertex3f(0.0, 0.0, 0.0);*/
	
	glColor3f(1.0, 1.0, 1.0);
	glEnd();

	/*glBegin(GL_QUADS);
	glVertex3f(0.0, 0.0, 2.0);
	glVertex3f(100.4, 0.0, 2.0);
	glVertex3f(100.4, 100.4, 2.0);
	glVertex3f(0.0, 100.4, 2.0);
	glVertex3f(0.0, 0.0, 2.0);
	glEnd();*/

	for (int i = 0; i<numBalls; i++)
	{
		glPushMatrix();
		glTranslatef(ob[i].getX(), ob[i].getY(), ob[i].getZ());
		ob[i].render();
		glCallList(_obn[i]);
		glPopMatrix();
	}

	for (int i = 0; i<numBalls; i++)
	{
		for (int j = i + 1; j<numBalls; j++)
		{
			if (col.spheresphere(&ob[i], &ob[j]))
			{
				col.spheresphereResponse(&ob[i], &ob[j]);
				audioManager->Play("ow.wav");
			}
		}
	}
	for (int i = 0; i<numBalls; i++)
	{
		if (col.sphereplane(&pUpper, &ob[i]))
		{
			//cout << "collision upper" << endl;
			//audioManager->Play("tick.wav");
		}
		else if (col.sphereplane(&pLower, &ob[i]))
		{
			//cout << "collision lower" << endl;
			//audioManager->Play("tick.wav");
		}

		else if (col.sphereplane(&pLeft, &ob[i]))
		{
			//cout << "collision left" << endl;
			//audioManager->Play("tick.wav");
		}


		else if (col.sphereplane(&pRight, &ob[i]))
		{
			//cout << "collision right" << endl;
			//audioManager->Play("tick.wav");
		}

		else if (col.sphereplane(&pFront, &ob[i]))
		{
			//cout << "collision front" << endl;
			//audioManager->Play("tick.wav");
		}

		else if (col.sphereplane(&pBack, &ob[i]))
		{
			//cout << "collision back" << endl;
			//audioManager->Play("tick.wav");
		}
	}

	ImGui_ImplSdl_NewFrame(ptr_window);

	{
		ImGui::Text("hello world");
		ImGui::SliderFloat("red", &lightR, 0.0f, 1.0f);
		ImGui::SliderFloat("green", &lightG, 0.0f, 1.0f);
		ImGui::SliderFloat("blue", &lightB, 0.0f, 1.0f);
		//ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("hello");
		ImGui::End();

	}

	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}

	ImGui::Render();

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



int main(int argc, char **argv)
{
	MainGame maingame;
	maingame.run();
	return 0;
}