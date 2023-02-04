#ifndef FORGE_FORGE_H
#define FORGE_FORGE_H

#include <cstdint>

namespace forge
{
	// Aliases for the built-in integer types

		// Represents a signed integer
		using Int = int;

	// Aliases for fixed width integer types

		// Signed types

			// A signed 32-bit integers
			using Int32 = std::int32_t;
			// A signed 64-bit integer
			using Int64 = std::int64_t;

		// Unsigned types

			// An unsigned 8-bit integer
			using UInt8 = std::uint8_t;
			// An unsigned 16-bit integer
			using UInt16 = std::uint16_t;
			// An unsigned 32-bit integer
			using UInt32 = std::uint32_t;
			// An unsigned 32-bit integer
			using UInt64 = std::uint64_t;

	using SizeType = decltype(sizeof(int));

	// Alias for the floating point types

		// Single precision floating point type
		using Float = float;
		// Double precision floating point type
		using Double = double;
}

#endif