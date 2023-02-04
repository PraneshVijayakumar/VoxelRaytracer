#ifndef FORGE_EVENT_H
#define FORGE_EVENT_H

#include <Forge/Source/Core/Containers/LinkedList.h>

namespace forge
{
	/// <summary>
	/// This class implements the Observer design pattern.
	/// It uses a functional programming based design, that allows functions to listen to an event object of this type.
	/// The listener functions must be be member functions
	/// </summary>
	/// <typeparam name="...ParameterTypes">The types of the paramters that functions that listen to this event must have</typeparam>
	template <typename... ParameterTypes>
	class Event
	{
	public:

		/// <summary>
		/// Add a member function as a listener for this event. The member function pointer is provided as a non-type template parameter
		/// </summary>
		/// <typeparam name="ClassType">The type of the class to which the member function belongs</typeparam>
		/// <typeparam name="pFunctionPointer">A pointer to the member function</typeparam>
		/// <param name="pPointerToAnIstanceOfAClass">A pointer to an instance of the class on which the member function is to be called</param>
		template <typename ClassType, void (ClassType::*pFunctionPointer)(ParameterTypes...)>
		void addListener(ClassType *pPointerToAnIstanceOfAClass);

		/// <summary>
		/// Remove a listener from this event. The member function pointer is provided as a non-type template parameter
		/// </summary>
		/// <typeparam name="ClassType">The type of the class to which the member function belongs</typeparam>
		/// <typeparam name="pFunctionPointer">A pointer to the member function</typeparam>
		/// <param name="pPointerToAnIstanceOfAClass">A pointer to an instance of the class on which the member function is to be called</param>
		template <typename ClassType, void (ClassType::*pFunctionPointer)(ParameterTypes...)>
		void removeListener(ClassType *pPointerToAnIstanceOfAClass);

		/// <summary>
		/// Invoke the event. This calls all the currently listening functions
		/// </summary>
		/// <param name="...pParameters">The parameters to pass on to the listener functions</param>
		void invoke(ParameterTypes... pParameters) const;

	private:

		// Holds all the data needed to call a member or non-member listener function
		class FunctionPointerDatum
		{
			friend class Event;

		public:

			// Defining operator== here because the LinkedList needs it 
			bool operator==(const FunctionPointerDatum &pOtherFunctionPointerDatum)
			{
				return this->pointerToAnIstanceOfAClass == pOtherFunctionPointerDatum.pointerToAnIstanceOfAClass &&
					   this->listnerFunctionCallerFunctionPointer == pOtherFunctionPointerDatum.listnerFunctionCallerFunctionPointer;
			}

		private:

			void *pointerToAnIstanceOfAClass = nullptr;
			void (*listnerFunctionCallerFunctionPointer)(void*, ParameterTypes...) = nullptr;
		};

		/// <summary>
		/// A function that is used to call a member listener function. A pointer to the member function is provided as a non-type template parameter.
		/// This function takes the same parameters as the function that calls non-member functions
		/// </summary>
		/// <typeparam name="ClassType">The type of the class of which the provided listener function is a member</typeparam>
		/// <param name="pPointerToAnIstanceOfAClass">A pointer to an instance of the class of which the provided listener function is a member</param>
		/// <param name="...pParameters"></param>
		template <typename ClassType, void (ClassType::*pListenerFunctionPointer)(ParameterTypes...)>
		static void memberFunctionCaller(void *pPointerToAnIstanceOfAClass, ParameterTypes... pParameters);

		// This linked list holds all the current listener function pointers and other data needed to call them
		LinkedList<FunctionPointerDatum> functionPointerDatumArray;
	};

	template <typename... ParameterTypes>
	template <typename ClassType, void (ClassType::*pListenerFunctionPointer)(ParameterTypes...)>
	inline
	void Event<ParameterTypes...>::addListener(ClassType *pPointerToAnIstanceOfAClass)
	{
		FunctionPointerDatum newFunctionPointerDatum;
		newFunctionPointerDatum.pointerToAnIstanceOfAClass = pPointerToAnIstanceOfAClass;
		newFunctionPointerDatum.listnerFunctionCallerFunctionPointer = &memberFunctionCaller<ClassType, pListenerFunctionPointer>;
		this->functionPointerDatumArray.append(newFunctionPointerDatum);
	}

	template <typename... ParameterTypes>
	template <typename ClassType, void (ClassType::*pFunctionPointer)(ParameterTypes...)>
	inline
	void Event<ParameterTypes...>::removeListener(ClassType *pPointerToAnIstanceOfAClass)
	{
		this->functionPointerDatumArray.resetElementIteration();
		while(FunctionPointerDatum *functionPointerDatum = this->functionPointerDatumArray.getNextIterationElement())
		{
			if(functionPointerDatum->pointerToAnIstanceOfAClass == pPointerToAnIstanceOfAClass)
			{
				this->functionPointerDatumArray.remove(*functionPointerDatum);
				break;
			}
		}
	}

	template <typename... ParameterTypes>
	inline
	void Event<ParameterTypes...>::invoke(ParameterTypes... pParameters) const
	{
		this->functionPointerDatumArray.resetElementIteration();
		while(const FunctionPointerDatum *functionPointerDatum = this->functionPointerDatumArray.getNextIterationElement())
		{
			(*functionPointerDatum->listnerFunctionCallerFunctionPointer)(functionPointerDatum->pointerToAnIstanceOfAClass, pParameters...);
		}
	}

	template <typename... ParameterTypes>
	template <typename ClassType, void (ClassType::*pListenerFunctionPointer)(ParameterTypes...)>
	inline
	void Event<ParameterTypes...>::memberFunctionCaller(void *pPointerToAnIstanceOfAClass, ParameterTypes... pParameters)
	{
		(static_cast<ClassType*>(pPointerToAnIstanceOfAClass)->*pListenerFunctionPointer)(pParameters...);
	}
}

#endif