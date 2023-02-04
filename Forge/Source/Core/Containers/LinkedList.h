#ifndef FORGE_LINKED_LIST_H
#define FORGE_LINKED_LIST_H

#include <Forge/Source/Core/Forge.h>

namespace forge
{
	/// <summary>
	/// An unsorted, doubly linked list
	/// </summary>
	/// <typeparam name="ElementType">The type of elements that the list can hold</typeparam>
	template <typename ElementType>
	class LinkedList final
	{
	public:

		~LinkedList();

		// Get a pointer to the first element in the list
		ElementType* getFirstElement();

		// Get a pointer to the last element in the list
		ElementType* getLastElement();

		// Get the number of elements that the list currently holds
		SizeType getNumberOfElements() const;

		/// <summary>
		/// Adds a new element to the end of the list
		/// </summary>
		/// <param name="pKey">The key of the new element to add</param>
		void append(const ElementType &pElement);

		/// <summary>
		/// Removes the first element in the list with the same value as the given element, if it exists
		/// </summary>
		/// <param name="pKey">The key of the element to remove</param>
		void remove(const ElementType &pElement);

		/// <summary>
		/// This function is used to iterate over all the elements in the list.
		/// Calling this return the will return the next iteration element and move the iteration element to the next element in the list
		/// Non-const version
		/// </summary>
		/// <returns>The next iteration element</returns>
		ElementType* getNextIterationElement();

		/// <summary>
		/// This function is used to iterate over all the elements in the list.
		/// Calling this return the will return the next iteration element and move the iteration element to the next element in the list.
		/// Const version
		/// </summary>
		/// <returns>The next iteration element</returns>
		const ElementType* getNextIterationElement() const;

		// Sets the next iteration element to be the first one in the list
		void resetElementIteration() const;

	private:

		// Holds an element and pointers to the previous and next nodes
		class Node
		{
			friend class LinkedList<ElementType>;

			// Making the constructor private, to prevent classes other than the LinkedList from creating Node objects
			Node() = default;
			// Making the destructor private, to prevent classes other than the LinkedList from destroying Node objects
			~Node() = default;

			// Constructor provided in order to be able to create nodes that hold elements that don't define a copy assignment operator
			Node(const ElementType &pElement, Node *pPreviousNode, Node *pNextNode) :
			element(pElement),
			previousNode(pPreviousNode),
			nextNode(pNextNode)
			{
			}

			// The key for this element
			ElementType element;
			// The previous node in the list
			Node *previousNode;
			// The next node in the list
			Node *nextNode;
		};

		/// <summary>
		/// The private implementation function for the public const and non-const element iteration functions.
		/// This function is used to iterate over all the elements in the list.
		/// Calling this return the will return the next iteration element and move the iteration element to the next element in the list
		/// </summary>
		/// <returns>The next iteration element</returns>
		ElementType* getNextIterationElementImplementation() const;

		// The first node in the list
		Node *firstNode = nullptr;
		// The last node in the list
		Node *lastNode = nullptr;
		// The number of elements currently in the list
		SizeType numberOfElements = 0;
		// The next iteration node. This pointer is used to iterate over all the node in the list
		mutable Node *nextIterationNode = nullptr;
	};

	template <typename ElementType>
	LinkedList<ElementType>::~LinkedList()
	{
		// Iterate over all the elements and delete them
		Node *currentNode = this->firstNode;
		while(currentNode)
		{
			Node *nextNode = currentNode->nextNode;
			delete currentNode;
			currentNode = nextNode;
		}
	}

	template <typename ElementType>
	ElementType* LinkedList<ElementType>::getFirstElement()
	{
		if(this->firstNode)
		{
			return &this->firstNode->element;
		}
		else
		{
			return nullptr;
		}
	}

	template <typename ElementType>
	ElementType* LinkedList<ElementType>::getLastElement()
	{
		if(this->lastNode)
		{
			return &this->lastNode->element;
		}
		else
		{
			return nullptr;
		}
	}

	template <typename ElementType>
	SizeType LinkedList<ElementType>::getNumberOfElements() const
	{
		return this->numberOfElements;
	}

	template <typename ElementType>
	void LinkedList<ElementType>::append(const ElementType &pElement)
	{
		// Create the new node
		Node *newNode = new Node(pElement, this->lastNode, nullptr);

		// The current last node needs to be updated before the new node can be set as the last node.
		// The way this needs to be done depends on whether a last node currently exists or not
		if(this->lastNode)
		{
			// If a last node currently exists, then set its next node to be the new node
			this->lastNode->nextNode = newNode;
		}
		else
		{
			// If a last node currently does not exist, that means the first node does not exist as well.
			// The new node is being added to an empty list. So set the first node to be the new node as well
			this->firstNode = newNode;

			// Update the next iteration node to be the first node
			this->nextIterationNode = newNode;
		}

		// Set the new node as the last node
		this->lastNode = newNode;

		++this->numberOfElements;
	}

	template <typename ElementType>
	void LinkedList<ElementType>::remove(const ElementType &pElement)
	{
		// Try to find a node in the list with the given element
		Node *currentNode = this->firstNode;
		// Using operator== here so that the operator ElementType needs to implement is operator==,
		// because its a bit cleaner than requiring operator!= 
		while(currentNode && !(currentNode->element == pElement))
		{
			currentNode = currentNode->nextNode;
		}

		// If an node with the given element is not found, it cannot be removed
		if(!currentNode)
		{
			return;
		}

		// Update the pointers of the previous and next nodes of the current node as needed
		{
			if(currentNode->previousNode)
			{
				currentNode->previousNode->nextNode = currentNode->nextNode;
			}
			else
			{
				this->firstNode = currentNode->nextNode;
			}
			if(currentNode->nextNode)
			{
				currentNode->nextNode->previousNode = currentNode->previousNode;
			}
			else
			{
				this->lastNode = currentNode->previousNode;
			}
		}

		// If the node to be removed is the next iteration node, reset the next iteration node
		if(currentNode == this->nextIterationNode)
		{
			this->nextIterationNode = this->firstNode;
		}

		delete currentNode;
		--this->numberOfElements;
	}

	template <typename ElementType>
	ElementType* LinkedList<ElementType>::getNextIterationElement()
	{
		return this->getNextIterationElementImplementation();
	}

	template <typename ElementType>
	const ElementType* LinkedList<ElementType>::getNextIterationElement() const
	{
		return this->getNextIterationElementImplementation();
	}

	template <typename ElementType>
	void LinkedList<ElementType>::resetElementIteration() const
	{
		this->nextIterationNode = this->firstNode;
	}

	template <typename ElementType>
	ElementType* LinkedList<ElementType>::getNextIterationElementImplementation() const
	{
		Node* nodeWithElementToReturn = this->nextIterationNode;
		if(nodeWithElementToReturn)
		{
			// Move the next iteration node to the next node
			this->nextIterationNode = nodeWithElementToReturn->nextNode;

			return &nodeWithElementToReturn->element;
		}
		else
		{
			return nullptr;
		}
	}
}

#endif