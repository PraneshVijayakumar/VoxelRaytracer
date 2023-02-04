#ifndef FORGE_LINE_DRAWING_H
#define FORGE_LINE_DRAWING_H

#include <Forge/Source/Platform/Windows/Framebuffer.h>
#include <Forge/Source/Core/Math/IntegerVector2.h>

namespace forge
{
	void drawLine(Framebuffer &pFramebuffer, IntegerVector2<Int> pFirstPoint, IntegerVector2<Int> pSecondPoint, const Color &pColor);
}

#endif