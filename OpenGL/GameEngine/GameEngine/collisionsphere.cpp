#include "collisionsphere.h"

collisionsphere::collisionsphere()
{
	center.x = center.y = center.z = 0;
	velocity.x = velocity.y = velocity.z = 0;
	accel.x = accel.z = 0;
	accel.y = -0.01;
	r = 0;
	current.pos = center;
	current.vel = velocity;
	previous.pos = center;
	previous.vel = velocity;
	previous = current;
	k1 = true;
}

collisionsphere::collisionsphere(vector3d c, double r2, vector3d vel)
{
	center = c;
	k1 = true;
	r = r2;
	velocity = vel;
	current.pos = center;
	current.vel = velocity;
	previous.pos = center;
	previous.vel = velocity;
	previous = current;

}

void collisionsphere::setValues(vector3d c, double r2, vector3d vel)
{
	center = c;
	r = r2;
	velocity = vel;
	current.pos = center;
	current.vel = velocity;
	previous.pos = center;
	previous.vel = velocity;
	previous = current;

}


void collisionsphere::simulate()
{

	center += velocity;
	velocity.y -= accel.y;
}



collisionsphere::Derivative collisionsphere::evaluate(State &initial,
	double t,
	double dt,
	Derivative &d)
{



	State state;
	double pArray1[3] = { initial.pos.x, initial.pos.y, initial.pos.z };
	double pArray2[3] = { d.dPos.x * dt, d.dPos.y * dt, d.dPos.z *dt };
	//using simd instead of the below addition
	//state.pos = initial.pos + d.dPos*dt;
	// simdAddition(pArray1,pArray2);
	state.pos.x = pArray1[0]; state.pos.y = pArray1[1]; state.pos.z = pArray1[2];
	double pArray3[3] = { velocity.x, velocity.y, velocity.z };
	double pArray4[3] = { d.dVel.x * dt, d.dVel.y * dt, d.dVel.z *dt };
	//using simd instead of the below addition
	// state.vel = initial.vel + d.dVel*dt;
	simdAddition(pArray3, pArray4);
	velocity.x = pArray3[0]; velocity.y = pArray3[1]; velocity.z = pArray3[2];


	Derivative output;
	output.dPos = velocity;//state.vel;
	output.dVel = acceleration(state, dt);
	return output;
}

void collisionsphere::simdAddition(double *pArray1, double *pArray2)
{
	__m128d X = _mm_loadu_pd(pArray1);
	__m128d Y = _mm_loadu_pd(pArray2);
	__m128d result = _mm_add_pd(Y, X);
	_mm_storeu_pd(pArray1, result);
}

vector3d collisionsphere::acceleration(State &state, double t)
{
	const double k = 0.01;
	const double b = 0;
	vector3d gravity;
	gravity.x = gravity.z = 0;
	gravity.y = 0.00f;
	state.vel = velocity;
	// return  state.pos * -k - state.vel * b;
	return gravity;
}

void collisionsphere::integrate(double t, double dt)
{


	previous = current;
	if (k1)
	{
		std::cout << velocity << std::endl;
		k1 = false;
	}
	Derivative a, b, c, d;

	a = evaluate(current, t);
	b = evaluate(current, t, dt*0.5f, a);
	c = evaluate(current, t, dt*0.5f, b);
	d = evaluate(current, t, dt, c);

	double temp = 1.0f / 6.0f;

	vector3d dPosdt = (a.dPos + (b.dPos + c.dPos) * 2.0 + d.dPos) * temp;

	vector3d dVeldt = (a.dVel + (b.dVel + c.dVel) * 2.0 + d.dVel)* temp;

	current.pos = current.pos + dPosdt * dt;
	// current.vel = current.vel + dVeldt * dt;
	velocity = velocity + dVeldt*dt;

}

collisionsphere::Derivative collisionsphere::evaluate(State &state, double t)
{
	Derivative output;
	output.dPos = velocity;//state.vel;
	return output;
}

void collisionsphere::render()
{
	double alpha = 1.0f;
	State state;
	state.pos = previous.pos*(1 - alpha) + current.pos*alpha;
	center = state.pos;
}


double collisionsphere::getX()
{
	return center.x;
}

double collisionsphere::getY()
{
	return center.y;
}

double collisionsphere::getZ()
{
	return center.z;
}