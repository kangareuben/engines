#ifndef COLLIDER_H
#define COLLIDER_H


#include "intersectData.h"
#include "math3d.h"
#include "referenceCounter.h"

//Base class for colliders to be used in the Physics Engine
class Collider : public ReferenceCounter
{
public:
	//Types of colliders
	enum
	{
		TYPE_SPHERE,
		TYPE_AABB,

		TYPE_SIZE
	};

	
	Collider(int type) :
		ReferenceCounter(),
		m_type(type) {}

	//Intersect data after collision
	IntersectData Intersect(const Collider& other) const;

	//Moving the collider (care should be taken to match with the object rendered from objloader)
	virtual void Transform(const Vector3f& translation) {}
	
	//Center of collider
	virtual Vector3f GetCenter() const { return Vector3f(0,0,0); }

	//getter
	inline int GetType() const { return m_type; }
private:

	int m_type;
};

#endif
