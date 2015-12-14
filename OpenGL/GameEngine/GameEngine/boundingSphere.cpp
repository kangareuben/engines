#include "boundingSphere.h"
#include <cassert>

IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere& other) const
{
	//The radius is the distance from any point on the sphere to the center.
	//
	//Therefore, by adding the radius of two spheres together, the result is
	//the distance between the centers of the spheres when they are touching.
	double radiusDistance = m_radius + other.GetRadius();
	Vector3f direction = (other.GetCenter() - m_center);
	double centerDistance = direction.Length();
	direction /= centerDistance;

	//Since the radiusDistance is the distance bwteen the centers of the
	//spheres are when they're touching, you can subtract that from the
	//distance between the centers of the spheres to get the actual distance
	//between the two spheres.
	double distance = centerDistance - radiusDistance;

	//Spheres can only be intersecting if the distance between them is less
	//than 0.
	return IntersectData(distance < 0, direction * distance);
}

void BoundingSphere::Transform(const Vector3f& translation)
{
	m_center += translation;
}


