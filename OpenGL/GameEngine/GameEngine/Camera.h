#pragma once
#include <math.h>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
using namespace std;
class Camera
{
private:

public:
	Camera();

	void lockCamera();
	void moveCamera(float, float);
	void moveCameraUp(float, float);
	void control(float moveVel, float mouseVel, bool mi, SDL_Window *ptr_window);
	float camX, camY, camZ,camYaw,camPitch;
	bool mouseIn;
	void updateCamera();
	float PI = 3.1415926535;
};

