#ifndef FORGE_MATH_UTILITY_H
#define FORGE_MATH_UTILITY_H

#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/NumericTypeInfo.h>
#include "Ray.h"
#include "Matrix3x3.h"
#include <cmath>

namespace forge
{
	constexpr Float pi = 3.141592f;
	// PI divided by 180
	constexpr Float piOver180 = pi / 180.0f;

	/// <summary>
	/// Converts an angle from degrees to radians
	/// </summary>
	/// <param name="degrees">The angle to convert, specified in degrees</param>
	/// <returns>The angle in radians</returns>
	inline
	const Float convertDegreesToRadians(Float pDegrees)
	{
		return piOver180 * pDegrees;
	}

	// Returns the smallest of the two given objects. If the objects are equal, the second object is returned.
	// So, the < operator is used for the comparison
	template <typename Type>
	inline
	Type min(const Type &pObject1, const Type &pObject2)
	{
		if(pObject1 < pObject2)
		{
			return pObject1;
		}
		else
		{
			return pObject2;
		}
	}

	// Returns the largest of the two given objects. If the objects are equal, the second object is returned.
	// So, the > operator is used for the comparison
	template <typename Type>
	inline
	Type max(const Type &pObject1, const Type &pObject2)
	{
		if(pObject1 > pObject2)
		{
			return pObject1;
		}
		else
		{
			return pObject2;
		}
	}

	/// <summary>
	/// Returns the absolute value of the given numeric value
	/// </summary>
	/// <typeparam name="Type">The type of the given object</typeparam>
	/// <param name="pObject">Must be a numeric value</param>
	/// <returns>The absolute value of the given numeric value</returns>
	template <typename Type>
	inline
	Type abs(const Type &pObject)
	{
		if(pObject < 0.0f)
		{
			return -pObject;
		}
		else
		{
			return pObject;
		}
	}

	inline
	Float floorFloat(Float pValue)
	{
		// TODO: Can a custom flooring algorithm be implemented and used here instead?
		return std::floor(pValue);
	}

	inline
	Float roundFloat(Float pValue)
	{
		// TODO: Can a custom rounding algorithm be implemented and used here instead?
		return std::round(pValue);
	}

	/// <summary>
	/// Rounds a floating point value to the nearest integer value
	/// </summary>
	/// <typeparam name="IntegerType">The type of the integer value to round to</typeparam>
	/// <param name="pFloatValue">The floating point value to round</param>
	/// <returns>An integer value that is equal to the floating point value rounded to the nearest integer</returns>
	template <typename IntegerType>
	inline
	IntegerType roundFloatToInteger(const Float &pFloatValue)
	{
		// TODO: Can a custom rounding algorithm be implemented and usedhere instead?
		return static_cast<IntegerType>(std::round(pFloatValue));
	}

	template <typename IntegerType>
	inline
	IntegerType roundDoubleToInteger(const Double &pDoubleValue)
	{
		// TODO: Can a custom rounding algorithm be implemented and usedhere instead?
		return static_cast<IntegerType>(std::round(pDoubleValue));
	}

	template <typename IntegerType>
	inline
	IntegerType floorFloatToInteger(const Float &pFloatValue)
	{
		// TODO: Can a custom floor algorithm be implemented here instead?
		return static_cast<IntegerType>(std::floor(pFloatValue));
	}

	template <typename IntegerType>
	inline
	IntegerType ceilingFloatToInteger(const Float &pFloatValue)
	{
		// TODO: Can a custom ceiling algorithm be implemented here instead?
		return static_cast<IntegerType>(std::ceil(pFloatValue));
	}

	template <typename IntegerType>
	inline
	IntegerType roundTowardsZeroFloatToInteger(const Float &pFloatValue)
	{
		return static_cast<IntegerType>(pFloatValue);
	}

	template <typename Type>
	inline
	Type clamp(const Type &pValueToClamp, const Type &pMinValue, const Type &pMaxValue)
	{
		return min(max(pValueToClamp, pMinValue), pMaxValue);
	}

	template <typename Type>
	inline
	Type power(Type pIntegerValue, Type pExponent)
	{
		if(pExponent == 0)
		{
			return 1;
		}

		Type powerValue = pIntegerValue;
		for(Type i = 1; i < pExponent; ++i)
		{
			powerValue *= pIntegerValue;
		}

		return powerValue;
	}

	/// <summary>
	/// Checks if the given integer value is a power of the given base
	/// </summary>
	/// <typeparam name="Type">The type of the integer value and base</typeparam>
	/// <param name="pIntegerValue">The integer value to check</param>
	/// <param name="pBase">The base</param>
	/// <returns>True if the given integer value is a power of the base, false if it is not</returns>
	template <typename Type>
	inline
	bool isPowerOf(Type pIntegerValue, Type pBase)
	{
		return false;
	}

	template <typename Type>
	inline
	Type lerp(Type pValue1, Type pValue2, Float pFactor)
	{
		return pValue1 + (pValue2 - pValue1) * pFactor;
	}
}

#endif