#include "collisionplane.h"

collisionplane::collisionplane(double a,double b, double c,    double d, double e, double f,   double g, double h, double i,double j,double k,double l)
{
	p[0].change(a,b,c);
	p[1].change(d,e,f);
	p[2].change(g,h,i);
	p[3].change(j,k,l);

	vector3d vec1 = p[1] - p[0];
    vector3d vec2 = p[2] - p[1];
    normal = vec2.crossproduct(vec1);
    normal.normalize();
}

vector3d collisionplane::get1point()
{
	return p[0];
}
vector3d collisionplane::get2point()
{
	return p[1];
}

vector3d collisionplane::get3point()
{
	return p[2];
}

vector3d collisionplane::get4point()
{
	return p[3];
}

vector3d collisionplane::getnormal()
{
	return normal;
}

bool collisionplane::isok()
{
	return (normal.x!=0 && normal.y!=0 && normal.z!=0);
}
