#include "aabb.h"
#include <cassert>

IntersectData AABB::IntersectAABB(const AABB& other) const
{
	//The distance between the AABB's on the X, Y, and Z axis.
	//Computed twice because there are two possible valid distances, depending
	//on the location of the AABB's.
	Vector3f distances1 = other.GetMinExtents() - m_maxExtents;
	Vector3f distances2 = m_minExtents - other.GetMaxExtents();

	//The correct distances will be whichever distance is larger for that
	//particular axis.
	Vector3f distances = Vector3f(distances1.Max(distances2));

	double maxDistance = distances.Max();

	//If there is any distance between the two AABB's, then max distance will
	//be greather than or equal to 0. If there is distance between the two
	//AABBs, then they aren't intersecting.
	//
	//Therefore, if the AABBs are intersecting, then the distance between them
	//must be less than zero.

	//TODO: This might actually need to return the minDistance if they are
	//intersecting.
	return IntersectData(maxDistance < 0, distances);
}

void AABB::Transform(const Vector3f& translation)
{
	m_minExtents += translation;
}
