#ifndef FORGE_LOGGING_H
#define FORGE_LOGGING_H

#include <iostream>

namespace forge
{
	/// <summary>
	/// Non-variadic function template that prints the given argument to the given stream.
	/// This is used to end the recursion of the variadic version of this function
	/// </summary>
	/// <typeparam name="TypeToPrint">The type of the argument to print</typeparam>
	/// <param name="stream">The stream to which the argument must be printed</param>
	/// <param name="variableToPrint">The argument to print</param>
	template <typename TypeToPrint>
	void printToStream(std::ostream &pStream, const TypeToPrint &pParameterToPrint)
	{
		pStream << pParameterToPrint;
	}

	/// <summary>
	/// Variadic function template that prints all of the given arguments to the given stream
	/// </summary>
	/// <typeparam name="TypeToPrint">The type of the first argument to print</typeparam>
	/// <typeparam name="...TypesToPrint">The types of the arguments to print, other than the first</typeparam>
	/// <param name="stream">The stream to which the arguments should be printed</param>
	/// <param name="variableToPrint">The first argument to print</param>
	/// <param name="...variablesToPrint">The arguments other than the first to be printed</param>
	template <typename TypeToPrint, typename... TypesToPrint>
	void printToStream(std::ostream &pStream, const TypeToPrint &pParameterToPrint, const TypesToPrint &... pParametersToPrint)
	{
		pStream << pParameterToPrint;
		printToStream(pStream, pParametersToPrint...);
	}
}

#define FORGE_DEBUG_LOG(...) \
do \
{ \
	std::cout << "Forge debug log : "; \
	forge::printToStream(std::cout, __VA_ARGS__); \
	std::cout << std::endl; \
}while(false)

#define FORGE_ERROR_LOG(...) \
do \
{ \
	std::cerr << "\n\nForge error log : "; \
	forge::printToStream(std::cerr, __VA_ARGS__); \
	std::cerr << std::endl; \
	std::cerr << "Translation unit info is given below " << std::endl; \
	std::cerr << "Filename : " << __FILE__ << std::endl; \
	std::cerr << "Line number : " << __LINE__ << std::endl; \
	std::cerr << "Translation date : " << __DATE__ << std::endl; \
	std::cerr << "Translation time : " << __TIME__ ; \
	std::cerr << std::endl << std::endl; \
}while(false)

#endif