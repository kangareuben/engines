#ifndef AABB_H
#define AABB_H

#include "math3d.h"
#include "intersectData.h"
#include "collider.h"

class AABB : public Collider
{
public:

	AABB(const Vector3f& minExtents, const Vector3f& maxExtents) :
        Collider(Collider::TYPE_AABB),
		m_minExtents(minExtents),
		m_maxExtents(maxExtents) {}


	IntersectData IntersectAABB(const AABB& other) const;

	inline const Vector3f& GetMinExtents() const { return m_minExtents; }
	inline const Vector3f& GetMaxExtents() const { return m_maxExtents; }
	virtual void Transform(const Vector3f& translation);

private:
    Vector3f m_minExtents;
	Vector3f m_maxExtents;
};
#endif // AABB_H
