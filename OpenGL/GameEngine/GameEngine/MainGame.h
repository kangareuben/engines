#pragma once
#include <SDL.h>
#include <glew.h>
#include "Camera.h"
#include "Object.h"
#include <time.h>
#include <ctime>
#include <algorithm>
#include <vector>
#include <map>
#include <boost/thread/thread.hpp>
enum class GameState { PLAY, EXIT };

struct ObjParams
{
	char* fileName;
	float x;
	float y;
	float z;
	float width;
	float height;
	float depth;
	float mass;
	float xVelo;
	float yVelo;
	float zVelo;
	bool hasGravity;
	int colliderType;
	void setObjParams(char* _fileName, float _x, float _y, float _z, float _width, float _height, float _depth,
					  float _mass, float _xVelo, float _yVelo, float _zVelo, bool _hasGravity, int _colliderType)
	{
		fileName = _fileName;
		x = _x;
		y = _y;
		z = _z;
		xVelo = _xVelo;
		yVelo = _yVelo;
		zVelo = _zVelo;
		width = _width;
		height = _height;
		depth = _depth;
		mass = _mass;
		hasGravity = _hasGravity;
		colliderType = _colliderType;
	}
};

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
	bool gravityEnabled;
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
	void createObject(ObjParams*/*char*, float, float, float, float, float, float, float, bool, int*/);
	//before gameloop return, clean the ObjMap
	void cleanObject();
	
	//loader thread
	bool	bIsLoaded;
	int		nLoadThreadCount;
	boost::mutex mtx;
	boost::mutex mtx2;
	boost::mutex mtx3;

private:
	SDL_Window *ptr_window;
	bool mouseIn;
	float _fps;
	float _frameTime;
	float _maxFPS;
	float _moveVel;
	float _mouseVel;
};
