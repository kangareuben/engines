#include "vector3d.h"
#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H
#include <stdio.h>
#include <xmmintrin.h>

class collisionsphere{
	public:
	double r;
	vector3d center;
	vector3d velocity;
	vector3d accel;
	collisionsphere(vector3d c,double rad,vector3d vel);
	collisionsphere();
	double getX();
	double getY();
	double getZ();
	void simulate();
	void setValues(vector3d pos, double rad, vector3d vel);
	void simdAddition(double *pArray1,double *pArray2);
    bool k1;
	struct State
	{
        vector3d pos;
        vector3d vel;
	}previous,current;

    vector3d acceleration(State &state, double t);
    void integrate(double t,double dt);

	struct Derivative
	{
        vector3d dPos;
        vector3d dVel;
	};

	Derivative evaluate(State &initial, double t,double dt,  Derivative &d);
    Derivative evaluate(State &initial, double t);
    void render();
    void setVelocity();
};

#endif
