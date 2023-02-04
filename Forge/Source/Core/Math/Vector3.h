#ifndef FORGE_VECTOR3_H
#define FORGE_VECTOR3_H

#include <cmath>
#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/Assert.h>

namespace forge
{
	class Matrix3x3;

	// A three component floating point vector
	class Vector3
	{
	public:

		static Float dot(const Vector3 &pFirstVector3, const Vector3 &pSecondVector3);

		static Float distance(const Vector3 &pFirstVector3, const Vector3 &pSecondVector3);

		static Vector3 cross(const Vector3 &pFirstVector3, const Vector3 &pSecondVector3);

		Vector3();

		Vector3(Float pX, Float pY, Float pZ);

		Float& operator[](SizeType index);

		const Float& operator[](SizeType index) const;

		Vector3& operator+=(const Vector3 &pOtherVector3);

		Vector3& operator-=(const Vector3 &pOtherVector3);

		Vector3& operator*=(Float pScalar);

		Vector3& operator/=(Float pScalar);

		Vector3& operator*=(const Matrix3x3 &pMatrix3x3);

		// Get the magnitude of the vector. The magnitude is the length of the vector
		Float getMagnitude() const;

		Vector3 getNormalized() const;

		// Converts the vector into a unit vector
		void normalize();

		Float x = 0;
		Float y = 0;
		Float z = 0;
	};

	inline
	Float Vector3::dot(const Vector3 &pFirstVector3, const Vector3 &pSecondVector3)
	{
		return pFirstVector3.x * pSecondVector3.x + pFirstVector3.y * pSecondVector3.y + pFirstVector3.z * pSecondVector3.z;
	}

	inline
	Vector3 Vector3::cross(const Vector3 &pFirstVector3, const Vector3 &pSecondVector3)
	{
		Vector3 crossProduct;
		crossProduct.x = pFirstVector3.y * pSecondVector3.z - pFirstVector3.z * pSecondVector3.y;
		crossProduct.y = pFirstVector3.z * pSecondVector3.x - pFirstVector3.x * pSecondVector3.z;
		crossProduct.z = pFirstVector3.x * pSecondVector3.y - pFirstVector3.y * pSecondVector3.x;
		return crossProduct;
	}

	inline
	Vector3::Vector3() :
	x(0.0f),
	y(0.0f),
	z(0.0f)
	{
	}

	inline
	Vector3::Vector3(Float pX, Float pY, Float pZ) :
	x(pX),
	y(pY),
	z(pZ)
	{
	}

	inline
	Float& Vector3::operator[](SizeType pIndex)
	{
		// TODO: Can this subscript operator be made faster?
		//		 Can the default case be handled better? Maybe it should it cause the program to crash?

		FORGE_ASSERT(pIndex < 3);

		switch(pIndex)
		{
		case 0:
			return this->x;
			break;
		case 1:
			return this->y;
			break;
		case 2:
			return this->z;
			break;
		default:
			return this->x;
			break;
		}
	}

	inline
	const Float& Vector3::operator[](SizeType pIndex) const
	{
		// TODO: Can this subscript operator be made faster?
		//		 Can the default case be handled better? Maybe it should it cause the program to crash?

		FORGE_ASSERT(pIndex < 3);

		switch(pIndex)
		{
		case 0:
			return this->x;
			break;
		case 1:
			return this->y;
			break;
		case 2:
			return this->z;
			break;
		default:
			return this->x;
			break;
		}
	}

	inline
	Vector3& Vector3::operator+=(const Vector3 &pOtherVector3)
	{
		this->x += pOtherVector3.x;
		this->y += pOtherVector3.y;
		this->z += pOtherVector3.z;
		return *this;
	}

	inline
	Vector3& Vector3::operator-=(const Vector3 &pOtherVector3)
	{
		this->x -= pOtherVector3.x;
		this->y -= pOtherVector3.y;
		this->z -= pOtherVector3.z;
		return *this;
	}

	inline
	Vector3& Vector3::operator*=(Float pScalar)
	{
		this->x *= pScalar;
		this->y *= pScalar;
		this->z *= pScalar;
		return *this;
	}

	inline
	Vector3& Vector3::operator/=(Float pScalar)
	{
		this->x /= pScalar;
		this->y /= pScalar;
		this->z /= pScalar;
		return *this;
	}

	inline
	Float Vector3::getMagnitude() const
	{
		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	inline
	Vector3 Vector3::getNormalized() const
	{
		Vector3 normalizedVector3 = (*this);
		normalizedVector3.normalize();
		return normalizedVector3;
	}

	inline
	void Vector3::normalize()
	{
		const Float magnitude = this->getMagnitude();
		FORGE_ASSERT(magnitude > 0.0f);
		(*this) /= magnitude;
	}

	inline
	Vector3 operator+(const Vector3 &pFirstVector3, const Vector3 &pSecondVector3)
	{
		Vector3 newVector3 = pFirstVector3;
		newVector3 += pSecondVector3;
		return newVector3;
	}

	inline
	Vector3 operator-(const Vector3 &pFirstVector3, const Vector3 &pSecondVector3)
	{
		Vector3 newVector3 = pFirstVector3;
		newVector3 -= pSecondVector3;
		return newVector3;
	}

	inline
	Vector3 operator*(const Vector3 &pVector3, const Float &pScalar)
	{
		Vector3 newVector3 = pVector3;
		newVector3 *= pScalar;
		return newVector3;
	}

	inline
	Vector3 operator/(const Vector3 &pVector3, const Float &pScalar)
	{
		Vector3 newVector3 = pVector3;
		newVector3 /= pScalar;
		return newVector3;
	}

	Vector3 operator*(const Vector3 &pVector3, const Matrix3x3 &pMatrix3x3);

	inline
	Float Vector3::distance(const Vector3 &pFirstVector3, const Vector3 &pSecondVector3)
	{
		return (pSecondVector3 - pFirstVector3).getMagnitude();
	}
}

#endif