#ifndef FORGE_THREAD_H
#define FORGE_THREAD_H

#include "../ForgeWindowsHeader.h"
#include <Forge/Source/Platform/Windows/WindowsLogging.h>

namespace forge
{
	class Thread
	{
	public:

		template <typename ClassType, void (ClassType::*pThreadFunctionPointer)()>
		void start(ClassType *pPointerToAnIstanceOfAClass);

		template <typename ClassType, typename FunctionArgumentType, void (ClassType::*pThreadFunctionPointer)(FunctionArgumentType)>
		void start(ClassType *pPointerToAnIstanceOfAClass, FunctionArgumentType pFunctionArgument);

		void waitForExit();

	private:

		template <typename ClassType, typename FunctionArgumentType>
		struct ThreadStartFunctionArguments
		{
			ClassType *classTypeObjectPointer = nullptr;
			FunctionArgumentType functionArgument;
		};

		template <typename ClassType, void (ClassType::*pThreadFunctionPointer)()>
		static DWORD WINAPI staticThreadFunctionWithoutParams(LPVOID pLpParameter);

		template <typename ClassType, typename FunctionArgumentType, void (ClassType::*pThreadFunctionPointer)(FunctionArgumentType)>
		static DWORD WINAPI staticThreadFunctionWithParams(LPVOID pLpParameter);

		HANDLE threadHandle;
		DWORD threadID;
	};

	template <typename ClassType, void (ClassType::*pThreadFunctionPointer)()>
	inline
	void Thread::start(ClassType *pPointerToAnIstanceOfAClass)
	{
		this->threadHandle = CreateThread(NULL, 0, this->staticThreadFunctionWithoutParams<ClassType, pThreadFunctionPointer>, (LPVOID)pPointerToAnIstanceOfAClass, 0, &this->threadID);
		if(this->threadHandle == NULL)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("CreateThread function failed");
		}
	}

	template <typename ClassType, typename FunctionArgumentType, void (ClassType::*pThreadFunctionPointer)(FunctionArgumentType)>
	inline
	void Thread::start(ClassType *pPointerToAnIstanceOfAClass, FunctionArgumentType pFunctionArgument)
	{
		ThreadStartFunctionArguments<ClassType, FunctionArgumentType> *threadStartFunctionArguments = new ThreadStartFunctionArguments<ClassType, FunctionArgumentType>;
		threadStartFunctionArguments->classTypeObjectPointer = pPointerToAnIstanceOfAClass;
		threadStartFunctionArguments->functionArgument = pFunctionArgument;
		this->threadHandle = CreateThread(NULL, 0, this->staticThreadFunctionWithParams<ClassType, FunctionArgumentType, pThreadFunctionPointer>, (LPVOID)threadStartFunctionArguments, 0, &this->threadID);
		if(this->threadHandle == NULL)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("CreateThread function failed");
		}
	}

	inline
	void Thread::waitForExit()
	{
		if(WaitForSingleObject(this->threadHandle, INFINITE) == WAIT_FAILED)
		{
			FORGE_WINDOWS_GET_AND_LOG_LAST_ERROR("WaitForSingleObject function failed when called on a Windows Thread");
		}
		int i = 0;
	}

	template <typename ClassType, void (ClassType::*pThreadFunctionPointer)()>
	inline
	DWORD WINAPI Thread::staticThreadFunctionWithoutParams(LPVOID pLpParameter)
	{
		ClassType *classTypeObject = (ClassType*)pLpParameter;
		(classTypeObject->*pThreadFunctionPointer)();
		return 0;
	}

	template <typename ClassType, typename FunctionArgumentType, void (ClassType::*pThreadFunctionPointer)(FunctionArgumentType)>
	inline
	DWORD WINAPI Thread::staticThreadFunctionWithParams(LPVOID pLpParameter)
	{
		ThreadStartFunctionArguments<ClassType, FunctionArgumentType> *threadStartFunctionArguments = (ThreadStartFunctionArguments<ClassType, FunctionArgumentType>*)pLpParameter;
		(threadStartFunctionArguments->classTypeObjectPointer->*pThreadFunctionPointer)(threadStartFunctionArguments->functionArgument);
		delete threadStartFunctionArguments;
		return 0;
	}
}

#endif