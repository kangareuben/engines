#include "MainGame.h"
#include "Errors.h"
#include "iostream"
#include "ObjLoader.h"
#include "boundingSphere.h"
#include "aabb.h"
#include "plane.h"
#include "physicsObject.h"
#include "physicsEngine.h"
using namespace std;

enum GameState { PLAY, EXIT };
PhysicsEngine physicsEngine;
GameState _gameState;
char ip[20];

MainGame::MainGame()
{
	ptr_window = NULL;
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
	_mouseVel = 0.5f;
	_moveVel = 0.5f;
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
	//glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Lighting test
	ambient1 = new GLfloat[4];
	ambient1[0] = 0.2;
	ambient1[1] = 0.5;
	ambient1[2] = 0.3;
	ambient1[3] = 1.0;
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	GLfloat diffuse1[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	GLfloat specular1[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	/*GLfloat position1[] = { 0, 50, 0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	GLfloat direction1[] = { 0, -1, 0 };
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
	GLfloat cutoff1 = 10;
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, &cutoff1);*/
	glEnable(GL_LIGHT1);

	GLfloat ambient2[] = { 0.5, 0.3, 0.2, 1.0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	//glEnable(GL_LIGHT2);

	GLfloat ambient3[] = { 0.2, 0.3, 0.5, 1.0 };
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);

	GLfloat ambient4[] = { 0.5, 0.5, 0.2, 1.0 };
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambient4);

	GLfloat ambient5[] = { 0.5, 0.2, 0.5, 1.0 };
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambient5);

	GLfloat ambient6[] = { 0.8, 0.2, 0.4, 1.0 };
	glLightfv(GL_LIGHT6, GL_AMBIENT, ambient6);

	//Set up OpenAL
	ALFWInit();
	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
	}

	audioManager = new AudioManager();

    //Set up AntTweakBar
    TwInit(TW_OPENGL, NULL);
	TwWindowSize(320, 160);
	
	void TW_CALL toggleLight1(void* clientData);
	void TW_CALL toggleLight2(void* clientData);
	void TW_CALL toggleLight3(void* clientData);

	twBar = TwNewBar("Tweak Color");
	TwAddButton(twBar, "Run", toggleLight1, NULL, "label='Color 1'"); 
	TwAddButton(twBar, "Run1", toggleLight2, NULL, "label='Color 2'");
	TwAddButton(twBar, "Run2", toggleLight3, NULL, "label='Color 3'");

	TwAddVarRW(twBar, "Width", TW_TYPE_INT32, &_windowWidth, "label='wndWidth'");
	TwAddVarRW(twBar, "Height", TW_TYPE_INT32, &_windowHeight, "label='wndheight'")
	if(isOnline)
        net = new Network(ip);
}

void TW_CALL toggleLight1(void* clientData)
{
	if (glIsEnabled(GL_LIGHT1))
	{
		glDisable(GL_LIGHT1);
	}
	else
	{
		glEnable(GL_LIGHT1);
	}
}

void TW_CALL toggleLight2(void* clientData)
{
	if (glIsEnabled(GL_LIGHT2))
	{
		glDisable(GL_LIGHT2);
	}
	else
	{
		glEnable(GL_LIGHT2);
	}
}

