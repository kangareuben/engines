#ifndef collisionplane_H
#define collisionplane_H
#include <iostream>
#include "vector3d.h"

class collisionplane{
	public:
        vector3d p[4];
        vector3d normal;
		collisionplane(double n1,double n2,double n3, double a,double b, double c,    double d, double e, double f,   double g, double h, double i);
		vector3d get1point();
		vector3d get2point();
		vector3d get3point();
		vector3d get4point();
		vector3d getnormal();
		bool isok();
		bool dir;
};
#endif
