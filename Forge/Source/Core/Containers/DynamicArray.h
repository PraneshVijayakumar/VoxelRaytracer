#ifndef FORGE_DYNAMIC_ARRAY_H
#define FORGE_DYNAMIC_ARRAY_H

#include <new>
#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/Math/MathUtility.h>

namespace forge
{
	/// <summary>
	/// This class represents an array of elements of the given type. Elements can be added to this array.
	/// The elements are stored in contiguous memory. This class allocates and deallocates its own dynamic memory.
	/// When an object of this type is destroyed, all the elements it holds are destroyed as well.
	/// The number of elements stored can be increased and decreased in an efficient way.
	/// </summary>
	/// <typeparam name="ElementType">The type of the elements in the array</typeparam>
	template <typename ElementType>
	class DynamicArray final
	{
	public:

		DynamicArray() = default;

		DynamicArray(const DynamicArray<ElementType> &pDynamicArrayToCopy);

		// If the number of elements is greater than zero, the destructor destroys all of them and deallocates all the memory that was used to store them
		~DynamicArray();

		DynamicArray<ElementType>& operator=(const DynamicArray<ElementType> &pDynamicArrayToCopy);

		// Gets the element in the array specified by the given index. Must only be used if the number of elements in the array is greater than zero.
		// Indices must be in the range [0, numberOfElements)
		ElementType& operator[](SizeType pIndex);

		// Gets the element in the array specified by the given index. Must only be used if the number of elements in the array is greater than zero.
		// Indices must be in the range [0, numberOfElements)
		const ElementType& operator[](SizeType pIndex) const;

		// Get the number of elements in the array
		SizeType getNumberOfElements() const;

		// Add a new element at the end of the array
		void append(const ElementType &pElementToAppend);
		
	private:

		/// <summary>
		/// Copies all the class members of the DynamicArray given in the parameter into the members passed as parameters
		/// </summary>
		/// <param name="pElementArrayToCopyTo">The elementArray member to copy into</param>
		/// <param name="pNumberOfElementsToCopyTo">The numberOfElements member to copy into</param>
		/// <param name="pMaxNumberOfElementsToCopyTo">The maxNumberOfElements member to copy into</param>
		/// <param name="pResizableArrayToCopy">The ResizableArray to copy the members from</param>
		void copyArrayMembers(ElementType *&pElementArrayToCopyTo,
							  SizeType &pNumberOfElementsToCopyTo,
							  SizeType &pMaxNumberOfElementsToCopyTo,
							  const DynamicArray<ElementType> &pDynamicArrayToCopy);

		// The number of elements in the array
		SizeType numberOfElements = 0;
		// The number of elements that the array can currently hold
		SizeType maxNumberOfElements = 0;
		// The array that holds all the elements
		ElementType *elementArray = nullptr;
	};

	template <typename ElementType>
	inline
	DynamicArray<ElementType>::DynamicArray(const DynamicArray<ElementType> &pDynamicArrayToCopy)
	{
		this->copyArrayMembers(this->elementArray, this->numberOfElements, this->maxNumberOfElements, pDynamicArrayToCopy);
	}

	template <typename ElementType>
	inline
	DynamicArray<ElementType>::~DynamicArray()
	{
		if(this->numberOfElements > 0)
		{
			// Destroy all the existing elements
			for(SizeType i = 0; i < this->numberOfElements; ++i)
			{
				this->elementArray[i].~ElementType();
			}
			// Deallocate the memory that was used to store the elements
			::operator delete(this->elementArray);
		}
	}

