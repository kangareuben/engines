#pragma once
#include "ObjLoader.h"
#include "ObjPhysics.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;
class Object
{
public:
	Object();
	~Object();
	void setModel(ObjLoader*);
	void setPhysics(ObjPhysics*);
	void setIndex(int nIndex);
	void Draw(ObjPhysics*);

	int objIndex;
	ObjLoader* model;
	ObjPhysics* physics;
};

