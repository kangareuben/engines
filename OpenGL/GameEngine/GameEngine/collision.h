#include "vector3d.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#ifndef COLLISION_H
#define COLLISION_H
#include "collisionsphere.h"
#include "collisionplane.h"
#include "SDL2/SDL.h"
class collision{
	public:
	 bool sphereplane(collisionplane *pl, collisionsphere *c);
	static bool raysphere(double xc,double yc,double zc,double xd,double yd,double zd,double xs,double ys,double zs,double r,double* dist=NULL,vector3d* collpoint=NULL);
	static bool rayplane(const double& nx,double ny,double nz,double x0,double y0,double z0,double xs,double ys,double zs,double xd,double yd,double zd,vector3d p1,vector3d p2,vector3d p3,vector3d p4,double* dis=NULL,vector3d* point=NULL);
	static double rayplanedist(double nx,double ny,double nz,double x0,double y0,double z0,double xs,double ys,double zs,double xd,double yd,double zd);
	static double trianglearea(vector3d p1,vector3d p2,vector3d p3);
	static bool spheresphere(collisionsphere *c1,collisionsphere *c2);
    static void spheresphereResponse(collisionsphere *c1,collisionsphere *c2);

	static double pointdistacesquare(vector3d p1,vector3d p2);
	static double pointdistace(vector3d p1,vector3d p2);
    static vector3d projectUonV(vector3d u, vector3d v);
    static vector3d scale(vector3d v, double a);
};


#endif
