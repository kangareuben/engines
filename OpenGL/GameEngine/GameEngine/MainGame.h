#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "objLoader.h"
#include "Camera.h"
#include "MainGame.h"
#include "Network.h"
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
	ObjLoader obj2;
	ObjLoader obj3;
	ObjLoader objList[50];
	int _obn1;
	int _obn2;
	int _obn3;
	int _obn[50];
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

private:
	SDL_Window *ptr_window;
	bool mouseIn;
	float _fps;
	float _frameTime;
	float _maxFPS;
	float _moveVel;
	float _mouseVel;
};
