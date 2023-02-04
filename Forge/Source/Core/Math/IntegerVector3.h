#ifndef FORGE_INTEGER_VECTOR3_H
#define FORGE_INTEGER_VECTOR3_H

#include <Forge/Source/Core/Forge.h>

namespace forge
{
	/// <summary>
	/// An 3-component vector in which all the components are integers
	/// </summary>
	/// <typeparam name="IntegerComponentType">The integer type that the components should have</typeparam>
	template <typename IntegerComponentType>
	class IntegerVector3
	{
	public:

		using ComponentType = IntegerComponentType;

		IntegerVector3() = default;

		IntegerVector3(IntegerComponentType pX, IntegerComponentType pY, IntegerComponentType pZ);

		IntegerComponentType& operator[](SizeType pIndex);

		const IntegerComponentType& operator[](SizeType pIndex) const;

		IntegerVector3& operator+=(const IntegerVector3 &pOtherIntegerVector3);

		IntegerComponentType x = 0;
		IntegerComponentType y = 0;
		IntegerComponentType z = 0;
	};

	template <typename IntegerComponentType>
	inline
	IntegerVector3<IntegerComponentType>::IntegerVector3(IntegerComponentType pX, IntegerComponentType pY, IntegerComponentType pZ) :
	x(pX),
	y(pY),
	z(pZ)
	{
	}

	template <typename IntegerComponentType>
	inline
	IntegerComponentType& IntegerVector3<IntegerComponentType>::operator[](SizeType pIndex)
	{
		// TODO: Can this subscript operator be made faster?
		//		 Can the default case be handled better? Maybe it should it cause the program to crash?

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

	template <typename IntegerComponentType>
	inline
	const IntegerComponentType& IntegerVector3<IntegerComponentType>::operator[](SizeType pIndex) const
	{
		// TODO: Can this subscript operator be made faster?
		//		 Can the default case be handled better? Maybe it should it cause the program to crash?

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

	template <typename IntegerComponentType>
	inline
	IntegerVector3<IntegerComponentType>& IntegerVector3<IntegerComponentType>::operator+=(const IntegerVector3<IntegerComponentType> &pOtherIntegerVector3)
	{
		this->x += pOtherIntegerVector3.x;
		this->y += pOtherIntegerVector3.y;
		this->z += pOtherIntegerVector3.z;
		return *this;
	}

	template <typename IntegerComponentType>
	inline
	bool operator<(const IntegerVector3<IntegerComponentType> &pFirstIntegerVector3, const IntegerVector3<IntegerComponentType> &pSecondIntegerVector3)
	{
		if(pFirstIntegerVector3.x < pSecondIntegerVector3.x)
		{
			return true;
		}
		else if(pSecondIntegerVector3.x < pFirstIntegerVector3.x)
		{
			return false;
		}

		if(pFirstIntegerVector3.y < pSecondIntegerVector3.y)
		{
			return true;
		}
		else if(pSecondIntegerVector3.y < pFirstIntegerVector3.y)
		{
			return false;
		}

		if(pFirstIntegerVector3.z < pSecondIntegerVector3.z)
		{
			return true;
		}
		else if(pSecondIntegerVector3.z < pFirstIntegerVector3.z)
		{
			return false;
		}

		return false;
	}

	template <typename IntegerComponentType>
	inline
	bool operator==(const IntegerVector3<IntegerComponentType> &pFirstIntegerVector3, const IntegerVector3<IntegerComponentType> &pSecondIntegerVector3)
	{
		return pFirstIntegerVector3.x == pSecondIntegerVector3.x &&
			   pFirstIntegerVector3.y == pSecondIntegerVector3.y &&
			   pFirstIntegerVector3.z == pSecondIntegerVector3.z;
	}

	template <typename IntegerComponentType>
	inline
	IntegerVector3<IntegerComponentType> operator+(const IntegerVector3<IntegerComponentType> &pFirstIntegerVector3, const IntegerVector3<IntegerComponentType> &pSecondIntegerVector3)
	{
		IntegerVector3 newIntegerVector3 = pFirstIntegerVector3;
		newIntegerVector3 += pSecondIntegerVector3;
		return newIntegerVector3;
	}
}

#endif