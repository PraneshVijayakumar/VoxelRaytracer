#ifndef DEBUG_AABB_DATA_H
#define DEBUG_AABB_DATA_H

#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/Math/AABB.h>
#include <Forge/Source/Core/Color.h>

namespace forge
{
	struct DebugAABBData
	{
		AABB aabb;
		Color color;
	};
}

#endif