#pragma once
#include <vector>

#include <time.h>
using namespace std;
class ObjLoader
{
public:
	ObjLoader();
	float *calculateNormal(float *ptr_coord1, float *ptr_coord2, float *ptr_coord3);
	int load(char *ptr_fileName,float,float,float,bool,int,float,float,float,float);
	void Draw();
	void Release();
	void update(float);
	float getX();
	float getY();
	float getZ();
	bool gravity;
	float colliderType;
	float *ptr_normals;
	float *ptr_facesTriangles;
	float *ptr_vertexBuffer;
	long _totalConnectedPoints;
	long _totalConnectedTriangles;
	float point1[3];
	float point2[3];
	float point3[3];
	float point4[3];
	float point5[3];
	float point6[3];
	float point7[3];
	float point8[3];
	float height;
	float width;
	bool fallMove;
	float depth;
	float mass;
	
	vector<float> position;
	vector<float> velocity;
//	float velocity;
	float lerp(float,float,float);
	float reverseSpeed();
	float gravityAcceleration;
	void setVelocity(float,float,float);
	
};