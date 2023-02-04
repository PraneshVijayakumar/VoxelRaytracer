#ifndef FORGE_COORDINATE_SPACE_H
#define FORGE_COORDINATE_SPACE_H

#include <Forge/Source/Core/Math/Vector3.h>
#include <Forge/Source/Core/Math/Orientation.h>

namespace forge
{
	// Represents a 3D coordinate space
	class CoordinateSpace
	{
	public:

		// The position of the coordinate space, specified in world space
		Vector3 position;
		// The orientation of the coordinate space, specified in world space
		Orientation orientation;
	};
}

#endif