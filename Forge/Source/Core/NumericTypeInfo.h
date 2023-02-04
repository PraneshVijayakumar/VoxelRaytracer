#ifndef FORGE_NUMERIC_TYPE_INFO_H
#define FORGE_NUMERIC_TYPE_INFO_H

#include <limits>
#include "Forge.h"

namespace forge
{
	// Gives info about the fundamental numeric types
	template <typename Type>
	class NumericTypeInfo
	{
	public:

		// The minimum value that an object of this type can have
		static constexpr Type minValue()
		{
			return 0;
		}

		// The maximum value that an object of this type can have
		static constexpr Type maxValue()
		{
			return 0;
		}
	};

	// NumericTypeInfo template specialization for the SizeType type
	template <>
	class NumericTypeInfo<SizeType>
	{
	public:

		static constexpr SizeType minValue()
		{
			return 0;
		}

		static constexpr SizeType maxValue()
		{
			return std::numeric_limits<SizeType>::max();
		}
	};

	// NumericTypeInfo template specialization for the Int64 type
	template <>
	class NumericTypeInfo<Int64>
	{
	public:

		static constexpr Int64 minValue()
		{
			return std::numeric_limits<Int64>::min();
		}

		static constexpr Int64 maxValue()
		{
			return std::numeric_limits<Int64>::max();
		}
	};

	// NumericTypeInfo template specialization for the Uint32 type
	template <>
	class NumericTypeInfo<UInt32>
	{
	public:

		static constexpr UInt32 minValue()
		{
			return std::numeric_limits<UInt32>::min();
		}

		static constexpr UInt32 maxValue()
		{
			return std::numeric_limits<UInt32>::max();
		}
	};

	template <>
	class NumericTypeInfo<Float>
	{
	public:

		static constexpr Float minValue()
		{
			return std::numeric_limits<Float>::min();
		}

		static constexpr Float maxValue()
		{
			return std::numeric_limits<Float>::max();
		}

		static constexpr Float epsilon()
		{
			return std::numeric_limits<Float>::epsilon();
		}
	};
}

#endif