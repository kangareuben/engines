#pragma once
class Object;
#include <vector>
#include <map>
using namespace std;
class ObjPhysics
{
public:
	ObjPhysics();
	ObjPhysics(float, float, float, float, float, float, float, float, float, float, bool, int);
	~ObjPhysics();

	void update(float);

	float getX();
	float getY();
	float getZ();
	bool gravity;
	float colliderType;
	float mass;
	float height;
	float width;
	float depth;

	vector<float> position;
	vector<float> velocity;

	//	float velocity;
	float lerp(float, float, float);
	float reverseSpeed();
	float gravityAcceleration;
	void setVelocity(float, float, float);

	float point1[3];
	float point2[3];
	float point3[3];
	float point4[3];
	float point5[3];
	float point6[3];
	float point7[3];
	float point8[3];
};

