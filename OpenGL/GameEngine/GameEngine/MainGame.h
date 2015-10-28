#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Camera.h"
#include "Object.h"
#include <time.h>
#include <ctime>
#include <algorithm>
#include <vector>
#include <map>
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
	bool checkCollision(ObjPhysics*,ObjPhysics*);

	map<int, Object*> objMap;
	int mapIndex;

	//Object obj;
	//Object obj2;
	//Object obj3;
	//Object box1;
	//Object box2;
	//Object box3;
	//Object particles[500];

	Object player;
	Camera mainCam;
	Object cam;
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
	void createObject(char*, float, float, float, float, float, float, float, bool, int);

private:
	SDL_Window *ptr_window;
	bool mouseIn;
	float _fps;
	float _frameTime;
	float _maxFPS;
	float _moveVel;
	float _mouseVel;
};
