#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "ObjLoader.h"
#include "Camera.h"
#include <time.h>
#include <ctime>
#include <algorithm>
enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();
	void initSystems();
	void gameLoop();
	void draw();
	void run();
	void processInput();
	void mouseLook(int, int);
	GameState _gameState;
	double currentTime;
	ObjLoader obj;
	bool checkCollision(ObjLoader*,ObjLoader*);
	ObjLoader obj2;
	ObjLoader obj3;
	ObjLoader box1;
	ObjLoader box2;
	ObjLoader box3;
	ObjLoader particles[500];
	ObjLoader player;
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
