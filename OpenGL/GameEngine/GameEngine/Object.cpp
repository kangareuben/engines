#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::setModel(ObjLoader* p_model)
{
	model = p_model;
}

void Object::setPhysics(ObjPhysics* p_physics)
{
	physics = p_physics;
}

void Object::setIndex(int nIndex)
{
	objIndex = nIndex;
}
void Object::Draw(ObjPhysics* pPhysics)
{
	model->Draw(pPhysics, objIndex);
}