void TW_CALL toggleLight3(void* clientData)
{
	if (glIsEnabled(GL_LIGHT3))
	{
		glDisable(GL_LIGHT3);
	}
	else
	{
		glEnable(GL_LIGHT3);
	}
}
void MainGame::run()
{
	initSystems();
	obj.load("Models/Docahedron.obj");
	obj2.load("Models/Docahedron.obj");
	objTerrain.load("Models/Terrain.obj");

//	BoundingSphere sphere1(Vector3f(10.0f,0.0f,0.0f),1.0f);
 //   BoundingSphere sphere2(Vector3f(0.0f,3.0f,0.0f),1.0f);
//	BoundingSphere sphere3(Vector3f(0.0f,0.0f,2.0f),1.0f);
//	BoundingSphere sphere4(Vector3f(1.0f,0.0f,0.0f),1.0f);
/*
	IntersectData sphereIntersectSphere2 = sphere1.IntersectBoundingSphere(sphere2);
    IntersectData sphereIntersectSphere3 = sphere1.IntersectBoundingSphere(sphere3);
	IntersectData sphereIntersectSphere4 = sphere1.IntersectBoundingSphere(sphere4);

	cout<<"Sphere 1 intersects sphere2: "<< sphereIntersectSphere2.GetDoesIntersect()<<sphereIntersectSphere2.GetDistance()<<endl;
    cout<<"Sphere 1 intersects sphere3: "<< sphereIntersectSphere3.GetDoesIntersect()<<sphereIntersectSphere3.GetDistance()<<endl;
	cout<<"Sphere 1 intersects sphere4: "<< sphereIntersectSphere4.GetDoesIntersect()<<sphereIntersectSphere4.GetDistance()<<endl;


/*
    AABB aabb1(Vector3d(0.0,0.0,0.0),Vector3d(1.0,1.0,1.0));
    AABB aabb2(Vector3d(1.0,1.0,1.0),Vector3d(2.0,2.0,2.0));
    AABB aabb3(Vector3d(1.0,0.0,0.0),Vector3d(2.0,1.0,1.0));
    AABB aabb4(Vector3d(0.0,0.0,-2.0),Vector3d(1.0,1.0,-1.0));
    AABB aabb5(Vector3d(0.0,0.5,0.0),Vector3d(1.0,1.5,1.0));

    IntersectData aabb1Intersectaabb2 = aabb1.IntersectAABB(aabb2);
    IntersectData aabb1Intersectaabb3 = aabb1.IntersectAABB(aabb3);
    IntersectData aabb1Intersectaabb4 = aabb1.IntersectAABB(aabb4);
    IntersectData aabb1Intersectaabb5 = aabb1.IntersectAABB(aabb5);


    cout<<"AABB1 intersect AABB2 : "<<aabb1Intersectaabb2.GetDoesIntersect()<<"Distance:"<<aabb1Intersectaabb2.GetDistance()<<endl;
    cout<<"AABB1 intersect AABB3 : "<<aabb1Intersectaabb3.GetDoesIntersect()<<"Distance:"<<aabb1Intersectaabb3.GetDistance()<<endl;
    cout<<"AABB1 intersect AABB4 : "<<aabb1Intersectaabb4.GetDoesIntersect()<<"Distance:"<<aabb1Intersectaabb4.GetDistance()<<endl;
    cout<<"AABB1 intersect AABB5 : "<<aabb1Intersectaabb5.GetDoesIntersect()<<"Distance:"<<aabb1Intersectaabb5.GetDistance()<<endl;

*/
  /*  Plane plane1(Vector3d(0.0,1.0,0.0),0.0);

    IntersectData plane1IntersectSphere1 = plane1.IntersectSphere(sphere1);
    IntersectData plane1IntersectSphere2 = plane1.IntersectSphere(sphere2);
    IntersectData plane1IntersectSphere3 = plane1.IntersectSphere(sphere3);
    IntersectData plane1IntersectSphere4 = plane1.IntersectSphere(sphere4);

    cout<<"Plane1 intersect Sphere1:" <<plane1IntersectSphere1.GetDoesIntersect()<<"Distance"<<plane1IntersectSphere1.GetDistance()<<endl;
    cout<<"Plane1 intersect Sphere2:" <<plane1IntersectSphere2.GetDoesIntersect()<<"Distance"<<plane1IntersectSphere2.GetDistance()<<endl;
    cout<<"Plane1 intersect Sphere3:" <<plane1IntersectSphere3.GetDoesIntersect()<<"Distance"<<plane1IntersectSphere3.GetDistance()<<endl;
    cout<<"Plane1 intersect Sphere4:" <<plane1IntersectSphere4.GetDoesIntersect()<<"Distance"<<plane1IntersectSphere4.GetDistance()<<endl;

*/
	//Terrain
	//physicsEngine.AddObject(PhysicsObject(new BoundingSphere(Vector3f(0, 0, 0), 0), Vector3f(0, 0, 0)));
	//start
	physicsEngine.AddObject(PhysicsObject(new BoundingSphere(Vector3f(5, 1, 5),1),Vector3f(0,0,0)));
	//end
	physicsEngine.AddObject(PhysicsObject(new BoundingSphere(Vector3f(95, 1, 95),1),Vector3f(0,0,0)));
   
	objAI.PathFinding(9, 9);
	gameLoop();
}

