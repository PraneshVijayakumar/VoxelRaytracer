#ifndef FORGE_AABB_H
#define FORGE_AABB_H

#include "Vector3.h"

namespace forge
{
	// Represents an axially aligned bounding box
	class AABB
	{
	public:

		AABB() = default;

		AABB(const Vector3 &pMinPoint, const Vector3 &pMaxPoint);

		Vector3 getMinPoint() const;

		void setMinPoint(const Vector3 &pMinPoint);

		Vector3 getMaxPoint() const;

		void setMaxPoint(const Vector3 &pMaxPoint);

		bool isPointInside(const Vector3 pPoint) const;

	private:

		Vector3 minPoint;
		Vector3 maxPoint;
	};

	inline
	AABB::AABB(const Vector3 &pMinPoint, const Vector3 &pMaxPoint) :
	minPoint(pMinPoint),
	maxPoint(pMaxPoint)
	{
	}

	inline
	Vector3 AABB::getMinPoint() const
	{
		return this->minPoint;
	}

	inline
	void AABB::setMinPoint(const Vector3 &pMinPoint)
	{
		this->minPoint = pMinPoint;
	}

	inline
	Vector3 AABB::getMaxPoint() const
	{
		return this->maxPoint;
	}

	inline
	void AABB::setMaxPoint(const Vector3 &pMaxPoint)
	{
		this->maxPoint = pMaxPoint;
	}

	inline
	bool AABB::isPointInside(const Vector3 pPoint) const
	{
		for(SizeType i = 0; i < 3; ++i)
		{
			if(pPoint[i] < this->minPoint[i] || pPoint[i] > this->maxPoint[i])
			{
				return false;
			}
		}

		return true;
	}
}

#endif