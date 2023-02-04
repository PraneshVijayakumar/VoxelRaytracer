#include "LineDrawing.h"
#include <Forge/Source/Core/Math/Vector2.h>
#include <Forge/Source/Core/Math/MathUtility.h>

namespace forge
{
	void drawLine(Framebuffer &pFramebuffer, IntegerVector2<Int> pFirstPoint, IntegerVector2<Int> pSecondPoint, const Color &pColor)
	{
		if(pFirstPoint == pSecondPoint)
		{
			pFramebuffer.setPixelColor(pFirstPoint.x, pFirstPoint.y, pColor);
			return;
		}

		bool wereXAndYSwapped = false;
		// If the slope of the line is greater than 1, swap the x and y coordinates of both points.
		// The x coordinate will be calculated based on the y coordinate in this case
		if(abs(pFirstPoint.x - pSecondPoint.x) < abs(pFirstPoint.y - pSecondPoint.y))
		{
			swap<Int>(pFirstPoint.x, pFirstPoint.y);
			swap<Int>(pSecondPoint.x, pSecondPoint.y);
			wereXAndYSwapped = true;
		}

		// Make sure that the first point is the one with the lower x value
		if(pFirstPoint.x > pSecondPoint.x)
		{
			swap<IntegerVector2<Int>>(pFirstPoint, pSecondPoint);
		}

		const Vector2 firstPointFloat(static_cast<Float>(pFirstPoint.x), static_cast<Float>(pFirstPoint.y));
		const Vector2 secondPointFloat(static_cast<Float>(pSecondPoint.x), static_cast<Float>(pSecondPoint.y));

		const Float slope = (secondPointFloat.y - firstPointFloat.y) / (secondPointFloat.x - firstPointFloat.x);
		const Float yIntercept = firstPointFloat.y - slope * firstPointFloat.x;

		if(wereXAndYSwapped)
		{
			for(Int x = pFirstPoint.x; x <= pSecondPoint.x; ++x)
			{
				Float y = slope * static_cast<Float>(x) + yIntercept;
				pFramebuffer.setPixelColor(roundFloatToInteger<Int>(y), x, pColor);
			}
		}
		else
		{
			for(Int x = pFirstPoint.x; x <= pSecondPoint.x; ++x)
			{
				Float y = slope * static_cast<Float>(x) + yIntercept;
				pFramebuffer.setPixelColor(x, roundFloatToInteger<Int>(y), pColor);
			}
		}
	}
}