#ifndef FORGE_UTILITY_H
#define FORGE_UTILITY_H

namespace forge
{
	/// <summary>
	/// Checks if the given character arrays are equal. Both the character arrays must be null terminated
	/// </summary>
	/// <param name="characterArray1">The first null ternimated character array</param>
	/// <param name="characterArray2">The second null ternimated character array</param>
	/// <returns>True if the character arrays are equal. False if they are not</returns>
	inline
	bool areCharacterArraysEqual(const char *pCharacterArray1, const char *pCharacterArray2)
	{
		// Check if the charcaters at the same index positions in the array are equal, until one or both of them are the null character
		while(*pCharacterArray1 != '\0' && *pCharacterArray2 != '\0')
		{
			// If the characters at the same index positions are not equal, the arrays are not equal
			if(*pCharacterArray1++ != *pCharacterArray2++)
			{
				return false;
			}
		}

		// If both of the next characters are null characters, then the arrays are equal.
		// If one of them is a null character and the other is not, then the arrays are not equal
		return *pCharacterArray1 == *pCharacterArray2;
	}

	inline
	SizeType getCharacterArrayLength(const char *pCharacterArray)
	{
		SizeType length = 0;

		while((*pCharacterArray) != '\0')
		{
			++length;
			++pCharacterArray;
		}

		return length;
	}

	/// <summary>
	/// Swaps the values of the two given objects
	/// </summary>
	/// <typeparam name="Type">The type of the given objects</typeparam>
	/// <param name="pObject1">The first object</param>
	/// <param name="pObject2">The second object</param>
	template <typename Type>
	inline
	void swap(Type &pObject1, Type &pObject2)
	{
		Type object1Copy = pObject1;
		pObject1 = pObject2;
		pObject2 = object1Copy;
	}

	template <typename Type>
	inline
	Type bitwiseLeftShiftInteger(Type pInteger, Type pAmountToShift)
	{
		return pInteger << pAmountToShift;
	}

	template <typename Type>
	inline
	Type bitwiseRightShiftInteger(Type pInteger, Type pAmountToShift)
	{
		return pInteger >> pAmountToShift;
	}
}

#endif