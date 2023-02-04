#ifndef FORGE_INTEGER_VECTOR2_H
#define FORGE_INTEGER_VECTOR2_H

#include <Forge/Source/Core/Forge.h>

namespace forge
{
	/// <summary>
	/// An 3-component vector in which all the components are integers
	/// </summary>
	/// <typeparam name="IntegerComponentType">The integer type that the components should have</typeparam>
	template <typename IntegerComponentType>
	class IntegerVector2
	{
	public:

		using ComponentType = IntegerComponentType;

		IntegerVector2() = default;

		IntegerVector2(IntegerComponentType pX, IntegerComponentType pY);

		IntegerComponentType& operator[](SizeType pIndex);

		const IntegerComponentType& operator[](SizeType pIndex) const;

		IntegerVector2& operator+=(const IntegerVector2 &pOtherIntegerVector2);

		IntegerComponentType x = 0;
		IntegerComponentType y = 0;
	};

	template <typename IntegerComponentType>
	inline
	IntegerVector2<IntegerComponentType>::IntegerVector2(IntegerComponentType pX, IntegerComponentType pY) :
	x(pX),
	y(pY)
	{
	}

	template <typename IntegerComponentType>
	inline
	IntegerComponentType& IntegerVector2<IntegerComponentType>::operator[](SizeType pIndex)
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

		default:
			return this->x;
			break;
		}
	}

	template <typename IntegerComponentType>
	inline
	const IntegerComponentType& IntegerVector2<IntegerComponentType>::operator[](SizeType pIndex) const
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

		default:
			return this->x;
			break;
		}
	}

	template <typename IntegerComponentType>
	inline
	IntegerVector2<IntegerComponentType>& IntegerVector2<IntegerComponentType>::operator+=(const IntegerVector2<IntegerComponentType> &pOtherIntegerVector2)
	{
		this->x += pOtherIntegerVector2.x;
		this->y += pOtherIntegerVector2.y;
		return *this;
	}

	template <typename IntegerComponentType>
	inline
	bool operator<(const IntegerVector2<IntegerComponentType> &pFirstIntegerVector3, const IntegerVector2<IntegerComponentType> &pSecondIntegerVector3)
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

		return false;
	}

	template <typename IntegerComponentType>
	inline
	bool operator==(const IntegerVector2<IntegerComponentType> &pFirstIntegerVector3, const IntegerVector2<IntegerComponentType> &pSecondIntegerVector3)
	{
		return pFirstIntegerVector3.x == pSecondIntegerVector3.x &&
			   pFirstIntegerVector3.y == pSecondIntegerVector3.y;
	}

	template <typename IntegerComponentType>
	inline
	IntegerVector2<IntegerComponentType> operator+(const IntegerVector2<IntegerComponentType> &pFirstIntegerVector2, const IntegerVector2<IntegerComponentType> &pSecondIntegerVector2)
	{
		IntegerVector2 newIntegerVector2 = pFirstIntegerVector2;
		newIntegerVector2 += pSecondIntegerVector2;
		return newIntegerVector2;
	}
}

#endif