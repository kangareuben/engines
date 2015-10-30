#include "ObjPhysics.h"
#include "Object.h"

ObjPhysics::ObjPhysics()
{
	gravityAcceleration = 0.01f;
	position.push_back(0);
	position.push_back(0);
	position.push_back(0);

	velocity.push_back(0);
	velocity.push_back(0);
	velocity.push_back(0);
}

ObjPhysics::ObjPhysics(float p_x, float p_y, float p_z, float p_width, float p_height, float p_depth, float p_mass,
					   float p_xVelo, float p_yVelo, float p_zVelo, bool p_gravity, int p_colliderType)
{
	gravityAcceleration = 0.01f;
	position.push_back(0);
	position.push_back(0);
	position.push_back(0);

	velocity.push_back(0);
	velocity.push_back(0);
	velocity.push_back(0);

	colliderType = p_colliderType;
	mass = p_mass;
	position[0] = p_x;
	position[1] = p_y;
	position[2] = p_z;
	gravity = p_gravity;
	width = p_width;
	height = p_height;
	depth = p_depth;

	setVelocity(p_xVelo, p_yVelo, p_zVelo);

	/*point1[0] = position[0] - width / 2; point1[1] = position[1] - height / 2; point1[2] = position[2] - depth / 2;
	point2[0] = position[0] + width / 2; point2[1] = position[1] - height / 2; point2[2] = position[2] - depth / 2;
	point3[0] = position[0] - width / 2; point3[1] = position[1] + height / 2; point3[2] = position[2] - depth / 2;
	point4[0] = position[0] + width / 2; point4[1] = position[1] + height / 2; point4[2] = position[2] - depth / 2;
	point5[0] = position[0] - width / 2; point5[1] = position[1] - height / 2; point5[2] = position[2] + depth / 2;
	point6[0] = position[0] + width / 2; point6[1] = position[1] - height / 2; point6[2] = position[2] + depth / 2;
	point7[0] = position[0] - width / 2; point7[1] = position[1] + height / 2; point7[2] = position[2] + depth / 2;
	point8[0] = position[0] + width / 2; point8[1] = position[1] + height / 2; point8[2] = position[2] + depth / 2;*/
}


ObjPhysics::~ObjPhysics()
{
}

float ObjPhysics::getX()
{
	return position[0];
}

float ObjPhysics::getY()
{
	return position[1];
}

float ObjPhysics::getZ()
{
	return position[2];
}

void ObjPhysics::update(float dt)
{

	if (gravity == true && position[1] != 0)
	{

		velocity[1] -= gravityAcceleration * dt;
		position[1] = position[1] + velocity[1] * dt;
	}

	position[0] += velocity[0] * dt;
	position[1] += velocity[1] * dt;
	position[2] += velocity[2] * dt;
}


void ObjPhysics::setVelocity(float a, float b, float c)
{
	velocity[0] = a;
	velocity[1] = b;
	velocity[2] = c;
}
