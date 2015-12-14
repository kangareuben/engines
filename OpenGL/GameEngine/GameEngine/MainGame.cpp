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
#include <AL/al.h>
#include <AL/alut.h>
enum GameState { PLAY, EXIT };
PhysicsEngine physicsEngine;
GameState _gameState;
collision col;


bool k1=true;
double t = 0.0;
double dt = 0.01;
int iSecret;
double currentTime = 0.0;
double accumulator = 0.0;
int bSize = 100;
int numBalls = 45;
//Position, Radius, Velocity

collisionsphere ob[50];
collisionplane pLower (0,0,0,bSize,0,0,bSize,0,bSize,0,0,bSize);
collisionplane pUpper(0,bSize,0,bSize,bSize,0,bSize,bSize,bSize,0,bSize,bSize);

collisionplane pLeft(0,0,0,0,0,bSize,0,bSize,bSize,0,bSize,0);
collisionplane pRight(bSize,0,0,bSize,0,bSize,bSize,bSize,bSize,bSize,bSize,0);

collisionplane pFront(0,0,0,bSize,0,0,bSize,bSize,0,0,bSize,0);
collisionplane pBack(0,0,bSize,bSize,0,bSize,bSize,bSize,bSize,0,bSize,bSize);


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

	if(isOnline)
	{
        std::cin.getline(ip,20);
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

	// Init openAL
	alutInit(0, NULL);
	// Clear Error Code (so we can catch any new errors)
	//alGetError();

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

	if(isOnline)
        net = new Network(ip);

}

void MainGame::run()
{
	initSystems();
	_obn1 = obj.load("Models/sphere.obj");
	_obn2 = obj2.load("Models/sphere.obj");

	for(int i=0;i<numBalls;i++)
	{
        _obn[i] = objList[i].load("Models/sphere.obj");
	}
    srand (time(NULL));
    float posRandX;
    float posRandY;
    float posRandZ;
    float velRandX;
    float velRandY;
    float velRandZ;
    posRandX = 2;
    posRandY = 2;


    for(int i=0;i<numBalls;i++)
    {
    //    posRandX = rand() % bSize + 1  ;
     //   posRandY = rand() % bSize  +1 ;
        posRandZ = rand() % bSize  +1 ;
        velRandX = rand() % 30 + 1;
        velRandY = rand() % 30 + 1;
        velRandZ = rand() % 10 + 1 ;
       // cout<<posRandZ<<endl;
        ob[i].setValues(vector3d( posRandX,posRandY,2),1.1,vector3d(velRandX,velRandY,0));
	posRandX +=1.5;
	posRandY +=1.5;
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

		if(isOnline)
		{
            net->Send(&mainCam);
        }




        double newTime = SDL_GetTicks();
        double frameTime = newTime - currentTime;
        if ( frameTime > 0.25 )
             frameTime = 0.25;
        currentTime = newTime;

        accumulator += frameTime;

        while ( accumulator >= dt )
        {
            for(int i=0;i<numBalls;i++)
            {
                ob[i].integrate(t,dt);
                t += dt;
                accumulator -= dt;
            }
        }





	}
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




    for(int i=0;i<numBalls;i++)
    {
        glPushMatrix();
        glTranslatef(ob[i].getX(),ob[i].getY(),ob[i].getZ());
        ob[i].render();
        glCallList(_obn[i]);
        glPopMatrix();
    }

    //cout<<"ob1: "<<ob[0].velocity<< " "<<ob[0].getZ()<<endl;

   for(int i=0;i<numBalls;i++)
   {
        for(int j=i+1;j<numBalls;j++)
        {
            if(col.spheresphere(&ob[i],&ob[j]))
            {
                col.spheresphereResponse(&ob[i],&ob[j]);
            }
        }
    }
    for(int i=0;i<numBalls;i++)
   {
        if(col.sphereplane(&pUpper,&ob[i]))
        {
            cout<<"collision upper"<<endl;
        }
        else if(col.sphereplane(&pLower,&ob[i]))
        {
            cout<<"collision lower"<<endl;
        }

        else if(col.sphereplane(&pLeft,&ob[i]))
        {
           cout<<"collision left"<<endl;
        }


        else if(col.sphereplane(&pRight,&ob[i]))
        {
            cout<<"collision right"<<endl;
        }

        else if(col.sphereplane(&pFront,&ob[i]))
        {
            cout<<"collision front"<<endl;
        }

        else if(col.sphereplane(&pBack,&ob[i]))
        {
            cout<<"collision back"<<endl;
        }
	
	

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



int main(int argc, char **argv)
{
    MainGame maingame;
    maingame.run();
    return 0;
}
