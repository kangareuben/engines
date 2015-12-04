#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include "ObjLoader.h"
#include "Camera.h"
//#include "MainGame.h"
#include "Network.h"
#include "ALFramework.h"
#include "AudioManager.h"
#include <AntTweakBar.h>
#include "ObjAI.h"

#define UNIT_LENGTH 10
class MainGame
{
public:
	MainGame();
	~MainGame();
	Network *net;
	bool isOnline;
	void initSystems();
	void gameLoop();
	void draw();
	void run();
	void processInput();
	void mouseLook(int, int);
	ObjLoader obj;
	ObjectAI objAI;
	ObjLoader obj2;
	ObjLoader objTerrain;
	Camera mainCam;
	ObjLoader cam;
	float _xDist;
	float _yDist;
	float _zDist;
	float _xRot;
	float _yRot;
	float _zRot;
	float _eyeX;
	int _windowHeight;
	int _windowWidth;
	void calculateFPS();
	AudioManager* audioManager;

private:
	SDL_Window *ptr_window;
	bool mouseIn;
	float _fps;
	float _frameTime;
	float _maxFPS;
	float _moveVel;
	float _mouseVel;
	GLfloat* ambient1;
    TwBar* twBar;
};
