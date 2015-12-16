#pragma once
#include <SDL.h>
#include <glew.h>
#include "ObjLoader.h"
#include "Camera.h"
#include "Network.h"
#include "ALFramework.h"
#include "AudioManager.h"
#include "imgui.h"
#include "ImGui_impl_Sdl.h"
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
	ObjLoader objList[100];
	int _obn1;
	int _obn2;
	int _obn3;
	int _obn[100];
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
	bool show_test_window;
	bool show_another_window;
	float lightR, lightG, lightB;
};