void MainGame::gameLoop()
{
	//Will loop until we set _gameState to EXIT
	while (_gameState != EXIT)
	{
		float _startTicks = SDL_GetTicks();
		processInput();
		//AI change speed
		if (objAI.m_nStep <= objAI.m_nPointer)
		{
			int delx = objAI.m_delta[objAI.m_Path[objAI.m_nStep]][0];
			int delz = objAI.m_delta[objAI.m_Path[objAI.m_nStep]][1];
			physicsEngine.GetObject(0).GetVelocity().SetX(delx*10);
			physicsEngine.GetObject(0).GetVelocity().SetZ(delz*10);

			int x = physicsEngine.GetObject(0).GetPosition().GetX();
			int y = physicsEngine.GetObject(0).GetPosition().GetY();
			int z = physicsEngine.GetObject(0).GetPosition().GetZ();

			if(x>(objAI.m_nCurrentPosX+delx)*UNIT_LENGTH && x<(objAI.m_nCurrentPosX + delx+1)*UNIT_LENGTH)
				if (z>(objAI.m_nCurrentPosZ + delz)*UNIT_LENGTH && z < (objAI.m_nCurrentPosZ + delz + 1)*UNIT_LENGTH)
				{
					objAI.m_nCurrentPosX += delx;
					objAI.m_nCurrentPosZ += delz;
					objAI.m_nStep++;
				}
		}
		else
		{
			physicsEngine.GetObject(0).GetVelocity().SetX(0);
			physicsEngine.GetObject(0).GetVelocity().SetZ(0);
		}

		draw();

		if(isOnline)
		{
            net->Send(&mainCam);
        }
		calculateFPS();
		//print only once every 10 frames
		static int _frameCounter = 0;
		_frameCounter++;
		if (_frameCounter == 10)
		{
			//	cout << _fps << endl;
			_frameCounter = 0;
		}

		float _frameTicks = SDL_GetTicks() - _startTicks;

		//limit FPS to max FPS
		if (1000.0f / _maxFPS > _frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - _frameTicks);
		}

		for (int i = 0; i < 3; i++)
		{
			//ambient1[i] += (((float)rand()) / RAND_MAX * 2 - 1) / 5;
			ambient1[i] += ((float)rand()) / RAND_MAX * .02;
			if (ambient1[i] >= 1.0)
			{
				ambient1[i] = 0.0;
			}
		}
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	}

	//Clean up OpenAL
	audioManager->Cleanup();

	ALFWShutdownOpenAL();
	ALFWShutdown();
	delete audioManager;

	//Clean up lighting
	delete[] ambient1;
}



void MainGame::processInput()
{
	SDL_Event evnt;
    int handled;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt))
	{
        //send event to ant tweak bar
		handled = TwEventSDL(&evnt, SDL_MAJOR_VERSION, SDL_MINOR_VERSION);

        //if event is not handled by AntTweakBar
        if (!handled)
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
                    
                //Press q to play a sound
                case SDLK_q:
                    ALfloat position[3] = { 500000, 500000, 500000 };
                    ALfloat velocity[3] = { 0, 0, 0 };
                    ALfloat orientation[6] = { 1, 0, 0, 0, 1, 0 };
                    audioManager->Play("thud.wav", position, velocity, orientation);
                    break;

                }
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

	//Draw a grid
	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < 11; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(i*UNIT_LENGTH, 1.0, 0.0);
		glVertex3f(i*UNIT_LENGTH, 1.0, 100);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(0, 1.0, i*UNIT_LENGTH);
		glVertex3f(100, 1.0, i*UNIT_LENGTH);
		glEnd();

	}
	//Start point
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 1, UNIT_LENGTH);
	glVertex3f(UNIT_LENGTH, 1 , UNIT_LENGTH);
	glVertex3f(UNIT_LENGTH, 1, 0);
	glEnd();

	//Draw the map
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			if (objAI.m_map[i][j])
			{
				glBegin(GL_QUADS);
				glVertex3f(i*UNIT_LENGTH, 1, j*UNIT_LENGTH);
				glVertex3f(i*UNIT_LENGTH, 1, (j+1)*UNIT_LENGTH);
				glVertex3f((i+1)*UNIT_LENGTH, 1, (j + 1)*UNIT_LENGTH);
				glVertex3f((i+1)*UNIT_LENGTH, 1, j*UNIT_LENGTH);
				glEnd();
			}
	}
	
	//end point
	glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(100, 1, 100);
	glVertex3f(90, 1, 100);
	glVertex3f(90, 1, 90);
	glVertex3f(100, 1, 90);
	glEnd();


	
	//Set color back
	glColor3f(1, 1, 1);

	glPushMatrix();
	glTranslatef(0, 0, 0);
	objTerrain.Draw();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(physicsEngine.GetObject(0).GetPosition().GetX(), physicsEngine.GetObject(0).GetPosition().GetY(), physicsEngine.GetObject(0).GetPosition().GetZ());
	obj.Draw();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(physicsEngine.GetObject(1).GetPosition().GetX(), physicsEngine.GetObject(1).GetPosition().GetY(), physicsEngine.GetObject(1).GetPosition().GetZ());
	obj2.Draw();
	glPopMatrix();
    physicsEngine.Simulate(0.02);
    physicsEngine.HandleCollisions();
	/*glPushMatrix();
	glTranslatef(5, 5, 0);
	obj3.Draw();
	glPopMatrix();*/
    TwDraw();

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
