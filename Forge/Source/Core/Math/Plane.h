#ifndef FORGE_PLANE_H
#define FORGE_PLANE_H

#include "Vector3.h"

namespace forge
{
	class Plane
	{
	public:

		Plane();

		Plane(const Vector3 &pNormal, Float pDistance);

		Plane(const Vector3 &pNormal, const Vector3 &pPointOnThePlane);

		Vector3 getNormal() const;

		void setNormal(const Vector3 &pNormal);

		Float getDistance() const;

		void setDistance(Float pDistance);

	private:

		Vector3 normal;
		Float distance;
	};

	inline
	Plane::Plane() :
	normal(0.0f, 0.0f, 0.0f),
	distance(0.0f)
	{
	}

	inline
	Plane::Plane(const Vector3 &pNormal, Float pDistance) :
	normal(pNormal),
	distance(pDistance)
	{
	}

	inline
	Plane::Plane(const Vector3 &pNormal, const Vector3 &pPointOnThePlane) :
	normal(pNormal)
	{
		this->distance = Vector3::dot(pPointOnThePlane, pNormal);
	}

	inline
	Vector3 Plane::getNormal() const
	{
		return this->normal;
	}

	inline
	void Plane::setNormal(const Vector3 &pNormal)
	{
		this->normal = pNormal;
	}

	inline
	Float Plane::getDistance() const
	{
		return this->distance;
	}

	inline
	void Plane::setDistance(Float pDistance)
	{
		this->distance = pDistance;
	}
}

#endif