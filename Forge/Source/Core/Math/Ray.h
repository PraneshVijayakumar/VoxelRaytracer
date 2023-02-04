#ifndef FORGE_RAY_H
#define FORGE_RAY_H

#include <Forge/Source/Core/Math/Vector3.h>

namespace forge
{
	// Represents a directed line segment
	class Ray
	{
	public:

		Ray(const Vector3 &pOrigin, const Vector3 &pDirection, const Float &pLength);

		Vector3 getOrigin() const;

		void setOrigin(const Vector3 &pOrigin);

		Vector3 getDirection() const;

		void setDirection(const Vector3 &pDirection);

		Float getLength() const;

		void setLength(const Float &pLength);

	private:

		// The origin is where the ray starts
		Vector3 origin;
		// The direction that the ray is facing
		Vector3 direction;
		// The length of the ray
		Float length;
	};

	inline
	Ray::Ray(const Vector3 &pOrigin, const Vector3 &pDirection, const Float &pLength) :
	origin(pOrigin),
	direction(pDirection),
	length(pLength)
	{
	}

	inline
	Vector3 Ray::getOrigin() const
	{
		return this->origin;
	}

	inline
	void Ray::setOrigin(const Vector3 &pOrigin)
	{
		this->origin = pOrigin;
	}

	inline
	Vector3 Ray::getDirection() const
	{
		return this->direction;
	}

	inline
	void Ray::setDirection(const Vector3 &pDirection)
	{
		this->direction = pDirection;
	}

	inline
	Float Ray::getLength() const
	{
		return this->length;
	}

	inline
	void Ray::setLength(const Float &pLength)
	{
		this->length = pLength;
	}
}

#endif