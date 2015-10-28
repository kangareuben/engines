#pragma once
class Object;
class ObjPhysics;
#include <fstream>
#include "Errors.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <map>
using namespace std;

class ObjLoader
{
public:
	ObjLoader();
	float *calculateNormal(float *ptr_coord1, float *ptr_coord2, float *ptr_coord3);
	int load(char *ptr_fileName);
	void Draw(ObjPhysics*, int);
	void Release();
	
	float *ptr_normals;
	float *ptr_facesTriangles;
	float *ptr_vertexBuffer;
	long _totalConnectedPoints;
	long _totalConnectedTriangles;
	bool fallMove;
};