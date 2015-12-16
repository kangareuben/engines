#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <cmath>

class vector3d{
		public:
		double x,y,z;
		vector3d();
		vector3d(double a,double b);
		vector3d(double a,double b,double c);
		~vector3d(){};
		double dotproduct(const vector3d& vec2);
		vector3d crossproduct(const vector3d& vec2);
		double length();
		void change(double a,double b,double c);
		void change(vector3d&);
		void change(vector3d);
		void changeX(double a);
		void changeY(double b);
		void changeZ(double c);
		void normalize();
		vector3d operator+(const vector3d& vec2);
		vector3d operator-(const vector3d& vec2);
		vector3d operator*(const double& num);
		vector3d operator/(const double& num);
		vector3d& operator+=(const vector3d& vec2);
		vector3d& operator-=(const vector3d& vec2);
		vector3d& operator*=(const double& num);
		vector3d& operator/=(const double& num);
		bool operator==(const vector3d& vec2);
		bool operator!=(const vector3d& vec2);
		friend std::ostream& operator<<(std::ostream& out,vector3d vec2);
};
#endif
