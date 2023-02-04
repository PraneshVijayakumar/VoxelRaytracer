#ifndef FORGE_QUEUE_H
#define FORGE_QUEUE_H

#include <Forge/Source/Core/Containers/ResizableArray.h>
#include <Forge/Source/Core/Assert.h>

namespace forge
{
	/// <summary>
	/// A first in first out queue
	/// </summary>
	/// <typeparam name="ElementType">The type of the elements that the queue holds</typeparam>
	template <typename ElementType>
	class Queue
	{
	public:

		Queue(SizeType pMaxNumberOfElements);

		ElementType& operator[](SizeType pIndex);

		const ElementType& operator[](SizeType pIndex) const;

		SizeType getMaxNumberOfElements() const;

		SizeType getNumberOfElements() const;

		const ElementType& getFirstElement() const;

		const ElementType& getLastElement() const;

		void append(const ElementType &pElementType);

		ElementType remove();

		void clear();

	private:

		SizeType getNextIndex(SizeType pIndex) const;
		
		SizeType calculateSubscriptOperatorIndex(SizeType pIndex) const;

		ResizableArray<ElementType> elementArray;
		SizeType firstElementIndex;
		SizeType lastElementIndex;
	};

	template <typename ElementType>
	inline
	Queue<ElementType>::Queue(SizeType pMaxNumberOfElements) :
	elementArray(pMaxNumberOfElements)
	{
		this->clear();
	}

	template <typename ElementType>
	inline
	ElementType& Queue<ElementType>::operator[](SizeType pIndex)
	{
		return this->elementArray[this->calculateSubscriptOperatorIndex(pIndex)];
	}

	template <typename ElementType>
	inline
	const ElementType& Queue<ElementType>::operator[](SizeType pIndex) const
	{
		return this->elementArray[this->calculateSubscriptOperatorIndex(pIndex)];
	}

	template <typename ElementType>
	inline
	SizeType Queue<ElementType>::getMaxNumberOfElements() const
	{
		return this->elementArray.getNumberOfElements() - 1;
	}

	template <typename ElementType>
	inline
	SizeType Queue<ElementType>::getNumberOfElements() const
	{
		if(this->lastElementIndex >= this->firstElementIndex)
		{
			return this->lastElementIndex - this->firstElementIndex;
		}
		else
		{
			return this->elementArray.getNumberOfElements() - (this->firstElementIndex - this->lastElementIndex);
		}
	}

	template <typename ElementType>
	inline
	const ElementType& Queue<ElementType>::getFirstElement() const
	{
		FORGE_ASSERT(this->getNumberOfElements() > 0);
		return this->elementArray[this->firstElementIndex];
	}

	template <typename ElementType>
	inline
	const ElementType& Queue<ElementType>::getLastElement() const
	{
		FORGE_ASSERT(this->getNumberOfElements() > 0);
		return this->elementArray[this->lastElementIndex];
	}

	template <typename ElementType>
	inline
	void Queue<ElementType>::append(const ElementType &pElementType)
	{
		const SizeType nextLastElementIndexIndex = this->getNextIndex(this->lastElementIndex);
		FORGE_ASSERT(nextLastElementIndexIndex != this->firstElementIndex);
		this->elementArray[nextLastElementIndexIndex] = pElementType;
		this->lastElementIndex = nextLastElementIndexIndex;
	}

	template <typename ElementType>
	inline
	ElementType Queue<ElementType>::remove()
	{
		FORGE_ASSERT(this->firstElementIndex != this->lastElementIndex);
		const SizeType previousFirstElementIndex = this->firstElementIndex;
		this->firstElementIndex = this->getNextIndex(this->firstElementIndex);
		return this->elementArray[previousFirstElementIndex];
	}

	template <typename ElementType>
	inline
	void Queue<ElementType>::clear()
	{
		this->firstElementIndex = 0;
		this->lastElementIndex = 0;
	}

	template <typename ElementType>
	inline
	SizeType Queue<ElementType>::getNextIndex(SizeType pIndex) const
	{
		FORGE_ASSERT(pIndex < this->elementArray.getNumberOfElements());

		if(pIndex == this->elementArray.getNumberOfElements() - 1)
		{
			return 0;
		}
		else
		{
			return pIndex + 1;
		}
	}

	template <typename ElementType>
	inline
	SizeType Queue<ElementType>::calculateSubscriptOperatorIndex(SizeType pIndex) const
	{
		FORGE_ASSERT(pIndex < this->getNumberOfElements());

		const SizeType indexAfterFirstElement = this->firstElementIndex + pIndex;
		if(indexAfterFirstElement >= this->elementArray.getNumberOfElements())
		{
			return pIndex - (this->elementArray.getNumberOfElements() - 1 - this->firstElementIndex);
		}
		else
		{
			return indexAfterFirstElement;
		}
	}
}

#endif