#ifndef FORGE_VECTOR2_H
#define FORGE_VECTOR2_H

#include <cmath>
#include <Forge/Source/Core/Forge.h>

namespace forge
{
	class Matrix3x3;

	// A three component floating point vector
	class Vector2
	{
	public:

		static Float dot(const Vector2 &pFirstVector3, const Vector2 &pSecondVector3);

		static Float distance(const Vector2 &pFirstVector2, const Vector2 &pSecondVector2);

		static Float distanceSquared(const Vector2 &pFirstVector2, const Vector2 &pSecondVector2);

		Vector2() = default;

		Vector2(Float pX, Float pY);

		Float& operator[](SizeType index);

		const Float& operator[](SizeType index) const;

		Vector2& operator+=(const Vector2 &pOtherVector3);

		Vector2& operator-=(const Vector2 &pOtherVector3);

		Vector2& operator*=(Float pScalar);

		Vector2& operator/=(Float pScalar);

		// Get the magnitude of the vector. The magnitude is the length of the vector
		Float getMagnitude() const;

		Float getSquaredMagnitude() const;

		Vector2 getNormalized() const;

		// Converts the vector into a unit vector
		void normalize();

		Float x = 0;
		Float y = 0;
	};

	inline
	Float Vector2::dot(const Vector2 &pFirstVector3, const Vector2 &pSecondVector3)
	{
		return pFirstVector3.x * pSecondVector3.x + pFirstVector3.y * pSecondVector3.y;
	}

	inline
	Vector2::Vector2(Float pX, Float pY) :
	x(pX),
	y(pY)
	{
	}

	inline
	Float& Vector2::operator[](SizeType pIndex)
	{
		// TODO: Can this subscript operator be made faster?
		//		 Can the default case be handled better? Maybe it should it cause the program to crash?

		FORGE_ASSERT(pIndex < 2);

		switch(pIndex)
		{
		case 0:
			return this->x;
			break;
		case 1:
			return this->y;
			break;
		default:
			return this->x;
			break;
		}
	}

	inline
	const Float& Vector2::operator[](SizeType pIndex) const
	{
		// TODO: Can this subscript operator be made faster?
		//		 Can the default case be handled better? Maybe it should it cause the program to crash?

		FORGE_ASSERT(pIndex < 2);

		switch(pIndex)
		{
		case 0:
			return this->x;
			break;
		case 1:
			return this->y;
			break;
		default:
			return this->x;
			break;
		}
	}

	inline
	Vector2& Vector2::operator+=(const Vector2 &pOtherVector3)
	{
		this->x += pOtherVector3.x;
		this->y += pOtherVector3.y;
		return *this;
	}

	inline
	Vector2& Vector2::operator-=(const Vector2 &pOtherVector3)
	{
		this->x -= pOtherVector3.x;
		this->y -= pOtherVector3.y;
		return *this;
	}

	inline
	Vector2& Vector2::operator*=(Float pScalar)
	{
		this->x *= pScalar;
		this->y *= pScalar;
		return *this;
	}

	inline
	Vector2& Vector2::operator/=(Float pScalar)
	{
		this->x /= pScalar;
		this->y /= pScalar;
		return *this;
	}

	inline
	Float Vector2::getMagnitude() const
	{
		return std::sqrt(this->getSquaredMagnitude());
	}

	inline
	Float Vector2::getSquaredMagnitude() const
	{
		return this->x * this->x + this->y * this->y;
	}

	inline
	Vector2 Vector2::getNormalized() const
	{
		Vector2 normalizedVector3 = (*this);
		normalizedVector3.normalize();
		return normalizedVector3;
	}

	inline
	void Vector2::normalize()
	{
		Float magnitude = this->getMagnitude();
		(*this) /= magnitude;
	}

	inline
	Vector2 operator+(const Vector2 &pFirstVector3, const Vector2 &pSecondVector3)
	{
		Vector2 newVector3 = pFirstVector3;
		newVector3 += pSecondVector3;
		return newVector3;
	}

	inline
	Vector2 operator-(const Vector2 &pFirstVector3, const Vector2 &pSecondVector3)
	{
		Vector2 newVector3 = pFirstVector3;
		newVector3 -= pSecondVector3;
		return newVector3;
	}

	inline
	Vector2 operator*(const Vector2 &pVector3, const Float &pScalar)
	{
		Vector2 newVector3 = pVector3;
		newVector3 *= pScalar;
		return newVector3;
	}

	inline
	Vector2 operator/(const Vector2 &pVector3, const Float &pScalar)
	{
		Vector2 newVector3 = pVector3;
		newVector3 /= pScalar;
		return newVector3;
	}

	inline
	Float Vector2::distance(const Vector2 &pFirstVector2, const Vector2 &pSecondVector2)
	{
		return (pSecondVector2 - pFirstVector2).getMagnitude();
	}

	inline
	Float Vector2::distanceSquared(const Vector2 &pFirstVector2, const Vector2 &pSecondVector2)
	{
		return (pSecondVector2 - pFirstVector2).getSquaredMagnitude();
	}
}

#endif