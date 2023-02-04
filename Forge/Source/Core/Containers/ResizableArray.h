#ifndef FORGE_RESIZABLE_ARRAY_H
#define FORGE_RESIZABLE_ARRAY_H

#include <new>
#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/Math/MathUtility.h>

namespace forge
{
	/// <summary>
	/// This class represents an array of elements of the given type. The number of elements can be changed.
	/// Any new elements that are created as a result of the number of elements being changed are value initialized.
	/// The elements are stored in contiguous memory. This class allocates and deallocates its own dynamic memory.
	/// When an object of this type is destroyed, all the elements it holds are destroyed as well
	/// </summary>
	/// <typeparam name="ElementType">The type of the elements in the array</typeparam>
	template <typename ElementType>
	class ResizableArray final
	{
	public:

		ResizableArray() = default;

		ResizableArray(const ResizableArray<ElementType> &pResizableArrayToCopy);

		ResizableArray(SizeType pNumberOfElements);

		// If the number of elements is greater than zero, the destructor destroys all of them and deallocates all the memory that was used to store them
		~ResizableArray();

		ResizableArray<ElementType>& operator=(const ResizableArray<ElementType> &pResizableArrayToCopy);

		// Gets the element in the array specified by the given index. Must only be used if the number of elements in the array is greater than zero.
		// Indices must be in the range [0, numberOfElements)
		ElementType& operator[](SizeType pIndex);

		// Gets the element in the array specified by the given index. Must only be used if the number of elements in the array is greater than zero.
		// Indices must be in the range [0, numberOfElements)
		const ElementType& operator[](SizeType pIndex) const;

		// Get the number of elements in the array
		SizeType getNumberOfElements() const;

		// Set the number of elements in the array. If new elements are created, they are value initialized
		void setNumberOfElements(SizeType pNewNumberOfElements);

	private:

		/// <summary>
		/// Copies all the class members of the ResizableArray given in the parameter into the members passed as parameters
		/// </summary>
		/// <param name="pElementArrayToCopyTo">The elementArray member to copy into</param>
		/// <param name="pNumberOfElementsToCopyTo">The numberOfElements member to copy into</param>
		/// <param name="pResizableArrayToCopy">The ResizableArray to copy the members from</param>
		void copyArrayMembers(ElementType *&pElementArrayToCopyTo, SizeType &pNumberOfElementsToCopyTo, const ResizableArray<ElementType> &pResizableArrayToCopy);

		// The array that holds all the elements
		ElementType *elementArray = nullptr;
		// The number of elements in the array
		SizeType numberOfElements = 0;
	};

	template <typename ElementType>
	inline
	ResizableArray<ElementType>::ResizableArray(const ResizableArray<ElementType> &pResizableArrayToCopy)
	{
		this->copyArrayMembers(this->elementArray, this->numberOfElements, pResizableArrayToCopy);
	}

	template <typename ElementType>
	inline
	ResizableArray<ElementType>::ResizableArray(SizeType pNumberOfElements)
	{
		this->setNumberOfElements(pNumberOfElements);
	}

	template <typename ElementType>
	inline
	ResizableArray<ElementType>::~ResizableArray()
	{
		// If any elements exist, destroy them all
		this->setNumberOfElements(0);
	}

	template <typename ElementType>
	ResizableArray<ElementType>& ResizableArray<ElementType>::operator=(const ResizableArray<ElementType> &pResizableArrayToCopy)
	{
		// Make a copy of the given array first. This is to handle self assignment
		ElementType *givelElementArrayCopy;
		SizeType givenElementArrayNumberOfElementsCopy;
		this->copyArrayMembers(givelElementArrayCopy, givenElementArrayNumberOfElementsCopy, pResizableArrayToCopy);

		// Delete this object's array if it exists
		// TODO: This check need not be here, if the element array is guaranteed to be nullptr if the number of elements is 0
		if(this->numberOfElements > 0)
		{
			delete[] this->elementArray;
		}
		
		// Copy the copies of the given ResizableArray's members into this ResizableArray's members
		this->elementArray = givelElementArrayCopy;
		this->numberOfElements = givenElementArrayNumberOfElementsCopy;

		return *this;
	}

	template <typename ElementType>
	inline
	ElementType& ResizableArray<ElementType>::operator[](SizeType pIndex)
	{
		return this->elementArray[pIndex];
	}

	template <typename ElementType>
	inline
	const ElementType& ResizableArray<ElementType>::operator[](SizeType pIndex) const
	{
		return this->elementArray[pIndex];
	}

	template <typename ElementType>
	inline
	SizeType ResizableArray<ElementType>::getNumberOfElements() const
	{
		return this->numberOfElements;
	}

	template <typename ElementType>
	void ResizableArray<ElementType>::setNumberOfElements(SizeType pNewNumberOfElements)
	{
		// If the new size is the same as the current size
		if(pNewNumberOfElements == this->numberOfElements)
		{
			return;
		}
		
		ElementType *newElementArray = nullptr;

		// If the requested number of elements is greater than zero
		if(pNewNumberOfElements > 0)
		{
			// Allocate a new array that holds the requested number of elements, and value initialize them
			newElementArray = new ElementType[pNewNumberOfElements]();

			// Calculate the number of elements to copy from the old array to the new one
			SizeType numberOfElementsToCopy = min(this->numberOfElements, pNewNumberOfElements);

			// Copy the elements in the old array to the new array
			for(SizeType i = 0; i < numberOfElementsToCopy; ++i)
			{
				newElementArray[i] = this->elementArray[i];
			}
		}

		// Delete the old array
		if(this->numberOfElements > 0)
		{
			// Destroy all the existing elements
			for(SizeType i = 0; i < this->numberOfElements; ++i)
			{
				this->elementArray[i].~ElementType();
			}
			// Deallocate the memory that was used to store the elements
			delete[] this->elementArray;
		}

		// Set the new array as the current array
		this->elementArray = newElementArray;
		this->numberOfElements = pNewNumberOfElements;
	}

	template<typename ElementType>
	inline
	void ResizableArray<ElementType>::copyArrayMembers(ElementType *&pElementArrayToCopyTo, SizeType &pNumberOfElementsToCopyTo, const ResizableArray<ElementType> &pResizableArrayToCopy)
	{
		if(pResizableArrayToCopy.numberOfElements == 0)
		{
			pElementArrayToCopyTo = nullptr;
			pNumberOfElementsToCopyTo = 0;
		}
		else
		{
			// Create a new array to hold the elements
			pElementArrayToCopyTo = new ElementType[pResizableArrayToCopy.numberOfElements];
			// Copy all the elements into the new array
			for(SizeType i = 0; i < pResizableArrayToCopy.numberOfElements; ++i)
			{
				pElementArrayToCopyTo[i] = pResizableArrayToCopy.elementArray[i];
			}

			pNumberOfElementsToCopyTo = pResizableArrayToCopy.numberOfElements;
		}
	}
}

#endif