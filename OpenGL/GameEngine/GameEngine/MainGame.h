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
	void loadObjects();
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
	void toggleLight(int);
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
	bool sceneLoaded = false;

#pragma region Lighting UI Variables
	float lightR1 = 1.0f;
	float lightR2 = 1.0f;
	float lightR3 = 1.0f;
	float lightR4 = 1.0f;
	float lightR5 = 1.0f;
	float lightR6 = 1.0f;
	float lightR7 = 1.0f;
	float lightG1 = 1.0f;
	float lightG2 = 1.0f;
	float lightG3 = 1.0f;
	float lightG4 = 1.0f;
	float lightG5 = 1.0f;
	float lightG6 = 1.0f;
	float lightG7 = 1.0f;
	float lightB1 = 1.0f;
	float lightB2 = 1.0f;
	float lightB3 = 1.0f;
	float lightB4 = 1.0f;
	float lightB5 = 1.0f;
	float lightB6 = 1.0f;
	float lightB7 = 1.0f;
	bool lightOn1 = true;
	bool lightOn2 = false;
	bool lightOn3 = false;
	bool lightOn4 = false;
	bool lightOn5 = false;
	bool lightOn6 = false;
	bool lightOn7 = false;
	bool prevLightOn1 = true;
	bool prevLightOn2 = false;
	bool prevLightOn3 = false;
	bool prevLightOn4 = false;
	bool prevLightOn5 = false;
	bool prevLightOn6 = false;
	bool prevLightOn7 = false;
	int curLight = 1;
#pragma endregion
};
