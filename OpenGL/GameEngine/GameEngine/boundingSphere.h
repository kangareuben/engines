#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H


#include "math3d.h"
#include "intersectData.h"
#include "collider.h"


class BoundingSphere : public Collider
{
public:

	BoundingSphere(const Vector3f& center, double radius) :
		Collider(Collider::TYPE_SPHERE),
		m_center(center),
		m_radius(radius) {}


	IntersectData IntersectBoundingSphere(const BoundingSphere& other) const;
	virtual void Transform(const Vector3f& translation);
	virtual Vector3f GetCenter() const { return m_center; }

	inline double GetRadius()           const { return m_radius; }

private:
	Vector3f m_center;
	double    m_radius;
};

#endif // BOUNDINGSPHERE_H
