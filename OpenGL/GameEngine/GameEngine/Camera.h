#pragma once
#include <math.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
using namespace std;
class Camera
{
private:

    int id;
public:
	Camera();
	void setId(int i);
	int getId();
    bool isready;
    bool isReady();
	void lockCamera();
	void moveCamera(float, float);
	void moveCameraUp(float, float);
	void control(float moveVel, float mouseVel, bool mi, SDL_Window *ptr_window);
	float camX, camY, camZ,camYaw,camPitch;
	bool mouseIn;
	void updateCamera();
	float PI = 3.1415926535;
};
