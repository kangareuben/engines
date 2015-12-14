#include "collision.h"


bool collision::raysphere(double xc,double yc,double zc,double xd,double yd,double zd,double xs,double ys,double zs,double r,double* dist,vector3d* collpoint)
{
	double b=2*(xd*(xs-xc)+yd*(ys-yc)+zd*(zs-zc));
	double c=xs*xs-2*xs*xc+xc*xc+ys*ys-2*ys*yc+yc*yc+zs*zs-2*zs*zc+zc*zc-r*r;
	double disc=(b*b-4*c);
	if(disc<0)	//if the discriminant is less then 0, there is no intersection
		return false;
	else{
		if(dist!=NULL)
		{
			(*dist)=(-b+sqrt(disc))/2;
			if(collpoint!=NULL)
			{
				double x=xs+(*dist)*xd;
				double y=ys+(*dist)*yd;
				double z=zs+(*dist)*zd;
				collpoint->change(x,y,z);
			}
		}
		return true;
	}
}

double collision::trianglearea(vector3d p1,vector3d p2,vector3d p3)
{
	//area of the triangle with the heron fomula
	double a=sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y)+(p2.z-p1.z)*(p2.z-p1.z));
	double b=sqrt((p3.x-p2.x)*(p3.x-p2.x)+(p3.y-p2.y)*(p3.y-p2.y)+(p3.z-p2.z)*(p3.z-p2.z));
	double c=sqrt((p1.x-p3.x)*(p1.x-p3.x)+(p1.y-p3.y)*(p1.y-p3.y)+(p1.z-p3.z)*(p1.z-p3.z));
	double s=(a+b+c)/2.;
	return (sqrt(s*((s-a)*(s-b)*(s-c))));
}
	//distance of 2 point (with 3d pythagoras formula)
double collision::pointdistace(vector3d p1,vector3d p2)
{
	vector3d vec(p2.x-p1.x,p2.y-p1.y,p2.z-p1.z);
	return (sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z));
}

double collision::pointdistacesquare(vector3d p1,vector3d p2)
{
	vector3d vec(p2.x-p1.x,p2.y-p1.y,p2.z-p1.z);
	return (vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
}


bool collision::rayplane(const double& nx,double ny,double nz,double x0,double y0,double z0,double xs,double ys,double zs,double xd,double yd,double zd,vector3d p1,vector3d p2,vector3d p3,vector3d p4,double* dis,vector3d* point)
{
	if((xd*nx+yd*ny+zd*nz)==0)	//if the two vector dot product is 0, then there is no intersection (we don't like to divide by 0)
	{
//		if(nx==0 && ny==1 && nz==0)
		//std::cout << "parallel, return false " << nx << " " << ny << " " << nz << std::endl;
		return false;
	}
	double t=((x0*nx+y0*ny+z0*nz-nx*xs-ny*ys-nz*zs)/(xd*nx+yd*ny+zd*nz));
	if(t<0)	//if t<0, the intersction point is in the opposite direction
	{

		return false;
	}
	double x=xs+t*xd;	//calculate the 3 point vector3d
	double y=ys+t*yd;
	double z=zs+t*zd;
	vector3d i(x,y,z);
		
	if((std::abs(trianglearea(p1,p2,p3)-(trianglearea(p1,p2,i)+trianglearea(p2,p3,i)+trianglearea(p1,p3,i)))<0.3) || std::abs(trianglearea(p1,p3,p4)-(trianglearea(p1,p3,i)+trianglearea(p3,p4,i)+trianglearea(p1,p4,i)))<0.3)	
	
	{
		if(dis!=NULL)
		{
			(*dis)=t;
			if(point!=NULL)
			{
				point->x=x;
				point->y=y;
				point->z=z;
			}
		}
		return true;
	}

	return false;	//else not
}

bool collision::sphereplane(collisionplane *pl, collisionsphere *sp)
{
	double dist1=0,dist2=0;
	//std::cout << "SP: " << sp << std::endl;
	if(rayplane(pl->normal.x,pl->normal.y,pl->normal.z,pl->p[0].x,pl->p[0].y,pl->p[0].z,sp->center.x,sp->center.y,sp->center.z,-pl->normal.x,-pl->normal.y,-pl->normal.z,pl->p[0],pl->p[1],pl->p[2],pl->p[3],&dist1) ||
	   rayplane(-pl->normal.x,-pl->normal.y,-pl->normal.z,pl->p[0].x,pl->p[0].y,pl->p[0].z,sp->center.x,sp->center.y,sp->center.z,pl->normal.x,pl->normal.y,pl->normal.z,pl->p[0],pl->p[1],pl->p[2],pl->p[3],&dist2))
	{
		if(dist1>sp->r || dist2>sp->r)
		{
			//std::cout << "returning false: " << pn;
			return false;
		}
		if(dist1>0)	//if not zero
		{
            double b = 1;
            vector3d temp1 = pl->normal * 2 * sp->velocity.dotproduct(pl->normal);
            temp1*=b;
            sp->velocity -= temp1;

  

		}
	else	//else if we are in the opposite side, then the normal vector
	{
        	 double b = 1;
       		 vector3d temp1 = pl->normal * 2 * sp->velocity.dotproduct(pl->normal);

         	 temp1*=b;
       		 sp->velocity -= temp1;
		}

		return true;	
	}

	return false;
}




bool collision::spheresphere(collisionsphere *c1,collisionsphere *c2)
{
	double dist=pointdistacesquare(c1->center,c2->center);
	if(dist<=(c1->r+c2->r)*(c1->r+c2->r))
	{

		return true;
	}
	return false;
}


void collision::spheresphereResponse(collisionsphere *c1,collisionsphere *c2)
{
	vector3d rVel = c2->velocity - c1->velocity;
	vector3d rDist = c2->center - c1->center;
	if(rVel.dotproduct(rDist) < 0)
	{
		vector3d temp = c2->velocity;
		c2->velocity = c1->velocity;
		c1->velocity = temp;
	}
}




vector3d collision ::  projectUonV(vector3d u, vector3d v) {
    vector3d r;
    r = scale(v, u.dotproduct(v) / v.dotproduct(v));
    return r;
}

vector3d collision :: scale(vector3d v, double a) {
    vector3d r;
    r.x = v.x * a;
    r.y = v.y * a;
    r.z = v.z * a;
    return r;
}