	template <typename ElementType>
	DynamicArray<ElementType>& DynamicArray<ElementType>::operator=(const DynamicArray<ElementType> &pDynamicArrayToCopy)
	{
		// Make a copy of the given array first. This is to handle self assignment
		ElementType *givenElementArrayCopy;
		SizeType givenNumberOfElementsCopy;
		SizeType givenMaxNumberOfElementsCopy;
		this->copyArrayMembers(this->elementArray, this->numberOfElements, this->maxNumberOfElements, pDynamicArrayToCopy);

		if(this->numberOfElements > 0)
		{
			// Destroy all the existing elements
			for(SizeType i = 0; i < this->numberOfElements; ++i)
			{
				this->elementArray[i].~ElementType();
			}
			// Deallocate the memory that was used to store the elements
			::operator delete(this->elementArray);
		}

		// Copy the copies of the given DynamicArray's members into this DynamicArray's members
		this->elementArray = givenElementArrayCopy;
		this->numberOfElements = givenNumberOfElementsCopy;
		this->maxNumberOfElements = givenMaxNumberOfElementsCopy;

		return *this;
	}

	template <typename ElementType>
	inline
	ElementType& DynamicArray<ElementType>::operator[](SizeType pIndex)
	{
		return this->elementArray[pIndex];
	}

	template <typename ElementType>
	inline
	const ElementType& DynamicArray<ElementType>::operator[](SizeType pIndex) const
	{
		return this->elementArray[pIndex];
	}

	template <typename ElementType>
	inline
	SizeType DynamicArray<ElementType>::getNumberOfElements() const
	{
		return this->numberOfElements;
	}

	template <typename ElementType>
	void DynamicArray<ElementType>::append(const ElementType &pElementToAppend)
	{
		// If the array is already full
		if(this->numberOfElements == this->maxNumberOfElements)
		// Increase the size of the array
		{
			// Calculate the new max number of elements
			if(this->maxNumberOfElements == 0)
			{
				this->maxNumberOfElements = 1;
			}
			else
			{
				this->maxNumberOfElements *= 2;
			}

			// The size of the new element array in bytes
			const SizeType newElementArraySize = sizeof(ElementType) * this->maxNumberOfElements;
			
			// Allocate a new array to hold the new max number of elements
			ElementType *newElementArray = static_cast<ElementType*>(::operator new(newElementArraySize));

			// Copy all the existing elements to the new array
			for(SizeType i = 0; i < this->numberOfElements; ++i)
			{
				new (newElementArray + i) ElementType(this->elementArray[i]);
			}

			// Delete the old array if it exists
			if(this->numberOfElements > 0)
			{
				// Destroy all the existing elements
				for(SizeType i = 0; i < this->numberOfElements; ++i)
				{
					this->elementArray[i].~ElementType();
				}
				// Deallocate all the memory that was used to store the elements
				::operator delete(this->elementArray);
			}

			// Set the new array as the current array
			this->elementArray = newElementArray;
		}

		// Append the new element to the array
		new (this->elementArray + this->numberOfElements) ElementType(pElementToAppend);
		++this->numberOfElements;
	}

	template<typename ElementType>
	inline
	void DynamicArray<ElementType>::copyArrayMembers(ElementType *&pElementArrayToCopyTo, SizeType &pNumberOfElementsToCopyTo, SizeType &pMaxNumberOfElementsToCopyTo, const DynamicArray<ElementType> &pDynamicArrayToCopy)
	{
		if(pDynamicArrayToCopy.numberOfElements == 0)
		{
			pElementArrayToCopyTo = nullptr;
			pNumberOfElementsToCopyTo = 0;
			pMaxNumberOfElementsToCopyTo = 0;
		}
		else
		{
			// Create a new array to hold the elements
			pElementArrayToCopyTo = static_cast<ElementType*>(::operator new(pDynamicArrayToCopy.maxNumberOfElements));
			// Copy all the elements into the new array
			for(SizeType i = 0; i < pDynamicArrayToCopy.numberOfElements; ++i)
			{
				new (pElementArrayToCopyTo + i) ElementType(pDynamicArrayToCopy.elementArray[i]);
			}

			pNumberOfElementsToCopyTo = pDynamicArrayToCopy.numberOfElements;
			pMaxNumberOfElementsToCopyTo = pDynamicArrayToCopy.maxNumberOfElements;
		}
	}
}

#endif