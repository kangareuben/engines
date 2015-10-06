#include "Camera.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#ifndef M_PI
#define M_PI  3.14159
#endif
Camera::Camera()
{
	camX = 0.0f;
	camY = 0.0f;
	camZ = 0.0f;
	bool mouseIn = true;
}

void Camera::lockCamera()
{
	if (camPitch > 90)
		camPitch = 90;

	if (camPitch < -90)
		camPitch = -90;

	if (camYaw < 0.0f)
		camYaw += 360.0f;

	if (camYaw > 360)
		camYaw -= 360.0f;
}


//moves the camera in w,s,a,d directions, the values are directly passed to gltranslatef
void Camera::moveCamera(float dist,float dir)
{
	float rad = (camYaw + dir) *M_PI / 180.0f;
	camX -= sin(rad)*dist;
	camZ -= cos(rad) * dist;
}


//move the camera using mouse
void Camera::moveCameraUp(float dist, float dir)
{
	float rad = (camPitch + dir) *M_PI / 180.0f;
	camY += sin(rad)*dist;
	
}

void Camera::control(float moveVel, float mouseVel, bool mi, SDL_Window *ptr_window)
{

	if (mi)
	{
		int midX = 320;
		int midY = 240;
		SDL_ShowCursor(SDL_DISABLE);
		int tmpX, tmpY;
		SDL_GetMouseState(&tmpX, &tmpY);
		camYaw += mouseVel*(midX - tmpX);
		camPitch += mouseVel*(midY - tmpY);
		lockCamera();
		SDL_WarpMouseInWindow(ptr_window,midX,midY);
		glRotatef(-camPitch, 1.0, 0.0, 0.0);
		glRotatef(-camYaw, 0.0, 1.0, 0.0);
	}
}

void Camera:: updateCamera()
{
	glTranslatef(-camX, -camY, -camZ);        //move the camera
}